/********************************************************************
 *  Shinobi Library Sample
 *  Copyright (c) 1998 SEGA
 *
 *  Library : Backup Library
 *  Module  : Library Sample(Dynamic memory allocation version)
 *  File    : backup.c
 *  Date    : 1999-01-14
 *  Version : 1.00
 *
 ********************************************************************/

#include <shinobi.h>
#include "_019340_8c014b8c_backup.h"

/*===============================================================*/
/* �Ή����郁�����[�J�[�h�̍ő�e��                              */
/* Mamimum volume to use.                                        */
/*===============================================================*/

#define MAX_CAPS BUD_CAPACITY_1MB


/*===============================================================*/
/* �Ή����郁�����[�J�[�h�̃h���C�u��                            */
/* Number of memory card to use.                                 */
/*===============================================================*/

#define MAX_DRIVES 8
#define USE_DRIVES BUD_USE_DRIVE_ALL


/*===============================================================*/
/* �������[�J�[�h�̏�Ԃ��i�[���Ă����\����                      */
/* (backup.h ���Q��)                                             */
/* Structure to store the information of memory card.            */
/* (See backup.h)                                                */
/*===============================================================*/

extern BACKUPINFO gBupInfo[8];



/*===============================================================*/
/*      �X�^�e�B�b�N�֐��̃v���g�^�C�v�錾                       */
/*      Prototypes of static functions.                          */
/*===============================================================*/

static Sint32 BupComplete(Sint32 drive, Sint32 op, Sint32 stat, Uint32 param);
static Sint32 BupProgress(Sint32 drive, Sint32 op, Sint32 count, Sint32 max);
static void BupInitCallback(void);
static void ClearInfo(Sint32 drive);


/*===============================================================*/
/*      �o�b�N�A�b�v���C�u����������                             */
/*      ���I�������m�ۂ����邽�߁A���[�N�A�h���X�ɂ�NULL���w��   */
/*      Initialize backup library.                               */
/*      Please set NULL to work address to allocate memory       */
/*      dynamically.                                             */
/*===============================================================*/

void BupInit(void)
{
    memset(gBupInfo, 0, sizeof(gBupInfo));
    buInit(MAX_CAPS, USE_DRIVES, NULL, BupInitCallback);
}


/*===============================================================*/
/*      �o�b�N�A�b�v���C�u�����I��                               */
/*      Finalize backup library.                                 */
/*===============================================================*/

void BupExit(void)
{
    do {} while (buExit() != BUD_ERR_OK);
}


/*===============================================================*/
/*      �������[�J�[�h�̏�Ԃ��擾                               */
/*      Get information of memory card.                          */
/*===============================================================*/

const BACKUPINFO* BupGetInfo(Sint32 drive)
{
    return (const BACKUPINFO*)&gBupInfo[drive];
}


/*===============================================================*/
/*      �t�@�C���̃��[�h                                         */
/*      Load a file.                                             */
/*===============================================================*/

Sint32 BupLoad(Sint32 drive, const char* fname, void* buf)
{
    return buLoadFile(drive, fname, buf, 0);
}


/*===============================================================*/
/*      �t�@�C���̃Z�[�u                                         */
/*      Save a file.                                             */
/*===============================================================*/

enum {
    Sunday = 0,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

extern SYS_RTC_DATE gBupTime;

Sint32 BupSave(Sint32 drive, const char* fname, void* buf, Sint32 nblock)
{
    syRtcGetDate(&gBupTime);
    return buSaveFile(drive, fname, buf, nblock, &gBupTime,
                            BUD_FLAG_VERIFY | BUD_FLAG_COPY(0));
}


/*===============================================================*/
/*      �t�@�C���̍폜                                           */
/*      Delete a file.                                           */
/*===============================================================*/

Sint32 BupDelete(Sint32 drive, const char* fname)
{
    return buDeleteFile(drive, fname);
}

/*===============================================================*/
/*      ���[�N�̊m�ۂƃ}�E���g                                   */
/*      ���̏������I�����A�}�E���g�R�[���o�b�N���Ԃ��Ă���       */
/*      ���܂łǂ���t�@�C���A�N�Z�X���\�ɂȂ�܂�             */
/*      Allocate memory and mount drive.                         */
/*      File access becomes possible as before after this        */
/*      operation is finished and mount callback occured.         */
/*===============================================================*/

void BupMount(Sint32 drive)
{
    BACKUPINFO* info;

    info = &gBupInfo[drive];

    if (info->Work) return;		/* ���łɃ}�E���g�ς� */

    info->Work = syMalloc(info->WorkSize);

    buMountDisk(drive, info->Work, info->WorkSize);
}

/*===============================================================*/
/*      �A���}�E���g                                             */
/*        ����ȍ~���̃h���C�u�ɂ̓A�N�Z�X�ł��܂���             */
/*        �ĂуA�N�Z�X�������ꍇ�́A������x���[�N�̊m�ۂ�       */
/*        �}�E���g���s���Ă�������                               */
/*      buUnmount()�g�p��̒���(���d�v)                          */
/*       �EBUD_OP_UNMOUNT�R�[���o�b�N�͔������܂���              */
/*       �EbuUnmount()���������Ȃ��ꍇ�A���̃h���C�u�̓A�N�Z�X�� */
/*         �ł��B���[�N���J�����Ă͂����܂���B                  */
/*       �EbuUnmount()�����������ꍇ�A����Ƀ��[�N���J�����Ă�   */
/*         ��肠��܂���                                        */
/*      Unmount                                                  */
/*        You cannot access the drive after this.                */
/*        Please allocate memory and mount when want to access   */
/*        again.                                                 */
/*      ATTENTION ON USE buUnmount():*** Important ***           */
/*        .When buUnmount() was not successful, the drive is in  */
/*         access, and must not open up work buffer              */
/*        .When buUnmount() was successful, you can open up work */
/*         buffer immmediately after.                            */
/*===============================================================*/

void BupUnmount(Sint32 drive)
{
    BACKUPINFO* info;
    Sint32 err;

    info = &gBupInfo[drive];

    if (info->Work == NULL) return;		/* �}�E���g����Ă��Ȃ�  */
                                        /* Drive is not mounted. */

    /* �A���}�E���g�֐����������Ȃ��ꍇ�A���̃h���C�u��       */
    /* �A�N�Z�X���ł���B���̏ꍇ�A���[�N���J�����Ă͂Ȃ�Ȃ� */
    /* When buUnmount() was not successful, the drive is in   */
    /* access, and must not open up work buffer               */

    if (buStat(drive) == BUD_STAT_READY) {
        buUnmount(drive);
        syFree(info->Work);
        ClearInfo(drive);
    }
}

static void ClearInfo(Sint32 drive)
{
    BACKUPINFO* info;

    info = &gBupInfo[drive];
    info->ProgressCount = 0;
    info->ProgressMax = 0;
    info->Operation = 0;
    info->Ready = FALSE;
    info->IsFormat = FALSE;
    info->Work = NULL;
    memset(&info->DiskInfo, 0, sizeof(BUS_DISKINFO));
}

/*===============================================================*/
/*      �G���[�R�[�h�𕶎���ɕϊ�                               */
/*      Convert error code into character string.                */
/*===============================================================*/

const char* BupGetErrorString(Sint32 err)
{
    switch (err) {
        // TODO: Fix as soon as we catch up with the constant integration
        case BUD_ERR_OK:				return (char*) 0x8c035dd4; /* "OK\0" */
        case BUD_ERR_BUSY:				return (char*) 0x8c035dd8; /* "BUSY\0" */
        case BUD_ERR_INVALID_PARAM:		return (char*) 0x8c035de0; /* "INVALID PARAMETER\0" */
        case BUD_ERR_ILLEGAL_DISK:		return (char*) 0x8c035df4; /* "ILLEGAL DISK\0" */
        case BUD_ERR_UNKNOWN_DISK:		return (char*) 0x8c035e04; /* "UNKNOWN DISK\0" */
        case BUD_ERR_NO_DISK:			return (char*) 0x8c035e14; /* "NO DISK\0" */
        case BUD_ERR_UNFORMAT:			return (char*) 0x8c035e20; /* "UNFORMAT\0" */
        case BUD_ERR_DISK_FULL:			return (char*) 0x8c035e2c; /* "DISK FULL\0" */
        case BUD_ERR_FILE_NOT_FOUND:	return (char*) 0x8c035e38; /* "FILE NOT FOUND\0" */
        case BUD_ERR_FILE_EXIST:		return (char*) 0x8c035e48; /* "FILE EXIST\0" */
        case BUD_ERR_CANNOT_OPEN:		return (char*) 0x8c035e54; /* "CANNOT OPEN\0" */
        case BUD_ERR_CANNOT_CREATE:		return (char*) 0x8c035e64; /* "CANNOT CREATE\0" */
        case BUD_ERR_EXECFILE_EXIST:	return (char*) 0x8c035e74; /* "EXECUTABLE FILE EXIST\0" */
        case BUD_ERR_ACCESS_DENIED:		return (char*) 0x8c035e8c; /* "ACCESS DENIED\0" */
        case BUD_ERR_VERIFY:			return (char*) 0x8c035e9c; /* "VERIFY ERROR\0" */
        default:						return (char*) 0x8c035eac; /* "GENERIC ERROR\0" */
    }
}


/*===============================================================*/
/*      �I�y���[�V�����R�[�h�𕶎���ɕϊ�                       */
/*      Convert operation code into character string.            */
/*===============================================================*/

const char* BupGetOperationString(Sint32 op)
{
    switch (op) {
        // TODO: Fix as soon as we catch up with the constant integration
        case BUD_OP_CONNECT:		return (char *) 0x8c035ebc; /* "CONNECTED\0" */
        case BUD_OP_MOUNT:			return (char *) 0x8c035ec8; /* "MOUNTED\0" */
        case BUD_OP_UNMOUNT:		return (char *) 0x8c035ed4; /* "UNMOUNTED\0" */
        case BUD_OP_FORMATDISK:		return (char *) 0x8c035ee0; /* "FORMATDISK\0" */
        case BUD_OP_DELETEFILE:		return (char *) 0x8c035eec; /* "DELETEFILE\0" */
        case BUD_OP_LOADFILE:		return (char *) 0x8c035ef8; /* "LOADFILE\0" */
        case BUD_OP_SAVEFILE:		return (char *) 0x8c035f04; /* "SAVEFILE\0" */
        default:					return (char *) 0x8c035f10; /* "UNKNOWN OPERATION\0" */
    }
}


/*===============================================================*/
/*      �R�[���o�b�N�֐�                                         */
/*      Callback functions.                                      */
/*===============================================================*/

/*
** �������I���R�[���o�b�N�֐�
** Callback function when initialization of library was finished.
*/

static void BupInitCallback(void)
{
    Sint32 i;

    /* �����A�o�߃R�[���o�b�N�֐���ݒ肷�� */
    buSetCompleteCallback(BupComplete);
    buSetProgressCallback(BupProgress);
}


/*
** �����R�[���o�b�N�֐�
** Callback function when operation was finished.
*/

static Sint32 BupComplete(Sint32 drive, Sint32 op, Sint32 stat, Uint32 param)
{
    BACKUPINFO* info;
    Sint32 ret;


    info = &gBupInfo[drive];

    switch (op) {
        /* �������[�J�[�h���ڑ����ꂽ */
        /* Memory card was connected.  */
        case BUD_OP_CONNECT:
            info->Connect = TRUE;
            info->WorkSize = BUM_WORK_SIZE(stat, 1);
            info->Capacity = stat;
            break;
        /* �o�b�N�A�b�vRAM���}�E���g���ꂽ */
        case BUD_OP_MOUNT:
            if (stat == BUD_ERR_OK) {
                info->Ready = TRUE;
                ret = buGetDiskInfo(drive, &info->DiskInfo);
                if (ret == BUD_ERR_OK) {
                    info->IsFormat = TRUE;
                }
                /* �G���[��Ԃ��N���A */
                /* Clear error status */
                info->LastError = BUD_ERR_OK;
            }
            break;
        /* �������[�J�[�h�����O���ꂽ */
        /* Memory card was removed.     */
        case BUD_OP_UNMOUNT:
            if (info->Work) syFree(info->Work);
            ClearInfo(drive);
            info->Connect = FALSE;
            break;
        default:
            /* �G���[��Ԃ��i�[ */
            /* Store error code. */
            info->LastError = stat;

            /* �����X�V���� */
            /* Update information of memory card. */
            buGetDiskInfo(drive, &info->DiskInfo);
    }

    /* �I�y���[�V�����R�[�h���N���A */
    /* Clear operation code.        */
    info->Operation = 0;

    return BUD_CBRET_OK;
}


/*
** �o�߃R�[���o�b�N�֐�
** Callback function through operation progress.
*/

static Sint32 BupProgress(Sint32 drive, Sint32 op, Sint32 count, Sint32 max)
{
    BACKUPINFO* info;

    info = &gBupInfo[drive];

    /* �o�ߏ�ԁA�I�y���[�V�����R�[�h���i�[ */
    /* Store operation code.                */
    info->ProgressCount = count;
    info->ProgressMax = max;
    info->Operation = op;

    return BUD_CBRET_OK;
}



/******************************* end of file *******************************/

/*
** sbinit.c Ver.0.70  1999/1/5
** Copyright (C) 1998 SEGA Enterprises Co.,Ltd
** All Rights Reserved
*/

#include <shinobi.h>      /* �E�w�b�_�t�@�C��                         */
                          /* Shinobi library header file              */
#include <sg_syhw.h>      /* �n�[�h�E�F�A���������C�u����             */
                          /* Hardware initialization library          */

/* ���[�NRAM�̐擪/�I���A�h���X                                       */
#define WORK_TOP 0x8c000000
#define WORK_END 0x8d000000

#define P1AREA   0x80000000

extern Uint8* _BSG_END;   /* BSG/BSG32�Z�N�V�����̏I���A�h���X        */
                          /* End address of BSG/BSG32 section         */


/* �����̋L�q�́A���C�u�����g�p�҂̃V�X�e���ɍ��킹��                 */
/* �ύX���邱�Ƃ��ł��܂��B                                           */
/* �ύX�̍ۂ͊֘A�h�L�������g���Q�Ƃ��A�e���ڂ�K�؂ɏC�����Ă������� */

/* You can change a description of this place according to a system   */
/* of library user.                                                   */
/* When change it, please refer to a document concerned and revise    */
/* each item adequately.                                              */


/***** GD�t�@�C���V�X�e�����g�����ǂ��� *******************************/
/***** Do you use GD filesystem ?                                     */
#define USE_GDFS 1        /* 0...�g��Ȃ�                             */
                          /* 0...Do not use.                          */
                          /* 1...�g��(DEFALUT)                        */
                          /* 1...Use (default)                        */

/***** B�Z�N�V�����̏I�����q�[�v�̐擪�Ƃ��邩 **********************/
/* Do you assign an end of B Section to the top op the heap area ?    */
#define USE_B_END 1       /* 0...���Ȃ�                               */
                          /* 0...Do not assign.                       */
                          /* 1...����(DEFALUT)                        */
                          /* 1...Assign (default)                     */


/* B�Z�N�V�����̏I�����q�[�v�̐擪�Ƃ��Ȃ��ꍇ�A                    */
/* HEAP_SIZE �Ƀq�[�v�e�ʂ��`���Ă��������B                         */
/* When you do not assign an end of B section to the top of the heap  */
/* area, please define the size of heap area in HEAP_SIZE macro.      */


/* GD:�����ɊJ�����Ƃ̂ł���ő�t�@�C����                            */
/* GD:Maximum file number opening at the same time.                   */
#define FILES 8

/* GD:�J�����g�f�B���N�g���o�b�t�@                                    */
/* GD:Current directory buffer                                        */
#define BUFFERS 2048

/* syMalloc()�Ŋm�ۂł��鍇�v�e��(��4MB)                              */
/* (B�Z�N�V�����̏I�����q�[�v�̐擪�Ƃ��Ȃ��ꍇ�̂ݗL��)            */
/* The total volume that can be secured with syMalloc() (about 4MB)   */
/* (This is effective whed does not assign an end of B section to the */
/* top of the heap area.                                              */
#if !USE_B_END
#define HEAP_SIZE 0x00400000
#endif









/* �O���[�o�����[�N�̐錾                                             */
/* Global work area                                                   */

// TODO: Remove extern
extern Uint8 gMapleRecvBuf[1024 * 24 * 2 + 32];
extern Uint8 gMapleSendBuf[1024 * 24 * 2 + 32];

#if USE_GDFS
// TODO: Remove extern (both)
extern Uint8 gdfswork[GDFS_WORK_SIZE(FILES) + 32];
extern Uint8 gdfscurdir[GDFS_DIRREC_SIZE(BUFFERS) + 32];
#endif

/* syMalloc()�̊Ǘ����ɒu���q�[�v�̐擪�A�h���X */
/* The top address of the heap area                                   */
#if USE_B_END
#define HEAP_AREA ((void*)((((Uint32)_BSG_END | P1AREA) & 0xffffffe0) + 0x20))
#define HEAP_SIZE (WORK_END - (Uint32)HEAP_AREA)
#else
#define HEAP_AREA ((void*)((Uint32)WORK_END - (Uint32)HEAP_SIZE))
#endif


/*
** �A�v���P�[�V�����̏������֐� (������njInitSystem()�݊�)
** Initialization function of the application.
**                  Arguments are compatible with njInitSystem().
*/

void sbInitSystem(Int mode, Int frame, Int count)
{
	/* �W���I�ȏ������������s���܂��B                             */
	/* ���̐ݒ�ŁA�����̃A�v���P�[�V�������ő�̃p�t�H�[�}���X�� */
	/* ���邱�Ƃ��ł��܂��B                                       */
	/* ���ɗ��R�̂Ȃ�����A�����̋L�q���C�����Ȃ��ł��������B     */
	/* �C������ꍇ�́A�e���C�u�����̎d�l���\������������A       */
	/* ���C�u�����g�p�҂̐ӔC�ɂ����ďC�����Ă��������B           */

	/* Do standard initialization.                                */
	/* By this setting, many applications can get the greatest    */
	/* performance.                                               */
	/* Without the special reason, please do not change a         */
	/* desctiption of this place.                                 */
	/* When change it, please understand specification of each    */
	/* library enough, and revise it in responsibility of library */
	/* user.                                                      */

	/* ���荞�݋֎~       */
	/* Disable interrupt. */
	set_imask(15);

	/* �n�[�h�E�F�A�̏����� */
	/* Initialize hardware.  */
	syHwInit();

	/* �������Ǘ��̏�����            */
	/* Initialize memory management. */
	syMallocInit(HEAP_AREA, HEAP_SIZE);

	/* Ninja/Kamui�̏�����     */
	/* Initialize Ninja/Kamui. */
	njInitSystem(mode, frame, count);

	/* �n�[�h�E�F�A�̏��������̂Q */
	/* Initialize hardware 2.     */
	syHwInit2();

	/* �R���g���[�����C�u�����̏����� */
	/* Initialize controller library. */
	pdInitPeripheral(PDD_PLOGIC_ACTIVE, gMapleRecvBuf, gMapleSendBuf);

	/* ���荞�݋���      */
	/* Enable interrupt. */
	set_imask(0);

#if USE_GDFS
	/* GD�t�@�C���V�X�e���̏�����                                 */
    /* Initialize GD filesystem.                                */
	{
		Uint8* wk;
		Uint8* dir;

		wk  = (Uint8*)(((Uint32)gdfswork & 0xffffffe0) + 0x20);
		dir = (Uint8*)(((Uint32)gdfscurdir & 0xffffffe0) + 0x20);
		gdFsInit(FILES, wk, BUFFERS, dir);
	}
#endif

	/* ���̑��̏�����                                           */
	/* ��ɂȂ����̑��̃��C�u�����̏������A�����               */
	/* ���[�U�[�ōs���ׂ�������������ꍇ�A�����ɋL�q�ł��܂��B */
	/* Other initializations.                                   */
	/* When there are initializations of the other libraries    */
	/* which there are not and the initializations that should  */
	/* be done by user aloft, please describe it here.          */
}


/*
** �A�v���P�[�V�����̏I���֐�
** Finalize application.
*/

void sbExitSystem(void)
{
    /* �R���g���[�����C�u�����̏I������ */
	/* Finalize controller library.     */
	pdExitPeripheral();

	/* Ninja/Kamui�̏I������ */
	/* Finalize Ninja/Kamui.       */
	njExitSystem();

	/* �������Ǘ��̏I������        */
	/* Finalize memory management. */
	syMallocFinish();

	/* �n�[�h�E�F�A�̏I������ */
	/* Finalize hardware.     */
	syHwFinish();

	/* ���荞�݋֎~       */
	/* Disable interrupt. */
	set_imask(15);

    // TODO
    _8c056358();
}


/******************************* end of file *******************************/


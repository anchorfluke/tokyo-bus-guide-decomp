typedef struct Test Test;

struct Test3 {
    Test* field_0x00;
    int field_0x04;
    int field_0x08;
    int field_0x0c;
}
typedef Test3;

struct Test2 {
    int field_0x00;
    Test3* field_0x04;
    int field_0x08;
}
typedef Test2;

struct Test {
    Test2* field_0x00;
    Test3* field_0x04;
    int field_0x08;
    int field_0x0c;
};

// extern Test _8c157a58;
extern Test3* _8c157a5c;
extern int aa;
extern int ab;

/* === Uninitialized vars === */
extern Test _8c157a58;

FUN_8c010fe8(Test2* p1, Test3* p2) {
    _8c157a58.field_0x00 = p1;

    /* 8c010ff4 */
    _8c157a58.field_0x0c = 0;

    /* 8c011004 */
    _8c157a58.field_0x00->field_0x08 = ((-32 + (int) p2) - (int) p1) * 32;

    /* 8c01100e */
    _8c157a58.field_0x00->field_0x04 = p2 - 2;

    /* _8c157a5c = _8c157a58.field_0x00->field_0x04;
    _8c157a58.field_0x00->field_0x04->field_0x0c = 0;
    _8c157a58.field_0x00->field_0x04->field_0x08 = 0;
    _8c157a58.field_0x00->field_0x04->field_0x00 = (Test3*) &_8c157a58;
    _8c157a58.field_0x00->field_0x04->field_0x04 = 0; */

    aa = 0;
    ab = 0;
}


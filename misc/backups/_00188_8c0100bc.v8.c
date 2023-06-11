/* Almost matching!
 * FUN_sound_8c0100bc matches perfectly.
 * FUN_mdiVol_8c010128 functionally matches, differs only by two instructions
 * and register allocation.
 */

#include <shinobi.h>
#include <sg_sd.h>

struct s_8c0fcd50 {
    int field_0x00;
    int field_0x04;
    int field_0x08;
    int field_0x0c;
    int field_0x10;
    int field_0x14;
    float field_0x18;
    float field_0x1c;
    float field_0x20;
}
typedef s_8c0fcd50;

extern s_8c0fcd50 _8c0fcd50;
/* TODO: Move to constant section */
extern const int const127_8c03bd90;
extern float _8c226468;
extern SDMIDI _midi_handle_8c0fcd28[7];

void FUN_sound_8c0100bc() {
    _8c0fcd50.field_0x18 = (float) const127_8c03bd90 / 2600;
    _8c0fcd50.field_0x1c = _8c0fcd50.field_0x18 * 2600 / 3000;
    _8c0fcd50.field_0x14 = const127_8c03bd90 * 30 / 100;
    _8c0fcd50.field_0x08 = const127_8c03bd90 * 40 / 100;
    _8c0fcd50.field_0x0c = _8c0fcd50.field_0x18 * 3000;
    _8c0fcd50.field_0x20 = (float) const127_8c03bd90 / 3900;
}

void FUN_mdiVol_8c010128() {
    SDMIDI *midi_handle_a;
    int r13_8c226468_as_int = _8c226468;
    float fr15_8c226468 = r13_8c226468_as_int;
    float tempfr;

    if ((_8c0fcd50.field_0x00 & 2) == 2) {
        midi_handle_a = &_midi_handle_8c0fcd28[7];

        if (fr15_8c226468 >= 10 && fr15_8c226468 < 3000) {
            sdMidiSetVol(
                *midi_handle_a,
                (float) _8c0fcd50.field_0x08 + (_8c0fcd50.field_0x18 * (fr15_8c226468 + -10)) + -127,
                0
            );
        /* 8c010192 */
        } else {
            if (fr15_8c226468 >= 3000) {
                /* 8c01019a */
                sdMidiSetVol(
                    *midi_handle_a,
                    (float) _8c0fcd50.field_0x0c - (_8c0fcd50.field_0x1c * (r13_8c226468_as_int + -3000)) + -127,
                    0
                );
            }
        }
    }

    /* LAB_8c0101bc */
    if ((_8c0fcd50.field_0x00 & 4) == 4) {
        sdMidiSetVol(
            _midi_handle_8c0fcd28[6],
            (float) _8c0fcd50.field_0x20 * (fr15_8c226468 + -1000) + -127,
           0
        );

        if (! (r13_8c226468_as_int >= 2100)) {
            /* 8c0101e2 */
            _8c0fcd50.field_0x00 &= -5;

            sdMidiSetVol(
                _midi_handle_8c0fcd28[6],
                -127,
                0
            );
        }
    }
}
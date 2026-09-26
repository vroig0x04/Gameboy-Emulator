

#include "apu.h"

void APU_Init(APU *apu) {
    /* global registers inicialization */
    apu->g_regs.NR52 = 0xff26;
    apu->g_regs.NR51 = 0xff25;
    apu->g_regs.NR50 = 0xff24;

    /* channel 1 inicialization */
    apu->ch1.NR10 = 0xff10;
    apu->ch1.NR11 = 0xff11;
    apu->ch1.NR12 = 0xff12;
    apu->ch1.NR13 = 0xff13;
    apu->ch1.NR14 = 0xff14;
    apu->ch1.timer = 0;

    /* Channel 2 inicialization */
    apu->ch2.NR21 = 0xff16;
    apu->ch2.NR22 = 0xff17;
    apu->ch2.NR23 = 0xff18;
    apu->ch2.NR24 = 0xff19;
    apu->ch2.timer = 0;

    /* Channel 3 inicialitzaion */
    apu->ch3.NR30 = 0xff1a;
    apu->ch3.NR31 = 0xff1b;
    apu->ch3.NR32 = 0xff1c;
    apu->ch3.NR33 = 0xff1d;
    apu->ch3.NR34 = 0xff1e;
    apu->ch3.timer = 0;

    /* Channel 4 inicialitzaion */
    apu->ch4.NR41 = 0xff20;
    apu->ch4.NR42 = 0Xff21;
    apu->ch4.NR43 = 0xff22;
    apu->ch4.NR44 = 0xff23;
    apu->ch4.timer = 0;

}

void APU_Pulse(APU *apu) {
    uint8_t pace = (apu->ch1.NR10 >> 4) & 0xf7;     
    uint8_t direction = (apu->ch1.NR10 >> 3) & 0xf1; // 0 addition, 1 substraction
    uint8_t step = apu->ch1.NR10 & 0xf7;
}
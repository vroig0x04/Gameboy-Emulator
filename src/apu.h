#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/

/**
 * Gameboy's Audio Processing Unit (APU)
 */

#ifndef APU_H
#define APU_H

typedef struct APU {
    /* global control registers */
    typedef struct Global_Reg {
        uint8_t NR52;  // FF22 - Audio master control
        uint8_t NR51;  // FF25 - Sound panning
        uint8_t NR50;  // FF24 - Master volume & VIN panning
    } Global_Reg;
    
    /* Sound channel 1 - Pulse with period sweep */
    typedef struct Channel_1 {
        uint8_t NR11;  // FF11 — Channel 1 length timer & duty cycle
        uint8_t NR12;  // FF12 — Channel 1 volume & envelope
        uint8_t NR13;  // FF13 — Channel 1 period low [write-only]
        uint8_t NR14;  // FF14 — Channel 1 period high & control

        int CH1_DAC;   // Channel 1 DAC active flag
        int CH1_flag;  // Channel 1 active flag
        int CH1_timer; // Channel 1 timer

    } Channel_1;

    /* Sound Channel 2 — Pulse */
    typedef struct Channel_2 {
        uint8_t NR21;  // FF16 → NR11
        uint8_t NR22;  // FF17 → NR12
        uint8_t NR23;  // FF18 → NR13
        uint8_t NR24;  // FF19 → NR14

        int CH2_DAC;   // Channel 2 DAC active flag
        int CH2_flag;  // Channel 2 active flag
        int CH2_timer; // Channel 2 timer

    } Channel_2;

    /* Sound Channel 3 — Wave output */
    typedef struct Channel_3 {
        uint8_t NR30;  // FF1A — Channel 3 DAC enable
        uint8_t NR31;  // FF1B — Channel 3 length timer [write-only]
        uint8_t NR32;  // FF1C — Channel 3 output level
        uint8_t NR33;  // FF1D — Channel 3 period low [write-only]
        uint8_t NR34;  // FF1E — Channel 3 period high & control

        int CH3_DAC;   // Channel 3 DAC active flag
        int CH3_flag;  // Channel 3 active flag
        int CH3_timer; // Channel 3 timer

    } Channel_3;

    /* FF30–FF3F — Wave pattern RAM */
    uint8_t wave_RAM[16];

    /* Sound Channel 4 — Noise */
    typedef struct Channel_4 {
        uint8_t NR41;  // FF20 — Channel 4 length timer [write-only]
        uint8_t NR42;  // FF21 — Channel 4 volume & envelope
        uint8_t NR43;  // FF22 — Channel 4 frequency & randomness
        uint8_t NR44;  // FF23 — Channel 4 control

        int CH4_DAC;   // Channel 4 DAC active flag
        int CH4_flag;  // Channel 4 active flag
        int CH3_timer; // Channel 4 timer

    } Channel_4;

    /* NOT used temporarly. Only available in Gameboy color */
    typedef struct PCM_Regs {
        uint8_t PCM12; // FF76 — (CGB Mode only): Digital outputs 1 & 2 [read-only]
        uint8_t PCM32; // FF77 — (CGB Mode only): Digital outputs 3 & 4 [read-only]
    } PCM_Regs;

    uint16_t LFSR;     // Linear-feedback Shift Register to generate pseudo-random noise
    int DIV_APU;       // increased when DIV's 4 bit goes from 1 to 0 

} APU;

#endif
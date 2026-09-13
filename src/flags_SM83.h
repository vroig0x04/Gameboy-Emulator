#include <stdint.h>
#include "sharp_SM83.h"

/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/

/*
Flag register (F) bits:
    7	6	5	4	3	2	1	0
    Z	N	H	C	0	0	0	0

    Z - Zero Flag
    N - Subtract Flag
    H - Half Carry Flag
    C - Carry Flag
    0 - Not used, always zero

*/

typedef enum {
    FLAG_C = 0x10, 
    FLAG_H = 0x20,
    FLAG_N = 0x40,
    FLAG_Z = 0x80
} SM83_Flags;


void cpu_set_flag(Sharp_SM83 *cpu, uint8_t flag, int cond);
uint8_t get_flag(Sharp_SM83 *cpu, uint8_t flag);
#include "flags_SM83.h"

/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/

void cpu_set_flag(Sharp_SM83 *cpu, uint8_t flag, int cond) {
    if (cond == 1) {
        cpu->regs.F |= flag;
    } else {
        cpu->regs.F &= ~flag;
    }
    cpu->regs.F &= 0xf0;
}

uint8_t get_flag(Sharp_SM83 *cpu, uint8_t flag) {
    return (cpu->regs.F & flag) ? 1 : 0;
}
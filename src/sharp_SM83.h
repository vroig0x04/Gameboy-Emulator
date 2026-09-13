
#include <stdint.h>

#ifndef SHARP_SM83_H
#define SHARP_SM83_H

/* Forward declaration to avoid circular include */
struct Sharp_MMU;


/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/

/*
    These registers are available and, therefore, visible for the programmer
    to use in assembly language for the Sharp_SM83.
    They have been defined according to an x86/64 architecture based on Little Endian
    where the most significat byte is stored at top addresses and the less significant byte
    at low adresses. 

    The architecture of this processor is a hybrid version of Intel8080 and
    Zilog 80 processor.

    As an example: 0x3412 in AF registers is A -> 0x34 and F -> 0x12

    In the GameBoy the fussion of A and FLAG registers is required. Therefore,
    a new structure will be defined as AF register. 

    Flag Register (AF):
        - Bit 7 (Z) : Zero flag
        - Bit 6 (N) : Add/Sub flag (BCD)
        - Bit 5 (H) : Half Carry flag (BCD) 
        - Bit 4 (C) : Carry flag 
        - Bit 3-0 : Not used (set to 0)
    
    In AF register, the 4 less significant bits are always zero, and can only
    be accesed through push/pop instructions

    The mode is set to 0 to run in GB (DMG) or 1 to run in GBC (CGB)

*/
typedef struct Sharp_Registers {
    
    union {
        struct { uint8_t F; uint8_t A; }; 
        uint16_t AF; 
    };

    union {
        struct { uint8_t C; uint8_t B; };
        uint16_t BC;
    };

    union {
        struct { uint8_t E; uint8_t D; };
        uint16_t DE;
    };

    union {
        struct { uint8_t L; uint8_t H; };
        uint16_t HL;
    };

    uint16_t PC; 
    uint16_t SP; 
    uint16_t BOOT_ROM;

} Sharp_Registers;

typedef struct Sharp_SM83 {
    Sharp_Registers regs;
    uint64_t cycles;
    int STOP;
    int HALT;
    int IS_PREFIXCB;
    int IME; /* disabled when game starts running 0/1*/
    int EI;
    uint8_t IE; // interrupt enable
    uint8_t IF; // interrupt flag 
    

} Sharp_SM83;

/* Include MMU after Sharp_SM83 is defined to avoid circular dependency */
#include "mmu.h"

typedef enum {
    ALU_ADDR8  = 0,
    ALU_ADDI8  = 1,
    ALU_ADDM16 = 2,
    ALU_SUB8   = 3,
    ALU_SUB16  = 4,
    ALU_ADCR8  = 5,
    ALU_ADCI8,
    ALU_ADCM16,
    ALU_SBC8,
    ALU_SBC16,
    ALU_AND8,
    ALU_AND16,
    ALU_XOR8,
    ALU_XOR16,
    ALU_OR8,
    ALU_OR16,
    ALU_CP8,
    ALU_CP16
} ALU_OP_G;

typedef enum {
    CPU_REG_LDA8,
    CPU_REG_LDA16,
    CPU_REG_LDB8,
    CPU_REG_LDB16,
    CPU_REG_LDC8,
    CPU_REG_LDC16,
    CPU_REG_LDD8,
    CPU_REG_LDD16,
    CPU_REG_LDE8,
    CPU_REG_LDE16,
    CPU_REG_LDH8,
    CPU_REG_LDH16,
    CPU_REG_LDL8,
    CPU_REG_LDL16,
    CPU_MEMHL_LDX,
    CPU_REGA_LDD8,
    CPU_REGB_LDD8,
    CPU_REGC_LDD8,
    CPU_REGD_LDD8,
    CPU_REGE_LDD8,
    CPU_REGH_LDD8,
    CPU_REGL_LDD8,
    CPU_MEMHL_LDD8,
    CPU_REGBC_LDD16,
    CPU_MEMBC_LDA,
    CPU_REGDE_LDD16,
    CPU_MEMDE_LDA,
    CPU_REGHL_LDD16,
    CPU_A16_LDSP,
    CPU_REGA_MEMBC,
    CPU_REGA_MEMDE,
    CPU_REGA_MEMHL,
    CPU_REGA_MEMHLI,
    CPU_REGA_MEMHLD,
    CPU_REGB_MEMHL,
    CPU_REGC_MEMHL,
    CPU_REGD_MEMHL,
    CPU_REGH_MEMHL,
    CPU_REGL_MEMHL,
    CPU_REGSP_LDD16,
    CPU_MEMHLD_REGA,
    CPU_A16_REGA,
    CPU_REGSP_LDHL,
    CPU_REGA_LDA16
} CPU_LD_OP;


/*

    GameBoy bus address is 16 bits. Therefore, the cpu
    is capable of addressing 2^16 memory positions.
    Thus, we have an address space of 64Kbytes for DMG version.

    For the CGB version, RAM scales up to 32Kbytes instead of 8KB. Therefore,
    we have 2^18 addresable memory positions. 
    Clock frequency also changes up to 8MHz. 

*/

static const float DMG_FREQ = 4.194304e+6f;
static const float CGB_FREQ = 8.388608e+6f;

//static const int DMG_MEM_SIZE = 65536;
//static const CGB_MEM_SIZE = 262144;

//uint8_t dmg_memory[DMG_MEM_SIZE];
//uint8_t cgb_memory[CGB_MEM_SIZE];


/*
    setup Sharp_SM83 registers.
    Params:
    - Sharp_Registers reference to initzialice
    - mode of GameBoy boot: DMG if 0, CGB if 1 
    This allows to skip the Boot ROM
*/

void SM83_Init_Regs(Sharp_SM83 *cpu, int mode);
void execute_cycle_instruction(Sharp_SM83 *cpu, Sharp_MMU *mmu);
void execute_cb_extended_table(Sharp_SM83 *cpu, uint8_t opcode);
void execute_standard_opcode(Sharp_SM83 *cpu, Sharp_MMU *mmu, uint8_t opcode);


#endif //EOF


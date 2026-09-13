

#include "sharp_SM83.h"
#include "mmu.h"
#include <stdint.h>

#ifndef ALU_SM83_H
#define ALU_SM83_H


/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/

/*
    This ALU is based on T-Cycles or Osscilator Cycles running at 4.19 MHz 
    instead of Machine Cycles to complete the cycle instruction. For instance, 
    NOP instruction may take 1 cycle in M-Cycles and 4 in T-Cycles. 
    In any case, both are correct. 

    The Sharp_SM83 processor uses operands as follows:
    OPCODE - DEST | SOURCE
    
    IMPORTANT:
    Half carry in 8-bit operations may happen in bit 3 -> bit 4
    and in 16-bit operations may happen in bit 11 -> bit 12


*/


/*
    Adds the given value to A register.
    ref.assem: ADD A,VALUE
    Info:
        This is an auxiliary function and is not corresponded
        with any instruction of SM83. 

*/
void alu_add(Sharp_SM83 *cpu, uint8_t value);

/*
    Substracts the value of any register from A register
    ref.assem: SUB A, VALUE
    Info:
        This is an auxiliary function and is not corresponded
        with any instruction of SM83. 
*/
void alu_sub(Sharp_SM83 *cpu, uint8_t value, int is_cp);

/*
    Adds the value of any register plus the Carry flag to A register.
    ref.assem: ADC A, VALUE
    Info:
        This is an auxiliary function and is not corresponded
        with any instruction of SM83. 
*/
void alu_adc(Sharp_SM83 *cpu, uint8_t value);

/*
    Substracts the value of any register plus the Carry flag to A register.
    ref.assem: ADC A, VALUE
    Info:
        This is an auxiliary function and is not corresponded
        with any instruction of SM83. 
*/
void alu_sbc(Sharp_SM83 *cpu, uint8_t value);


void alu_and(Sharp_SM83 *cpu, uint8_t value);

void alu_xor(Sharp_SM83 *cpu, uint8_t value);

void alu_or(Sharp_SM83 *cpu, uint8_t value);

void alu_inc(Sharp_SM83 *cpu, void *value, int is_16bit);

void alu_dec(Sharp_SM83 *cpu, void *value, int is_16bit);

/*
    NOP (no operation) instruction. 
    Info:
        - opcode: 0x00
        -  byte length
        - 4 cycle to be completed
        - no flags updated
*/
void nop(Sharp_SM83 *cpu);

/*
    LD instruction. Loads a 16 bit data into BC register.
    ref.assem: LD BC,d16
    Info:
        - opcode: 0x01
        - 3 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void ld_bc_d16(Sharp_SM83 *cpu, uint16_t d16);

/*
    LD instruction. Loads the content of a register into the
    memory position pointed by mem_BC.
    ref.assem: LD (BC),A
    Info:
        - opcode: 0x02
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_bc_a(Sharp_SM83 *cpu, Sharp_MMU *memory);

/*
    16 bit INC instruction. Adds 1 to BC register.
    ref.assem: INC BC
    Info:
        - opcode: 0x03
        - 1 byte length
        - 8 cyles to be completed
        - no flags updated
*/
void inc_bc(Sharp_SM83 *cpu);


/*
    8 bit INC instruction. Adds 1 to B register as single.
    ref.assem: INC B
    Info:
        - opcode: 0x04
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H -
*/
void inc_b(Sharp_SM83 *cpu);

/*
    8 bit DEC instruction. Substracts 1 to B register as single.
    ref.assem: DEC B
    Info:
        - opcode: 0x05
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H -
*/
void dec_b(Sharp_SM83 *cpu);

/*
    LD instuction. Loads 8 bit data from memory to B register as single.
    ref.assem: LD B,d8
    Info:
        - opcode: 0x06
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated

*/
void ld_b_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    8 bit Rotate Left Accumulator.  
    ref.assem: RLCA (A reg)
    Info:
        - opcode: 0x07
        - 1 byte length 
        - 4 cycles to be completed
        - flags updated: 0 0 0 C
*/
void rlca(Sharp_SM83 *cpu);

/*
    Loads the content pointed by the memory address to the stack pointer
    ref.assem: LD (a16),SP
    Info:
        - opcode: 0x08
        - 3 byte length
        - 20 cycles to be completed
        - no flags updated

*/
void ld_a16_sp(Sharp_MMU *memory, uint16_t a16, Sharp_SM83 *cpu);

/*
    Performs add operation with HL and BC registers. 
    The result is stored in HL.
    ref.assem: ADD HL,BC
    Info:
        - opcode: 0x09
        - 1 byte length
        - 8 cyles to be completed
        - flags updated: 0 H C -
*/
void add_hl_bc(Sharp_SM83 *cpu);

/*
    Loads the content pointed by BC register into A memory position.
    ref.assem: LD A,(BC)
    Info:
        - opcode: 0x0A
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_a_bc(Sharp_MMU *memory, Sharp_SM83 *cpu);

/*
    Substracts 1 from the BC register as single.
    ref.assem: DEC BC
    Info:
        - opcode: 0x0B
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void dec_bc(Sharp_SM83 *cpu);

/*
    Adds 1 to C register as single.
    ref.assem: INC C
    Info:
        - opcode: 0x0C
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H -
*/
void inc_c(Sharp_SM83 *cpu);

/*
    Substracts 1 to C register as single.
    ref.assem: DEC C
    Info:
        - opcode: 0x0D
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H -
*/
void dec_c(Sharp_SM83 *cpu);

/*
    Loads 8 bit data into C register.
    ref.assem: LD C,d8
    Info:
        - opcode: 0x0E
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_c_d8(Sharp_SM83 *cpu, uint8_t d8);


/*
    8 bit Rotate Right Accumulator.
    ref.assem: RRCA
    Info:
        - opcode: 0x0F
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: 0 0 0 C
*/
void rrca(Sharp_SM83 *cpu);


/*
    STOP instruction. Since it ends with 0, reset is required.
    ref.assem: STOP 0
    Info:
        - opcode 0x10
        - 2 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void stop();

/*
    Load 16 bit data into DE register.
    ref.assem: LD DE,d16
    Info:
        - opcode 0x11
        - 3 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void ld_de_d16(Sharp_SM83 *cpu, uint16_t d16);

/*
    Stores the value of A register into the memory address pointed by DE.
    ref.assem: LD (DE),A
    Info:
        - opcode: 0x12
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_de_a(Sharp_SM83 *cpu, Sharp_MMU *memory);

/*
    Increments the DE register pair by 1.
    ref.assem: INC DE
    Info:
        - opcode: 0x13
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void inc_de(Sharp_SM83 *cpu);

/*
    Increments the D register by 1.
    ref.assem: INC D
    Info:
        - opcode: 0x14
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H -
*/
void inc_d(Sharp_SM83 *cpu);

/*
    Decrements the D register by 1.
    ref.assem: DEC D
    Info:
        - opcode: 0x15
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H -
*/
void dec_d(Sharp_SM83 *cpu);

/*
    Loads 8 bit data into D register.
    ref.assem: LD D,d8
    Info:
        - opcode: 0x16
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_d_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    8 bit Rotate Left Accumulator through carry.
    ref.assem: RLA
    Info:
        - opcode: 0x17
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: 0 0 0 C
*/
void rla(Sharp_SM83 *cpu);

/*
    Relative jump by adding signed 8 bit offset to PC.
    ref.assem: JR r8
    Info:
        - opcode: 0x18
        - 2 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void jr_r8(Sharp_SM83 *cpu, int8_t r8);

/*
    Adds DE register pair to HL register pair.
    ref.assem: ADD HL,DE
    Info:
        - opcode: 0x19
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: - 0 H C
*/
void add_hl_de(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by DE into A register.
    ref.assem: LD A,(DE)
    Info:
        - opcode: 0x1A
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_a_de(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Decrements the DE register pair by 1.
    ref.assem: DEC DE
    Info:
        - opcode: 0x1B
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void dec_de(Sharp_SM83 *cpu);

/*
    Increments the E register by 1.
    ref.assem: INC E
    Info:
        - opcode: 0x1C
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H -
*/
void inc_e(Sharp_SM83 *cpu);

/*
    Decrements the E register by 1.
    ref.assem: DEC E
    Info:
        - opcode: 0x1D
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H -
*/
void dec_e(Sharp_SM83 *cpu);

/*
    Loads 8 bit data into E register.
    ref.assem: LD E,d8
    Info:
        - opcode: 0x1E
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_e_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    8 bit Rotate Right Accumulator through carry.
    ref.assem: RRA
    Info:
        - opcode: 0x1F
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: 0 0 0 C
*/
void rra(Sharp_SM83 *cpu);

/*
    Relative jump by adding signed 8 bit offset to PC if Zero flag is not set.
    ref.assem: JR NZ,r8
    Info:
        - opcode: 0x20
        - 2 byte length
        - 12 cycles if branch taken, 8 cycles if not taken
        - no flags updated
*/
void jr_nz_r8(Sharp_SM83 *cpu, int8_t r8);

/*
    Loads 16 bit data into HL register pair.
    ref.assem: LD HL,d16
    Info:
        - opcode: 0x21
        - 3 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void ld_hl_d16(Sharp_SM83 *cpu, uint16_t d16);

/*
    Stores the value of A register into the memory address pointed by HL, then increments HL.
    ref.assem: LD (HL+),A
    Info:
        - opcode: 0x22
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_hli_a(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Increments the HL register pair by 1.
    ref.assem: INC HL
    Info:
        - opcode: 0x23
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void inc_hl(Sharp_SM83 *cpu);

/*
    Increments the H register by 1.
    ref.assem: INC H
    Info:
        - opcode: 0x24
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H -
*/
void inc_h(Sharp_SM83 *cpu);

/*
    Decrements the H register by 1.
    ref.assem: DEC H
    Info:
        - opcode: 0x25
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H -
*/
void dec_h(Sharp_SM83 *cpu);

/*
    Loads 8 bit data into H register.
    ref.assem: LD H,d8
    Info:
        - opcode: 0x26
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_h_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Decimal Adjust Accumulator. Adjusts A register to obtain correct BCD 
    representation after BCD addition/subtraction.
    ref.assem: DAA
    Info:
        - opcode: 0x27
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z - 0 C
*/
void daa(Sharp_SM83 *cpu);

/*
    Relative jump by adding signed 8 bit offset to PC if Zero flag is set.
    ref.assem: JR Z,r8
    Info:
        - opcode: 0x28
        - 2 byte length
        - 12 cycles if branch taken, 8 cycles if not taken
        - no flags updated
*/
void jr_z_r8(Sharp_SM83 *cpu, int8_t r8);

/*
    Adds HL register pair to itself and stores the result in HL.
    ref.assem: ADD HL,HL
    Info:
        - opcode: 0x29
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: - 0 H C
*/
void add_hl_hl(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by HL into A register, then increments HL.
    ref.assem: LD A,(HL+)
    Info:
        - opcode: 0x2A
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_a_hli(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Decrements the HL register pair by 1.
    ref.assem: DEC HL
    Info:
        - opcode: 0x2B
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void dec_hl(Sharp_SM83 *cpu);

/*
    Increments the L register by 1.
    ref.assem: INC L
    Info:
        - opcode: 0x2C
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H -
*/
void inc_l(Sharp_SM83 *cpu);

/*
    Decrements the L register by 1.
    ref.assem: DEC L
    Info:
        - opcode: 0x2D
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H -
*/
void dec_l(Sharp_SM83 *cpu);

/*
    Loads 8 bit data into L register.
    ref.assem: LD L,d8
    Info:
        - opcode: 0x2E
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_l_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Complements all bits of A register (bitwise NOT).
    ref.assem: CPL
    Info:
        - opcode: 0x2F
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: - 1 1 -
*/
void cpl(Sharp_SM83 *cpu);

/*
    Relative jump by adding signed 8 bit offset to PC if Carry flag is not set.
    ref.assem: JR NC,r8
    Info:
        - opcode: 0x30
        - 2 byte length
        - 12 cycles if branch taken, 8 cycles if not taken
        - no flags updated
*/
void jr_nc_r8(Sharp_SM83 *cpu, int8_t r8);

/*
    Loads 16 bit data into SP register.
    ref.assem: LD SP,d16
    Info:
        - opcode: 0x31
        - 3 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void ld_sp_d16(Sharp_SM83 *cpu, uint16_t d16);

/*
    Stores the value of A register into the memory address pointed by HL, then decrements HL.
    ref.assem: LD (HL-),A
    Info:
        - opcode: 0x32
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_hld_a(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Increments the SP register by 1.
    ref.assem: INC SP
    Info:
        - opcode: 0x33
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void inc_sp(Sharp_SM83 *cpu);

/*
    Increments the value at the memory address pointed by HL by 1.
    ref.assem: INC (HL)
    Info:
        - opcode: 0x34
        - 1 byte length
        - 12 cycles to be completed
        - flags updated: Z 0 H -
*/
void inc_hl_ind(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Decrements the value at the memory address pointed by HL by 1.
    ref.assem: DEC (HL)
    Info:
        - opcode: 0x35
        - 1 byte length
        - 12 cycles to be completed
        - flags updated: Z 1 H -
*/
void dec_hl_ind(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Loads 8 bit data into the memory address pointed by HL.
    ref.assem: LD (HL),d8
    Info:
        - opcode: 0x36
        - 2 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void ld_hl_d8(Sharp_SM83 *cpu, Sharp_MMU *mmu, uint8_t d8);

/*
    Sets the Carry flag.
    ref.assem: SCF
    Info:
        - opcode: 0x37
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: - 0 0 1
*/
void scf(Sharp_SM83 *cpu);

/*
    Relative jump by adding signed 8 bit offset to PC if Carry flag is set.
    ref.assem: JR C,r8
    Info:
        - opcode: 0x38
        - 2 byte length
        - 12 cycles if branch taken, 8 cycles if not taken
        - no flags updated
*/
void jr_c_r8(Sharp_SM83 *cpu, int8_t r8);

/*
    Adds SP register to HL register pair and stores the result in HL.
    ref.assem: ADD HL,SP
    Info:
        - opcode: 0x39
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: - 0 H C
*/
void add_hl_sp(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by HL into A register, then decrements HL.
    ref.assem: LD A,(HL-)
    Info:
        - opcode: 0x3A
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_a_hld(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Decrements the SP register by 1.
    ref.assem: DEC SP
    Info:
        - opcode: 0x3B
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void dec_sp(Sharp_SM83 *cpu);

/*
    Increments the A register by 1.
    ref.assem: INC A
    Info:
        - opcode: 0x3C
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H -
*/
void inc_a(Sharp_SM83 *cpu);

/*
    Decrements the A register by 1.
    ref.assem: DEC A
    Info:
        - opcode: 0x3D
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H -
*/
void dec_a(Sharp_SM83 *cpu);

/*
    Loads 8 bit data into A register.
    ref.assem: LD A,d8
    Info:
        - opcode: 0x3E
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_a_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Complements the Carry flag, and resets N and H flags.
    ref.assem: CCF
    Info:
        - opcode: 0x3F
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: - 0 0 C
*/
void ccf(Sharp_SM83 *cpu);

/*
    Loads the value of B register into B register.
    ref.assem: LD B,B
    Info:
        - opcode: 0x40
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_b_b(Sharp_SM83 *cpu);

/*
    Loads the value of C register into B register.
    ref.assem: LD B,C
    Info:
        - opcode: 0x41
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_b_c(Sharp_SM83 *cpu);

/*
    Loads the value of D register into B register.
    ref.assem: LD B,D
    Info:
        - opcode: 0x42
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_b_d(Sharp_SM83 *cpu);

/*
    Loads the value of E register into B register.
    ref.assem: LD B,E
    Info:
        - opcode: 0x43
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_b_e(Sharp_SM83 *cpu);

/*
    Loads the value of H register into B register.
    ref.assem: LD B,H
    Info:
        - opcode: 0x44
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_b_h(Sharp_SM83 *cpu);

/*
    Loads the value of L register into B register.
    ref.assem: LD B,L
    Info:
        - opcode: 0x45
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_b_l(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by HL into B register.
    ref.assem: LD B,(HL)
    Info:
        - opcode: 0x46
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_b_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Loads the value of A register into B register.
    ref.assem: LD B,A
    Info:
        - opcode: 0x47
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_b_a(Sharp_SM83 *cpu);

/*
    Loads the value of B register into C register.
    ref.assem: LD C,B
    Info:
        - opcode: 0x48
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_c_b(Sharp_SM83 *cpu);

/*
    Loads the value of C register into C register.
    ref.assem: LD C,C
    Info:
        - opcode: 0x49
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_c_c(Sharp_SM83 *cpu);

/*
    Loads the value of D register into C register.
    ref.assem: LD C,D
    Info:
        - opcode: 0x4A
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_c_d(Sharp_SM83 *cpu);

/*
    Loads the value of E register into C register.
    ref.assem: LD C,E
    Info:
        - opcode: 0x4B
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_c_e(Sharp_SM83 *cpu);

/*
    Loads the value of H register into C register.
    ref.assem: LD C,H
    Info:
        - opcode: 0x4C
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_c_h(Sharp_SM83 *cpu);

/*
    Loads the value of L register into C register.
    ref.assem: LD C,L
    Info:
        - opcode: 0x4D
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_c_l(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by HL into C register.
    ref.assem: LD C,(HL)
    Info:
        - opcode: 0x4E
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_c_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Loads the value of A register into C register.
    ref.assem: LD C,A
    Info:
        - opcode: 0x4F
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_c_a(Sharp_SM83 *cpu);

/*
    Loads the value of B register into D register.
    ref.assem: LD D,B
    Info:
        - opcode: 0x50
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_d_b(Sharp_SM83 *cpu);

/*
    Loads the value of C register into D register.
    ref.assem: LD D,C
    Info:
        - opcode: 0x51
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_d_c(Sharp_SM83 *cpu);

/*
    Loads the value of D register into D register.
    ref.assem: LD D,D
    Info:
        - opcode: 0x52
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_d_d(Sharp_SM83 *cpu);

/*
    Loads the value of E register into D register.
    ref.assem: LD D,E
    Info:
        - opcode: 0x53
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_d_e(Sharp_SM83 *cpu);

/*
    Loads the value of H register into D register.
    ref.assem: LD D,H
    Info:
        - opcode: 0x54
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_d_h(Sharp_SM83 *cpu);

/*
    Loads the value of L register into D register.
    ref.assem: LD D,L
    Info:
        - opcode: 0x55
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_d_l(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by HL into D register.
    ref.assem: LD D,(HL)
    Info:
        - opcode: 0x56
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_d_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Loads the value of A register into D register.
    ref.assem: LD D,A
    Info:
        - opcode: 0x57
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_d_a(Sharp_SM83 *cpu);

/*
    Loads the value of B register into E register.
    ref.assem: LD E,B
    Info:
        - opcode: 0x58
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_e_b(Sharp_SM83 *cpu);

/*
    Loads the value of C register into E register.
    ref.assem: LD E,C
    Info:
        - opcode: 0x59
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_e_c(Sharp_SM83 *cpu);

/*
    Loads the value of D register into E register.
    ref.assem: LD E,D
    Info:
        - opcode: 0x5A
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_e_d(Sharp_SM83 *cpu);

/*
    Loads the value of E register into E register.
    ref.assem: LD E,E
    Info:
        - opcode: 0x5B
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_e_e(Sharp_SM83 *cpu);

/*
    Loads the value of H register into E register.
    ref.assem: LD E,H
    Info:
        - opcode: 0x5C
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_e_h(Sharp_SM83 *cpu);

/*
    Loads the value of L register into E register.
    ref.assem: LD E,L
    Info:
        - opcode: 0x5D
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_e_l(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by HL into E register.
    ref.assem: LD E,(HL)
    Info:
        - opcode: 0x5E
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_e_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Loads the value of A register into E register.
    ref.assem: LD E,A
    Info:
        - opcode: 0x5F
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_e_a(Sharp_SM83 *cpu);

/*
    Loads the value of B register into H register.
    ref.assem: LD H,B
    Info:
        - opcode: 0x60
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_h_b(Sharp_SM83 *cpu);

/*
    Loads the value of C register into H register.
    ref.assem: LD H,C
    Info:
        - opcode: 0x61
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_h_c(Sharp_SM83 *cpu);

/*
    Loads the value of D register into H register.
    ref.assem: LD H,D
    Info:
        - opcode: 0x62
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_h_d(Sharp_SM83 *cpu);

/*
    Loads the value of E register into H register.
    ref.assem: LD H,E
    Info:
        - opcode: 0x63
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_h_e(Sharp_SM83 *cpu);

/*
    Loads the value of H register into H register.
    ref.assem: LD H,H
    Info:
        - opcode: 0x64
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_h_h(Sharp_SM83 *cpu);

/*
    Loads the value of L register into H register.
    ref.assem: LD H,L
    Info:
        - opcode: 0x65
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_h_l(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by HL into H register.
    ref.assem: LD H,(HL)
    Info:
        - opcode: 0x66
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_h_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Loads the value of A register into H register.
    ref.assem: LD H,A
    Info:
        - opcode: 0x67
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_h_a(Sharp_SM83 *cpu);

/*
    Loads the value of B register into L register.
    ref.assem: LD L,B
    Info:
        - opcode: 0x68
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_l_b(Sharp_SM83 *cpu);

/*
    Loads the value of C register into L register.
    ref.assem: LD L,C
    Info:
        - opcode: 0x69
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_l_c(Sharp_SM83 *cpu);

/*
    Loads the value of D register into L register.
    ref.assem: LD L,D
    Info:
        - opcode: 0x6A
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_l_d(Sharp_SM83 *cpu);

/*
    Loads the value of E register into L register.
    ref.assem: LD L,E
    Info:
        - opcode: 0x6B
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_l_e(Sharp_SM83 *cpu);

/*
    Loads the value of H register into L register.
    ref.assem: LD L,H
    Info:
        - opcode: 0x6C
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_l_h(Sharp_SM83 *cpu);

/*
    Loads the value of L register into L register.
    ref.assem: LD L,L
    Info:
        - opcode: 0x6D
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_l_l(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by HL into L register.
    ref.assem: LD L,(HL)
    Info:
        - opcode: 0x6E
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_l_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Loads the value of A register into L register.
    ref.assem: LD L,A
    Info:
        - opcode: 0x6F
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_l_a(Sharp_SM83 *cpu);

/*
    Stores the value of B register into the memory address pointed by HL.
    ref.assem: LD (HL),B
    Info:
        - opcode: 0x70
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_hl_b(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Stores the value of C register into the memory address pointed by HL.
    ref.assem: LD (HL),C
    Info:
        - opcode: 0x71
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_hl_c(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Stores the value of D register into the memory address pointed by HL.
    ref.assem: LD (HL),D
    Info:
        - opcode: 0x72
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_hl_d(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Stores the value of E register into the memory address pointed by HL.
    ref.assem: LD (HL),E
    Info:
        - opcode: 0x73
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_hl_e(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Stores the value of H register into the memory address pointed by HL.
    ref.assem: LD (HL),H
    Info:
        - opcode: 0x74
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_hl_h(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Stores the value of L register into the memory address pointed by HL.
    ref.assem: LD (HL),L
    Info:
        - opcode: 0x75
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_hl_l(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Halts the CPU until an interrupt is requested.
    ref.assem: HALT
    Info:
        - opcode: 0x76
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void halt(Sharp_SM83 *cpu);

/*
    Stores the value of A register into the memory address pointed by HL.
    ref.assem: LD (HL),A
    Info:
        - opcode: 0x77
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_hl_a(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Loads the value of B register into A register.
    ref.assem: LD A,B
    Info:
        - opcode: 0x78
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_a_b(Sharp_SM83 *cpu);

/*
    Loads the value of C register into A register.
    ref.assem: LD A,C
    Info:
        - opcode: 0x79
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_a_c(Sharp_SM83 *cpu);

/*
    Loads the value of D register into A register.
    ref.assem: LD A,D
    Info:
        - opcode: 0x7A
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_a_d(Sharp_SM83 *cpu);

/*
    Loads the value of E register into A register.
    ref.assem: LD A,E
    Info:
        - opcode: 0x7B
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_a_e(Sharp_SM83 *cpu);

/*
    Loads the value of H register into A register.
    ref.assem: LD A,H
    Info:
        - opcode: 0x7C
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_a_h(Sharp_SM83 *cpu);

/*
    Loads the value of L register into A register.
    ref.assem: LD A,L
    Info:
        - opcode: 0x7D
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_a_l(Sharp_SM83 *cpu);

/*
    Loads the value at the memory address pointed by HL into A register.
    ref.assem: LD A,(HL)
    Info:
        - opcode: 0x7E
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_a_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Loads the value of A register into A register.
    ref.assem: LD A,A
    Info:
        - opcode: 0x7F
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ld_a_a(Sharp_SM83 *cpu);

/*
    Adds the value of B register to A register.
    ref.assem: ADD A,B
    Info:
        - opcode: 0x80
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void add_a_b(Sharp_SM83 *cpu);

/*
    Adds the value of C register to A register.
    ref.assem: ADD A,C
    Info:
        - opcode: 0x81
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void add_a_c(Sharp_SM83 *cpu);

/*
    Adds the value of D register to A register.
    ref.assem: ADD A,D
    Info:
        - opcode: 0x82
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void add_a_d(Sharp_SM83 *cpu);

/*
    Adds the value of E register to A register.
    ref.assem: ADD A,E
    Info:
        - opcode: 0x83
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void add_a_e(Sharp_SM83 *cpu);

/*
    Adds the value of H register to A register.
    ref.assem: ADD A,H
    Info:
        - opcode: 0x84
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void add_a_h(Sharp_SM83 *cpu);

/*
    Adds the value of L register to A register.
    ref.assem: ADD A,L
    Info:
        - opcode: 0x85
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void add_a_l(Sharp_SM83 *cpu);

/*
    Adds the value at the memory address pointed by HL to A register.
    ref.assem: ADD A,(HL)
    Info:
        - opcode: 0x86
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 H C
*/
void add_a_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Adds the value of A register to itself.
    ref.assem: ADD A,A
    Info:
        - opcode: 0x87
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void add_a_a(Sharp_SM83 *cpu);

/*
    Adds the value of B register plus the Carry flag to A register.
    ref.assem: ADC A,B
    Info:
        - opcode: 0x88
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void adc_a_b(Sharp_SM83 *cpu);

/*
    Adds the value of C register plus the Carry flag to A register.
    ref.assem: ADC A,C
    Info:
        - opcode: 0x89
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void adc_a_c(Sharp_SM83 *cpu);

/*
    Adds the value of D register plus the Carry flag to A register.
    ref.assem: ADC A,D
    Info:
        - opcode: 0x8A
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void adc_a_d(Sharp_SM83 *cpu);

/*
    Adds the value of E register plus the Carry flag to A register.
    ref.assem: ADC A,E
    Info:
        - opcode: 0x8B
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void adc_a_e(Sharp_SM83 *cpu);

/*
    Adds the value of H register plus the Carry flag to A register.
    ref.assem: ADC A,H
    Info:
        - opcode: 0x8C
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void adc_a_h(Sharp_SM83 *cpu);

/*
    Adds the value of L register plus the Carry flag to A register.
    ref.assem: ADC A,L
    Info:
        - opcode: 0x8D
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void adc_a_l(Sharp_SM83 *cpu);

/*
    Adds the value at the memory address pointed by HL plus the Carry flag to A register.
    ref.assem: ADC A,(HL)
    Info:
        - opcode: 0x8E
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 H C
*/
void adc_a_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Adds the value of A register plus the Carry flag to itself.
    ref.assem: ADC A,A
    Info:
        - opcode: 0x8F
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 H C
*/
void adc_a_a(Sharp_SM83 *cpu);

/*
    Subtracts the value of B register from A register.
    ref.assem: SUB B
    Info:
        - opcode: 0x90
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sub_b(Sharp_SM83 *cpu);

/*
    Subtracts the value of C register from A register.
    ref.assem: SUB C
    Info:
        - opcode: 0x91
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sub_c(Sharp_SM83 *cpu);

/*
    Subtracts the value of D register from A register.
    ref.assem: SUB D
    Info:
        - opcode: 0x92
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sub_d(Sharp_SM83 *cpu);

/*
    Subtracts the value of E register from A register.
    ref.assem: SUB E
    Info:
        - opcode: 0x93
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sub_e(Sharp_SM83 *cpu);

/*
    Subtracts the value of H register from A register.
    ref.assem: SUB H
    Info:
        - opcode: 0x94
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sub_h(Sharp_SM83 *cpu);

/*
    Subtracts the value of L register from A register.
    ref.assem: SUB L
    Info:
        - opcode: 0x95
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sub_l(Sharp_SM83 *cpu);

/*
    Subtracts the value at the memory address pointed by HL from A register.
    ref.assem: SUB (HL)
    Info:
        - opcode: 0x96
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: Z 1 H C
*/
void sub_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Subtracts the value of A register from itself, always resulting in zero.
    ref.assem: SUB A
    Info:
        - opcode: 0x97
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sub_a(Sharp_SM83 *cpu);

/*
    Subtracts the value of B register plus the Carry flag from A register.
    ref.assem: SBC A,B
    Info:
        - opcode: 0x98
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sbc_a_b(Sharp_SM83 *cpu);

/*
    Subtracts the value of C register plus the Carry flag from A register.
    ref.assem: SBC A,C
    Info:
        - opcode: 0x99
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sbc_a_c(Sharp_SM83 *cpu);

/*
    Subtracts the value of D register plus the Carry flag from A register.
    ref.assem: SBC A,D
    Info:
        - opcode: 0x9A
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sbc_a_d(Sharp_SM83 *cpu);

/*
    Subtracts the value of E register plus the Carry flag from A register.
    ref.assem: SBC A,E
    Info:
        - opcode: 0x9B
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sbc_a_e(Sharp_SM83 *cpu);

/*
    Subtracts the value of H register plus the Carry flag from A register.
    ref.assem: SBC A,H
    Info:
        - opcode: 0x9C
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sbc_a_h(Sharp_SM83 *cpu);

/*
    Subtracts the value of L register plus the Carry flag from A register.
    ref.assem: SBC A,L
    Info:
        - opcode: 0x9D
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sbc_a_l(Sharp_SM83 *cpu);

/*
    Subtracts the value at the memory address pointed by HL plus the Carry flag from A register.
    ref.assem: SBC A,(HL)
    Info:
        - opcode: 0x9E
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: Z 1 H C
*/
void sbc_a_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Subtracts the value of A register plus the Carry flag from itself.
    ref.assem: SBC A,A
    Info:
        - opcode: 0x9F
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void sbc_a_a(Sharp_SM83 *cpu);

/*
    Bitwise AND between A register and B register, result stored in A.
    ref.assem: AND B
    Info:
        - opcode: 0xA0
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 1 0
*/
void and_b(Sharp_SM83 *cpu);

/*
    Bitwise AND between A register and C register, result stored in A.
    ref.assem: AND C
    Info:
        - opcode: 0xA1
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 1 0
*/
void and_c(Sharp_SM83 *cpu);

/*
    Bitwise AND between A register and D register, result stored in A.
    ref.assem: AND D
    Info:
        - opcode: 0xA2
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 1 0
*/
void and_d(Sharp_SM83 *cpu);

/*
    Bitwise AND between A register and E register, result stored in A.
    ref.assem: AND E
    Info:
        - opcode: 0xA3
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 1 0
*/
void and_e(Sharp_SM83 *cpu);

/*
    Bitwise AND between A register and H register, result stored in A.
    ref.assem: AND H
    Info:
        - opcode: 0xA4
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 1 0
*/
void and_h(Sharp_SM83 *cpu);

/*
    Bitwise AND between A register and L register, result stored in A.
    ref.assem: AND L
    Info:
        - opcode: 0xA5
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 1 0
*/
void and_l(Sharp_SM83 *cpu);

/*
    Bitwise AND between A register and the value at the memory address pointed by HL, result stored in A.
    ref.assem: AND (HL)
    Info:
        - opcode: 0xA6
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 0
*/
void and_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Bitwise AND between A register and itself, result stored in A.
    ref.assem: AND A
    Info:
        - opcode: 0xA7
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 1 0
*/
void and_a(Sharp_SM83 *cpu);

/*
    Bitwise XOR between A register and B register, result stored in A.
    ref.assem: XOR B
    Info:
        - opcode: 0xA8
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void xor_b(Sharp_SM83 *cpu);

/*
    Bitwise XOR between A register and C register, result stored in A.
    ref.assem: XOR C
    Info:
        - opcode: 0xA9
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void xor_c(Sharp_SM83 *cpu);

/*
    Bitwise XOR between A register and D register, result stored in A.
    ref.assem: XOR D
    Info:
        - opcode: 0xAA
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void xor_d(Sharp_SM83 *cpu);

/*
    Bitwise XOR between A register and E register, result stored in A.
    ref.assem: XOR E
    Info:
        - opcode: 0xAB
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void xor_e(Sharp_SM83 *cpu);

/*
    Bitwise XOR between A register and H register, result stored in A.
    ref.assem: XOR H
    Info:
        - opcode: 0xAC
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void xor_h(Sharp_SM83 *cpu);

/*
    Bitwise XOR between A register and L register, result stored in A.
    ref.assem: XOR L
    Info:
        - opcode: 0xAD
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void xor_l(Sharp_SM83 *cpu);

/*
    Bitwise XOR between A register and the value at the memory address pointed by HL, result stored in A.
    ref.assem: XOR (HL)
    Info:
        - opcode: 0xAE
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void xor_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Bitwise XOR between A register and itself, always resulting in zero.
    ref.assem: XOR A
    Info:
        - opcode: 0xAF
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void xor_a(Sharp_SM83 *cpu);

/*
    Bitwise OR between A register and B register, result stored in A.
    ref.assem: OR B
    Info:
        - opcode: 0xB0
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void or_b(Sharp_SM83 *cpu);

/*
    Bitwise OR between A register and C register, result stored in A.
    ref.assem: OR C
    Info:
        - opcode: 0xB1
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void or_c(Sharp_SM83 *cpu);

/*
    Bitwise OR between A register and D register, result stored in A.
    ref.assem: OR D
    Info:
        - opcode: 0xB2
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void or_d(Sharp_SM83 *cpu);

/*
    Bitwise OR between A register and E register, result stored in A.
    ref.assem: OR E
    Info:
        - opcode: 0xB3
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void or_e(Sharp_SM83 *cpu);

/*
    Bitwise OR between A register and H register, result stored in A.
    ref.assem: OR H
    Info:
        - opcode: 0xB4
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void or_h(Sharp_SM83 *cpu);

/*
    Bitwise OR between A register and L register, result stored in A.
    ref.assem: OR L
    Info:
        - opcode: 0xB5
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void or_l(Sharp_SM83 *cpu);

/*
    Bitwise OR between A register and the value at the memory address pointed by HL, result stored in A.
    ref.assem: OR (HL)
    Info:
        - opcode: 0xB6
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void or_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Bitwise OR between A register and itself, result stored in A.
    ref.assem: OR A
    Info:
        - opcode: 0xB7
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 0 0 0
*/
void or_a(Sharp_SM83 *cpu);

/*
    Compares A register with B register by subtracting B from A without storing the result.
    ref.assem: CP B
    Info:
        - opcode: 0xB8
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void cp_b(Sharp_SM83 *cpu);

/*
    Compares A register with C register by subtracting C from A without storing the result.
    ref.assem: CP C
    Info:
        - opcode: 0xB9
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void cp_c(Sharp_SM83 *cpu);

/*
    Compares A register with D register by subtracting D from A without storing the result.
    ref.assem: CP D
    Info:
        - opcode: 0xBA
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void cp_d(Sharp_SM83 *cpu);

/*
    Compares A register with E register by subtracting E from A without storing the result.
    ref.assem: CP E
    Info:
        - opcode: 0xBB
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void cp_e(Sharp_SM83 *cpu);

/*
    Compares A register with H register by subtracting H from A without storing the result.
    ref.assem: CP H
    Info:
        - opcode: 0xBC
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void cp_h(Sharp_SM83 *cpu);

/*
    Compares A register with L register by subtracting L from A without storing the result.
    ref.assem: CP L
    Info:
        - opcode: 0xBD
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void cp_l(Sharp_SM83 *cpu);

/*
    Compares A register with the value at the memory address pointed by HL by subtracting it from A without storing the result.
    ref.assem: CP (HL)
    Info:
        - opcode: 0xBE
        - 1 byte length
        - 8 cycles to be completed
        - flags updated: Z 1 H C
*/
void cp_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu);

/*
    Compares A register with itself by subtracting A from A without storing the result, always resulting in Z set.
    ref.assem: CP A
    Info:
        - opcode: 0xBF
        - 1 byte length
        - 4 cycles to be completed
        - flags updated: Z 1 H C
*/
void cp_a(Sharp_SM83 *cpu);

/*
    Returns from subroutine if Zero flag is not set.
    ref.assem: RET NZ
    Info:
        - opcode: 0xC0
        - 1 byte length
        - 20 cycles if branch taken, 8 cycles if not taken
        - no flags updated
*/
void ret_nz(Sharp_SM83 *cpu);

/*
    Pops two bytes from the stack and stores them into BC register pair.
    ref.assem: POP BC
    Info:
        - opcode: 0xC1
        - 1 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void pop_bc(Sharp_SM83 *cpu);

/*
    Jumps to the 16 bit address if Zero flag is not set.
    ref.assem: JP NZ,a16
    Info:
        - opcode: 0xC2
        - 3 byte length
        - 16 cycles if branch taken, 12 cycles if not taken
        - no flags updated
*/
void jp_nz_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Jumps unconditionally to the 16 bit address.
    ref.assem: JP a16
    Info:
        - opcode: 0xC3
        - 3 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void jp_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Calls the subroutine at the 16 bit address if Zero flag is not set.
    ref.assem: CALL NZ,a16
    Info:
        - opcode: 0xC4
        - 3 byte length
        - 24 cycles if branch taken, 12 cycles if not taken
        - no flags updated
*/
void call_nz_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Pushes BC register pair onto the stack.
    ref.assem: PUSH BC
    Info:
        - opcode: 0xC5
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void push_bc(Sharp_SM83 *cpu);

/*
    Adds 8 bit immediate data to A register.
    ref.assem: ADD A,d8
    Info:
        - opcode: 0xC6
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 H C
*/
void add_a_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Calls the subroutine at address 0x0000.
    ref.assem: RST 00H
    Info:
        - opcode: 0xC7
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void rst_00h(Sharp_SM83 *cpu);

/*
    Returns from subroutine if Zero flag is set.
    ref.assem: RET Z
    Info:
        - opcode: 0xC8
        - 1 byte length
        - 20 cycles if branch taken, 8 cycles if not taken
        - no flags updated
*/
void ret_z(Sharp_SM83 *cpu);

/*
    Returns unconditionally from subroutine.
    ref.assem: RET
    Info:
        - opcode: 0xC9
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void ret(Sharp_SM83 *cpu);

/*
    Jumps to the 16 bit address if Zero flag is set.
    ref.assem: JP Z,a16
    Info:
        - opcode: 0xCA
        - 3 byte length
        - 16 cycles if branch taken, 12 cycles if not taken
        - no flags updated
*/
void jp_z_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Switches to the CB-prefixed instruction set for the next opcode.
    ref.assem: PREFIX CB
    Info:
        - opcode: 0xCB
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void prefix_cb(Sharp_SM83 *cpu);

/*
    Calls the subroutine at the 16 bit address if Zero flag is set.
    ref.assem: CALL Z,a16
    Info:
        - opcode: 0xCC
        - 3 byte length
        - 24 cycles if branch taken, 12 cycles if not taken
        - no flags updated
*/
void call_z_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Pushes PC onto the stack and jumps unconditionally to the 16 bit address.
    ref.assem: CALL a16
    Info:
        - opcode: 0xCD
        - 3 byte length
        - 24 cycles to be completed
        - no flags updated
*/
void call_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Adds 8 bit immediate data plus the Carry flag to A register.
    ref.assem: ADC A,d8
    Info:
        - opcode: 0xCE
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 H C
*/
void adc_a_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Calls the subroutine at address 0x0008.
    ref.assem: RST 08H
    Info:
        - opcode: 0xCF
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void rst_08h(Sharp_SM83 *cpu);

/*
    Returns from subroutine if Carry flag is not set.
    ref.assem: RET NC
    Info:
        - opcode: 0xD0
        - 1 byte length
        - 20 cycles if branch taken, 8 cycles if not taken
        - no flags updated
*/
void ret_nc(Sharp_SM83 *cpu);

/*
    Pops two bytes from the stack and stores them into DE register pair.
    ref.assem: POP DE
    Info:
        - opcode: 0xD1
        - 1 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void pop_de(Sharp_SM83 *cpu);

/*
    Jumps to the 16 bit address if Carry flag is not set.
    ref.assem: JP NC,a16
    Info:
        - opcode: 0xD2
        - 3 byte length
        - 16 cycles if branch taken, 12 cycles if not taken
        - no flags updated
*/
void jp_nc_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Calls the subroutine at the 16 bit address if Carry flag is not set.
    ref.assem: CALL NC,a16
    Info:
        - opcode: 0xD4
        - 3 byte length
        - 24 cycles if branch taken, 12 cycles if not taken
        - no flags updated
*/
void call_nc_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Pushes DE register pair onto the stack.
    ref.assem: PUSH DE
    Info:
        - opcode: 0xD5
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void push_de(Sharp_SM83 *cpu);

/*
    Subtracts 8 bit immediate data from A register.
    ref.assem: SUB d8
    Info:
        - opcode: 0xD6
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 1 H C
*/
void sub_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Calls the subroutine at address 0x0010.
    ref.assem: RST 10H
    Info:
        - opcode: 0xD7
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void rst_10h(Sharp_SM83 *cpu);

/*
    Returns from subroutine if Carry flag is set.
    ref.assem: RET C
    Info:
        - opcode: 0xD8
        - 1 byte length
        - 20 cycles if branch taken, 8 cycles if not taken
        - no flags updated
*/
void ret_c(Sharp_SM83 *cpu);

/*
    Returns from subroutine and enables interrupts.
    ref.assem: RETI
    Info:
        - opcode: 0xD9
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void reti(Sharp_SM83 *cpu);

/*
    Jumps to the 16 bit address if Carry flag is set.
    ref.assem: JP C,a16
    Info:
        - opcode: 0xDA
        - 3 byte length
        - 16 cycles if branch taken, 12 cycles if not taken
        - no flags updated
*/
void jp_c_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Calls the subroutine at the 16 bit address if Carry flag is set.
    ref.assem: CALL C,a16
    Info:
        - opcode: 0xDC
        - 3 byte length
        - 24 cycles if branch taken, 12 cycles if not taken
        - no flags updated
*/
void call_c_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Subtracts 8 bit immediate data plus the Carry flag from A register.
    ref.assem: SBC A,d8
    Info:
        - opcode: 0xDE
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 1 H C
*/
void sbc_a_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Calls the subroutine at address 0x0018.
    ref.assem: RST 18H
    Info:
        - opcode: 0xDF
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void rst_18h(Sharp_SM83 *cpu);

/*
    Stores the value of A register into the memory address 0xFF00 + 8 bit immediate offset.
    ref.assem: LDH (a8),A
    Info:
        - opcode: 0xE0
        - 2 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void ldh_a8_a(Sharp_SM83 *cpu, uint8_t a8);

/*
    Pops two bytes from the stack and stores them into HL register pair.
    ref.assem: POP HL
    Info:
        - opcode: 0xE1
        - 1 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void pop_hl(Sharp_SM83 *cpu);

/*
    Stores the value of A register into the memory address 0xFF00 + C register.
    ref.assem: LD (C),A
    Info:
        - opcode: 0xE2
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_c_ind_a(Sharp_SM83 *cpu);

/*
    Pushes HL register pair onto the stack.
    ref.assem: PUSH HL
    Info:
        - opcode: 0xE5
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void push_hl(Sharp_SM83 *cpu);

/*
    Bitwise AND between A register and 8 bit immediate data, result stored in A.
    ref.assem: AND d8
    Info:
        - opcode: 0xE6
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 0
*/
void and_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Calls the subroutine at address 0x0020.
    ref.assem: RST 20H
    Info:
        - opcode: 0xE7
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void rst_20h(Sharp_SM83 *cpu);

/*
    Adds signed 8 bit immediate data to SP register.
    ref.assem: ADD SP,r8
    Info:
        - opcode: 0xE8
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: 0 0 H C
*/
void add_sp_r8(Sharp_SM83 *cpu, int8_t r8);

/*
    Jumps unconditionally to the address contained in HL register pair.
    ref.assem: JP (HL)
    Info:
        - opcode: 0xE9
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void jp_hl(Sharp_SM83 *cpu);

/*
    Stores the value of A register into the 16 bit immediate memory address.
    ref.assem: LD (a16),A
    Info:
        - opcode: 0xEA
        - 3 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void ld_a16_a(Sharp_SM83 *cpu, uint16_t a16);

/*
    Bitwise XOR between A register and 8 bit immediate data, result stored in A.
    ref.assem: XOR d8
    Info:
        - opcode: 0xEE
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void xor_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Calls the subroutine at address 0x0028.
    ref.assem: RST 28H
    Info:
        - opcode: 0xEF
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void rst_28h(Sharp_SM83 *cpu);

/*
    Loads the value at memory address 0xFF00 + 8 bit immediate offset into A register.
    ref.assem: LDH A,(a8)
    Info:
        - opcode: 0xF0
        - 2 byte length
        - 12 cycles to be completed
        - no flags updated
*/
void ldh_a_a8(Sharp_SM83 *cpu, uint8_t a8);

/*
    Pops two bytes from the stack and stores them into AF register pair.
    ref.assem: POP AF
    Info:
        - opcode: 0xF1
        - 1 byte length
        - 12 cycles to be completed
        - flags updated: Z N H C
*/
void pop_af(Sharp_SM83 *cpu);

/*
    Loads the value at memory address 0xFF00 + C register into A register.
    ref.assem: LD A,(C)
    Info:
        - opcode: 0xF2
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_a_c_ind(Sharp_SM83 *cpu);

/*
    Disables interrupts by clearing the IME flag.
    ref.assem: DI
    Info:
        - opcode: 0xF3
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void di(Sharp_SM83 *cpu);

/*
    Pushes AF register pair onto the stack.
    ref.assem: PUSH AF
    Info:
        - opcode: 0xF5
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void push_af(Sharp_SM83 *cpu);

/*
    Bitwise OR between A register and 8 bit immediate data, result stored in A.
    ref.assem: OR d8
    Info:
        - opcode: 0xF6
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void or_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Calls the subroutine at address 0x0030.
    ref.assem: RST 30H
    Info:
        - opcode: 0xF7
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void rst_30h(Sharp_SM83 *cpu);

/*
    Loads the value of SP register plus signed 8 bit immediate offset into HL register pair.
    ref.assem: LD HL,SP+r8
    Info:
        - opcode: 0xF8
        - 2 byte length
        - 12 cycles to be completed
        - flags updated: 0 0 H C
*/
void ld_hl_sp_r8(Sharp_SM83 *cpu, int8_t r8);

/*
    Loads the value of HL register pair into SP register.
    ref.assem: LD SP,HL
    Info:
        - opcode: 0xF9
        - 1 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void ld_sp_hl(Sharp_SM83 *cpu);

/*
    Loads the value at the 16 bit immediate memory address into A register.
    ref.assem: LD A,(a16)
    Info:
        - opcode: 0xFA
        - 3 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void ld_a_a16(Sharp_SM83 *cpu, uint16_t a16);

/*
    Enables interrupts by setting the IME flag after the next instruction.
    ref.assem: EI
    Info:
        - opcode: 0xFB
        - 1 byte length
        - 4 cycles to be completed
        - no flags updated
*/
void ei(Sharp_SM83 *cpu);

/*
    Compares A register with 8 bit immediate data by subtracting it from A without storing the result.
    ref.assem: CP d8
    Info:
        - opcode: 0xFE
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 1 H C
*/
void cp_d8(Sharp_SM83 *cpu, uint8_t d8);

/*
    Calls the subroutine at address 0x0038.
    ref.assem: RST 38H
    Info:
        - opcode: 0xFF
        - 1 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void rst_38h(Sharp_SM83 *cpu);

/* 
   CB-PREFIXED INSTRUCTIONS — Sharp SM83 (Game Boy)
   Opcodes 0xCB00 – 0xCBFF
   All instructions are 2 bytes long (0xCB + opcode byte).
*/

/* 
   RLC — Rotate Left Circular
   Opcodes 0xCB00 – 0xCB07
   flags: Z 0 0 C
*/

/*
    Rotates B register left circularly. Bit 7 is copied into C flag and bit 0.
    ref.assem: RLC B
    Info:
        - opcode: 0xCB00
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rlc_b(Sharp_SM83 *cpu);

/*
    Rotates C register left circularly. Bit 7 is copied into C flag and bit 0.
    ref.assem: RLC C
    Info:
        - opcode: 0xCB01
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rlc_c(Sharp_SM83 *cpu);

/*
    Rotates D register left circularly. Bit 7 is copied into C flag and bit 0.
    ref.assem: RLC D
    Info:
        - opcode: 0xCB02
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rlc_d(Sharp_SM83 *cpu);

/*
    Rotates E register left circularly. Bit 7 is copied into C flag and bit 0.
    ref.assem: RLC E
    Info:
        - opcode: 0xCB03
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rlc_e(Sharp_SM83 *cpu);

/*
    Rotates H register left circularly. Bit 7 is copied into C flag and bit 0.
    ref.assem: RLC H
    Info:
        - opcode: 0xCB04
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rlc_h(Sharp_SM83 *cpu);

/*
    Rotates L register left circularly. Bit 7 is copied into C flag and bit 0.
    ref.assem: RLC L
    Info:
        - opcode: 0xCB05
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rlc_l(Sharp_SM83 *cpu);

/*
    Rotates the value at the memory address pointed by HL left circularly. Bit 7 is copied into C flag and bit 0.
    ref.assem: RLC (HL)
    Info:
        - opcode: 0xCB06
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rlc_hl(Sharp_SM83 *cpu);

/*
    Rotates A register left circularly. Bit 7 is copied into C flag and bit 0.
    ref.assem: RLC A
    Info:
        - opcode: 0xCB07
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rlc_a(Sharp_SM83 *cpu);

/*
   RRC — Rotate Right Circular
   Opcodes 0xCB08 – 0xCB0F
   flags: Z 0 0 C
*/

/*
    Rotates B register right circularly. Bit 0 is copied into C flag and bit 7.
    ref.assem: RRC B
    Info:
        - opcode: 0xCB08
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rrc_b(Sharp_SM83 *cpu);

/*
    Rotates C register right circularly. Bit 0 is copied into C flag and bit 7.
    ref.assem: RRC C
    Info:
        - opcode: 0xCB09
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rrc_c(Sharp_SM83 *cpu);

/*
    Rotates D register right circularly. Bit 0 is copied into C flag and bit 7.
    ref.assem: RRC D
    Info:
        - opcode: 0xCB0A
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rrc_d(Sharp_SM83 *cpu);

/*
    Rotates E register right circularly. Bit 0 is copied into C flag and bit 7.
    ref.assem: RRC E
    Info:
        - opcode: 0xCB0B
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rrc_e(Sharp_SM83 *cpu);

/*
    Rotates H register right circularly. Bit 0 is copied into C flag and bit 7.
    ref.assem: RRC H
    Info:
        - opcode: 0xCB0C
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rrc_h(Sharp_SM83 *cpu);

/*
    Rotates L register right circularly. Bit 0 is copied into C flag and bit 7.
    ref.assem: RRC L
    Info:
        - opcode: 0xCB0D
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rrc_l(Sharp_SM83 *cpu);

/*
    Rotates the value at the memory address pointed by HL right circularly. Bit 0 is copied into C flag and bit 7.
    ref.assem: RRC (HL)
    Info:
        - opcode: 0xCB0E
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rrc_hl(Sharp_SM83 *cpu);

/*
    Rotates A register right circularly. Bit 0 is copied into C flag and bit 7.
    ref.assem: RRC A
    Info:
        - opcode: 0xCB0F
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rrc_a(Sharp_SM83 *cpu);

/*
   RL — Rotate Left through Carry
   Opcodes 0xCB10 – 0xCB17
   flags: Z 0 0 C
*/

/*
    Rotates B register left through the Carry flag. Bit 7 is copied into C flag, old C flag into bit 0.
    ref.assem: RL B
    Info:
        - opcode: 0xCB10
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rl_b(Sharp_SM83 *cpu);

/*
    Rotates C register left through the Carry flag. Bit 7 is copied into C flag, old C flag into bit 0.
    ref.assem: RL C
    Info:
        - opcode: 0xCB11
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rl_c(Sharp_SM83 *cpu);

/*
    Rotates D register left through the Carry flag. Bit 7 is copied into C flag, old C flag into bit 0.
    ref.assem: RL D
    Info:
        - opcode: 0xCB12
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rl_d(Sharp_SM83 *cpu);

/*
    Rotates E register left through the Carry flag. Bit 7 is copied into C flag, old C flag into bit 0.
    ref.assem: RL E
    Info:
        - opcode: 0xCB13
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rl_e(Sharp_SM83 *cpu);

/*
    Rotates H register left through the Carry flag. Bit 7 is copied into C flag, old C flag into bit 0.
    ref.assem: RL H
    Info:
        - opcode: 0xCB14
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rl_h(Sharp_SM83 *cpu);

/*
    Rotates L register left through the Carry flag. Bit 7 is copied into C flag, old C flag into bit 0.
    ref.assem: RL L
    Info:
        - opcode: 0xCB15
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rl_l(Sharp_SM83 *cpu);

/*
    Rotates the value at the memory address pointed by HL left through the Carry flag. Bit 7 is copied into C flag, old C flag into bit 0.
    ref.assem: RL (HL)
    Info:
        - opcode: 0xCB16
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rl_hl(Sharp_SM83 *cpu);

/*
    Rotates A register left through the Carry flag. Bit 7 is copied into C flag, old C flag into bit 0.
    ref.assem: RL A
    Info:
        - opcode: 0xCB17
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rl_a(Sharp_SM83 *cpu);

/*
   RR — Rotate Right through Carry
   Opcodes 0xCB18 – 0xCB1F
   flags: Z 0 0 C
*/

/*
    Rotates B register right through the Carry flag. Bit 0 is copied into C flag, old C flag into bit 7.
    ref.assem: RR B
    Info:
        - opcode: 0xCB18
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rr_b(Sharp_SM83 *cpu);

/*
    Rotates C register right through the Carry flag. Bit 0 is copied into C flag, old C flag into bit 7.
    ref.assem: RR C
    Info:
        - opcode: 0xCB19
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rr_c(Sharp_SM83 *cpu);

/*
    Rotates D register right through the Carry flag. Bit 0 is copied into C flag, old C flag into bit 7.
    ref.assem: RR D
    Info:
        - opcode: 0xCB1A
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rr_d(Sharp_SM83 *cpu);

/*
    Rotates E register right through the Carry flag. Bit 0 is copied into C flag, old C flag into bit 7.
    ref.assem: RR E
    Info:
        - opcode: 0xCB1B
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rr_e(Sharp_SM83 *cpu);

/*
    Rotates H register right through the Carry flag. Bit 0 is copied into C flag, old C flag into bit 7.
    ref.assem: RR H
    Info:
        - opcode: 0xCB1C
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rr_h(Sharp_SM83 *cpu);

/*
    Rotates L register right through the Carry flag. Bit 0 is copied into C flag, old C flag into bit 7.
    ref.assem: RR L
    Info:
        - opcode: 0xCB1D
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rr_l(Sharp_SM83 *cpu);

/*
    Rotates the value at the memory address pointed by HL right through the Carry flag. Bit 0 is copied into C flag, old C flag into bit 7.
    ref.assem: RR (HL)
    Info:
        - opcode: 0xCB1E
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rr_hl(Sharp_SM83 *cpu);

/*
    Rotates A register right through the Carry flag. Bit 0 is copied into C flag, old C flag into bit 7.
    ref.assem: RR A
    Info:
        - opcode: 0xCB1F
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_rr_a(Sharp_SM83 *cpu);

/*
   SLA — Shift Left Arithmetic
   Opcodes 0xCB20 – 0xCB27
   flags: Z 0 0 C
*/

/*
    Shifts B register left arithmetically. Bit 7 is copied into C flag, bit 0 is set to 0.
    ref.assem: SLA B
    Info:
        - opcode: 0xCB20
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_sla_b(Sharp_SM83 *cpu);

/*
    Shifts C register left arithmetically. Bit 7 is copied into C flag, bit 0 is set to 0.
    ref.assem: SLA C
    Info:
        - opcode: 0xCB21
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_sla_c(Sharp_SM83 *cpu);

/*
    Shifts D register left arithmetically. Bit 7 is copied into C flag, bit 0 is set to 0.
    ref.assem: SLA D
    Info:
        - opcode: 0xCB22
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_sla_d(Sharp_SM83 *cpu);

/*
    Shifts E register left arithmetically. Bit 7 is copied into C flag, bit 0 is set to 0.
    ref.assem: SLA E
    Info:
        - opcode: 0xCB23
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_sla_e(Sharp_SM83 *cpu);

/*
    Shifts H register left arithmetically. Bit 7 is copied into C flag, bit 0 is set to 0.
    ref.assem: SLA H
    Info:
        - opcode: 0xCB24
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_sla_h(Sharp_SM83 *cpu);

/*
    Shifts L register left arithmetically. Bit 7 is copied into C flag, bit 0 is set to 0.
    ref.assem: SLA L
    Info:
        - opcode: 0xCB25
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_sla_l(Sharp_SM83 *cpu);

/*
    Shifts the value at the memory address pointed by HL left arithmetically. Bit 7 is copied into C flag, bit 0 is set to 0.
    ref.assem: SLA (HL)
    Info:
        - opcode: 0xCB26
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_sla_hl(Sharp_SM83 *cpu);

/*
    Shifts A register left arithmetically. Bit 7 is copied into C flag, bit 0 is set to 0.
    ref.assem: SLA A
    Info:
        - opcode: 0xCB27
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_sla_a(Sharp_SM83 *cpu);

/*
   SRA — Shift Right Arithmetic
   Opcodes 0xCB28 – 0xCB2F
   flags: Z 0 0 0
*/

/*
    Shifts B register right arithmetically. Bit 0 is copied into C flag, bit 7 retains its value.
    ref.assem: SRA B
    Info:
        - opcode: 0xCB28
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_sra_b(Sharp_SM83 *cpu);

/*
    Shifts C register right arithmetically. Bit 0 is copied into C flag, bit 7 retains its value.
    ref.assem: SRA C
    Info:
        - opcode: 0xCB29
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_sra_c(Sharp_SM83 *cpu);

/*
    Shifts D register right arithmetically. Bit 0 is copied into C flag, bit 7 retains its value.
    ref.assem: SRA D
    Info:
        - opcode: 0xCB2A
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_sra_d(Sharp_SM83 *cpu);

/*
    Shifts E register right arithmetically. Bit 0 is copied into C flag, bit 7 retains its value.
    ref.assem: SRA E
    Info:
        - opcode: 0xCB2B
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_sra_e(Sharp_SM83 *cpu);

/*
    Shifts H register right arithmetically. Bit 0 is copied into C flag, bit 7 retains its value.
    ref.assem: SRA H
    Info:
        - opcode: 0xCB2C
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_sra_h(Sharp_SM83 *cpu);

/*
    Shifts L register right arithmetically. Bit 0 is copied into C flag, bit 7 retains its value.
    ref.assem: SRA L
    Info:
        - opcode: 0xCB2D
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_sra_l(Sharp_SM83 *cpu);

/*
    Shifts the value at the memory address pointed by HL right arithmetically. Bit 0 is copied into C flag, bit 7 retains its value.
    ref.assem: SRA (HL)
    Info:
        - opcode: 0xCB2E
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_sra_hl(Sharp_SM83 *cpu);

/*
    Shifts A register right arithmetically. Bit 0 is copied into C flag, bit 7 retains its value.
    ref.assem: SRA A
    Info:
        - opcode: 0xCB2F
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_sra_a(Sharp_SM83 *cpu);

/*
   SWAP — Swap upper and lower nibbles
   Opcodes 0xCB30 – 0xCB37
   flags: Z 0 0 0
*/

/*
    Swaps the upper and lower nibbles of B register.
    ref.assem: SWAP B
    Info:
        - opcode: 0xCB30
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_swap_b(Sharp_SM83 *cpu);

/*
    Swaps the upper and lower nibbles of C register.
    ref.assem: SWAP C
    Info:
        - opcode: 0xCB31
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_swap_c(Sharp_SM83 *cpu);

/*
    Swaps the upper and lower nibbles of D register.
    ref.assem: SWAP D
    Info:
        - opcode: 0xCB32
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_swap_d(Sharp_SM83 *cpu);

/*
    Swaps the upper and lower nibbles of E register.
    ref.assem: SWAP E
    Info:
        - opcode: 0xCB33
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_swap_e(Sharp_SM83 *cpu);

/*
    Swaps the upper and lower nibbles of H register.
    ref.assem: SWAP H
    Info:
        - opcode: 0xCB34
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_swap_h(Sharp_SM83 *cpu);

/*
    Swaps the upper and lower nibbles of L register.
    ref.assem: SWAP L
    Info:
        - opcode: 0xCB35
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_swap_l(Sharp_SM83 *cpu);

/*
    Swaps the upper and lower nibbles of the value at the memory address pointed by HL.
    ref.assem: SWAP (HL)
    Info:
        - opcode: 0xCB36
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_swap_hl(Sharp_SM83 *cpu);

/*
    Swaps the upper and lower nibbles of A register.
    ref.assem: SWAP A
    Info:
        - opcode: 0xCB37
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 0
*/
void cb_swap_a(Sharp_SM83 *cpu);

/*
   SRL — Shift Right Logical
   Opcodes 0xCB38 – 0xCB3F
   flags: Z 0 0 C
*/

/*
    Shifts B register right logically. Bit 0 is copied into C flag, bit 7 is set to 0.
    ref.assem: SRL B
    Info:
        - opcode: 0xCB38
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_srl_b(Sharp_SM83 *cpu);

/*
    Shifts C register right logically. Bit 0 is copied into C flag, bit 7 is set to 0.
    ref.assem: SRL C
    Info:
        - opcode: 0xCB39
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_srl_c(Sharp_SM83 *cpu);

/*
    Shifts D register right logically. Bit 0 is copied into C flag, bit 7 is set to 0.
    ref.assem: SRL D
    Info:
        - opcode: 0xCB3A
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_srl_d(Sharp_SM83 *cpu);

/*
    Shifts E register right logically. Bit 0 is copied into C flag, bit 7 is set to 0.
    ref.assem: SRL E
    Info:
        - opcode: 0xCB3B
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_srl_e(Sharp_SM83 *cpu);

/*
    Shifts H register right logically. Bit 0 is copied into C flag, bit 7 is set to 0.
    ref.assem: SRL H
    Info:
        - opcode: 0xCB3C
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_srl_h(Sharp_SM83 *cpu);

/*
    Shifts L register right logically. Bit 0 is copied into C flag, bit 7 is set to 0.
    ref.assem: SRL L
    Info:
        - opcode: 0xCB3D
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_srl_l(Sharp_SM83 *cpu);

/*
    Shifts the value at the memory address pointed by HL right logically. Bit 0 is copied into C flag, bit 7 is set to 0.
    ref.assem: SRL (HL)
    Info:
        - opcode: 0xCB3E
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_srl_hl(Sharp_SM83 *cpu);

/*
    Shifts A register right logically. Bit 0 is copied into C flag, bit 7 is set to 0.
    ref.assem: SRL A
    Info:
        - opcode: 0xCB3F
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 0 C
*/
void cb_srl_a(Sharp_SM83 *cpu);

/*
   BIT — Test bit
   Opcodes 0xCB40 – 0xCB7F
   flags: Z 0 1 -
*/

/*
    Tests bit 0 of B register. Sets Z flag if bit is 0.
    ref.assem: BIT 0,B
    Info:
        - opcode: 0xCB40
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_0_b(Sharp_SM83 *cpu);

/*
    Tests bit 0 of C register. Sets Z flag if bit is 0.
    ref.assem: BIT 0,C
    Info:
        - opcode: 0xCB41
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_0_c(Sharp_SM83 *cpu);

/*
    Tests bit 0 of D register. Sets Z flag if bit is 0.
    ref.assem: BIT 0,D
    Info:
        - opcode: 0xCB42
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_0_d(Sharp_SM83 *cpu);

/*
    Tests bit 0 of E register. Sets Z flag if bit is 0.
    ref.assem: BIT 0,E
    Info:
        - opcode: 0xCB43
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_0_e(Sharp_SM83 *cpu);

/*
    Tests bit 0 of H register. Sets Z flag if bit is 0.
    ref.assem: BIT 0,H
    Info:
        - opcode: 0xCB44
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_0_h(Sharp_SM83 *cpu);

/*
    Tests bit 0 of L register. Sets Z flag if bit is 0.
    ref.assem: BIT 0,L
    Info:
        - opcode: 0xCB45
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_0_l(Sharp_SM83 *cpu);

/*
    Tests bit 0 of the value at the memory address pointed by HL. Sets Z flag if bit is 0.
    ref.assem: BIT 0,(HL)
    Info:
        - opcode: 0xCB46
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_0_hl(Sharp_SM83 *cpu);

/*
    Tests bit 0 of A register. Sets Z flag if bit is 0.
    ref.assem: BIT 0,A
    Info:
        - opcode: 0xCB47
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_0_a(Sharp_SM83 *cpu);

/*
    Tests bit 1 of B register. Sets Z flag if bit is 0.
    ref.assem: BIT 1,B
    Info:
        - opcode: 0xCB48
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_1_b(Sharp_SM83 *cpu);

/*
    Tests bit 1 of C register. Sets Z flag if bit is 0.
    ref.assem: BIT 1,C
    Info:
        - opcode: 0xCB49
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_1_c(Sharp_SM83 *cpu);

/*
    Tests bit 1 of D register. Sets Z flag if bit is 0.
    ref.assem: BIT 1,D
    Info:
        - opcode: 0xCB4A
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_1_d(Sharp_SM83 *cpu);

/*
    Tests bit 1 of E register. Sets Z flag if bit is 0.
    ref.assem: BIT 1,E
    Info:
        - opcode: 0xCB4B
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_1_e(Sharp_SM83 *cpu);

/*
    Tests bit 1 of H register. Sets Z flag if bit is 0.
    ref.assem: BIT 1,H
    Info:
        - opcode: 0xCB4C
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_1_h(Sharp_SM83 *cpu);

/*
    Tests bit 1 of L register. Sets Z flag if bit is 0.
    ref.assem: BIT 1,L
    Info:
        - opcode: 0xCB4D
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_1_l(Sharp_SM83 *cpu);

/*
    Tests bit 1 of the value at the memory address pointed by HL. Sets Z flag if bit is 0.
    ref.assem: BIT 1,(HL)
    Info:
        - opcode: 0xCB4E
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_1_hl(Sharp_SM83 *cpu);

/*
    Tests bit 1 of A register. Sets Z flag if bit is 0.
    ref.assem: BIT 1,A
    Info:
        - opcode: 0xCB4F
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_1_a(Sharp_SM83 *cpu);

/*
    Tests bit 2 of B register. Sets Z flag if bit is 0.
    ref.assem: BIT 2,B
    Info:
        - opcode: 0xCB50
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_2_b(Sharp_SM83 *cpu);

/*
    Tests bit 2 of C register. Sets Z flag if bit is 0.
    ref.assem: BIT 2,C
    Info:
        - opcode: 0xCB51
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_2_c(Sharp_SM83 *cpu);

/*
    Tests bit 2 of D register. Sets Z flag if bit is 0.
    ref.assem: BIT 2,D
    Info:
        - opcode: 0xCB52
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_2_d(Sharp_SM83 *cpu);

/*
    Tests bit 2 of E register. Sets Z flag if bit is 0.
    ref.assem: BIT 2,E
    Info:
        - opcode: 0xCB53
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_2_e(Sharp_SM83 *cpu);

/*
    Tests bit 2 of H register. Sets Z flag if bit is 0.
    ref.assem: BIT 2,H
    Info:
        - opcode: 0xCB54
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_2_h(Sharp_SM83 *cpu);

/*
    Tests bit 2 of L register. Sets Z flag if bit is 0.
    ref.assem: BIT 2,L
    Info:
        - opcode: 0xCB55
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_2_l(Sharp_SM83 *cpu);

/*
    Tests bit 2 of the value at the memory address pointed by HL. Sets Z flag if bit is 0.
    ref.assem: BIT 2,(HL)
    Info:
        - opcode: 0xCB56
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_2_hl(Sharp_SM83 *cpu);

/*
    Tests bit 2 of A register. Sets Z flag if bit is 0.
    ref.assem: BIT 2,A
    Info:
        - opcode: 0xCB57
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_2_a(Sharp_SM83 *cpu);

/*
    Tests bit 3 of B register. Sets Z flag if bit is 0.
    ref.assem: BIT 3,B
    Info:
        - opcode: 0xCB58
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_3_b(Sharp_SM83 *cpu);

/*
    Tests bit 3 of C register. Sets Z flag if bit is 0.
    ref.assem: BIT 3,C
    Info:
        - opcode: 0xCB59
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_3_c(Sharp_SM83 *cpu);

/*
    Tests bit 3 of D register. Sets Z flag if bit is 0.
    ref.assem: BIT 3,D
    Info:
        - opcode: 0xCB5A
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_3_d(Sharp_SM83 *cpu);

/*
    Tests bit 3 of E register. Sets Z flag if bit is 0.
    ref.assem: BIT 3,E
    Info:
        - opcode: 0xCB5B
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_3_e(Sharp_SM83 *cpu);

/*
    Tests bit 3 of H register. Sets Z flag if bit is 0.
    ref.assem: BIT 3,H
    Info:
        - opcode: 0xCB5C
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_3_h(Sharp_SM83 *cpu);

/*
    Tests bit 3 of L register. Sets Z flag if bit is 0.
    ref.assem: BIT 3,L
    Info:
        - opcode: 0xCB5D
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_3_l(Sharp_SM83 *cpu);

/*
    Tests bit 3 of the value at the memory address pointed by HL. Sets Z flag if bit is 0.
    ref.assem: BIT 3,(HL)
    Info:
        - opcode: 0xCB5E
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_3_hl(Sharp_SM83 *cpu);

/*
    Tests bit 3 of A register. Sets Z flag if bit is 0.
    ref.assem: BIT 3,A
    Info:
        - opcode: 0xCB5F
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_3_a(Sharp_SM83 *cpu);

/*
    Tests bit 4 of B register. Sets Z flag if bit is 0.
    ref.assem: BIT 4,B
    Info:
        - opcode: 0xCB60
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_4_b(Sharp_SM83 *cpu);

/*
    Tests bit 4 of C register. Sets Z flag if bit is 0.
    ref.assem: BIT 4,C
    Info:
        - opcode: 0xCB61
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_4_c(Sharp_SM83 *cpu);

/*
    Tests bit 4 of D register. Sets Z flag if bit is 0.
    ref.assem: BIT 4,D
    Info:
        - opcode: 0xCB62
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_4_d(Sharp_SM83 *cpu);

/*
    Tests bit 4 of E register. Sets Z flag if bit is 0.
    ref.assem: BIT 4,E
    Info:
        - opcode: 0xCB63
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_4_e(Sharp_SM83 *cpu);

/*
    Tests bit 4 of H register. Sets Z flag if bit is 0.
    ref.assem: BIT 4,H
    Info:
        - opcode: 0xCB64
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_4_h(Sharp_SM83 *cpu);

/*
    Tests bit 4 of L register. Sets Z flag if bit is 0.
    ref.assem: BIT 4,L
    Info:
        - opcode: 0xCB65
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_4_l(Sharp_SM83 *cpu);

/*
    Tests bit 4 of the value at the memory address pointed by HL. Sets Z flag if bit is 0.
    ref.assem: BIT 4,(HL)
    Info:
        - opcode: 0xCB66
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_4_hl(Sharp_SM83 *cpu);

/*
    Tests bit 4 of A register. Sets Z flag if bit is 0.
    ref.assem: BIT 4,A
    Info:
        - opcode: 0xCB67
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_4_a(Sharp_SM83 *cpu);

/*
    Tests bit 5 of B register. Sets Z flag if bit is 0.
    ref.assem: BIT 5,B
    Info:
        - opcode: 0xCB68
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_5_b(Sharp_SM83 *cpu);

/*
    Tests bit 5 of C register. Sets Z flag if bit is 0.
    ref.assem: BIT 5,C
    Info:
        - opcode: 0xCB69
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_5_c(Sharp_SM83 *cpu);

/*
    Tests bit 5 of D register. Sets Z flag if bit is 0.
    ref.assem: BIT 5,D
    Info:
        - opcode: 0xCB6A
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_5_d(Sharp_SM83 *cpu);

/*
    Tests bit 5 of E register. Sets Z flag if bit is 0.
    ref.assem: BIT 5,E
    Info:
        - opcode: 0xCB6B
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_5_e(Sharp_SM83 *cpu);

/*
    Tests bit 5 of H register. Sets Z flag if bit is 0.
    ref.assem: BIT 5,H
    Info:
        - opcode: 0xCB6C
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_5_h(Sharp_SM83 *cpu);

/*
    Tests bit 5 of L register. Sets Z flag if bit is 0.
    ref.assem: BIT 5,L
    Info:
        - opcode: 0xCB6D
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_5_l(Sharp_SM83 *cpu);

/*
    Tests bit 5 of the value at the memory address pointed by HL. Sets Z flag if bit is 0.
    ref.assem: BIT 5,(HL)
    Info:
        - opcode: 0xCB6E
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_5_hl(Sharp_SM83 *cpu);

/*
    Tests bit 5 of A register. Sets Z flag if bit is 0.
    ref.assem: BIT 5,A
    Info:
        - opcode: 0xCB6F
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_5_a(Sharp_SM83 *cpu);

/*
    Tests bit 6 of B register. Sets Z flag if bit is 0.
    ref.assem: BIT 6,B
    Info:
        - opcode: 0xCB70
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_6_b(Sharp_SM83 *cpu);

/*
    Tests bit 6 of C register. Sets Z flag if bit is 0.
    ref.assem: BIT 6,C
    Info:
        - opcode: 0xCB71
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_6_c(Sharp_SM83 *cpu);

/*
    Tests bit 6 of D register. Sets Z flag if bit is 0.
    ref.assem: BIT 6,D
    Info:
        - opcode: 0xCB72
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_6_d(Sharp_SM83 *cpu);

/*
    Tests bit 6 of E register. Sets Z flag if bit is 0.
    ref.assem: BIT 6,E
    Info:
        - opcode: 0xCB73
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_6_e(Sharp_SM83 *cpu);

/*
    Tests bit 6 of H register. Sets Z flag if bit is 0.
    ref.assem: BIT 6,H
    Info:
        - opcode: 0xCB74
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_6_h(Sharp_SM83 *cpu);

/*
    Tests bit 6 of L register. Sets Z flag if bit is 0.
    ref.assem: BIT 6,L
    Info:
        - opcode: 0xCB75
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_6_l(Sharp_SM83 *cpu);

/*
    Tests bit 6 of the value at the memory address pointed by HL. Sets Z flag if bit is 0.
    ref.assem: BIT 6,(HL)
    Info:
        - opcode: 0xCB76
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_6_hl(Sharp_SM83 *cpu);

/*
    Tests bit 6 of A register. Sets Z flag if bit is 0.
    ref.assem: BIT 6,A
    Info:
        - opcode: 0xCB77
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_6_a(Sharp_SM83 *cpu);

/*
    Tests bit 7 of B register. Sets Z flag if bit is 0.
    ref.assem: BIT 7,B
    Info:
        - opcode: 0xCB78
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_7_b(Sharp_SM83 *cpu);

/*
    Tests bit 7 of C register. Sets Z flag if bit is 0.
    ref.assem: BIT 7,C
    Info:
        - opcode: 0xCB79
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_7_c(Sharp_SM83 *cpu);

/*
    Tests bit 7 of D register. Sets Z flag if bit is 0.
    ref.assem: BIT 7,D
    Info:
        - opcode: 0xCB7A
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_7_d(Sharp_SM83 *cpu);

/*
    Tests bit 7 of E register. Sets Z flag if bit is 0.
    ref.assem: BIT 7,E
    Info:
        - opcode: 0xCB7B
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_7_e(Sharp_SM83 *cpu);

/*
    Tests bit 7 of H register. Sets Z flag if bit is 0.
    ref.assem: BIT 7,H
    Info:
        - opcode: 0xCB7C
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_7_h(Sharp_SM83 *cpu);

/*
    Tests bit 7 of L register. Sets Z flag if bit is 0.
    ref.assem: BIT 7,L
    Info:
        - opcode: 0xCB7D
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_7_l(Sharp_SM83 *cpu);

/*
    Tests bit 7 of the value at the memory address pointed by HL. Sets Z flag if bit is 0.
    ref.assem: BIT 7,(HL)
    Info:
        - opcode: 0xCB7E
        - 2 byte length
        - 16 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_7_hl(Sharp_SM83 *cpu);

/*
    Tests bit 7 of A register. Sets Z flag if bit is 0.
    ref.assem: BIT 7,A
    Info:
        - opcode: 0xCB7F
        - 2 byte length
        - 8 cycles to be completed
        - flags updated: Z 0 1 -
*/
void cb_bit_7_a(Sharp_SM83 *cpu);

/*
   RES — Reset bit
   Opcodes 0xCB80 – 0xCBBF
   flags: - - - -
*/

/*
    Resets bit 0 of B register to 0.
    ref.assem: RES 0,B
    Info:
        - opcode: 0xCB80
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_0_b(Sharp_SM83 *cpu);

/*
    Resets bit 0 of C register to 0.
    ref.assem: RES 0,C
    Info:
        - opcode: 0xCB81
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_0_c(Sharp_SM83 *cpu);

/*
    Resets bit 0 of D register to 0.
    ref.assem: RES 0,D
    Info:
        - opcode: 0xCB82
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_0_d(Sharp_SM83 *cpu);

/*
    Resets bit 0 of E register to 0.
    ref.assem: RES 0,E
    Info:
        - opcode: 0xCB83
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_0_e(Sharp_SM83 *cpu);

/*
    Resets bit 0 of H register to 0.
    ref.assem: RES 0,H
    Info:
        - opcode: 0xCB84
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_0_h(Sharp_SM83 *cpu);

/*
    Resets bit 0 of L register to 0.
    ref.assem: RES 0,L
    Info:
        - opcode: 0xCB85
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_0_l(Sharp_SM83 *cpu);

/*
    Resets bit 0 of the value at the memory address pointed by HL to 0.
    ref.assem: RES 0,(HL)
    Info:
        - opcode: 0xCB86
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_res_0_hl(Sharp_SM83 *cpu);

/*
    Resets bit 0 of A register to 0.
    ref.assem: RES 0,A
    Info:
        - opcode: 0xCB87
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_0_a(Sharp_SM83 *cpu);

/*
    Resets bit 1 of B register to 0.
    ref.assem: RES 1,B
    Info:
        - opcode: 0xCB88
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_1_b(Sharp_SM83 *cpu);

/*
    Resets bit 1 of C register to 0.
    ref.assem: RES 1,C
    Info:
        - opcode: 0xCB89
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_1_c(Sharp_SM83 *cpu);

/*
    Resets bit 1 of D register to 0.
    ref.assem: RES 1,D
    Info:
        - opcode: 0xCB8A
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_1_d(Sharp_SM83 *cpu);

/*
    Resets bit 1 of E register to 0.
    ref.assem: RES 1,E
    Info:
        - opcode: 0xCB8B
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_1_e(Sharp_SM83 *cpu);

/*
    Resets bit 1 of H register to 0.
    ref.assem: RES 1,H
    Info:
        - opcode: 0xCB8C
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_1_h(Sharp_SM83 *cpu);

/*
    Resets bit 1 of L register to 0.
    ref.assem: RES 1,L
    Info:
        - opcode: 0xCB8D
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_1_l(Sharp_SM83 *cpu);

/*
    Resets bit 1 of the value at the memory address pointed by HL to 0.
    ref.assem: RES 1,(HL)
    Info:
        - opcode: 0xCB8E
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_res_1_hl(Sharp_SM83 *cpu);

/*
    Resets bit 1 of A register to 0.
    ref.assem: RES 1,A
    Info:
        - opcode: 0xCB8F
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_1_a(Sharp_SM83 *cpu);

/*
    Resets bit 2 of B register to 0.
    ref.assem: RES 2,B
    Info:
        - opcode: 0xCB90
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_2_b(Sharp_SM83 *cpu);

/*
    Resets bit 2 of C register to 0.
    ref.assem: RES 2,C
    Info:
        - opcode: 0xCB91
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_2_c(Sharp_SM83 *cpu);

/*
    Resets bit 2 of D register to 0.
    ref.assem: RES 2,D
    Info:
        - opcode: 0xCB92
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_2_d(Sharp_SM83 *cpu);

/*
    Resets bit 2 of E register to 0.
    ref.assem: RES 2,E
    Info:
        - opcode: 0xCB93
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_2_e(Sharp_SM83 *cpu);

/*
    Resets bit 2 of H register to 0.
    ref.assem: RES 2,H
    Info:
        - opcode: 0xCB94
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_2_h(Sharp_SM83 *cpu);

/*
    Resets bit 2 of L register to 0.
    ref.assem: RES 2,L
    Info:
        - opcode: 0xCB95
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_2_l(Sharp_SM83 *cpu);

/*
    Resets bit 2 of the value at the memory address pointed by HL to 0.
    ref.assem: RES 2,(HL)
    Info:
        - opcode: 0xCB96
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_res_2_hl(Sharp_SM83 *cpu);

/*
    Resets bit 2 of A register to 0.
    ref.assem: RES 2,A
    Info:
        - opcode: 0xCB97
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_2_a(Sharp_SM83 *cpu);

/*
    Resets bit 3 of B register to 0.
    ref.assem: RES 3,B
    Info:
        - opcode: 0xCB98
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_3_b(Sharp_SM83 *cpu);

/*
    Resets bit 3 of C register to 0.
    ref.assem: RES 3,C
    Info:
        - opcode: 0xCB99
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_3_c(Sharp_SM83 *cpu);

/*
    Resets bit 3 of D register to 0.
    ref.assem: RES 3,D
    Info:
        - opcode: 0xCB9A
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_3_d(Sharp_SM83 *cpu);

/*
    Resets bit 3 of E register to 0.
    ref.assem: RES 3,E
    Info:
        - opcode: 0xCB9B
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_3_e(Sharp_SM83 *cpu);

/*
    Resets bit 3 of H register to 0.
    ref.assem: RES 3,H
    Info:
        - opcode: 0xCB9C
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_3_h(Sharp_SM83 *cpu);

/*
    Resets bit 3 of L register to 0.
    ref.assem: RES 3,L
    Info:
        - opcode: 0xCB9D
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_3_l(Sharp_SM83 *cpu);

/*
    Resets bit 3 of the value at the memory address pointed by HL to 0.
    ref.assem: RES 3,(HL)
    Info:
        - opcode: 0xCB9E
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_res_3_hl(Sharp_SM83 *cpu);

/*
    Resets bit 3 of A register to 0.
    ref.assem: RES 3,A
    Info:
        - opcode: 0xCB9F
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_3_a(Sharp_SM83 *cpu);

/*
    Resets bit 4 of B register to 0.
    ref.assem: RES 4,B
    Info:
        - opcode: 0xCBA0
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_4_b(Sharp_SM83 *cpu);

/*
    Resets bit 4 of C register to 0.
    ref.assem: RES 4,C
    Info:
        - opcode: 0xCBA1
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_4_c(Sharp_SM83 *cpu);

/*
    Resets bit 4 of D register to 0.
    ref.assem: RES 4,D
    Info:
        - opcode: 0xCBA2
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_4_d(Sharp_SM83 *cpu);

/*
    Resets bit 4 of E register to 0.
    ref.assem: RES 4,E
    Info:
        - opcode: 0xCBA3
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_4_e(Sharp_SM83 *cpu);

/*
    Resets bit 4 of H register to 0.
    ref.assem: RES 4,H
    Info:
        - opcode: 0xCBA4
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_4_h(Sharp_SM83 *cpu);

/*
    Resets bit 4 of L register to 0.
    ref.assem: RES 4,L
    Info:
        - opcode: 0xCBA5
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_4_l(Sharp_SM83 *cpu);

/*
    Resets bit 4 of the value at the memory address pointed by HL to 0.
    ref.assem: RES 4,(HL)
    Info:
        - opcode: 0xCBA6
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_res_4_hl(Sharp_SM83 *cpu);

/*
    Resets bit 4 of A register to 0.
    ref.assem: RES 4,A
    Info:
        - opcode: 0xCBA7
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_4_a(Sharp_SM83 *cpu);

/*
    Resets bit 5 of B register to 0.
    ref.assem: RES 5,B
    Info:
        - opcode: 0xCBA8
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_5_b(Sharp_SM83 *cpu);

/*
    Resets bit 5 of C register to 0.
    ref.assem: RES 5,C
    Info:
        - opcode: 0xCBA9
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_5_c(Sharp_SM83 *cpu);

/*
    Resets bit 5 of D register to 0.
    ref.assem: RES 5,D
    Info:
        - opcode: 0xCBAA
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_5_d(Sharp_SM83 *cpu);

/*
    Resets bit 5 of E register to 0.
    ref.assem: RES 5,E
    Info:
        - opcode: 0xCBAB
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_5_e(Sharp_SM83 *cpu);

/*
    Resets bit 5 of H register to 0.
    ref.assem: RES 5,H
    Info:
        - opcode: 0xCBAC
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_5_h(Sharp_SM83 *cpu);

/*
    Resets bit 5 of L register to 0.
    ref.assem: RES 5,L
    Info:
        - opcode: 0xCBAD
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_5_l(Sharp_SM83 *cpu);

/*
    Resets bit 5 of the value at the memory address pointed by HL to 0.
    ref.assem: RES 5,(HL)
    Info:
        - opcode: 0xCBAE
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_res_5_hl(Sharp_SM83 *cpu);

/*
    Resets bit 5 of A register to 0.
    ref.assem: RES 5,A
    Info:
        - opcode: 0xCBAF
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_5_a(Sharp_SM83 *cpu);

/*
    Resets bit 6 of B register to 0.
    ref.assem: RES 6,B
    Info:
        - opcode: 0xCBB0
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_6_b(Sharp_SM83 *cpu);

/*
    Resets bit 6 of C register to 0.
    ref.assem: RES 6,C
    Info:
        - opcode: 0xCBB1
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_6_c(Sharp_SM83 *cpu);

/*
    Resets bit 6 of D register to 0.
    ref.assem: RES 6,D
    Info:
        - opcode: 0xCBB2
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_6_d(Sharp_SM83 *cpu);

/*
    Resets bit 6 of E register to 0.
    ref.assem: RES 6,E
    Info:
        - opcode: 0xCBB3
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_6_e(Sharp_SM83 *cpu);

/*
    Resets bit 6 of H register to 0.
    ref.assem: RES 6,H
    Info:
        - opcode: 0xCBB4
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_6_h(Sharp_SM83 *cpu);

/*
    Resets bit 6 of L register to 0.
    ref.assem: RES 6,L
    Info:
        - opcode: 0xCBB5
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_6_l(Sharp_SM83 *cpu);

/*
    Resets bit 6 of the value at the memory address pointed by HL to 0.
    ref.assem: RES 6,(HL)
    Info:
        - opcode: 0xCBB6
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_res_6_hl(Sharp_SM83 *cpu);

/*
    Resets bit 6 of A register to 0.
    ref.assem: RES 6,A
    Info:
        - opcode: 0xCBB7
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_6_a(Sharp_SM83 *cpu);

/*
    Resets bit 7 of B register to 0.
    ref.assem: RES 7,B
    Info:
        - opcode: 0xCBB8
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_7_b(Sharp_SM83 *cpu);

/*
    Resets bit 7 of C register to 0.
    ref.assem: RES 7,C
    Info:
        - opcode: 0xCBB9
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_7_c(Sharp_SM83 *cpu);

/*
    Resets bit 7 of D register to 0.
    ref.assem: RES 7,D
    Info:
        - opcode: 0xCBBA
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_7_d(Sharp_SM83 *cpu);

/*
    Resets bit 7 of E register to 0.
    ref.assem: RES 7,E
    Info:
        - opcode: 0xCBBB
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_7_e(Sharp_SM83 *cpu);

/*
    Resets bit 7 of H register to 0.
    ref.assem: RES 7,H
    Info:
        - opcode: 0xCBBC
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_7_h(Sharp_SM83 *cpu);

/*
    Resets bit 7 of L register to 0.
    ref.assem: RES 7,L
    Info:
        - opcode: 0xCBBD
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_7_l(Sharp_SM83 *cpu);

/*
    Resets bit 7 of the value at the memory address pointed by HL to 0.
    ref.assem: RES 7,(HL)
    Info:
        - opcode: 0xCBBE
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_res_7_hl(Sharp_SM83 *cpu);

/*
    Resets bit 7 of A register to 0.
    ref.assem: RES 7,A
    Info:
        - opcode: 0xCBBF
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_res_7_a(Sharp_SM83 *cpu);

/*
   SET — Set bit
   Opcodes 0xCBC0 – 0xCBFF
   flags: - - - -
*/

/*
    Sets bit 0 of B register to 1.
    ref.assem: SET 0,B
    Info:
        - opcode: 0xCBC0
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_0_b(Sharp_SM83 *cpu);

/*
    Sets bit 0 of C register to 1.
    ref.assem: SET 0,C
    Info:
        - opcode: 0xCBC1
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_0_c(Sharp_SM83 *cpu);

/*
    Sets bit 0 of D register to 1.
    ref.assem: SET 0,D
    Info:
        - opcode: 0xCBC2
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_0_d(Sharp_SM83 *cpu);

/*
    Sets bit 0 of E register to 1.
    ref.assem: SET 0,E
    Info:
        - opcode: 0xCBC3
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_0_e(Sharp_SM83 *cpu);

/*
    Sets bit 0 of H register to 1.
    ref.assem: SET 0,H
    Info:
        - opcode: 0xCBC4
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_0_h(Sharp_SM83 *cpu);

/*
    Sets bit 0 of L register to 1.
    ref.assem: SET 0,L
    Info:
        - opcode: 0xCBC5
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_0_l(Sharp_SM83 *cpu);

/*
    Sets bit 0 of the value at the memory address pointed by HL to 1.
    ref.assem: SET 0,(HL)
    Info:
        - opcode: 0xCBC6
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_set_0_hl(Sharp_SM83 *cpu);

/*
    Sets bit 0 of A register to 1.
    ref.assem: SET 0,A
    Info:
        - opcode: 0xCBC7
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_0_a(Sharp_SM83 *cpu);

/*
    Sets bit 1 of B register to 1.
    ref.assem: SET 1,B
    Info:
        - opcode: 0xCBC8
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_1_b(Sharp_SM83 *cpu);

/*
    Sets bit 1 of C register to 1.
    ref.assem: SET 1,C
    Info:
        - opcode: 0xCBC9
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_1_c(Sharp_SM83 *cpu);

/*
    Sets bit 1 of D register to 1.
    ref.assem: SET 1,D
    Info:
        - opcode: 0xCBCA
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_1_d(Sharp_SM83 *cpu);

/*
    Sets bit 1 of E register to 1.
    ref.assem: SET 1,E
    Info:
        - opcode: 0xCBCB
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_1_e(Sharp_SM83 *cpu);

/*
    Sets bit 1 of H register to 1.
    ref.assem: SET 1,H
    Info:
        - opcode: 0xCBCC
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_1_h(Sharp_SM83 *cpu);

/*
    Sets bit 1 of L register to 1.
    ref.assem: SET 1,L
    Info:
        - opcode: 0xCBCD
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_1_l(Sharp_SM83 *cpu);

/*
    Sets bit 1 of the value at the memory address pointed by HL to 1.
    ref.assem: SET 1,(HL)
    Info:
        - opcode: 0xCBCE
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_set_1_hl(Sharp_SM83 *cpu);

/*
    Sets bit 1 of A register to 1.
    ref.assem: SET 1,A
    Info:
        - opcode: 0xCBCF
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_1_a(Sharp_SM83 *cpu);

/*
    Sets bit 2 of B register to 1.
    ref.assem: SET 2,B
    Info:
        - opcode: 0xCBD0
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_2_b(Sharp_SM83 *cpu);

/*
    Sets bit 2 of C register to 1.
    ref.assem: SET 2,C
    Info:
        - opcode: 0xCBD1
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_2_c(Sharp_SM83 *cpu);

/*
    Sets bit 2 of D register to 1.
    ref.assem: SET 2,D
    Info:
        - opcode: 0xCBD2
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_2_d(Sharp_SM83 *cpu);

/*
    Sets bit 2 of E register to 1.
    ref.assem: SET 2,E
    Info:
        - opcode: 0xCBD3
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_2_e(Sharp_SM83 *cpu);

/*
    Sets bit 2 of H register to 1.
    ref.assem: SET 2,H
    Info:
        - opcode: 0xCBD4
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_2_h(Sharp_SM83 *cpu);

/*
    Sets bit 2 of L register to 1.
    ref.assem: SET 2,L
    Info:
        - opcode: 0xCBD5
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_2_l(Sharp_SM83 *cpu);

/*
    Sets bit 2 of the value at the memory address pointed by HL to 1.
    ref.assem: SET 2,(HL)
    Info:
        - opcode: 0xCBD6
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_set_2_hl(Sharp_SM83 *cpu);

/*
    Sets bit 2 of A register to 1.
    ref.assem: SET 2,A
    Info:
        - opcode: 0xCBD7
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_2_a(Sharp_SM83 *cpu);

/*
    Sets bit 3 of B register to 1.
    ref.assem: SET 3,B
    Info:
        - opcode: 0xCBD8
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_3_b(Sharp_SM83 *cpu);

/*
    Sets bit 3 of C register to 1.
    ref.assem: SET 3,C
    Info:
        - opcode: 0xCBD9
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_3_c(Sharp_SM83 *cpu);

/*
    Sets bit 3 of D register to 1.
    ref.assem: SET 3,D
    Info:
        - opcode: 0xCBDA
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_3_d(Sharp_SM83 *cpu);

/*
    Sets bit 3 of E register to 1.
    ref.assem: SET 3,E
    Info:
        - opcode: 0xCBDB
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_3_e(Sharp_SM83 *cpu);

/*
    Sets bit 3 of H register to 1.
    ref.assem: SET 3,H
    Info:
        - opcode: 0xCBDC
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_3_h(Sharp_SM83 *cpu);

/*
    Sets bit 3 of L register to 1.
    ref.assem: SET 3,L
    Info:
        - opcode: 0xCBDD
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_3_l(Sharp_SM83 *cpu);

/*
    Sets bit 3 of the value at the memory address pointed by HL to 1.
    ref.assem: SET 3,(HL)
    Info:
        - opcode: 0xCBDE
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_set_3_hl(Sharp_SM83 *cpu);

/*
    Sets bit 3 of A register to 1.
    ref.assem: SET 3,A
    Info:
        - opcode: 0xCBDF
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_3_a(Sharp_SM83 *cpu);

/*
    Sets bit 4 of B register to 1.
    ref.assem: SET 4,B
    Info:
        - opcode: 0xCBE0
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_4_b(Sharp_SM83 *cpu);

/*
    Sets bit 4 of C register to 1.
    ref.assem: SET 4,C
    Info:
        - opcode: 0xCBE1
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_4_c(Sharp_SM83 *cpu);

/*
    Sets bit 4 of D register to 1.
    ref.assem: SET 4,D
    Info:
        - opcode: 0xCBE2
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_4_d(Sharp_SM83 *cpu);

/*
    Sets bit 4 of E register to 1.
    ref.assem: SET 4,E
    Info:
        - opcode: 0xCBE3
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_4_e(Sharp_SM83 *cpu);

/*
    Sets bit 4 of H register to 1.
    ref.assem: SET 4,H
    Info:
        - opcode: 0xCBE4
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_4_h(Sharp_SM83 *cpu);

/*
    Sets bit 4 of L register to 1.
    ref.assem: SET 4,L
    Info:
        - opcode: 0xCBE5
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_4_l(Sharp_SM83 *cpu);

/*
    Sets bit 4 of the value at the memory address pointed by HL to 1.
    ref.assem: SET 4,(HL)
    Info:
        - opcode: 0xCBE6
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_set_4_hl(Sharp_SM83 *cpu);

/*
    Sets bit 4 of A register to 1.
    ref.assem: SET 4,A
    Info:
        - opcode: 0xCBE7
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_4_a(Sharp_SM83 *cpu);

/*
    Sets bit 5 of B register to 1.
    ref.assem: SET 5,B
    Info:
        - opcode: 0xCBE8
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_5_b(Sharp_SM83 *cpu);

/*
    Sets bit 5 of C register to 1.
    ref.assem: SET 5,C
    Info:
        - opcode: 0xCBE9
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_5_c(Sharp_SM83 *cpu);

/*
    Sets bit 5 of D register to 1.
    ref.assem: SET 5,D
    Info:
        - opcode: 0xCBEA
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_5_d(Sharp_SM83 *cpu);

/*
    Sets bit 5 of E register to 1.
    ref.assem: SET 5,E
    Info:
        - opcode: 0xCBEB
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_5_e(Sharp_SM83 *cpu);

/*
    Sets bit 5 of H register to 1.
    ref.assem: SET 5,H
    Info:
        - opcode: 0xCBEC
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_5_h(Sharp_SM83 *cpu);

/*
    Sets bit 5 of L register to 1.
    ref.assem: SET 5,L
    Info:
        - opcode: 0xCBED
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_5_l(Sharp_SM83 *cpu);

/*
    Sets bit 5 of the value at the memory address pointed by HL to 1.
    ref.assem: SET 5,(HL)
    Info:
        - opcode: 0xCBEE
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_set_5_hl(Sharp_SM83 *cpu);

/*
    Sets bit 5 of A register to 1.
    ref.assem: SET 5,A
    Info:
        - opcode: 0xCBEF
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_5_a(Sharp_SM83 *cpu);

/*
    Sets bit 6 of B register to 1.
    ref.assem: SET 6,B
    Info:
        - opcode: 0xCBF0
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_6_b(Sharp_SM83 *cpu);

/*
    Sets bit 6 of C register to 1.
    ref.assem: SET 6,C
    Info:
        - opcode: 0xCBF1
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_6_c(Sharp_SM83 *cpu);

/*
    Sets bit 6 of D register to 1.
    ref.assem: SET 6,D
    Info:
        - opcode: 0xCBF2
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_6_d(Sharp_SM83 *cpu);

/*
    Sets bit 6 of E register to 1.
    ref.assem: SET 6,E
    Info:
        - opcode: 0xCBF3
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_6_e(Sharp_SM83 *cpu);

/*
    Sets bit 6 of H register to 1.
    ref.assem: SET 6,H
    Info:
        - opcode: 0xCBF4
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_6_h(Sharp_SM83 *cpu);

/*
    Sets bit 6 of L register to 1.
    ref.assem: SET 6,L
    Info:
        - opcode: 0xCBF5
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_6_l(Sharp_SM83 *cpu);

/*
    Sets bit 6 of the value at the memory address pointed by HL to 1.
    ref.assem: SET 6,(HL)
    Info:
        - opcode: 0xCBF6
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_set_6_hl(Sharp_SM83 *cpu);

/*
    Sets bit 6 of A register to 1.
    ref.assem: SET 6,A
    Info:
        - opcode: 0xCBF7
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_6_a(Sharp_SM83 *cpu);

/*
    Sets bit 7 of B register to 1.
    ref.assem: SET 7,B
    Info:
        - opcode: 0xCBF8
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_7_b(Sharp_SM83 *cpu);

/*
    Sets bit 7 of C register to 1.
    ref.assem: SET 7,C
    Info:
        - opcode: 0xCBF9
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_7_c(Sharp_SM83 *cpu);

/*
    Sets bit 7 of D register to 1.
    ref.assem: SET 7,D
    Info:
        - opcode: 0xCBFA
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_7_d(Sharp_SM83 *cpu);

/*
    Sets bit 7 of E register to 1.
    ref.assem: SET 7,E
    Info:
        - opcode: 0xCBFB
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_7_e(Sharp_SM83 *cpu);

/*
    Sets bit 7 of H register to 1.
    ref.assem: SET 7,H
    Info:
        - opcode: 0xCBFC
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_7_h(Sharp_SM83 *cpu);

/*
    Sets bit 7 of L register to 1.
    ref.assem: SET 7,L
    Info:
        - opcode: 0xCBFD
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_7_l(Sharp_SM83 *cpu);

/*
    Sets bit 7 of the value at the memory address pointed by HL to 1.
    ref.assem: SET 7,(HL)
    Info:
        - opcode: 0xCBFE
        - 2 byte length
        - 16 cycles to be completed
        - no flags updated
*/
void cb_set_7_hl(Sharp_SM83 *cpu);

/*
    Sets bit 7 of A register to 1.
    ref.assem: SET 7,A
    Info:
        - opcode: 0xCBFF
        - 2 byte length
        - 8 cycles to be completed
        - no flags updated
*/
void cb_set_7_a(Sharp_SM83 *cpu);

#endif 

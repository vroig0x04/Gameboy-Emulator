
#include "alu_SM83.h"
#include "flags_SM83.h"
#include "sharp_SM83.h"
#include "mmu.h"

/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/

/*
 * This file implements all instructions defined in the 
 * alu_SM83.h file 
*/

void alu_add(Sharp_SM83 *cpu, uint8_t value) {
    uint16_t sum = cpu->regs.A + value;

    cpu_set_flag(cpu, FLAG_Z, (uint8_t)sum == 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, ((cpu->regs.A & 0x0f) + (value & 0x0f) > 0x0f));
    cpu_set_flag(cpu, FLAG_C, (sum > 0xff));

    cpu->regs.A = (uint8_t)sum;
}

void alu_sub(Sharp_SM83 *cpu, uint8_t value, int is_cp) {
    uint16_t sub = cpu->regs.A - value;

    cpu_set_flag(cpu, FLAG_Z, (uint8_t)sub == 0);
    cpu_set_flag(cpu, FLAG_N, 1);
    cpu_set_flag(cpu, FLAG_H, (cpu->regs.A & 0x0f) < (value & 0x0f));
    cpu_set_flag(cpu, FLAG_C, cpu->regs.A < value);

    if (!is_cp) cpu->regs.A = (uint8_t)sub;
}

void alu_adc(Sharp_SM83 *cpu, uint8_t value) {
    uint8_t flag_c = get_flag(cpu, FLAG_C);
    uint16_t sum = cpu->regs.A + value + flag_c;

    cpu_set_flag(cpu, FLAG_Z, (uint8_t)sum == 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, ((cpu->regs.A & 0x0f) + (value & 0x0f) + flag_c) > 0x0f);
    cpu_set_flag(cpu, FLAG_C, (sum > 0xff));

    cpu->regs.A = (uint8_t)sum;
}

void alu_sbc(Sharp_SM83 *cpu, uint8_t value) {
    uint8_t flag_c = get_flag(cpu, FLAG_C);
    uint16_t sub = cpu->regs.A - value - flag_c;

    cpu_set_flag(cpu, FLAG_Z, (uint8_t)sub == 0);
    cpu_set_flag(cpu, FLAG_N, 1);
    cpu_set_flag(cpu, FLAG_H, ((cpu->regs.A & 0x0f) < ((value & 0x0f) + flag_c)));
    cpu_set_flag(cpu, FLAG_C, cpu->regs.A < (value + flag_c));

    cpu->regs.A = (uint8_t)sub;
}

void alu_and(Sharp_SM83 *cpu, uint8_t value) {
    uint8_t and = cpu->regs.A & value;

    cpu_set_flag(cpu, FLAG_Z, and == 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 1);
    cpu_set_flag(cpu, FLAG_C, 0);

    cpu->regs.A = and;
}

void alu_xor(Sharp_SM83 *cpu, uint8_t value) {
    uint8_t xor = cpu->regs.A ^ value;

    cpu_set_flag(cpu, FLAG_Z, xor == 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, 0);

    cpu->regs.A = xor;
}

void alu_or(Sharp_SM83 *cpu, uint8_t value) {
    uint8_t or = cpu->regs.A | value;

    cpu_set_flag(cpu, FLAG_Z, or == 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, 0);

    cpu->regs.A = or;
}

void alu_inc(Sharp_SM83 *cpu, void *value, int is_16bit) {
    if (is_16bit == 0) { /* 8 bit */
        uint8_t *d8_ptr = (uint8_t *)value;
        uint8_t ori = *d8_ptr;
        uint8_t nv = ori + 1;

        cpu_set_flag(cpu, FLAG_Z, (nv == 0));
        cpu_set_flag(cpu, FLAG_N, 0);
        cpu_set_flag(cpu, FLAG_H, ((ori & 0x0f) == 0x0f));

        *d8_ptr= nv;
    }
    else { /* 16 bit */
        uint16_t *d16_ptr = (uint16_t *)value;
        (*d16_ptr)++;  
    }
}

void alu_dec(Sharp_SM83 *cpu, void *value, int is_16bit) {
    if (is_16bit == 0) { /* 8 bit */
        uint8_t *d8_ptr = (uint8_t *)value;
        uint8_t ori = *d8_ptr;
        uint8_t nv = ori - 1;

        cpu_set_flag(cpu, FLAG_Z, (nv == 0));
        cpu_set_flag(cpu, FLAG_N, 1);
        cpu_set_flag(cpu, FLAG_H, ((ori & 0x0f) == 0x00));

        *d8_ptr = nv;
    }
    else { /* 16 bit */
        uint16_t *d16_ptr = (uint16_t *)value;
        (*d16_ptr)--;  
    }
}

/* Ordinary 256 SM83 instructions */

void nop(Sharp_SM83 *cpu) {
    /* T-cycles is incremented in UC */
}

void ld_bc_d16(Sharp_SM83 *cpu, uint16_t d16) {
    cpu->regs.BC = d16;
}

void ld_bc_a(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint16_t addr = cpu->regs.BC;
    mmu_write8(mmu, addr, cpu->regs.A);
    
}

void inc_bc(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.BC, 1);
}

void inc_b(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.B, 0);
}

void dec_b(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.B, 0);
}

void ld_b_d8(Sharp_SM83 *cpu, uint8_t d8) {
    cpu->regs.B = d8;
}

void rlca(Sharp_SM83 *cpu) {
    uint8_t prev = cpu->regs.A;
    cpu->regs.A <<= 1;

    cpu_set_flag(cpu, FLAG_Z, 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, ((prev & 0x80) != 0));
}

void ld_a16_sp(Sharp_MMU *mmu, uint16_t a16, Sharp_SM83 *cpu) {
    mmu_write16(mmu, a16, cpu->regs.SP);
}

void add_hl_bc(Sharp_SM83 *cpu) {
    uint32_t sum = cpu->regs.BC + cpu->regs.HL;

    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, ((cpu->regs.BC & 0x0fff) + (cpu->regs.HL & 0x0fff) > 0x0fff));
    cpu_set_flag(cpu, FLAG_C, (sum > 0xffff));

    cpu->regs.HL = (uint16_t)sum;
}

void ld_a_bc(Sharp_MMU *mmu, Sharp_SM83 *cpu) {
    uint16_t addr = cpu->regs.BC;
    uint8_t data = mmu_read(mmu, addr);
    cpu->regs.A = data;
}

void dec_bc(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.BC, 1);
}

void inc_c(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.C, 0);
}

void dec_c(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.C, 0);
}

void ld_c_d8(Sharp_SM83 *cpu, uint8_t d8) {
    cpu->regs.C = d8;
}

void rrca(Sharp_SM83 *cpu) {
    uint8_t prev = cpu->regs.A;
    uint8_t bit0 = prev & 0x01;
    cpu->regs.A = (prev >> 1) | (bit0 << 7);   

    cpu_set_flag(cpu, FLAG_Z, 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, bit0 != 0);     
}

void stop() {
    /* TODO 
       STOP may cause the DMG to behave different. 
       Must not be confused with HALT */
}

void ld_de_d16(Sharp_SM83 *cpu, uint16_t d16) {
    cpu->regs.DE = d16;
}

void ld_de_a(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint16_t addr = cpu->regs.DE;
    mmu_write8(mmu, addr, cpu->regs.A);
}

void inc_de(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.DE, 1);
}

void inc_d(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.D, 0);
}

void dec_d(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.D, 0);

}

void ld_d_d8(Sharp_SM83 *cpu, uint8_t d8) {
    cpu->regs.D = d8;
}

void rla(Sharp_SM83 *cpu) {
    uint8_t prev = cpu->regs.A;
    uint8_t old_c = get_flag(cpu, FLAG_C);
    cpu->regs.A <<= 1;

    if (old_c) cpu->regs.A |= 0x01;

    cpu_set_flag(cpu, FLAG_Z, 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, ((prev & 0x80) == 0x80));   
}

void jr_r8(Sharp_SM83 *cpu, int8_t r8) {
    /* DONE IN CPU */
}

void add_hl_de(Sharp_SM83 *cpu) {
    uint32_t sum = cpu->regs.DE + cpu->regs.HL;

    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, ((cpu->regs.DE & 0x0fff) + (cpu->regs.HL & 0x0fff) > 0x0fff));
    cpu_set_flag(cpu, FLAG_C, (sum > 0xffff));

    cpu->regs.HL = (uint16_t)sum;
}

void ld_a_de(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint16_t addr = cpu->regs.DE;
    cpu->regs.A = mmu_read(mmu, addr);
}

void dec_de(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.DE, 1);
}

void inc_e(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.E, 0);
}

void dec_e(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.E, 0);
}

void ld_e_d8(Sharp_SM83 *cpu, uint8_t d8) {
    cpu->regs.E = d8;
}

void rra(Sharp_SM83 *cpu) {
    uint8_t prev = cpu->regs.A;
    uint8_t old_c = get_flag(cpu, FLAG_C);
    cpu->regs.A >>= 1;
    if (old_c) cpu->regs.A |= 0x80;

    cpu_set_flag(cpu, FLAG_Z, 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, (prev & 0x01) != 0);  
}

void jr_nz_r8(Sharp_SM83 *cpu, int8_t r8) {
    if (get_flag(cpu, FLAG_Z) == 0) {
        cpu->regs.PC += r8;
    }
}

void ld_hl_d16(Sharp_SM83 *cpu, uint16_t d16) {
    cpu->regs.HL = d16;
}

void ld_hli_a(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    mmu_write8(mmu, cpu->regs.HL, cpu->regs.A);
    cpu->regs.HL += 1;
}

void inc_hl(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.HL, 1);
}

void inc_h(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.H, 0);
}

void dec_h(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.H, 0);
}

void ld_h_d8(Sharp_SM83 *cpu, uint8_t d8) {
    cpu->regs.H = d8;
}

void daa(Sharp_SM83 *cpu) {
    uint8_t corr = 0;
    uint8_t carry = 0;

    if (get_flag(cpu, FLAG_N) == 0) { 
        if (get_flag(cpu, FLAG_H) || (cpu->regs.A & 0x0f) > 0x09) {
            corr |= 0x06;
        }
        if (get_flag(cpu, FLAG_C) || cpu->regs.A > 0x99) {
            corr |= 0x60;
            carry = 1;
        }
        cpu->regs.A += corr;
    } else { 
        if (get_flag(cpu, FLAG_H)) {
            corr |= 0x06;         
        }
        if (get_flag(cpu, FLAG_C)) {
            corr |= 0x60;
            carry = 1;
        }
        cpu->regs.A -= corr;
    }

    cpu_set_flag(cpu, FLAG_Z, (cpu->regs.A == 0));
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, carry);
}

void jr_z_r8(Sharp_SM83 *cpu, int8_t r8) {
    if (get_flag(cpu, FLAG_Z) == 1) {
        cpu->regs.PC += r8;
    }
}

void add_hl_hl(Sharp_SM83 *cpu) {
    uint32_t sum = cpu->regs.HL + cpu->regs.HL;

    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, ((cpu->regs.HL & 0x0fff) + (cpu->regs.HL & 0x0fff) > 0x0fff));
    cpu_set_flag(cpu, FLAG_C, (sum > 0xffff));

    cpu->regs.HL = (uint16_t)sum;
}

void ld_a_hli(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint16_t addr = cpu->regs.HL;
    cpu->regs.A = mmu_read(mmu, addr);
    cpu->regs.HL++;
}

void dec_hl(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.HL, 1);
}

void inc_l(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.L, 0);
}

void dec_l(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.L, 0);
}

void ld_l_d8(Sharp_SM83 *cpu, uint8_t d8) {
    cpu->regs.L = d8;
}

void cpl(Sharp_SM83 *cpu) {
    cpu->regs.A = ~cpu->regs.A;

    cpu_set_flag(cpu, FLAG_N, 1);
    cpu_set_flag(cpu, FLAG_H, 1);
}

void jr_nc_r8(Sharp_SM83 *cpu, int8_t r8) {
    if (get_flag(cpu, FLAG_C) == 0) {
        cpu->regs.PC += r8;
    }
}

void ld_sp_d16(Sharp_SM83 *cpu, uint16_t d16) {
    cpu->regs.SP = d16;
}

void ld_hld_a(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    mmu_write8(mmu, cpu->regs.HL, cpu->regs.A);
    cpu->regs.HL -= 1;
}

void inc_sp(Sharp_SM83 *cpu) {
    cpu->regs.SP++;
}

void inc_hl_ind(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint8_t data = mmu_read(mmu, cpu->regs.HL);
    uint8_t n_data = data + 1;
    mmu_write8(mmu, cpu->regs.HL, n_data);

    cpu_set_flag(cpu, FLAG_Z, n_data == 0); 
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, (data & 0x0f) == 0x0f);

}

void dec_hl_ind(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint8_t data = mmu_read(mmu, cpu->regs.HL);
    uint8_t n_data = data - 1;
    mmu_write8(mmu, cpu->regs.HL, n_data);

    cpu_set_flag(cpu, FLAG_Z, n_data == 0);         
    cpu_set_flag(cpu, FLAG_N, 1);
    cpu_set_flag(cpu, FLAG_H, (data & 0x0f) == 0x00); 
}

void ld_hl_d8(Sharp_SM83 *cpu, Sharp_MMU *mmu, uint8_t d8) {
    mmu_write8(mmu, cpu->regs.HL, d8);
}

void scf(Sharp_SM83 *cpu) {
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, 1);
}

void jr_c_r8(Sharp_SM83 *cpu, int8_t r8) {
    if (get_flag(cpu, FLAG_C) == 1) {
        cpu->regs.PC += r8;
    }
}

void add_hl_sp(Sharp_SM83 *cpu) {
    uint32_t sum = cpu->regs.HL + cpu->regs.SP;

    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, ((cpu->regs.HL & 0x0fff) + (cpu->regs.SP & 0x0fff) > 0x0fff));
    cpu_set_flag(cpu, FLAG_C, (sum > 0xffff));

    cpu->regs.HL = (uint16_t)sum;
}

void ld_a_hld(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    cpu->regs.A = mmu_read(mmu, cpu->regs.HL);
    cpu->regs.HL--;
}

void dec_sp(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.SP, 1);
}

void inc_a(Sharp_SM83 *cpu) {
    alu_inc(cpu, &cpu->regs.A, 0);

}

void dec_a(Sharp_SM83 *cpu) {
    alu_dec(cpu, &cpu->regs.A, 0);
}

void ld_a_d8(Sharp_SM83 *cpu, uint8_t d8) {
    cpu->regs.A = d8;
}

void ccf(Sharp_SM83 *cpu) {
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);
    cpu_set_flag(cpu, FLAG_C, (get_flag(cpu, FLAG_C) == 1 ? 0 : 1));
}

void ld_b_b(Sharp_SM83 *cpu) {
    /* NOP, must increment cycles and PC though */
}

void ld_b_c(Sharp_SM83 *cpu) {
    cpu->regs.B = cpu->regs.C;
}

void ld_b_d(Sharp_SM83 *cpu) {
    cpu->regs.B = cpu->regs.D;
}

void ld_b_e(Sharp_SM83 *cpu) {
    cpu->regs.B = cpu->regs.E;
}

void ld_b_h(Sharp_SM83 *cpu) {
    cpu->regs.B = cpu->regs.H;
}

void ld_b_l(Sharp_SM83 *cpu) {
    cpu->regs.B = cpu->regs.L;
}

void ld_b_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    cpu->regs.B = mmu_read(mmu, cpu->regs.HL);
}

void ld_b_a(Sharp_SM83 *cpu) {
    cpu->regs.B = cpu->regs.A;
}

void ld_c_b(Sharp_SM83 *cpu) {
    cpu->regs.C = cpu->regs.B;
}

void ld_c_c(Sharp_SM83 *cpu) {
    /* NOP, must increment cycles and PC though*/
}

void ld_c_d(Sharp_SM83 *cpu) {
    cpu->regs.C = cpu->regs.D;
}

void ld_c_e(Sharp_SM83 *cpu) {
    cpu->regs.C = cpu->regs.E;
}

void ld_c_h(Sharp_SM83 *cpu) {
    cpu->regs.C = cpu->regs.H;
}

void ld_c_l(Sharp_SM83 *cpu) {
    cpu->regs.C = cpu->regs.L;
}

void ld_c_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    cpu->regs.C = mmu_read(mmu, cpu->regs.HL);
}

void ld_c_a(Sharp_SM83 *cpu) {
    cpu->regs.C = cpu->regs.A;
}

void ld_d_b(Sharp_SM83 *cpu) {
    cpu->regs.D = cpu->regs.B;
}

void ld_d_c(Sharp_SM83 *cpu) {  
    cpu->regs.D = cpu->regs.C;
}

void ld_d_d(Sharp_SM83 *cpu) {  
    /* NOP, must increment cycles and PC though*/
}

void ld_d_e(Sharp_SM83 *cpu) {
    cpu->regs.D = cpu->regs.E;
}

void ld_d_h(Sharp_SM83 *cpu) {
    cpu->regs.D = cpu->regs.H;
}

void ld_d_l(Sharp_SM83 *cpu) {
    cpu->regs.D = cpu->regs.L;
}

void ld_d_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    cpu->regs.D = mmu_read(mmu, cpu->regs.HL);
}

void ld_d_a(Sharp_SM83 *cpu) {
    cpu->regs.D = cpu->regs.A;
}

void ld_e_b(Sharp_SM83 *cpu) {
    cpu->regs.E = cpu->regs.B;
}

void ld_e_c(Sharp_SM83 *cpu) {  
    cpu->regs.E = cpu->regs.C;
}

void ld_e_d(Sharp_SM83 *cpu) {
    cpu->regs.E = cpu->regs.D;
}

void ld_e_e(Sharp_SM83 *cpu) {  
    /* NOP, must increment cycles and PC though*/
}

void ld_e_h(Sharp_SM83 *cpu) {
    cpu->regs.E = cpu->regs.H;
}

void ld_e_l(Sharp_SM83 *cpu) {
    cpu->regs.E = cpu->regs.L;
}

void ld_e_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    cpu->regs.E = mmu_read(mmu, cpu->regs.HL);
}

void ld_e_a(Sharp_SM83 *cpu) {
    cpu->regs.E = cpu->regs.A;
}

void ld_h_b(Sharp_SM83 *cpu) {
    cpu->regs.H = cpu->regs.B;
}

void ld_h_c(Sharp_SM83 *cpu) {
    cpu->regs.H = cpu->regs.C;
}

void ld_h_d(Sharp_SM83 *cpu) {
    cpu->regs.H = cpu->regs.D;
}

void ld_h_e(Sharp_SM83 *cpu) {
    cpu->regs.H = cpu->regs.E;
}

void ld_h_h(Sharp_SM83 *cpu) {
    /* NOP, must increment cycles and PC*/
}

void ld_h_l(Sharp_SM83 *cpu) {
    cpu->regs.H = cpu->regs.L;
}

void ld_h_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    cpu->regs.H = mmu_read(mmu, cpu->regs.HL);
}

void ld_h_a(Sharp_SM83 *cpu) {  
    cpu->regs.H = cpu->regs.A;
}

void ld_l_b(Sharp_SM83 *cpu) {
    cpu->regs.L = cpu->regs.B;
}

void ld_l_c(Sharp_SM83 *cpu) {
    cpu->regs.L = cpu->regs.C;
}

void ld_l_d(Sharp_SM83 *cpu) {
    cpu->regs.L = cpu->regs.D;
}

void ld_l_e(Sharp_SM83 *cpu) {
    cpu->regs.L = cpu->regs.E;
}

void ld_l_h(Sharp_SM83 *cpu) {
    cpu->regs.L = cpu->regs.H;
}

void ld_l_l(Sharp_SM83 *cpu) {
    /* NOP, must increment cycles and PC though*/
}

void ld_l_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    cpu->regs.L = mmu_read(mmu, cpu->regs.HL);
}   

void ld_l_a(Sharp_SM83 *cpu) {
    cpu->regs.L = cpu->regs.A;
}

void ld_hl_b(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    mmu_write8(mmu, cpu->regs.HL, cpu->regs.B);
}

void ld_hl_c(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    mmu_write8(mmu, cpu->regs.HL, cpu->regs.C);
}

void ld_hl_d(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    mmu_write8(mmu, cpu->regs.HL, cpu->regs.D);
}

void ld_hl_e(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    mmu_write8(mmu, cpu->regs.HL, cpu->regs.E);
}

void ld_hl_h(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    mmu_write8(mmu, cpu->regs.HL, cpu->regs.H);
}

void ld_hl_l(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    mmu_write8(mmu, cpu->regs.HL, cpu->regs.L);
}

void halt(Sharp_SM83 *cpu) {
    /* HALT the CPU, ISR is required */
}

void ld_hl_a(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    mmu_write8(mmu, cpu->regs.HL, cpu->regs.A);
}

void ld_a_b(Sharp_SM83 *cpu) {
    cpu->regs.A = cpu->regs.B;
}

void ld_a_c(Sharp_SM83 *cpu) {
    cpu->regs.A = cpu->regs.C;
}

void ld_a_d(Sharp_SM83 *cpu) {
    cpu->regs.A = cpu->regs.D;
}

void ld_a_e(Sharp_SM83 *cpu) {
    cpu->regs.A = cpu->regs.E;
}

void ld_a_h(Sharp_SM83 *cpu) {
    cpu->regs.A = cpu->regs.H;
}

void ld_a_l(Sharp_SM83 *cpu) {
    cpu->regs.A = cpu->regs.L;
}

void ld_a_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    cpu->regs.A = mmu_read(mmu, cpu->regs.HL);
}

void ld_a_a(Sharp_SM83 *cpu) {
    /* NOP, must increment cycles and PC though*/
}

void add_a_b(Sharp_SM83 *cpu) {
    alu_add(cpu, cpu->regs.B);
}

void add_a_c(Sharp_SM83 *cpu) {
    alu_add(cpu, cpu->regs.C);
}

void add_a_d(Sharp_SM83 *cpu) {
    alu_add(cpu, cpu->regs.D);
}

void add_a_e(Sharp_SM83 *cpu) {
    alu_add(cpu, cpu->regs.E);
}

void add_a_h(Sharp_SM83 *cpu) {
    alu_add(cpu, cpu->regs.H);
}

void add_a_l(Sharp_SM83 *cpu) {
    alu_add(cpu, cpu->regs.L);
}

void add_a_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    alu_add(cpu, mmu_read(mmu, cpu->regs.HL));
}

void add_a_a(Sharp_SM83 *cpu) {
    alu_add(cpu, cpu->regs.A);
}

void adc_a_b(Sharp_SM83 *cpu) {
    alu_adc(cpu, cpu->regs.B);
}

void adc_a_c(Sharp_SM83 *cpu) {
    alu_adc(cpu, cpu->regs.C);
}

void adc_a_d(Sharp_SM83 *cpu) {
    alu_adc(cpu, cpu->regs.D);
}

void adc_a_e(Sharp_SM83 *cpu) {
    alu_adc(cpu, cpu->regs.E);
}

void adc_a_h(Sharp_SM83 *cpu) {
    alu_adc(cpu, cpu->regs.H);
}

void adc_a_l(Sharp_SM83 *cpu) {
    alu_adc(cpu, cpu->regs.L);
}

void adc_a_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    alu_adc(cpu, mmu_read(mmu, cpu->regs.HL));
}

void adc_a_a(Sharp_SM83 *cpu) {
    alu_adc(cpu, cpu->regs.A);
}

void sub_b(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.B, 0);
}

void sub_c(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.C, 0);
}

void sub_d(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.D, 0);
}

void sub_e(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.E, 0);
}

void sub_h(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.H, 0);
}

void sub_l(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.L, 0);
}

void sub_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    alu_sub(cpu, mmu_read(mmu, cpu->regs.HL), 0);
}

void sub_a(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.A, 0);
}

void sbc_a_b(Sharp_SM83 *cpu) {
    alu_sbc(cpu, cpu->regs.B);
}

void sbc_a_c(Sharp_SM83 *cpu) {
    alu_sbc(cpu, cpu->regs.C);
}

void sbc_a_d(Sharp_SM83 *cpu) {
    alu_sbc(cpu, cpu->regs.D);
}

void sbc_a_e(Sharp_SM83 *cpu) {
    alu_sbc(cpu, cpu->regs.E);
}

void sbc_a_h(Sharp_SM83 *cpu) {
    alu_sbc(cpu, cpu->regs.H);
}

void sbc_a_l(Sharp_SM83 *cpu) {
    alu_sbc(cpu, cpu->regs.L);
}

void sbc_a_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    alu_sbc(cpu, mmu_read(mmu, cpu->regs.HL));
}

void sbc_a_a(Sharp_SM83 *cpu) {
    alu_sbc(cpu, cpu->regs.A);
}

void and_b(Sharp_SM83 *cpu) {
    alu_and(cpu, cpu->regs.B);
}

void and_c(Sharp_SM83 *cpu) {
    alu_and(cpu, cpu->regs.C);
}

void and_d(Sharp_SM83 *cpu) {
    alu_and(cpu, cpu->regs.D);
}

void and_e(Sharp_SM83 *cpu) {
    alu_and(cpu, cpu->regs.E);
}

void and_h(Sharp_SM83 *cpu) {
    alu_and(cpu, cpu->regs.H);
}

void and_l(Sharp_SM83 *cpu) {
    alu_and(cpu, cpu->regs.L);
}

void and_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    alu_and(cpu, mmu_read(mmu, cpu->regs.HL));
}

void and_a(Sharp_SM83 *cpu) {
    alu_and(cpu, cpu->regs.A);
}

void xor_b(Sharp_SM83 *cpu) {
    alu_xor(cpu, cpu->regs.B);
}

void xor_c(Sharp_SM83 *cpu) {
    alu_xor(cpu, cpu->regs.C);
}

void xor_d(Sharp_SM83 *cpu) {
    alu_xor(cpu, cpu->regs.D);
}

void xor_e(Sharp_SM83 *cpu) {
    alu_xor(cpu, cpu->regs.E);
}

void xor_h(Sharp_SM83 *cpu) {
    alu_xor(cpu, cpu->regs.H);
}

void xor_l(Sharp_SM83 *cpu) {
    alu_xor(cpu, cpu->regs.L);
}

void xor_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    alu_xor(cpu, mmu_read(mmu, cpu->regs.HL));
}

void xor_a(Sharp_SM83 *cpu) {
    alu_xor(cpu, cpu->regs.A);
}

void or_b(Sharp_SM83 *cpu) {
    alu_or(cpu, cpu->regs.B);
}

void or_c(Sharp_SM83 *cpu) {
    alu_or(cpu, cpu->regs.C);
}

void or_d(Sharp_SM83 *cpu) {
    alu_or(cpu, cpu->regs.D);
}

void or_e(Sharp_SM83 *cpu) {
    alu_or(cpu, cpu->regs.E);
}

void or_h(Sharp_SM83 *cpu) {
    alu_or(cpu, cpu->regs.H);
}

void or_l(Sharp_SM83 *cpu) {
    alu_or(cpu, cpu->regs.L);
}

void or_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    alu_or(cpu, mmu_read(mmu, cpu->regs.HL));
}

void or_a(Sharp_SM83 *cpu) {
    alu_or(cpu, cpu->regs.A);
}

void cp_b(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.B, 1);
}

void cp_c(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.C, 1);
}

void cp_d(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.D, 1);
}

void cp_e(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.E, 1);
}

void cp_h(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.H, 1);
}

void cp_l(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.L, 1);
}

void cp_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    alu_sub(cpu, mmu_read(mmu, cpu->regs.HL), 1);
}

void cp_a(Sharp_SM83 *cpu) {
    alu_sub(cpu, cpu->regs.A, 1);
}

void ret_nz(Sharp_SM83 *cpu) {

}

void pop_bc(Sharp_SM83 *cpu) {

}

void jp_nz_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void jp_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void call_nz_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void push_bc(Sharp_SM83 *cpu) {

}

void add_a_d8(Sharp_SM83 *cpu, uint8_t d8) {
    alu_add(cpu, d8);
}

void rst_00h(Sharp_SM83 *cpu) {

}

void ret_z(Sharp_SM83 *cpu) {

}

void ret(Sharp_SM83 *cpu) {

}

void jp_z_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void prefix_cb(Sharp_SM83 *cpu) {

}

void call_z_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void call_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void adc_a_d8(Sharp_SM83 *cpu, uint8_t d8) {
    alu_adc(cpu, d8);
}

void rst_08h(Sharp_SM83 *cpu) {

}

void ret_nc(Sharp_SM83 *cpu) {

}

void pop_de(Sharp_SM83 *cpu) {

}

void jp_nc_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void call_nc_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void push_de(Sharp_SM83 *cpu) {

}

void sub_d8(Sharp_SM83 *cpu, uint8_t d8) {
    alu_sub(cpu, d8, 0);
}

void rst_10h(Sharp_SM83 *cpu) {

}

void ret_c(Sharp_SM83 *cpu) {

}

void reti(Sharp_SM83 *cpu) {

}

void jp_c_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void call_c_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void sbc_a_d8(Sharp_SM83 *cpu, uint8_t d8) {
    alu_sbc(cpu, d8);
}

void rst_18h(Sharp_SM83 *cpu) {

}

void ldh_a8_a(Sharp_SM83 *cpu, uint8_t a8) {

}

void pop_hl(Sharp_SM83 *cpu) {

}

void ld_c_ind_a(Sharp_SM83 *cpu) {

}

void push_hl(Sharp_SM83 *cpu) {

}

void and_d8(Sharp_SM83 *cpu, uint8_t d8) {
    alu_and(cpu, d8);
}

void rst_20h(Sharp_SM83 *cpu) {

}

void add_sp_r8(Sharp_SM83 *cpu, int8_t r8) {
    uint16_t sp = cpu->regs.SP;
    cpu->regs.SP += r8;

    cpu_set_flag(cpu, FLAG_Z, 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, ((sp & 0x0f) + ((uint8_t)r8 & 0x0f) > 0x0f));
    cpu_set_flag(cpu, FLAG_C,((sp & 0xff) + (uint8_t)r8) > 0xff);
}

void jp_hl(Sharp_SM83 *cpu) {

}

void ld_a16_a(Sharp_SM83 *cpu, uint16_t a16) {

}

void xor_d8(Sharp_SM83 *cpu, uint8_t d8) {
    alu_xor(cpu, d8);
}

void rst_28h(Sharp_SM83 *cpu) {

}

void ldh_a_a8(Sharp_SM83 *cpu, uint8_t a8) {

}

void pop_af(Sharp_SM83 *cpu) {

}

void ld_a_c_ind(Sharp_SM83 *cpu) {

}

void di(Sharp_SM83 *cpu) {

}

void push_af(Sharp_SM83 *cpu) {

}

void or_d8(Sharp_SM83 *cpu, uint8_t d8) {
    alu_or(cpu, d8);
}

void rst_30h(Sharp_SM83 *cpu) {

}

void ld_hl_sp_r8(Sharp_SM83 *cpu, int8_t r8) {

}

void ld_sp_hl(Sharp_SM83 *cpu) {

}

void ld_a_a16(Sharp_SM83 *cpu, uint16_t a16) {

}

void ei(Sharp_SM83 *cpu) {

}

void cp_d8(Sharp_SM83 *cpu, uint8_t d8) {
    alu_sub(cpu, d8, 1);
}

void rst_38h(Sharp_SM83 *cpu) {

}

/* 
   CB-PREFIXED INSTRUCTIONS — Sharp SM83 (Game Boy)
*/

void cb_update_flags(Sharp_SM83 *cpu, uint8_t result,  uint8_t bit) {

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, 0);
    cpu_set_flag(cpu, FLAG_H, 0);  
    cpu_set_flag(cpu, FLAG_C, bit);
}

uint8_t alu_rlc(Sharp_SM83 *cpu, uint8_t d) {
    uint8_t bit7 = (d & 0x80) >> 7;
    d <<= 1;
    uint8_t result = d | (bit7 << 0);

    cb_update_flags(cpu, result, bit7);
    return result;
}

void cb_rlc_b(Sharp_SM83 *cpu) {
    cpu->regs.B = alu_rlc(cpu, cpu->regs.B);
}

void cb_rlc_c(Sharp_SM83 *cpu) {
    cpu->regs.C = alu_rlc(cpu, cpu->regs.C);
}

void cb_rlc_d(Sharp_SM83 *cpu) {
    cpu->regs.D = alu_rlc(cpu, cpu->regs.D);
}

void cb_rlc_e(Sharp_SM83 *cpu) {
    cpu->regs.E = alu_rlc(cpu, cpu->regs.E);
}

void cb_rlc_h(Sharp_SM83 *cpu) {
    cpu->regs.H = alu_rlc(cpu, cpu->regs.H);
}

void cb_rlc_l(Sharp_SM83 *cpu) {
    cpu->regs.L = alu_rlc(cpu, cpu->regs.L);
}

void cb_rlc_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint8_t d8 = mmu_read(mmu, cpu->regs.HL);
    mmu_write8(mmu, cpu->regs.HL, alu_rlc(cpu, d8));
}

void cb_rlc_a(Sharp_SM83 *cpu) {
    cpu->regs.A = alu_rlc(cpu, cpu->regs.A);
}

uint8_t alu_rrc(Sharp_SM83 *cpu, uint8_t d) {
    uint8_t bit0 = (d & 0x01);
    d >>= 1;
    uint8_t result = d | bit0 << 7;

    cb_update_flags(cpu, result, bit0);
    return result;
}

void cb_rrc_b(Sharp_SM83 *cpu) {
    cpu->regs.B = alu_rrc(cpu, cpu->regs.B);
}

void cb_rrc_c(Sharp_SM83 *cpu) {
    cpu->regs.C = alu_rrc(cpu, cpu->regs.C);
}

void cb_rrc_d(Sharp_SM83 *cpu) {
    cpu->regs.D = alu_rrc(cpu, cpu->regs.D);
}

void cb_rrc_e(Sharp_SM83 *cpu) {
    cpu->regs.E = alu_rrc(cpu, cpu->regs.E);
}   

void cb_rrc_h(Sharp_SM83 *cpu) {
    cpu->regs.H = alu_rrc(cpu, cpu->regs.H);
}

void cb_rrc_l(Sharp_SM83 *cpu) {
    cpu->regs.L = alu_rrc(cpu, cpu->regs.L);
}

void cb_rrc_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint8_t d8 = mmu_read(mmu, cpu->regs.HL);
    mmu_write8(mmu, cpu->regs.HL, alu_rrc(cpu, d8));
}

void cb_rrc_a(Sharp_SM83 *cpu) {
    cpu->regs.A = alu_rrc(cpu, cpu->regs.A);
}

uint8_t alu_rl(Sharp_SM83 *cpu, uint8_t d) {
    uint8_t bit7 = (d & 0x80) >> 7;
    bit7 <<= 1;
    uint8_t result = bit7 | (get_flag(cpu, FLAG_C) << 0);

    cb_update_flags(cpu, result, bit7);
    return result;
}

void cb_rl_b(Sharp_SM83 *cpu) {
    cpu->regs.B = alu_rl(cpu, cpu->regs.B);
}

void cb_rl_c(Sharp_SM83 *cpu) {
    cpu->regs.C = alu_rl(cpu, cpu->regs.C);
}

void cb_rl_d(Sharp_SM83 *cpu) {
    cpu->regs.D = alu_rl(cpu, cpu->regs.D);
}

void cb_rl_e(Sharp_SM83 *cpu) {
    cpu->regs.E = alu_rl(cpu, cpu->regs.E);
}

void cb_rl_h(Sharp_SM83 *cpu) {
    cpu->regs.H = alu_rl(cpu, cpu->regs.H);
}

void cb_rl_l(Sharp_SM83 *cpu) {
    cpu->regs.L = alu_rl(cpu, cpu->regs.L);
}

void cb_rl_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint8_t d8 = mmu_read(mmu, cpu->regs.HL);
    mmu_write8(mmu, cpu->regs.HL, alu_rl(cpu, d8));
}

void cb_rl_a(Sharp_SM83 *cpu) {
    cpu->regs.A = alu_rl(cpu, cpu->regs.A);
}

uint8_t alu_rr(Sharp_SM83 *cpu, uint8_t d) {
    uint8_t bit0 = (d & 0x01);
    d >>= 1;
    uint8_t result = d | (get_flag(cpu, FLAG_C) << 7);

    cb_update_flags(cpu, result, bit0);
    return result;
}

void cb_rr_b(Sharp_SM83 *cpu) {
    cpu->regs.B = alu_rr(cpu, cpu->regs.B);
}

void cb_rr_c(Sharp_SM83 *cpu) {
    cpu->regs.C = alu_rr(cpu, cpu->regs.C);
}

void cb_rr_d(Sharp_SM83 *cpu) {
    cpu->regs.D = alu_rr(cpu, cpu->regs.D);
}

void cb_rr_e(Sharp_SM83 *cpu) {
    cpu->regs.E = alu_rr(cpu, cpu->regs.E);
}

void cb_rr_h(Sharp_SM83 *cpu) {
    cpu->regs.H = alu_rr(cpu, cpu->regs.H);
}

void cb_rr_l(Sharp_SM83 *cpu) {
    cpu->regs.L = alu_rr(cpu, cpu->regs.L);
}

void cb_rr_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint8_t d8 = mmu_read(mmu, cpu->regs.HL);
    mmu_write8(mmu, cpu->regs.HL, alu_rr(cpu, d8));
}

void cb_rr_a(Sharp_SM83 *cpu) {
    cpu->regs.A = alu_rr(cpu, cpu->regs.A);
}

uint8_t alu_sla(Sharp_SM83 *cpu, uint8_t d) {
    uint8_t bit7 = (d & 0x80) >> 7; 
    uint8_t result = d << 1;   
    
    cb_update_flags(cpu, result, bit7);
    return result;
}

void cb_sla_b(Sharp_SM83 *cpu) { 
    cpu->regs.B = alu_sla(cpu, cpu->regs.B); 
}

void cb_sla_c(Sharp_SM83 *cpu) { 
    cpu->regs.C = alu_sla(cpu, cpu->regs.C); 
}

void cb_sla_d(Sharp_SM83 *cpu) { 
    cpu->regs.D = alu_sla(cpu, cpu->regs.D); 
}

void cb_sla_e(Sharp_SM83 *cpu) { 
    cpu->regs.E = alu_sla(cpu, cpu->regs.E); 
}

void cb_sla_h(Sharp_SM83 *cpu) { 
    cpu->regs.H = alu_sla(cpu, cpu->regs.H); 
}

void cb_sla_l(Sharp_SM83 *cpu) { 
    cpu->regs.L = alu_sla(cpu, cpu->regs.L); 
}

void cb_sla_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint8_t d8 = mmu_read(mmu, cpu->regs.HL);
    d8 = alu_sla(cpu, d8);
    mmu_write8(mmu, cpu->regs.HL, d8);
}

void cb_sla_a(Sharp_SM83 *cpu) { 
    cpu->regs.A = alu_sla(cpu, cpu->regs.A); 
}

uint8_t alu_sra(Sharp_SM83 *cpu, uint8_t d) {
    uint8_t bit0 = d & 0x01;
    uint8_t bit7 = d & 0x80;

    uint8_t result = (d >> 1) | bit7; 
    cb_update_flags(cpu, result, bit0);
    return result;
}


void cb_sra_b(Sharp_SM83 *cpu) {
    cpu->regs.B = alu_sra(cpu, cpu->regs.B);
}

void cb_sra_c(Sharp_SM83 *cpu) {
    cpu->regs.C = alu_sra(cpu, cpu->regs.C);
}

void cb_sra_d(Sharp_SM83 *cpu) {
    cpu->regs.D = alu_sra(cpu, cpu->regs.D);
}

void cb_sra_e(Sharp_SM83 *cpu) {
    cpu->regs.E = alu_sra(cpu, cpu->regs.E);
}

void cb_sra_h(Sharp_SM83 *cpu) {
    cpu->regs.H = alu_sra(cpu, cpu->regs.H);
}

void cb_sra_l(Sharp_SM83 *cpu) {
    cpu->regs.L = alu_sra(cpu, cpu->regs.L);
}

void cb_sra_hl(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint8_t d8 = mmu_read(mmu, cpu->regs.HL);
    mmu_write8(mmu, cpu->regs.HL, alu_sra(cpu, d8));
}

void cb_sra_a(Sharp_SM83 *cpu) {
    cpu->regs.A = alu_sra(cpu, cpu->regs.A);
}   

void cb_swap_b(Sharp_SM83 *cpu) {

}

void cb_swap_c(Sharp_SM83 *cpu) {

}

void cb_swap_d(Sharp_SM83 *cpu) {

}

void cb_swap_e(Sharp_SM83 *cpu) {

}

void cb_swap_h(Sharp_SM83 *cpu) {

}

void cb_swap_l(Sharp_SM83 *cpu) {

}

void cb_swap_hl(Sharp_SM83 *cpu) {

}

void cb_swap_a(Sharp_SM83 *cpu) {

}

void cb_srl_b(Sharp_SM83 *cpu) {

}

void cb_srl_c(Sharp_SM83 *cpu) {

}

void cb_srl_d(Sharp_SM83 *cpu) {

}

void cb_srl_e(Sharp_SM83 *cpu) {

}

void cb_srl_h(Sharp_SM83 *cpu) {

}

void cb_srl_l(Sharp_SM83 *cpu) {

}

void cb_srl_hl(Sharp_SM83 *cpu) {

}

void cb_srl_a(Sharp_SM83 *cpu) {

}

void cb_bit_0_b(Sharp_SM83 *cpu) {

}

void cb_bit_0_c(Sharp_SM83 *cpu) {

}

void cb_bit_0_d(Sharp_SM83 *cpu) {

}

void cb_bit_0_e(Sharp_SM83 *cpu) {

}

void cb_bit_0_h(Sharp_SM83 *cpu) {

}

void cb_bit_0_l(Sharp_SM83 *cpu) {

}

void cb_bit_0_hl(Sharp_SM83 *cpu) {

}

void cb_bit_0_a(Sharp_SM83 *cpu) {

}

void cb_bit_1_b(Sharp_SM83 *cpu) {

}

void cb_bit_1_c(Sharp_SM83 *cpu) {

}

void cb_bit_1_d(Sharp_SM83 *cpu) {

}

void cb_bit_1_e(Sharp_SM83 *cpu) {

}

void cb_bit_1_h(Sharp_SM83 *cpu) {

}

void cb_bit_1_l(Sharp_SM83 *cpu) {

}

void cb_bit_1_hl(Sharp_SM83 *cpu) {

}

void cb_bit_1_a(Sharp_SM83 *cpu) {

}

void cb_bit_2_b(Sharp_SM83 *cpu) {

}

void cb_bit_2_c(Sharp_SM83 *cpu) {

}

void cb_bit_2_d(Sharp_SM83 *cpu) {

}

void cb_bit_2_e(Sharp_SM83 *cpu) {

}

void cb_bit_2_h(Sharp_SM83 *cpu) {

}

void cb_bit_2_l(Sharp_SM83 *cpu) {

}

void cb_bit_2_hl(Sharp_SM83 *cpu) {

}

void cb_bit_2_a(Sharp_SM83 *cpu) {

}

void cb_bit_3_b(Sharp_SM83 *cpu) {

}

void cb_bit_3_c(Sharp_SM83 *cpu) {

}

void cb_bit_3_d(Sharp_SM83 *cpu) {

}

void cb_bit_3_e(Sharp_SM83 *cpu) {

}

void cb_bit_3_h(Sharp_SM83 *cpu) {

}

void cb_bit_3_l(Sharp_SM83 *cpu) {

}

void cb_bit_3_hl(Sharp_SM83 *cpu) {

}

void cb_bit_3_a(Sharp_SM83 *cpu) {

}

void cb_bit_4_b(Sharp_SM83 *cpu) {

}

void cb_bit_4_c(Sharp_SM83 *cpu) {

}

void cb_bit_4_d(Sharp_SM83 *cpu) {

}

void cb_bit_4_e(Sharp_SM83 *cpu) {

}

void cb_bit_4_h(Sharp_SM83 *cpu) {

}

void cb_bit_4_l(Sharp_SM83 *cpu) {

}

void cb_bit_4_hl(Sharp_SM83 *cpu) {

}

void cb_bit_4_a(Sharp_SM83 *cpu) {

}

void cb_bit_5_b(Sharp_SM83 *cpu) {

}

void cb_bit_5_c(Sharp_SM83 *cpu) {

}

void cb_bit_5_d(Sharp_SM83 *cpu) {

}

void cb_bit_5_e(Sharp_SM83 *cpu) {

}

void cb_bit_5_h(Sharp_SM83 *cpu) {

}

void cb_bit_5_l(Sharp_SM83 *cpu) {

}

void cb_bit_5_hl(Sharp_SM83 *cpu) {

}

void cb_bit_5_a(Sharp_SM83 *cpu) {

}

void cb_bit_6_b(Sharp_SM83 *cpu) {

}

void cb_bit_6_c(Sharp_SM83 *cpu) {

}

void cb_bit_6_d(Sharp_SM83 *cpu) {

}

void cb_bit_6_e(Sharp_SM83 *cpu) {

}

void cb_bit_6_h(Sharp_SM83 *cpu) {

}

void cb_bit_6_l(Sharp_SM83 *cpu) {

}

void cb_bit_6_hl(Sharp_SM83 *cpu) {

}

void cb_bit_6_a(Sharp_SM83 *cpu) {

}

void cb_bit_7_b(Sharp_SM83 *cpu) {

}

void cb_bit_7_c(Sharp_SM83 *cpu) {

}

void cb_bit_7_d(Sharp_SM83 *cpu) {

}

void cb_bit_7_e(Sharp_SM83 *cpu) {

}

void cb_bit_7_h(Sharp_SM83 *cpu) {

}

void cb_bit_7_l(Sharp_SM83 *cpu) {

}

void cb_bit_7_hl(Sharp_SM83 *cpu) {

}

void cb_bit_7_a(Sharp_SM83 *cpu) {

}

void cb_res_0_b(Sharp_SM83 *cpu) {

}

void cb_res_0_c(Sharp_SM83 *cpu) {

}

void cb_res_0_d(Sharp_SM83 *cpu) {

}

void cb_res_0_e(Sharp_SM83 *cpu) {

}

void cb_res_0_h(Sharp_SM83 *cpu) {

}

void cb_res_0_l(Sharp_SM83 *cpu) {

}

void cb_res_0_hl(Sharp_SM83 *cpu) {

}

void cb_res_0_a(Sharp_SM83 *cpu) {

}

void cb_res_1_b(Sharp_SM83 *cpu) {

}

void cb_res_1_c(Sharp_SM83 *cpu) {

}

void cb_res_1_d(Sharp_SM83 *cpu) {

}

void cb_res_1_e(Sharp_SM83 *cpu) {

}

void cb_res_1_h(Sharp_SM83 *cpu) {

}

void cb_res_1_l(Sharp_SM83 *cpu) {

}

void cb_res_1_hl(Sharp_SM83 *cpu) {

}

void cb_res_1_a(Sharp_SM83 *cpu) {

}

void cb_res_2_b(Sharp_SM83 *cpu) {

}

void cb_res_2_c(Sharp_SM83 *cpu) {

}

void cb_res_2_d(Sharp_SM83 *cpu) {

}

void cb_res_2_e(Sharp_SM83 *cpu) {

}

void cb_res_2_h(Sharp_SM83 *cpu) {

}

void cb_res_2_l(Sharp_SM83 *cpu) {

}

void cb_res_2_hl(Sharp_SM83 *cpu) {

}

void cb_res_2_a(Sharp_SM83 *cpu) {

}

void cb_res_3_b(Sharp_SM83 *cpu) {

}

void cb_res_3_c(Sharp_SM83 *cpu) {

}

void cb_res_3_d(Sharp_SM83 *cpu) {

}

void cb_res_3_e(Sharp_SM83 *cpu) {

}

void cb_res_3_h(Sharp_SM83 *cpu) {

}

void cb_res_3_l(Sharp_SM83 *cpu) {

}

void cb_res_3_hl(Sharp_SM83 *cpu) {

}

void cb_res_3_a(Sharp_SM83 *cpu) {

}

void cb_res_4_b(Sharp_SM83 *cpu) {

}

void cb_res_4_c(Sharp_SM83 *cpu) {

}

void cb_res_4_d(Sharp_SM83 *cpu) {

}

void cb_res_4_e(Sharp_SM83 *cpu) {

}

void cb_res_4_h(Sharp_SM83 *cpu) {

}

void cb_res_4_l(Sharp_SM83 *cpu) {

}

void cb_res_4_hl(Sharp_SM83 *cpu) {

}

void cb_res_4_a(Sharp_SM83 *cpu) {

}

void cb_res_5_b(Sharp_SM83 *cpu) {

}

void cb_res_5_c(Sharp_SM83 *cpu) {

}

void cb_res_5_d(Sharp_SM83 *cpu) {

}

void cb_res_5_e(Sharp_SM83 *cpu) {

}

void cb_res_5_h(Sharp_SM83 *cpu) {

}

void cb_res_5_l(Sharp_SM83 *cpu) {

}

void cb_res_5_hl(Sharp_SM83 *cpu) {

}

void cb_res_5_a(Sharp_SM83 *cpu) {

}

void cb_res_6_b(Sharp_SM83 *cpu) {

}

void cb_res_6_c(Sharp_SM83 *cpu) {

}

void cb_res_6_d(Sharp_SM83 *cpu) {

}

void cb_res_6_e(Sharp_SM83 *cpu) {

}

void cb_res_6_h(Sharp_SM83 *cpu) {

}

void cb_res_6_l(Sharp_SM83 *cpu) {

}

void cb_res_6_hl(Sharp_SM83 *cpu) {

}

void cb_res_6_a(Sharp_SM83 *cpu) {

}

void cb_res_7_b(Sharp_SM83 *cpu) {

}

void cb_res_7_c(Sharp_SM83 *cpu) {

}

void cb_res_7_d(Sharp_SM83 *cpu) {

}

void cb_res_7_e(Sharp_SM83 *cpu) {

}

void cb_res_7_h(Sharp_SM83 *cpu) {

}

void cb_res_7_l(Sharp_SM83 *cpu) {

}

void cb_res_7_hl(Sharp_SM83 *cpu) {

}

void cb_res_7_a(Sharp_SM83 *cpu) {

}

void cb_set_0_b(Sharp_SM83 *cpu) {

}

void cb_set_0_c(Sharp_SM83 *cpu) {

}

void cb_set_0_d(Sharp_SM83 *cpu) {

}

void cb_set_0_e(Sharp_SM83 *cpu) {

}

void cb_set_0_h(Sharp_SM83 *cpu) {

}

void cb_set_0_l(Sharp_SM83 *cpu) {

}

void cb_set_0_hl(Sharp_SM83 *cpu) {

}

void cb_set_0_a(Sharp_SM83 *cpu) {

}

void cb_set_1_b(Sharp_SM83 *cpu) {

}

void cb_set_1_c(Sharp_SM83 *cpu) {

}

void cb_set_1_d(Sharp_SM83 *cpu) {

}

void cb_set_1_e(Sharp_SM83 *cpu) {

}

void cb_set_1_h(Sharp_SM83 *cpu) {

}

void cb_set_1_l(Sharp_SM83 *cpu) {

}

void cb_set_1_hl(Sharp_SM83 *cpu) {

}

void cb_set_1_a(Sharp_SM83 *cpu) {

}

void cb_set_2_b(Sharp_SM83 *cpu) {

}

void cb_set_2_c(Sharp_SM83 *cpu) {

}

void cb_set_2_d(Sharp_SM83 *cpu) {

}

void cb_set_2_e(Sharp_SM83 *cpu) {

}

void cb_set_2_h(Sharp_SM83 *cpu) {

}

void cb_set_2_l(Sharp_SM83 *cpu) {

}

void cb_set_2_hl(Sharp_SM83 *cpu) {

}

void cb_set_2_a(Sharp_SM83 *cpu) {

}

void cb_set_3_b(Sharp_SM83 *cpu) {

}

void cb_set_3_c(Sharp_SM83 *cpu) {

}

void cb_set_3_d(Sharp_SM83 *cpu) {

}

void cb_set_3_e(Sharp_SM83 *cpu) {

}

void cb_set_3_h(Sharp_SM83 *cpu) {

}

void cb_set_3_l(Sharp_SM83 *cpu) {

}

void cb_set_3_hl(Sharp_SM83 *cpu) {

}

void cb_set_3_a(Sharp_SM83 *cpu) {

}

void cb_set_4_b(Sharp_SM83 *cpu) {

}

void cb_set_4_c(Sharp_SM83 *cpu) {

}

void cb_set_4_d(Sharp_SM83 *cpu) {

}

void cb_set_4_e(Sharp_SM83 *cpu) {

}

void cb_set_4_h(Sharp_SM83 *cpu) {

}

void cb_set_4_l(Sharp_SM83 *cpu) {

}

void cb_set_4_hl(Sharp_SM83 *cpu) {

}

void cb_set_4_a(Sharp_SM83 *cpu) {

}

void cb_set_5_b(Sharp_SM83 *cpu) {

}

void cb_set_5_c(Sharp_SM83 *cpu) {

}

void cb_set_5_d(Sharp_SM83 *cpu) {

}

void cb_set_5_e(Sharp_SM83 *cpu) {

}

void cb_set_5_h(Sharp_SM83 *cpu) {

}

void cb_set_5_l(Sharp_SM83 *cpu) {

}

void cb_set_5_hl(Sharp_SM83 *cpu) {

}

void cb_set_5_a(Sharp_SM83 *cpu) {

}

void cb_set_6_b(Sharp_SM83 *cpu) {

}

void cb_set_6_c(Sharp_SM83 *cpu) {

}

void cb_set_6_d(Sharp_SM83 *cpu) {

}

void cb_set_6_e(Sharp_SM83 *cpu) {

}

void cb_set_6_h(Sharp_SM83 *cpu) {

}

void cb_set_6_l(Sharp_SM83 *cpu) {

}

void cb_set_6_hl(Sharp_SM83 *cpu) {

}

void cb_set_6_a(Sharp_SM83 *cpu) {

}

void cb_set_7_b(Sharp_SM83 *cpu) {

}

void cb_set_7_c(Sharp_SM83 *cpu) {

}

void cb_set_7_d(Sharp_SM83 *cpu) {

}

void cb_set_7_e(Sharp_SM83 *cpu) {

}

void cb_set_7_h(Sharp_SM83 *cpu) {

}

void cb_set_7_l(Sharp_SM83 *cpu) {

}

void cb_set_7_hl(Sharp_SM83 *cpu) {

}

void cb_set_7_a(Sharp_SM83 *cpu) {

}

// EOF


#include "sharp_SM83.h"
#include "alu_SM83.h"
#include "flags_SM83.h"

void SM83_Init_Regs(Sharp_SM83 *cpu, int mode) {
    if (mode == 0) {
        cpu->regs.AF = 0x01b0;
        cpu->regs.BC = 0x0013;
        cpu->regs.DE = 0x00d8;
        cpu->regs.HL = 0x014d;
        cpu->regs.SP = 0xfffe;
        cpu->regs.PC = 0x0100;

    } else {
        cpu->regs.AF = 0x1180;
        cpu->regs.BC = 0x0000;
        cpu->regs.DE = 0xff56;
        cpu->regs.HL = 0x000d;
        cpu->regs.SP = 0xfffe;
        cpu->regs.PC = 0x0100;
    }
}

void check_interrupts(Sharp_SM83 *cpu, Sharp_MMU *mmu) {

}

void execute_cycle_instruction(Sharp_SM83 *cpu, Sharp_MMU *mmu) {
    uint8_t opcode = mmu_read(mmu, cpu->regs.PC++);

    if (cpu->IS_PREFIXCB) {
        cpu->IS_PREFIXCB = 0;
        execute_cb_extended_table(cpu, opcode);
    } else {
        execute_standard_opcode(cpu, mmu, opcode);
        if (cpu->EI) {
            cpu->IME = 1;
        }
    }

    /* after last instruction opcode, check for interrupts */
    check_interrupts(cpu, mmu);
}

void execute_standard_opcode(Sharp_SM83 *cpu, Sharp_MMU *mmu, uint8_t opcode) {
     switch(opcode) {

        case 0x00: {
            cpu->cycles += 4;
            break;
        }
        case 0x01: {
            uint8_t low = mmu_read(mmu, cpu->regs.PC++);
            uint8_t high = mmu_read(mmu, cpu->regs.PC++);
            uint16_t d16 = (high << 8) | low;
            ld_bc_d16(cpu, d16);
            cpu->cycles += 12;
            break;
        }
        case 0x02: {
            ld_bc_a(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x03: {
            inc_bc(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x04: {
            inc_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x05: {
            dec_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x06: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            ld_b_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0x07: {
            rlca(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x08: {    
            uint8_t low = mmu_read(mmu, cpu->regs.PC++);
            uint8_t high = mmu_read(mmu, cpu->regs.PC++);
            uint16_t a16 = (high << 8) | low;
            ld_a16_sp(mmu, a16, cpu);
            cpu->cycles += 20;
            break;
        }
        case 0x09: {
            add_hl_bc(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x0a: {
            ld_a_bc(mmu, cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x0b: {
            dec_bc(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x0c: {
            inc_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x0d: {
            dec_c(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x0e: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            ld_c_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0x0f: {
            rrca(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x10: {
            uint8_t zero = mmu_read(mmu, cpu->regs.PC++);
            cpu->STOP = 1;
            cpu->cycles += 4;
            break;
        }
        case 0x11: {
            uint8_t low = mmu_read(mmu, cpu->regs.PC++);
            uint8_t high = mmu_read(mmu, cpu->regs.PC++);
            uint16_t d16 = (high << 8) | low; 
            ld_de_d16(cpu, d16);
            cpu->cycles += 12;
            break;
        }
        case 0x12: {
            ld_de_a(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x13: {
            inc_de(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x14: {
            inc_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x15: {
            dec_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x16: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            ld_d_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0x17: {
            rla(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x18: {
            int8_t r8 = (int8_t) mmu_read(mmu, cpu->regs.PC++);
            cpu->regs.PC += r8;
            cpu->cycles += 12;
            break;
        }
        case 0x19: {
            add_hl_de(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x1a: {
            ld_a_de(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x1b: {
            dec_de(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x1c: {
            inc_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x1d: {
            dec_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x1e: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            ld_e_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0x1f: {
            rra(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x20: {
            if (!get_flag(cpu, FLAG_Z)) {
                int8_t r8 = (int8_t) mmu_read(mmu, cpu->regs.PC++);
                cpu->regs.PC += r8;
                cpu->cycles += 12;
                break;
            } else {
                cpu->cycles += 8;
                break;
            }
        }
        case 0x21: {
            uint8_t low = mmu_read(mmu, cpu->regs.PC++);
            uint8_t high = mmu_read(mmu, cpu->regs.PC++);
            uint16_t d16 = (high << 8) | low;
            ld_hl_d16(cpu, d16);
            cpu->cycles += 12;
            break;
        }
        case 0x22: {
            ld_hli_a(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x23: {
            inc_hl(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x24: {
            inc_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x25: {
            dec_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x26: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            ld_h_d8(cpu,  d8);
            cpu->cycles += 8;
            break;
        }
        case 0x27: {
            daa(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x28: {
            if (get_flag(cpu, FLAG_Z)) {
                int8_t r8 = (int8_t) mmu_read(mmu, cpu->regs.PC++);
                cpu->regs.PC += r8;
                cpu->cycles += 12;
                break;
            } else {
                cpu->cycles += 8;
                break;
            }
        }
        case 0x29: {
            add_hl_hl(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x2a: {
            ld_a_hli(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x2b: {
            dec_hl(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x2c: {
            inc_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x2d: {
            dec_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x2e: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            ld_l_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0x2f: {
            cpl(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x30: {
            if (!get_flag(cpu, FLAG_C)) {
                int8_t r8 = (int8_t) mmu_read(mmu, cpu->regs.PC++);
                cpu->regs.PC += r8;
                cpu->cycles += 12;
                break;
            } else {
                cpu->cycles += 8;
                break;
            }
        }
        case 0x31: {
            uint8_t low = mmu_read(mmu, cpu->regs.PC++);
            uint8_t high = mmu_read(mmu, cpu->regs.PC++);
            uint16_t d16 = (high << 8) | low;
            ld_sp_d16(cpu, d16);
            cpu->cycles += 12;
            break;
        }
        case 0x32: {
            ld_hld_a(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x33: {
            inc_sp(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x34: {
            inc_hl_ind(cpu, mmu);
            cpu->cycles += 12;
            break;
        }
        case 0x35: {
            dec_hl_ind(cpu, mmu);
            cpu->cycles += 12;
            break;
        }
        case 0x36: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            ld_hl_d8(cpu, mmu, d8);
            cpu->cycles += 12;
            break;
        }
        case 0x37: {
            scf(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x38: {
            if (get_flag(cpu, FLAG_C)) {
                int8_t r8 = (int8_t) mmu_read(mmu, cpu->regs.PC++);
                cpu->regs.PC += r8;
                cpu->cycles += 12;
                break;
            } else {
                cpu->cycles += 8;
                break;
            }
        }
        case 0x39: {
            add_hl_sp(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x3a: {
            ld_a_hld(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x3b: {
            dec_sp(cpu);
            cpu->cycles += 8;
            break;
        }
        case 0x3c: {
            inc_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x3d: {
            dec_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x3e: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            ld_a_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0x3f: {
            ccf(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x40: {
            cpu->cycles += 4;
            break;
        }
        case 0x41: {
            ld_b_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x42: {
            ld_b_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x43: {
            ld_b_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x44: {
            ld_b_h(cpu);
            cpu->cycles += 4;
            break;   
        }
        case 0x45: {
            ld_b_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x46: {
            ld_b_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x47: {
            ld_b_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x48: {
            ld_c_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x49: {
            cpu->cycles += 4;
            break;
        }
        case 0x4a: {
            ld_c_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x4b: {
            ld_c_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x4c: {
            ld_c_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x4d: {
            ld_c_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x4e: {
            ld_c_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x4f: {
            ld_c_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x50: {
            ld_d_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x51: {
            ld_d_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x52: {
            cpu->cycles += 4;
            break;
        }
        case 0x53: {
            ld_d_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x54: {
            ld_d_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x55: {
            ld_d_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x56: {
            ld_d_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x57: {
            ld_d_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x58: {
            ld_e_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x59: {
            ld_e_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x5a: {
            ld_e_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x5b: {
            cpu->cycles += 4;
            break;
        }
        case 0x5c: {
            ld_e_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x5d: {
            ld_e_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x5e: {
            ld_e_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x5f: {
            ld_e_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x60: {
            ld_h_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x61: {
            ld_h_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x62: {
            ld_h_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x63: {
            ld_h_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x64: {
            cpu->cycles += 4;
            break;
        }
        case 0x65: {
            ld_h_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x66: {
            ld_h_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x67: {
            ld_h_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x68: {
            ld_l_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x69: {
            ld_l_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x6a: {
            ld_l_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x6b: {
            ld_l_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x6c: {
            ld_l_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x6d: {
            cpu->cycles += 4;
            break;
        }
        case 0x6e: {
            ld_l_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x6f: {
            ld_l_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x70: {
            ld_hl_b(cpu, mmu);
            cpu->cycles += 8;
            break;   
        }
        case 0x71: {
            ld_hl_c(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x72: {
            ld_hl_d(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x73: {
            ld_hl_e(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x74: {
            ld_hl_h(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x75: {
            ld_hl_l(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x76: {
            cpu->HALT = 1;
            cpu->cycles += 4;
            break;
        }
        case 0x77: {
            ld_hl_a(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x78: {
            ld_a_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x79: {
            ld_a_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x7a: {
            ld_a_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x7b: {
            ld_a_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x7c: {
            ld_a_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x7d: {
            ld_a_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x7e: {
            ld_a_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x7f: {
            cpu->cycles += 4;
            break;
        }
        case 0x80: {
            add_a_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x81: {
            add_a_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x82: {
            add_a_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x83: {
            add_a_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x84: {
            add_a_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x85: {
            add_a_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x86: {
            add_a_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x87: {
            add_a_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x88: {
            adc_a_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x89: {
            adc_a_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x8a: {
            adc_a_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x8b: {
            adc_a_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x8c: {
            adc_a_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x8d: {
            adc_a_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x8e: {
            adc_a_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x8f: {
            adc_a_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x90: {
            sub_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x91: {
            sub_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x92: {
            sub_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x93: {
            sub_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x94: {
            sub_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x95: {
            sub_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x96: {
            sub_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x97: {
            sub_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x98: {
            sbc_a_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x99: {
            sbc_a_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x9a: {
            sbc_a_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x9b: {
            sbc_a_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x9c: {
            sbc_a_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x9d: {
            sbc_a_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0x9e: {
            sbc_a_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0x9f: {
            sbc_a_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xa0: {
            and_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xa1: {
            and_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xa2: {
            and_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xa3: {
            and_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xa4: {
            and_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xa5: {
            and_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xa6: {
            and_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0xa7: {
            and_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xa8: {
            xor_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xa9: {
            xor_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xaa: {
            xor_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xab: {
            xor_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xac: {
            xor_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xad: {
            xor_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xae: {
            xor_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0xaf: {
            xor_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xb0: {
            or_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xb1: {
            or_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xb2: {
            or_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xb3: {
            or_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xb4: {
            or_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xb5: {
            or_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xb6: {
            or_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0xb7: {
            or_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xb8: {
            cp_b(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xb9: {
            cp_c(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xba: {
            cp_d(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xbb: {
            cp_e(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xbc: {
            cp_h(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xbd: {
            cp_l(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xbe: {
            cp_hl(cpu, mmu);
            cpu->cycles += 8;
            break;
        }
        case 0xbf: {
            cp_a(cpu);
            cpu->cycles += 4;
            break;
        }
        case 0xc0: {
            if (!get_flag(cpu, FLAG_Z)) {
                uint8_t low_ret = mmu_read(mmu, cpu->regs.SP++);
                uint8_t high_ret = mmu_read(mmu, cpu->regs.SP++);
                uint16_t ret_addr = (high_ret << 8) | low_ret;
                cpu->regs.PC = ret_addr;
                cpu->cycles += 20;
                break;
            } else {
                cpu->cycles += 8;
                break;
            }
        }
        case 0xc1: {
            uint8_t low = mmu_read(mmu, cpu->regs.SP++);
            uint8_t high = mmu_read(mmu, cpu->regs.SP++);
            cpu->regs.BC = (high << 8) | low;
            cpu->cycles += 12;
            break;
        }
        case 0xc2: {
            if (!get_flag(cpu, FLAG_Z)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
                uint16_t jmp_addr = (high_addr << 8) | low_addr;
                cpu->regs.PC = jmp_addr;
                cpu->cycles += 16;
                break;
            } else {
                cpu->cycles += 12;
                break;
            }
        }
        case 0xc3: {
            uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
            uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
            uint16_t jmp_addr = (high_addr << 8) | low_addr;
            cpu->regs.PC = jmp_addr;
            cpu->cycles += 16;
            break;
        }
        case 0xc4: {
            if (!get_flag(cpu, FLAG_Z)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
                uint16_t call_addr = (high_addr << 8) | low_addr;
                cpu->regs.SP -= 1;
                mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
                cpu->regs.PC = call_addr;
                cpu->cycles += 24;
                break;
            } else {
                cpu->cycles += 12;
                break;
            }
        }
        case 0xc5: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.BC);
            cpu->cycles += 16;
            break;
        }
        case 0xc6: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            add_a_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0xc7: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
            cpu->regs.PC = 0x0000;
            cpu->cycles += 16;
            break;
        }
        case 0xc8: {
            if (get_flag(cpu, FLAG_Z)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.SP++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.SP++);
                uint16_t ret_addr = (high_addr << 8) | low_addr;
                cpu->regs.PC = ret_addr;
                cpu->cycles += 20;
                break;
            } else {
                cpu->cycles += 8;
                break;
            }
        }
        case 0xc9: {
            uint8_t low_addr = mmu_read(mmu, cpu->regs.SP++);
            uint8_t high_addr = mmu_read(mmu, cpu->regs.SP++);
            uint16_t ret_addr = (high_addr << 8) | low_addr;
            cpu->regs.PC = ret_addr;
            cpu->cycles += 16;
            break;
        }
        case 0xca: {
            if (get_flag(cpu, FLAG_Z)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
                uint16_t jmp_addr = (high_addr << 8) | low_addr;
                cpu->regs.PC = jmp_addr;
                cpu->cycles += 16;
                break;
            } else {
                cpu->cycles += 12;
                break;
            }
        }
        case 0xcb: {
            cpu->IS_PREFIXCB = 1;
            cpu->cycles += 4;
            break;
        }
        case 0xcc: {
            if (get_flag(cpu, FLAG_Z)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
                uint16_t jmp_addr = (high_addr << 8) | low_addr;
                cpu->regs.SP -= 1;
                mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
                cpu->regs.PC = jmp_addr;
                cpu->cycles += 24;
                break;
            } else {
                cpu->cycles += 12;
                break;
            }
        }
        case 0xcd: {
            uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
            uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
            uint16_t jmp_addr = (high_addr << 8) | low_addr;
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
            cpu->regs.PC = jmp_addr;
            cpu->cycles += 24;
            break;
        }
        case 0xce: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            adc_a_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0xcf: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
            cpu->regs.PC = 0x0008;
            cpu->cycles += 16;
            break;
        }
        case 0xd0: {
            if (!get_flag(cpu, FLAG_C)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.SP++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.SP++);
                uint16_t ret_addr = (high_addr << 8) | low_addr;
                cpu->regs.PC = ret_addr;
                cpu->cycles += 20;
                break;
            } else {
                cpu->cycles += 8;
                break;
            }
        }
        case 0xd1: {
            uint8_t low_addr = mmu_read(mmu, cpu->regs.SP++);
            uint8_t high_addr = mmu_read(mmu, cpu->regs.SP++);
            uint16_t d16 = (high_addr << 8) | low_addr;
            cpu->regs.DE = d16;
            cpu->cycles += 12;
            break;
        }
        case 0xd2: {
            if (!get_flag(cpu, FLAG_C)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
                uint16_t a16 = (high_addr << 8) | low_addr;
                cpu->regs.PC = a16;
                cpu->cycles += 16;
                break;
            } else {
                cpu->cycles += 12;
                break;
            }
        }
        case 0xd4: {
            if (!get_flag(cpu, FLAG_C)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
                uint16_t a16 = (high_addr << 8) | low_addr;
                cpu->regs.SP -= 1;
                mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
                cpu->regs.PC = a16;
                cpu->cycles += 24;
                break;
            } else {
                cpu->cycles += 12;
                break;
            }
        }
        case 0xd5: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.DE);
            cpu->cycles += 16;
            break;
        }
        case 0xd6: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            sub_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0xd7: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
            cpu->regs.PC = 0x0010;
            cpu->cycles += 16;
            break;
        }
        case 0xd8: {
            if (get_flag(cpu, FLAG_C)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.SP++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.SP++);
                uint16_t ret_addr = (high_addr << 8) | low_addr;
                cpu->regs.PC = ret_addr;
                cpu->cycles += 20;
                break;
            } else {
                cpu->cycles += 8;
                break;
            }
        }
        case 0xd9: { /* reti */
            uint8_t low_addr = mmu_read(mmu, cpu->regs.SP++);
            uint8_t high_addr = mmu_read(mmu, cpu->regs.SP++);
            uint16_t ret_addr = (high_addr << 8) | low_addr;
            cpu->regs.PC = ret_addr;
            cpu->IME = 1; 
            cpu->cycles += 16;
            break;
        }
        case 0xda: {
            if (get_flag(cpu, FLAG_C)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.SP++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.SP++);
                uint16_t jmp_addr = (high_addr << 8) | low_addr;
                cpu->regs.PC = jmp_addr;
                cpu->cycles += 16;
                break;
            } else {
                cpu->cycles += 12;
                break;
            }
        }
        case 0xdc: {
            if (get_flag(cpu, FLAG_C)) {
                uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
                uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
                uint16_t a16 = (high_addr << 8) | low_addr;
                cpu->regs.SP -= 1;
                mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
                cpu->regs.PC = a16;
                cpu->cycles += 24;
                break;
            } else {
                cpu->cycles += 12;
                break;
            }
        }
        case 0xde: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            sbc_a_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0xdf: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
            cpu->regs.PC = 0x0018;
            cpu->cycles += 16;
            break;
        }
        case 0xe0: {
            uint8_t offset8 = mmu_read(mmu, cpu->regs.PC++);
            uint16_t a16 = 0xff00 + offset8;
            mmu_write8(mmu, a16, cpu->regs.A);
            cpu->cycles += 12;
            break;
        }
        case 0xe1: {
            uint8_t low_addr = mmu_read(mmu, cpu->regs.SP++);
            uint8_t high_addr = mmu_read(mmu, cpu->regs.SP++);
            uint16_t d16 = (high_addr << 8) | low_addr;
            cpu->regs.HL = d16;
            cpu->cycles += 12;
            break;
        }
        case 0xe2: {
            uint16_t a16 = 0xff00 + cpu->regs.C;
            mmu_write8(mmu, a16, cpu->regs.A);
            cpu->cycles += 8;
            break;
        }
        case 0xe5: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.HL);
            cpu->cycles += 16;
            break;
        }
        case 0xe6: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            and_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0xe7: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
            cpu->regs.PC = 0x0020;
            cpu->cycles += 16;
            break;
        }
        case 0xe8: {
            int8_t r8 = mmu_read(mmu, cpu->regs.PC++);
            add_sp_r8(cpu, r8);
            cpu->cycles += 16;
            break;
        }
        case 0xe9: {
            uint16_t jmp_addr = cpu->regs.HL;
            cpu->regs.PC = jmp_addr;
            cpu->cycles += 4;
            break;
        }
        case 0xea: {
            uint8_t low_addr = mmu_read(mmu, cpu->regs.PC++);
            uint8_t high_addr = mmu_read(mmu, cpu->regs.PC++);
            uint16_t a16 = (high_addr << 8) | low_addr;
            mmu_write8(mmu, a16, cpu->regs.A);
            cpu->cycles += 16;
            break;
        }
        case 0xee: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            xor_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0xef: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
            cpu->regs.PC = 0x0028;
            cpu->cycles += 16;
            break;
        }
        case 0xf0: {
            uint8_t a8 = mmu_read(mmu, cpu->regs.PC++);
            uint16_t addr = 0xff00 + a8;
            cpu->regs.A = mmu_read(mmu, addr);
            cpu->cycles += 12;
            break;
        }
        case 0xf1: {
            uint8_t f = mmu_read(mmu, cpu->regs.SP++) & 0xf0;
            uint8_t a = mmu_read(mmu, cpu->regs.SP++);
            uint16_t af = (a << 8) | f;
            cpu->regs.AF = af;
            cpu->cycles += 12;
            break;
        }
        case 0xf2: {
            uint16_t addr = 0xff00 + cpu->regs.C;
            cpu->regs.A = mmu_read(mmu, addr);
            cpu->cycles += 8;
            break;
        }
        case 0xf3: {
            cpu->IME = 0;   
            cpu->cycles += 4;
            break;
        }
        case 0xf5: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.AF);
            cpu->cycles += 16;
            break;
        }
        case 0xf6: {
            alu_or(cpu, cpu->regs.PC++);
            cpu->cycles += 8;
            break;
        }
        case 0xf7: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
            cpu->regs.PC = 0x0030;
            cpu->cycles += 16;
            break;
        }
        case 0xf8: {
            int8_t d8 = (int8_t) mmu_read(mmu, cpu->regs.PC++);
            cpu->regs.HL= cpu->regs.SP + d8;

            cpu_set_flag(cpu, FLAG_Z, 0);
            cpu_set_flag(cpu, FLAG_N, 0);
            cpu_set_flag(cpu, FLAG_H, ((cpu->regs.SP & 0x0f) + ((uint8_t)d8 & 0x0f) > 0x0f));
            cpu_set_flag(cpu, FLAG_C, ((cpu->regs.SP & 0xff) + (uint8_t)d8) > 0xff);

            cpu->cycles += 12;
            break;
        }
        case 0xf9: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.HL);
            cpu->cycles += 8;
            break;
        }
        case 0xfa: {
            uint8_t low = mmu_read(mmu, cpu->regs.PC++);
            uint8_t high = mmu_read(mmu, cpu->regs.PC++);
            uint16_t addr = (high << 8) | low;
            uint8_t d8 = mmu_read(mmu, addr);
            cpu->regs.A = d8;
            cpu->cycles += 16;
            break;
        }
        case 0xfb: {
            cpu->EI = 1;
            cpu->cycles += 4;
            break;
            /* NOT SURE IF THIS DONE CORRECTLY */
        }
        case 0xfe: {
            uint8_t d8 = mmu_read(mmu, cpu->regs.PC++);
            cp_d8(cpu, d8);
            cpu->cycles += 8;
            break;
        }
        case 0xff: {
            cpu->regs.SP -= 1;
            mmu_write16(mmu, cpu->regs.SP, cpu->regs.PC);
            cpu->regs.PC = 0x0038;
            cpu->cycles += 16;
            break;
        }
        default: {
            /* Invalid opcode */
            break;
        }
    }
}

void execute_cb_extended_table(Sharp_SM83 *cpu, uint8_t opcode) {
    uint8_t next_op = cpu->regs.PC++;
    cpu->IS_PREFIXCB = 0;

    switch (next_op) {
        default: {
            /* Invalid CB opcode */
            break;
        }
    }
}

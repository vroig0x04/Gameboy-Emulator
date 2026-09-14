

/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/

#include "ppu.h"

void PPU_Init(PPU *ppu) {
    /* PPU registers are initzialiced to 0 since Boot ROM (256 bytes) will change their values */
    ppu->LCDC = 0;
    ppu->STAT = 0;
    ppu->SCY  = 0;
    ppu->SCX  = 0;
    ppu->LY   = 0;
    ppu->LYC  = 0;
    ppu->DMA  = 0;
    ppu->BGP  = 0;
    ppu->OBP0 = 0;
    ppu->OBP1 = 0;
    ppu->WY   = 0;
    ppu->WX   = 0;

    ppu->last_mode = 0xff;
}

void PPU_Advance(PPU *ppu, int cycles) {
    ppu->current_dot += cycles;

    while (ppu->current_dot >= DOTS) { 
        ppu->current_dot -= DOTS;
        ppu->LY++; // next line
        
        if (ppu->LY == SCANLINES) {
            ppu->LY = 0;
        }
    }

    // check if LY == LYC each time its value changes
    if (ppu->LYC == ppu->LY) {
        ppu->STAT |= (1 << 2); // Bit 2 is SET to 1

        if (ppu->STAT & (1 << 6)) { // interrupt by coincidence
            // LCD STAT Bit 1 of IF 
        } else {
            ppu->STAT |= ~(1 << 2);
        }
    }

    if (ppu->LY >= 144) { // V-Blank from 144 to 153
        ppu->current_mode = MODE_VERTICAL_BLANK;
    } else {
        // from 0 to 143
        if (ppu->current_dot < 80) {
            ppu->current_mode = MODE_OAM_SCAN;
        } else if (ppu->current_dot < 252) {
            ppu->current_mode = MODE_DRAWING_PIXELS;
        } else {
            ppu->current_mode = MODE_HORIZONTAL_BLANK;
        }
    }

    // update STAT register with current mode
    ppu->STAT = (ppu->STAT & 0xfc ) | ppu->current_mode;

    // check if mode changed since last cycle to activate interrupts 

    if (ppu->current_mode != ppu->last_mode) {
        ppu->last_mode = ppu->current_mode;

        if (ppu->current_mode == MODE_HORIZONTAL_BLANK && (ppu->STAT & (1 << 3))) {
            // LCD STAT interrupt
        } else if (ppu->current_mode == MODE_VERTICAL_BLANK && (ppu->STAT & (1 << 4))) {
            // LCD STAT interrupt as so as Bit 0 of IF 
        } else if (ppu->current_mode == MODE_OAM_SCAN && (ppu->STAT & (1 << 5))) {
            // LCD STAT interrupt
        }
    }

}

void PPU_RenderScanline(PPU *ppu) {

}
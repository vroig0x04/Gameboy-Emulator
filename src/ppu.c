

/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/


#include "ppu.h"

/* PPU registers are initzialiced to 0 since Boot ROM (256 bytes) will change their values */
void PPU_Init(PPU *ppu) {
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

void PPU_Advance(PPU *ppu, Sharp_MMU *mmu, int cycles) {
    if (!(ppu->LCDC & (1 << 7))) {
        ppu->current_dot = 0;
        ppu->LY = 0;
        ppu->window_line = 0;
        ppu->STAT = (ppu->STAT & 0xfc) | MODE_HORIZONTAL_BLANK;
        ppu->last_mode = MODE_HORIZONTAL_BLANK;
        return;
    }

    ppu->current_dot += cycles;

    while (ppu->current_dot >= DOTS) { 
        ppu->current_dot -= DOTS;
        ppu->LY++; // next line
        
        if (ppu->LY == SCANLINES) {
            ppu->LY = 0;
            ppu->window_line = 0;
        }

        // check if LY == LYC each time its value changes
        if (ppu->LYC == ppu->LY) {
            ppu->STAT |= (1 << 2);

            if (ppu->STAT & (1 << 6)) { // interrupt by coincidence
                // LCD STAT Bit 1 of IF 
            } else {
                ppu->STAT |= ~(1 << 2);
            }
        }
    }

    if (ppu->LY >= 144) { // V-Blank from 144 to 153
        ppu->current_mode = MODE_VERTICAL_BLANK;
    } else {
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
        
        if (ppu->current_mode == MODE_DRAWING_PIXELS) {
            PPU_RenderScanline(ppu, mmu);

            if (ppu->LCDC & (1 << 1)) {
                PPU_RenderSprites(ppu, mmu);
            }
        }

        if (ppu->current_mode == MODE_HORIZONTAL_BLANK && (ppu->STAT & (1 << 3))) {
            // TODO: LCD STAT interrupt
        }
        else if (ppu->current_mode == MODE_VERTICAL_BLANK) {
             // TODO: cpu->IF |= (1 << 0);

             if (ppu->STAT & (1 << 4)) {
                // TODO: LCD STAT (Bit 1 of IF)
            }
        }
        else if (ppu->current_mode == MODE_OAM_SCAN && (ppu->STAT & (1 << 5))) {
            // TODO: LCD STAT interrupt
        }

        ppu->last_mode = ppu->current_mode;
    }
}

static int PPU_window_check(PPU *ppu, int pixel_x) {
    if (
        (ppu->LCDC >> 5) & 0x01 == 1 &&
        (ppu->LY >= ppu->WY) &&
        (pixel_x >= ppu->WX - 7)
    ) {
        return 1;
    }
    return 0;
}

/*  In order to perform rendering, the scanline method is used for simplicity 
    and performance. The original Gameboy used FIFO rendering based on a way more
    complex algorithm for accomplishing the same results.

    The Gameboy has two accessible background maps depending on whether the bit 3 
    of the LCDC register is active or not. Therefore, if bit 3 is 0, the background is found
    between addresses 0x9800 - 0x9BFF and if bit 3 is 1 the backround is found between 0x9C00 - 0x9FFF

    The window has also its own line counter that must be restarted when the window was at least one time active
    during the scanline. This window is likely to be activated by games to display constant and non-scrollable features. 

*/
void PPU_RenderScanline(PPU *ppu, Sharp_MMU *mmu) {
    int was_window_active = 0;

    for (int pixel_x = 0; pixel_x < SCN_WIDTH; pixel_x++) {

        uint16_t map_addr;
        int cord_x, cord_y;
        if (!PPU_window_check(ppu, pixel_x)) 
        {
            cord_x = (pixel_x + ppu->SCX) % 256;
            cord_y = (ppu->LY + ppu->SCY) % 256;
            map_addr = (ppu->LCDC & (1 << 3)) ? 0x9c00 : 0x9800;

        } else 
        {
            cord_x = pixel_x - (ppu->WX - 7);
            cord_y = ppu->LY - ppu->WY;
            map_addr = (ppu->LCDC & (1 << 6)) ? 0x9c00 : 0x9800;
        }
        
        int drw_col = cord_x % 8;
        int drw_row = cord_y % 8;
        
        int y_tile = cord_y / 8;
        int x_tile = cord_x / 8;

        uint16_t tile_index = (y_tile * 32) + y_tile;
        uint16_t tile_id_addr = map_addr + tile_index;
        uint8_t tile_id = mmu_read(mmu, tile_id_addr);

        uint16_t tile_data_addr;

        if (ppu->LCDC & (1 << 4)) { /* Unsigend mode (0x8000)*/
            tile_data_addr = 0x8000 + (tile_id * 16); 
        } else { /* Signed mode (0x8800 / Base 0x9000)*/
            tile_data_addr = 0x9000 + ((int8_t)tile_id * 16);
        }

        uint8_t byte1 = mmu_read(mmu, tile_data_addr + (drw_row * 2));
        uint8_t byte2 = mmu_read(mmu, tile_data_addr + (drw_row * 2) + 1);

        int l_bit = 7 - drw_col;

        uint8_t bit1 = (byte2 >> l_bit) & 1;
        uint8_t bit0 = (byte1 >> l_bit) & 1;
        uint8_t color_id = (bit1 << 1) | bit0;

        // get final color from BGP register since each color is 2 bits in the register: color_id * 2
        uint8_t color = (ppu->BGP >> (color_id * 2)) & 0x03;
        ppu->bg_color_ids[pixel_x] = color;

        int buff_pos = (ppu->LY * 160) + pixel_x;
        ppu->screen_buffer[buff_pos] = color;

        if (was_window_active) {
            ppu->window_line++;
        }   
    }
}

/*  PPU_sort_sprites is a private function designed to order sprites by index.
    Since it is expected to render a maximum of 10 sprites per scanline, 
    an insertion sort alogorithm might be enough despite the O(n^2) cost
*/
static void PPU_sort_sprites(PPU *ppu, Sprite *sprites, int size) {
    for (int i = 1; i < size; i++) {
        Sprite k_sprite = sprites[i];
        int j;

        for (j = i - 1; j >= 0 && sprites[j].OAM_index < k_sprite.OAM_index; j--) {
            sprites[i + 1] = sprites[j];
        }
        sprites[i + 1] = k_sprite;
    }
}

void PPU_RenderSprites(PPU *ppu, Sharp_MMU *mmu) {
    if (!(ppu->LCDC >> 1) & 0x01) return;

    Sprite sprite_buffer[10];
    int visible_sprites = 0;

    for (int sprite_i = 0; sprite_i < DMG_OAM; sprite_i++) {
        Sprite sprite;

        int spr_cord_y = (int)mmu_read(mmu, 0xfe00 + (sprite_i * 4)) - 16;
        int spr_cord_x = (int)mmu_read(mmu, 0xfe00 + (sprite_i * 4) + 1) - 8;

        int spr_height = (ppu->LCDC & (1 << 2)) ? 16 : 8;

        if ((ppu->LY >= spr_cord_y) && (ppu->LY < (spr_cord_y + spr_height))) {
            sprite.x = spr_cord_x;
            sprite.y = spr_cord_y;
            sprite.OAM_index = sprite_i;
            visible_sprites++;
        }
    }  

    PPU_sort_sprites(ppu, sprite_buffer, visible_sprites);

    // render sprites from lower to mayor priority
    for (int i = 0; i < visible_sprites; i++) {
        Sprite sprite = sprite_buffer[i];
    
        int i_row = ppu->LY - sprite.y;

        int spr_height = (ppu->LCDC & (1 << 2)) ? 16 : 8;
        if (sprite.flags & (1 << 6)) {
            i_row = (spr_height - 1) - i_row;
        }

        uint16_t vram_addr;
        if (spr_height == 16) 
        { // 8x16 tiles
            uint8_t base_addr = (sprite.tile_index & 0xfe);
            vram_addr = 0x8000 + (base_addr * 16);
        } else 
        { // 8x16 tiles
            vram_addr = (sprite.tile_index * 16);
        } 

        uint8_t byte_1 = mmu_read(mmu, vram_addr + (i_row * 2));
        uint8_t byte_2 = mmu_read(mmu, vram_addr + (i_row * 2) + 1);

        int is_x_flip = (sprite.flags & (1 << 5)) ? 1 : 0;
        for (int bit = 0; bit < 8; bit++) {
            int pixel_x = sprite.x + bit;

            if (pixel_x < 0 || pixel_x >= 160) continue;

            int l_bit = is_x_flip ? bit : (7 - bit);

            uint8_t bit1 = (byte_2 >> l_bit) & 1;
            uint8_t bit0 = (byte_1 >> l_bit) & 1;
            uint8_t color_id = (bit1 << 1) | bit0;

            if (color_id) continue; // transparent pixel

            uint8_t background_color_id = ppu->bg_color_ids[pixel_x];

            if ((sprite.flags & (1 << 7)) && (background_color_id != 0)) continue; // background overlaps sprite's pixel

            uint8_t obj_palette = (sprite.flags & (1 << 4)) ? ppu->OBP1 : ppu->OBP0;
            uint8_t color = (obj_palette >> (color_id * 2)) & 0x03;

            ppu->screen_buffer[(ppu->LY * 160) + pixel_x] = color;

        }
    }
}

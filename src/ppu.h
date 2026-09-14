
/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/

#include <stdint.h>

#ifndef PPU_H
#define PPU_H

#define DOTS 456        // number of cycles of PPU to draw one line (one scanline)
#define SCANLINES 154   // number of scanlines
#define SCN_HEIGHT 144  // scanlines where screen is drown from top to bottom
#define SCN_WIDTH 160
#define FPS 59.7        // the Game Boy runs slightly slower than 60 Hz, as one frame takes ~16.74 ms. 70224 dots


/* The PPU is in charge of rendering the calculus performed by the CPU and is
   exclusivelly connected to the 8KB of VRAM 
   
   Tiles are 8x8-pixel chunk of graphics and are stored in VRAM 0x8000 - 0x97FF
   whith each tile taking 16 bytes. Capacity for 384 tiles. Tiles have a color depth of
   2 bits per pixel, allowing 4 different monochromatic shades. Background/Windows displayable. 
   If color 0: the object is transparent and the background and other objects can show through 
*/


/*
    7                       6         5                      4                  3         2        1                 0
LCD & PPU enable	Window tile map	Window enable	BG & Window tiles	BG tile map	  OBJ size	OBJ enable	BG & Window enable / priority

*/

typedef struct {
    uint8_t LCDC; // LCD control (0xFF40)
    uint8_t STAT; // LCD state (0xFF41)
    uint8_t SCY;  // Y background displacement (0xFF42)
    uint8_t SCX;  // X background displacement (0xFF43) 
    uint8_t LY;   // LCD current line (0xFF44)
    uint8_t LYC;  // LY comparison (0xFF45)
    uint8_t DMA;  // DMA transfer (0xFF46)
    uint8_t BGP;  // Background palette (0XFF47)
    uint8_t OBP0; // Object palette 0 (0xFF48)
    uint8_t OBP1; // Object palette 1 (0xFF49)
    uint8_t WY;   // Y window position (0XFF4A)
    uint8_t WX;   // X window position (0XFF4B)
    
    int current_dot; // counter of cycles from 0 to 455 

    uint8_t screen_buffer[SCN_HEIGHT * SCN_WIDTH]; // buffer to store rendered pixels

    uint8_t current_mode;
    uint8_t last_mode;
    int CGB_CMG_MODE;

} PPU;

/* all this modes are cycled during a one single PPU frame */
typedef enum {
    MODE_OAM_SCAN         = 2, // 0 - 79 dots, 160 bytes 
    MODE_DRAWING_PIXELS   = 3, // 80 - 251 dots    
    MODE_HORIZONTAL_BLANK = 0, // 252 - 455 dots
    MODE_VERTICAL_BLANK   = 1, // 144 - 153 scanlines
    
} PPU_Mode;

void PPU_Init(PPU *ppu);
/* advance PPU ticks acordding to current CPU cycles */
void PPU_Advance(PPU *ppu, int cycles); 
void PPU_RenderScanline(PPU *ppu);

#endif
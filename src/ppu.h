
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

#define FRAME 154     // number of scanlines
#define SCN_DRAW 144  // scanlines where screen is drown from top to bottom
#define FPS 59.7      // the Game Boy runs slightly slower than 60 Hz, as one frame takes ~16.74 ms. 70224 dots


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



    uint8_t LCDC;
    uint8_t LY;
    uint8_t SCY;
    uint8_t SCX;
    uint8_t DMA;
    uint8_t DMG;

    int CGB_CMG_MODE;
} PPU;

/* all this modes are cycled during a one single PPU frame */
typedef enum {
    HORIZONTAL_BLANK = 0, // 87 - 204 dots
    DRAWING_PIXELS   = 1, // 172 - 289 dots
    OAM_SCAN         = 2, // 80 dots, 160 bytes 
    LENGTH           = 3 // minimum mode 3 length: 160 + 12 = 172 dots. (screen will be scaled)
} PPU_Mode;

void PPU_Init_Regs(PPU *ppu);

#endif
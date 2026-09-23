
/*******************************************************************************
 * Copyright (c) 2026 Vicent Roig. All rights reserved.
 *
 * WARNING: This software is the proprietary property of Vicent Roig.
 * Unauthorized copying, modification, or distribution of this file, via any 
 * medium, is strictly prohibited. Proprietary and confidential.
 ******************************************************************************/

/**
 * 
 * GAME BOY PICTURE PROCESSING UNIT (PPU) - SUMMARY & ARCHITECTURE
 * 
 * The PPU is an independent graphics processor responsible for translating the
 * data stored in VRAM (Video RAM) and OAM (Object Attribute Memory) into visual
 * pixels on the Game Boy's LCD screen (160x144 pixels).
 * 
 * 1. GRAPHICS LAYERS:
 *  - Background (BG): A giant 256x256 pixel wrapping map (32x32 tiles). Its
 *    visible region is repositioned via the SCX and SCY registers.
 *  - Window (WIN): A static overlay layer that displays text or HUD elements.
 *    It ignores scrolling and is triggered by WX and WY registers, using an
 *    internal line counter to ensure visual continuity.
 *  - Sprites (OBJ): Up to 40 moving objects (max 10 rendered per scanline).
 *    Supports 8x8 or 8x16 modes, vertical/horizontal flipping, and hardware-based
 *    priority/transparency layers (Color index 0 is always transparent).
 * 
 * 2. PPU TIMING & OPERATIONAL MODES:
 * The PPU operates continuously on a frame-by-frame basis. One complete frame
 * takes 154 scanlines (70,224 dots / T-cycles). Each individual scanline
 * lasts exactly 456 dots and cycles through specific hardware modes:
 * 
 *  - Mode 2 (OAM Scan): The first 80 dots. The PPU searches the 40 slots
 *    of OAM memory to find up to 10 sprites that cross the current line (LY).
 *  - Mode 3 (Drawing Pixels): Lasts between 172 to 289 dots depending on the
 *    number of sprites on the line. Píxels are pushed to the LCD screen.
 *    (In this scanline emulator, the entire line is rendered instantly upon 
 *    entering Mode 3 for optimal performance).
 *  - Mode 0 (Horizontal Blank / H-Blank): The remainder of the 456 dots.
 *    The PPU enters a low-power state for the rest of the current line.
 * 
 * When LY reaches line 144, the screen drawing finishes, and the PPU spends
 * lines 144 to 153 (~10 lines) in a single state:
 *  - Mode 1 (Vertical Blank / V-Blank): Lasts 4560 dots. The CPU gains full,
 *    unrestricted access to VRAM and OAM, making it the perfect window for
 *    games to load new graphics, maps, or perform OAM DMA transfers.
 * 
 * 3. INTERRUPTS & SYNCHRONIZATION:
 * The PPU updates the STAT register (0xFF41) to signal its current mode and 
 * whether a matching scanline event occurred (LYC == LY). If enabled by the
 * game, entering Mode 0, 1, 2, or achieving an LYC coincidence fires a 
 * hardware LCD STAT Interrupt to allow on-the-fly graphical effects.
 * 
 * A general V-Blank interrupt is also fired natively upon entering Mode 1.
 * 
 * 4. EMULATION NOTES:
 * If the LCDC register Bit 7 is set to 0, the PPU turns off completely. 
 * The screen clears to blank white, registers LY and internal counters reset,
 * and the mode forces H-Blank until re-enabled by the CPU.
 * 
 * 5. REFERENCES:
 * The information presented here is largely based on the Pan Docs
 * documentation. I do not claim ownership of the original material;
 * it is referenced here as a resource to help readers understand
 * how the PPU works.
 *
 * The information related to the Game Boy's architecture is based
 * on Rodrigo Copetti's analysis. I do not claim ownership of that
 * material either; it is referenced here as a resource to help
 * readers understand the Game Boy's general architecture.
 * 
 */

#include <stdint.h>
#include "mmu.h"

#ifndef PPU_H
#define PPU_H

#define DOTS 456        // number of cycles of PPU to draw one line (one scanline)
#define SCANLINES 154   // number of scanlines
#define SCN_HEIGHT 144  // window height
#define SCN_WIDTH 160   // window width
#define DMG_OAM 160     // 160 bytes in total, 40 sprites 4 bytes each
#define MAX_SPRITES 40  // maximum number of sprites to read from OAM 
#define FPS 59.7        // the Game Boy runs slightly slower than 60 Hz, as one frame takes ~16.74 ms. 70224 dots


typedef struct PPU {
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
    int window_line; // line counter of window 

    uint8_t screen_buffer[SCN_HEIGHT * SCN_WIDTH]; // buffer to store rendered pixels
    uint8_t bg_color_ids[160];

    uint8_t current_mode;
    uint8_t last_mode;
    int CGB_DMG_MODE;

} PPU;

/* all this modes are cycled during a one single PPU frame */
typedef enum PPU_Mode {
    MODE_OAM_SCAN         = 2, // 0 - 79 dots, 160 bytes 
    MODE_DRAWING_PIXELS   = 3, // 80 - 251 dots    
    MODE_HORIZONTAL_BLANK = 0, // 252 - 455 dots
    MODE_VERTICAL_BLANK   = 1, // 144 - 153 scanlines
    
} PPU_Mode;

/*  Sprite structure definition 
  
    flags information:
        -Bit 7: if set to 0, the sprite is drawn on top of the background, if 
         0 the window covers the sprite
        -Bit 6: if set to 1, the sprite is inversely drawn
        -Bit 5: if set to 1, the sprite is horizonal and inversely drawn
        -Bit 4: Defines what palette of objects to use. If 0: 0BP0, if 1: OBP1

        Bits 3-0 are reserved for CGB mode. 

        The gameboy is only able of drawing a total of 10 sprites per horizontal line
*/
typedef struct Sprite {
    uint8_t y;  // y position on screen + 16
    uint8_t x;  // x psition on screen + 8
    uint8_t tile_index; // tile number (8x8 or 8x16)
    uint8_t flags;  // other useful sprite information

    int OAM_index;

} Sprite;

void PPU_Init(PPU *ppu);
void PPU_Advance(PPU *ppu, Sharp_MMU *mmu, int cycles);  // advance PPU ticks acordding to current CPU cycles 
void PPU_RenderScanline(PPU *ppu, Sharp_MMU *mmu);
void PPU_RenderSprites(PPU *ppu, Sharp_MMU *mmu);

#endif

#include "mmu.h"
#include "sharp_SM83.h"

/*
 MEMORY STRUCTURE

    In the standarg Gameboy (DMG), memory is divided in 5 sectors:
    
    - ROM: 0x0000 - 0x7fff up to 32KB with banking
    - RAM (WRAM): 0xc000 - 0x0dfff or 8KB
    - VRAM: 0x8000 - 0x9fff or 8KB
    - HRAM: 0xff80 - 0xfffe or 127 bytes

    BOOT ROMS:

    - DMG: Mapped to 0000h – 00FFh.
    - MGB: Mapped to 0000h – 00FFh. It has been dumped from a Game Boy Pocket, it is unknown if 
    Game Boy Light has a different one. Probably not, since DMG and MGB are only 1 byte different.
    - SGB: Mapped to 0000h – 00FFh.- CGB: Mapped to 0000h – 00FFh and 0200h – 08FFh
*/

int mbc_read_rom_bank_x(uint8_t *cartridge, uint16_t addr) {

}

int io_registers_read(Sharp_MMU *mmu, uint16_t addr) {

}

uint8_t mmu_read(Sharp_MMU *mmu, uint16_t addr) {
    /* reaading will be done here eventuaylly */

    switch(addr) {
        /* CPU ROM bank 0 usually fixed */
        case 0x0000 ... 0x3FFF:
            return mmu->rom_bank0[addr];

        /* Cartridge ROM bank 1 switchable with 0 */
        case 0x4000 ... 0x7FFF: 
            return mbc_read_rom_bank_x(mmu->cartridge, addr);

        /* VRAM, in CGB switchable bank 0/1 */
        case 0x8000 ... 0x9FFF: 
            return mmu->vram[addr - 0x8000];

        /* 8 KB of WRAM */
        case 0xC000 ... 0xDFFF:
            return mmu->wram[addr - 0xC000];

        /* Echo RAM. Not usable according to Nintendo */
        case 0xE000 ... 0xFDFF:
            return mmu->wram[addr - 0xE000]; // redirect to wram

        /* OAM Object attribute memory */
        case 0xFE00	... 0xFE9F:
            return mmu->OAM[0xFE00 - addr];

        /* I/O Registers */
        case 0xFF00	... 0xFF7F:
            return io_registers_read(mmu, addr);

        /* High RAM (HRAM) */
        case 0xFF80	... 0xFFFE:
            return mmu->wram[0xFF80 - addr];

        /* Interrupt Enable register (IE) */
        case 0xFFFF	... 0xFFFF:
            return mmu->cpu->IE;

        /* Not usable memory regions */
        default:    
            return 0xFF;

    }
}

void mmu_write8(Sharp_MMU *mmu, uint16_t a16, uint8_t d8) {
    if (a16 < 0x8000) {
        /* since writing in ROM is totally prohibited, we ignore 
           this bad address as this check is performed via sending
           commands to the cartridge in order to change the bank
        */
        return;
    }

   // mmu->memory[a16] = d8; temp
}

void mmu_write16(Sharp_MMU *mmu, uint16_t a16, uint16_t d16) {
    if (a16 < 0x8000) {
        /* since writing in ROM is totally prohibited, we ignore 
           this bad address as this check is performed via sending
           commands to the cartridge in order to change the bank
        */
        return;
    }

   // mmu->memory[a16] = d16 & 0x00ff;              temp
   // mmu->memory[a16 + 1] = (d16 >> 8) & 0x00ff;
}


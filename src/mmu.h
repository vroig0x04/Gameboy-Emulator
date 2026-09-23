
#ifndef MMU_H
#define MMU_H

#include <stdint.h>

/* Forward declaration to avoid circular include */
//struct Sharp_SM83;

#define DMG_ADDR      65536 // 64KB
#define DMG_ROM_SIZE  16384 // 16KB
#define DMG_VRAM_SIZE 8192  // 8KB
#define DMG_WRAM_SIZE 8182  // 8KB divided in two blocks 
#define TEMP_OAMDMG 1 


typedef struct Sharp_MMU {

    uint8_t rom_bank0[DMG_ROM_SIZE];
    uint8_t vram[DMG_VRAM_SIZE];
    uint8_t wram[DMG_WRAM_SIZE];

    /* This space is not from the Gameboy itself, but from the cartridge instead. 
       It uses MCB or Memory Bank Controllers to expand the available address space by
       bank switching.
    */
    uint8_t cartridge[DMG_ROM_SIZE];
    uint8_t OAM[TEMP_OAMDMG];

    struct Sharp_SM83 *cpu;
    
} Sharp_MMU;

uint8_t mmu_read(Sharp_MMU *mmu, uint16_t a16);

int mbc_read_rom_bank_x(uint8_t *cartridge, uint16_t addr);
int io_registers_read(Sharp_MMU *mmu, uint16_t addr);

void mmu_write8(Sharp_MMU *mmu, uint16_t a16, uint8_t d8);
void mmu_write16(Sharp_MMU *mmu, uint16_t a16, uint16_t d16);



#endif // EOF
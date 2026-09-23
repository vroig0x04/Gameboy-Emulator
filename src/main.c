
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "alu_SM83.h"
#include "sharp_SM83.h"
#include "window.h"

#define WIDTH 160
#define HEIGTH 144

void boot_cpu() {
    Sharp_SM83 *cpu = malloc(sizeof(Sharp_SM83));
    Sharp_MMU *mmu = malloc(sizeof(Sharp_MMU));
    SM83_Init_Regs(cpu, 0);

    /* cycle instruction where programs are executed */
    do {
        execute_cycle_instruction(cpu, mmu);
    }while(!(cpu->STOP && cpu->HALT));  
}


int main(int argc, char *argv[]) {
    Window_Boot();
    //Window_RenderGameboy();

    return 0;
}





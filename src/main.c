
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include "alu_SM83.h"
#include "alu_SM83.h"

#define WIDTH 160
#define HEIGTH 144

void boot_cpu() {
    Sharp_SM83 *cpu = malloc(sizeof(Sharp_SM83));
    Sharp_MMU *mmu = malloc(sizeof(Sharp_MMU));
    init_regs(cpu, 0);

    /* cycle instruction where programs are executed */
    do {
        execute_cycle_instruction(cpu, mmu);
    }while(!(cpu->STOP && cpu->HALT));  
}

int boot_window() {
    /* set up everything */
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL init failure: %s\n", SDL_GetError());
        return -1;
    }
    /* create window */
    SDL_Window *window = SDL_CreateWindow("GameBoy Emulator", WIDTH, HEIGTH, (SDL_WINDOW_RESIZABLE));

    if (!window) {
        fprintf(stderr, "Error while creating the window: %s\n", SDL_GetError());
        return -1;
    }

    int running = 1;
    SDL_Event event;

    while (running) {
        /* check if user performed any action */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }

        /* set 60 FPS */
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    boot_cpu();
    //boot_window();
    

    return 0;
}





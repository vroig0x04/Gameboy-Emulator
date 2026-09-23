
#include "window.h"


int Window_Boot() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error on SDL_Init: %s\n", SDL_GetError());
        return SDL_ERROR_INIT;
    }

    SDL_Window *window = SDL_CreateWindow("VRJ Gameboy Emulator", GAMEBOY_WIDTH, GAMEBOY_HEIGHT, (SDL_WINDOW_RESIZABLE));
    if (!window) {
        SDL_Log("Error while creating the window: %s\n", SDL_GetError());
        SDL_Quit();
        return SDL_ERROR_ON_WINDOW;
    }

    if (!SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED)) {
        SDL_Log("Error while centering window: %s\n", SDL_GetError());
        return SDL_ERROR_CENT_WINDOW;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Error on renderer %s:\n", SDL_GetError());
        return -1;
    }

    Window_RenderGameboy(renderer);
    SDL_RenderPresent(renderer);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return HALTED_EMULATOR;
}

int Window_RenderGameboy(SDL_Renderer *renderer) {
    SDL_Rect *rect;

    SDL_SetRenderDrawColor(renderer, 189, 191, 195, SDL_ALPHA_OPAQUE);
    if (SDL_RenderClear(renderer) < 0) { 
        fprintf(stderr, "Error al limpiar pantalla: %s\n", SDL_GetError());
        return -1;
    }

    if (!SDL_RenderFillRect(renderer, NULL)) {
        fprintf(stderr, "Y se fue a la puta %s\n", SDL_GetError());
        return -1;
    }

    
}
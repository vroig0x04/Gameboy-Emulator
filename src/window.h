#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <stdio.h>


#define GAMEBOY_WIDTH 800
#define GAMEBOY_HEIGHT 1000

#define SDL_ERROR_INIT 1
#define SDL_ERROR_ON_WINDOW 2
#define SDL_ERROR_CENT_WINDOW 3

#define HALTED_EMULATOR 0

int Window_Boot();
int Window_RenderGameboy();

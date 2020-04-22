#include <stdio.h>
#include "SDL.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Something failed!");
        return 1;
    }
    
    SDL_Event event;

    const char *window_title = "Lambda - A Fucking Awesome Renderer";
    SDL_Window *window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, NULL);

    while (true) {

        SDL_Surface *surface = SDL_GetWindowSurface(window);

        char *curr = (char *)surface->pixels;
        for (int y = 0; y < surface->h; ++y) {
            SDL_Delay(100);
            for (int x = 0; x < surface->w; ++x) {
                *curr++ = (x / (float)surface->w) * 255;
                *curr++ = (y / (float)surface->w) * 255;
                *curr++ = 0;
                *curr++ = 255;
            }
            SDL_UpdateWindowSurface(window);

            while (SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT) {
                    return 0;
                }
            }
        }
    }

    return 0;
}

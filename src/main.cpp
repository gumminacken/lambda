#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <stack>

#include "SDL.h"

const int WIDTH = 640;
const int HEIGHT = 480;
const unsigned int NUMTHREADS = std::thread::hardware_concurrency();

struct Scanline {
    int y;
    int width;
    char pixels[WIDTH * 4];
};

struct Scanlines {
    std::stack<Scanline *> *scanlines_to_render;
    std::stack<Scanline *> *scanlines_done;
};

SDL_mutex *rendermutex = SDL_CreateMutex();
SDL_mutex *buffermutex = SDL_CreateMutex();

static int thread_worker(void *data) {
    Scanlines *scanlines = (Scanlines *)data;

    while (true) {
        SDL_LockMutex(rendermutex);
        // pop scanline from the stack
        if (scanlines->scanlines_to_render->empty()) {
            break;
        }

        Scanline *scanline = scanlines->scanlines_to_render->top();
        scanlines->scanlines_to_render->pop();

        SDL_UnlockMutex(rendermutex);

        char *curr = scanline->pixels;
        for (int x = 0; x < scanline->width; ++x) {
            *curr++ = (x / (float)scanline->width) * 255;
            *curr++ = (scanline->y / (float)scanline->width) * 255;
            *curr++ = 0;
            *curr++ = 255;
        }
        SDL_Delay(rand() % 500);

        SDL_LockMutex(buffermutex);
        // write scanline data into buffer
        scanlines->scanlines_done->push(scanline);
        SDL_UnlockMutex(buffermutex);
    }
    return 0;
}


int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Something failed!");
        return 1;
    }
    
    SDL_Event event;

    const char *window_title = "Lambda - A Fucking Awesome Renderer";
    SDL_Window *window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL);

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    std::vector<SDL_Thread *> threads(NUMTHREADS);
    Scanlines scanlines;

    bool done = false;

    for (int y = 0; y < HEIGHT; ++y) {
        Scanline *scanline = (Scanline *)SDL_malloc(sizeof(Scanline));
        scanline->y = y;
        scanline->width = WIDTH;
        scanlines.scanlines_to_render->push(scanline);
    }

    for (int t = 0; t < NUMTHREADS; ++t) {
        threads[t] = SDL_CreateThread(thread_worker, "", &scanlines);
    }

    while (!done) {
        if (!rendermutex && scanlines.scanlines_to_render->empty()) {
            for (int t = 0; t < NUMTHREADS; ++t) {
                SDL_WaitThread(threads[t], NULL);
            }
            done = true;
        }
        if (!buffermutex) {
            while (!scanlines.scanlines_done->empty()) {
                Scanline *scanline = scanlines.scanlines_done->top();
                scanlines.scanlines_done->pop();
                // write data in the pixel buffer
                SDL_memcpy((char *)surface->pixels + (surface->pitch * scanline->y), scanline->pixels, WIDTH * 4);
                // Update WindowSurface
                SDL_UpdateWindowSurface(window);
            }
        }

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }
    }
    return 0;
}

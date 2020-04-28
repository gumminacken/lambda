#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <vector>

#include "SDL.h"

const int WIDTH = 640;
const int HEIGHT = 480;
const unsigned int NUMTHREADS = std::thread::hardware_concurrency();

struct Scanline {
    int y;
    int width;
    int threadid;
    char threadname[128];
    bool done;
    char pixels[WIDTH * 4];
};


static int render_scanline(void *data)
{
    Scanline *scanline = (Scanline *)data;

    char *curr = scanline->pixels;
    for (int x = 0; x < scanline->width; ++x) {
        *curr++ = (x / (float)scanline->width) * 255;
        *curr++ = (scanline->y / (float)scanline->width) * 255;
        *curr++ = 0;
        *curr++ = 255;
    }
    SDL_Delay(rand() % 500);
    scanline->done = true;
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
    std::vector<Scanline *> scanlines(NUMTHREADS);
    //SDL_Thread *threads[NUMTHREADS] = {};
    //Scanline *scanlines[NUMTHREADS] = {};

    int y = 0;
    bool done = false;

    while (!done) {
        for (int t = 0; t < NUMTHREADS; ++t) {
            if (y == HEIGHT) {
                done = true;
                break;
            }
            if (threads[t] == 0) {
                // start of render
                scanlines[t] = (Scanline *)SDL_malloc(sizeof(Scanline));
                scanlines[t]->y = y;
                scanlines[t]->width = WIDTH;
                scanlines[t]->done = false;
                sprintf(scanlines[t]->threadname, "%d", y);
                threads[t] = SDL_CreateThread(render_scanline, scanlines[t]->threadname, scanlines[t]);
                scanlines[t]->threadid = SDL_GetThreadID(threads[t]);
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Thread Created %s", scanlines[t]->threadname);
                ++y;
            } else if (scanlines[t]->done) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Thread done %s", scanlines[t]->threadname);
                // the thread is done
                // wait for the thread to finish
                SDL_WaitThread(threads[t], NULL);
                // write data in the pixel buffer
                SDL_memcpy((char *)surface->pixels + (surface->pitch * scanlines[t]->y), scanlines[t]->pixels, WIDTH * 4);
                // Update WindowSurface
                SDL_UpdateWindowSurface(window);
                // create new scanline
                scanlines[t] = (Scanline *)SDL_malloc(sizeof(Scanline));
                scanlines[t]->y = y;
                scanlines[t]->width = WIDTH;
                scanlines[t]->done = false;
                threads[t] = SDL_CreateThread(render_scanline, scanlines[t]->threadname, scanlines[t]);
                scanlines[t]->threadid = SDL_GetThreadID(threads[t]);
                sprintf(scanlines[t]->threadname, "%d", y);
                ++y;
            }
        }

        SDL_UpdateWindowSurface(window);

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }
    }
    return 0;
}

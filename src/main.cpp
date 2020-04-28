#include <stdio.h>
#include "SDL.h"

const int WIDTH = 640;
const int HEIGHT = 480;
const int NUMTHREADS = 1;

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

    SDL_Thread *threads[NUMTHREADS] = {};
    Scanline *scanlines[NUMTHREADS] = {};

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
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Thread Created %d", scanlines[t]->threadid);
                ++y;
            } else if (scanlines[t]->done) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Thread done %d", scanlines[t]->threadid);
                // the thread is done
                // detach thread
                SDL_WaitThread(threads[t], NULL);
                // write data in the pixel buffer
                SDL_memcpy(&(surface->pixels) + y * 4, scanlines[t]->pixels, WIDTH * 4);
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

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <time.h>

#include "lambda.h"

Scanlines *create_scanlines() {
    Scanlines *scanlines = (Scanlines *)SDL_malloc(sizeof(Scanlines));
    scanlines->todo = (ScanlineStack *)SDL_malloc(sizeof(ScanlineStack));
    scanlines->todo->top = -1;
    scanlines->done = (ScanlineStack *)SDL_malloc(sizeof(ScanlineStack));
    scanlines->done->top = -1;
    return scanlines;
}

void stack_push(ScanlineStack *stack, Scanline* scanline) {
    ++stack->top;
    stack->elements[stack->top] = scanline;
}

Scanline *stack_pop(ScanlineStack *stack) {
    Scanline *scanline = stack->elements[stack->top];
    --stack->top;
    return scanline;
}

bool stack_empty(ScanlineStack *stack) {
   return (stack->top == -1);
}

// TODO: Replace with Spinlocks?
SDL_mutex *rendermutex = SDL_CreateMutex();
SDL_mutex *buffermutex = SDL_CreateMutex();

// temp circle
static const Lt_Scene scene = create_scene(100, 5.f, 20.f);

static int thread_worker(void *data) {
    Scanlines *scanlines = (Scanlines *)data;
    ScanlineStack *scanlines_todo = scanlines->todo;
    ScanlineStack *scanlines_done = scanlines->done;

    while (true) {
        SDL_LockMutex(rendermutex);
        // pop scanline from the stack
        if (stack_empty(scanlines_todo)) {
            break;
        }

        Scanline *scanline = stack_pop(scanlines_todo);

        SDL_UnlockMutex(rendermutex);

        char *curr = scanline->pixels;
        for (int x = 0; x < scanline->width; ++x) {
            Sample2D *samples = sample2D_random(x * scanline->y, SAMPLES);
            float r = 0;
            float g = 0;
            float b = 0;
            for (int i = 0; i < SAMPLES; ++i) {
                Sample2D sample = Sample2D {
                    (float)x + samples[i].x,
                    (float)scanline->y + samples[i].y
                };
                if (intersect(scene, sample)) {
                    r += (x / (float)scanline->width) * 255;
                    g += (scanline->y / (float)scanline->width) * 255;
                }
            }
            SDL_free(samples);

            *curr++ = r / SAMPLES;
            *curr++ = g / SAMPLES;
            *curr++ = b / SAMPLES;
            *curr++ = 255;
        }
        // SDL_Delay(rand() % 200);

        SDL_LockMutex(buffermutex);
        // write scanline data into buffer
        stack_push(scanlines_done, scanline);
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
    SDL_Window *window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    std::vector<SDL_Thread *> threads(NUMTHREADS);
    Scanlines *scanlines = make_scanlines();
    ScanlineStack *scanlines_todo = scanlines->todo;
    ScanlineStack *scanlines_done = scanlines->done;

    bool done = false;

    for (int y = HEIGHT-1; y >= 0; --y) {
        Scanline *scanline = (Scanline *)SDL_malloc(sizeof(Scanline));
        scanline->y = y;
        scanline->width = WIDTH;
        stack_push(scanlines_todo, scanline);
    }

    for (unsigned int t = 0; t < NUMTHREADS; ++t) {
        threads[t] = SDL_CreateThread(thread_worker, "", (void *)scanlines);
    }

    while (!done) {
        if (stack_empty(scanlines_todo) && stack_empty(scanlines_done)) {
            for (unsigned int t = 0; t < NUMTHREADS; ++t) {
                SDL_DetachThread(threads[t]);
            }
            //done = true;
        }

        SDL_LockMutex(buffermutex);
        while (!stack_empty(scanlines_done)) {
            Scanline *scanline = stack_pop(scanlines_done);
            // write data in the pixel buffer
            SDL_memcpy((char *)surface->pixels + (surface->pitch * scanline->y), scanline->pixels, WIDTH * 4);
            // Update WindowSurface
            SDL_UpdateWindowSurface(window);
        }
        SDL_UnlockMutex(buffermutex);

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }
    }
    return 0;
}

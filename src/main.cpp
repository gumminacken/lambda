#include "lambda.h"

Lt::Scanlines *create_scanlines() {
    Lt::Scanlines *scanlines = (Lt::Scanlines *)SDL_malloc(sizeof(Lt::Scanlines));
    scanlines->todo = Li::stack_create_empty(HEIGHT, sizeof(Lt::Scanline));
    scanlines->done = Li::stack_create_empty(HEIGHT, sizeof(Lt::Scanline));
    return scanlines;
}

// TODO: Replace with Spinlocks?
SDL_mutex *rendermutex = SDL_CreateMutex();
SDL_mutex *buffermutex = SDL_CreateMutex();

// temp circle
static const Lt::Scene scene = Li::create_scene(1000, 16, 1.f, 3.f);

static int thread_worker(void *data) {
    Lt::Scanlines *scanlines = (Lt::Scanlines *)data;
    Lt::Stack *scanlines_todo = scanlines->todo;
    Lt::Stack *scanlines_done = scanlines->done;

    while (true) {
        SDL_LockMutex(rendermutex);
        // pop scanline from the stack
        if (Li::stack_empty(scanlines_todo)) {
            break;
        }

        Lt::Scanline *scanline = (Lt::Scanline *)Li::stack_pop(scanlines_todo);

        SDL_UnlockMutex(rendermutex);

        char *curr = scanline->pixels;
        for (int x = 0; x < scanline->width; ++x) {
            Lt::Sample2D *samples = Lt::sample2D_random(x * scanline->y, SAMPLES);
            float r = 0;
            float g = 0;
            float b = 0;
            for (int i = 0; i < SAMPLES; ++i) {
                Lt::Sample2D sample = Lt::Sample2D {
                    (float)x + samples[i].x,
                    (float)scanline->y + samples[i].y
                };
                if (Li::intersect(scene, sample)) {
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
        //SDL_Delay(rand() % 300);

        SDL_LockMutex(buffermutex);
        // write scanline data into buffer
        Li::stack_push(scanlines_done, scanline);
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
    Lt::Scanlines *scanlines = create_scanlines();
    Lt::Stack *scanlines_todo = scanlines->todo;
    Lt::Stack *scanlines_done = scanlines->done;

    bool done = false;

    Lt::Scanline *scanliness = (Lt::Scanline *)malloc(sizeof(Lt::Scanline) * HEIGHT);

    for (int y = HEIGHT-1; y >= 0; --y) {
        Lt::Scanline *scanline = scanliness + y;
        scanline->y = y;
        scanline->width = WIDTH;
        Li::stack_push(scanlines_todo, scanline);
    }

    for (unsigned int t = 0; t < NUMTHREADS; ++t) {
        threads[t] = SDL_CreateThread(thread_worker, "", (void *)scanlines);
    }

    while (!done) {
        if (Li::stack_empty(scanlines_todo) && Li::stack_empty(scanlines_done)) {
            for (unsigned int t = 0; t < NUMTHREADS; ++t) {
                if (threads[t] != NULL) {
                    SDL_DetachThread(threads[t]);
                    threads[t] = NULL;
                }
            }
            // done = true;
        }

        SDL_LockMutex(buffermutex);
        while (!Li::stack_empty(scanlines_done)) {
            Lt::Scanline *scanline = (Lt::Scanline *)Li::stack_pop(scanlines_done);
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

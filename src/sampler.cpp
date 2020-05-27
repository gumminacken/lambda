#include <stdlib.h>

#include "lambda.h"


Sample2D* sample2D_random(unsigned int seed, size_t elements) {
    Sample2D *samples = (Sample2D *)SDL_malloc(sizeof(Sample2D) * elements);
    srand(seed);
    double fraction = 1.0 / RAND_MAX;
    for (size_t i=0; i < elements; ++i) {
        samples[i].x = rand() * fraction;
        samples[i].y = rand() * fraction;
    }
    return samples;
}

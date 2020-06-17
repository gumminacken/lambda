#include "lambda.h"


Lt::Sample2D* Li::sample2D_random(unsigned int seed, size_t elements) {
    Lt::Sample2D *samples = (Lt::Sample2D *)SDL_malloc(sizeof(Lt::Sample2D) * elements);
    srand(seed);
    double fraction = 1.0 / RAND_MAX;
    for (size_t i=0; i < elements; ++i) {
        samples[i].x = rand() * fraction;
        samples[i].y = rand() * fraction;
    }
    return samples;
}

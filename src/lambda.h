# pragma once

// Sampler
struct Sample2D {
    float x;
    float y;
};

Sample2D* sample2D_random(unsigned int seed, size_t elements);




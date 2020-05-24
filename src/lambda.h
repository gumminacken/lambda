# pragma once

#include <stdio.h>

// Sampler
struct Sample2D {
    float x;
    float y;
};

Sample2D* sample2D_random(unsigned int seed, size_t elements);

// Math
struct Lt_Vec3f {
    float x;
    float y;
    float z;
};

Lt_Vec3f Li_Vec3f(float x, float y, float z);
Lt_Vec3f operator+(Lt_Vec3f a, Lt_Vec3f b);
Lt_Vec3f operator-(Lt_Vec3f a, Lt_Vec3f b);
float Vec3f_length(Lt_Vec3f vector);

// Intersect
struct Lt_Circle {
    float radius;
    Lt_Vec3f center;
};

Lt_Circle Li_Circle(float radius, Lt_Vec3f center);

bool intersect(Lt_Circle object, Sample2D sample);

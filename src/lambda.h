#pragma once

#include <float.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <vector>

#include "glm.hpp"
#include "rtcore.h"
#include "SDL.h"

const int WIDTH = 640;
const int HEIGHT = 480;
const int SAMPLES = 32;
const unsigned int NUMTHREADS = std::thread::hardware_concurrency();

namespace Lt {
    // Sampler //
    struct Sample2D {
        float x;
        float y;
    };

    // Math //
    typedef glm::vec3 Vec3f;

    // Scene //
    struct Circle {
        float radius;
        Vec3f center;
    };

    struct Grid {
        size_t num_circles;
        int index;
        int stride;
        float minx;
        float miny;
        float maxx;
        float maxy;
        Circle *circles;
    };

    struct Scene {
        size_t num_circles;
        size_t num_grids;
        size_t gridsize;
        Circle *circles;
        Grid *grids;
    };

    // Stack //
    struct Stack {
        size_t elements;
        size_t elementsize;
        void *top;
        void *data;
    };

    // Scanlines //
    struct Scanline {
        int y;
        int width;
        char pixels[WIDTH * 4];
    };

    struct Scanlines {
        Stack *todo;
        Stack *done;
    };

    // Renderer //
    struct Ray {
        Vec3f origin;
        Vec3f direction;
    };

}


namespace Li {
    // Sampler //
    Lt::Sample2D* sample2D_random(unsigned int seed, size_t elements);

    // Math //
    Lt::Vec3f Vec3f(float x, float y, float z);

    // Scene //
    Lt::Circle Circle(float radius, Lt::Vec3f center);
    Lt::Scene scene_create(size_t num_circles, size_t gridsize, float minsize, float maxsize);

    bool intersect(Lt::Circle *object, Lt::Sample2D sample);
    bool intersect(Lt::Scene object, Lt::Sample2D sample);

    Lt::Grid *get_intersected_grid(Lt::Scene *scene, int x, int y);
    int *get_intersected_grid_indices(Lt::Scene *scene, Lt::Circle *circle);

    // Stack //
    void stack_push(Lt::Stack *stack, void* element);
    void *stack_pop(Lt::Stack *stack);
    bool stack_empty(Lt::Stack *stack);
    Lt::Stack *stack_create_empty(size_t elements, size_t elementsize);

    // Renderer //
    bool ray_cast(RTCScene *scene, Lt::Ray ray);
    Lt::Ray ray_create(Lt::Vec3f origin , Lt::Vec3f direction);
}

void errorFunction(void* userPtr, enum RTCError error, const char* str);
RTCDevice initializeDevice();

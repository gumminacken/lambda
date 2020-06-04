# pragma once

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <thread>
#include <math.h>

#include "SDL.h"

// Rendersettings
const int WIDTH = 640;
const int HEIGHT = 480;
const int SAMPLES = 32;
const unsigned int NUMTHREADS = std::thread::hardware_concurrency();

// Sampler
struct Lt_Sample2D {
    float x;
    float y;
};

Lt_Sample2D* sample2D_random(unsigned int seed, size_t elements);

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

// Scene
struct Lt_Circle {
    float radius;
    Lt_Vec3f center;
};

struct Lt_Grid {
    size_t num_circles;
    int index;
    int stride;
    float minx;
    float miny;
    float maxx;
    float maxy;
    Lt_Circle *circles;
};

struct Lt_Scene {
    size_t num_circles;
    size_t num_grids;
    size_t gridsize;
    Lt_Circle *circles;
    Lt_Grid *grids;
};

Lt_Scene create_scene(size_t num_circles, size_t gridsize, float minsize, float maxsize);
void create_acceleration_structure(Lt_Scene *scene);

Lt_Circle Li_Circle(float radius, Lt_Vec3f center);

bool intersect(Lt_Circle *object, Lt_Sample2D sample);
bool intersect(Lt_Scene object, Lt_Sample2D sample);
Lt_Grid *get_intersected_grid(Lt_Scene *scene, int x, int y);
int *get_intersected_grid_indices(Lt_Scene *scene, Lt_Circle *circle);

// Stack
struct Stack {
    size_t elements;
    size_t elementsize;
    void *top;
    void *data;
};

void stack_push(Stack *stack, void* element);
void *stack_pop(Stack *stack);
bool stack_empty(Stack *stack);
Stack *stack_create_empty(size_t elements, size_t elementsize);

// Scanlines
struct Lt_Scanline {
    int y;
    int width;
    char pixels[WIDTH * 4];
};

struct Lt_Scanlines {
    Stack *todo;
    Stack *done;
};

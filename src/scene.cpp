#include <malloc.h>
#include <stdlib.h>

#include "lambda.h"

Lt_Circle Li_Circle(float radius, Lt_Vec3f center) {
    return Lt_Circle {
        radius,
        center
    };
}

bool intersect(Lt_Circle object, Lt_Sample2D sample) {
    Lt_Vec3f sample_vector = Li_Vec3f(sample.x, sample.y, 0);
    return (Vec3f_length(sample_vector - object.center) <= object.radius);
}

bool intersect(Lt_Scene scene, Lt_Sample2D sample) {
    for (size_t i = 0; i < scene.elements; ++i) {
        if (intersect(scene.circles[i], sample)) {
            return true;
        };
    }
    return false;
}

Lt_Scene create_scene(size_t elements, float minsize, float maxsize) {
    Lt_Scene scene = {
        elements,
        (Lt_Circle *)malloc(sizeof(Lt_Circle) * elements)
    };

    Lt_Sample2D *samples = sample2D_random(12345, elements);

    for (size_t i = 0; i < elements; ++i) {
        float size = minsize + ((rand() / float(RAND_MAX)) * (maxsize - minsize));
        printf("%g\n", size);
        Lt_Circle circle = {size, Li_Vec3f(samples[i].x * 640, samples[i].y * 480, 0)};
        scene.circles[i] = circle;
    }
    return scene;
}

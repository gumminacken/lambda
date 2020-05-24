#include "lambda.h"

Lt_Circle Li_Circle(float radius, Lt_Vec3f center) {
    return Lt_Circle {
        radius,
        center
    };
}

bool intersect(Lt_Circle object, Sample2D sample) {
    Lt_Vec3f sample_vector = Li_Vec3f(sample.x, sample.y, 0);
    return (Vec3f_length(sample_vector - object.center) <= object.radius);
}

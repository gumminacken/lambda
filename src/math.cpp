#include "lambda.h"

Lt::Vec3f Li::Vec3f(float x, float y, float z) {
    return Lt::Vec3f {x, y, z};
}

Lt::Vec3f Lt::operator+(Lt::Vec3f a, Lt::Vec3f b) {
    return Lt::Vec3f {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

Lt::Vec3f Lt::operator-(Lt::Vec3f a, Lt::Vec3f b) {
    return Lt::Vec3f {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

float Li::Vec3f_length(Lt::Vec3f vector) {
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

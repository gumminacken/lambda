#include "lambda.h"
#include "math.h"

Lt_Vec3f Li_Vec3f(float x, float y, float z) {
    return Lt_Vec3f {x, y, z};
}

Lt_Vec3f operator+(Lt_Vec3f a, Lt_Vec3f b) {
    return Lt_Vec3f {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

Lt_Vec3f operator-(Lt_Vec3f a, Lt_Vec3f b) {
    return Lt_Vec3f {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

float Vec3f_length(Lt_Vec3f vector) {
    return sqrtf32(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

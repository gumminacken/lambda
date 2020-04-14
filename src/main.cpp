#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <math.h>
#include <ai.h>
#include <stb_image_write.h>

struct Bucket {
    int index;
    int min[2];
    int max[2];
    uint8_t *pixels;
    uint8_t *current_pixel;
};

int main() {
    AiBegin();
    
    AiMsgInfo("Entered the Universe!!");
    AiASSLoad("scenes/cornell.ass");
    
    AtNode *options = AiUniverseGetOptions();
    AiNodeSetBool(options, "skip_license_check", true);

    AiRender(AI_RENDER_MODE_FREE);

    int w = 320;
    int h = 320;
    int samples = 4;

    int bucket_size = 160;

    AtSampler *sampler = AiSampler(12345, samples, 2);
    AtShaderGlobals *sample_globals = AiShaderGlobals();

    AtVector origin = AtVector(0, 5, 10);


    // NOTE: Why is this not segfaulting if we deliberately choose a buffer that's too small?
    uint8_t *buffer = (uint8_t *)malloc(w * h * 3 * sizeof(char));
    uint8_t *curr = buffer;

    int x_buckets = ceil(w / (float)bucket_size);
    int y_buckets = ceil(h / (float)bucket_size);

    for (int bucket_index = 0; bucket_index < x_buckets * y_buckets; ++bucket_index) {
        Bucket bucket {
            bucket_index,
            (bucket_index % x_buckets)  * bucket_size, (bucket_index / x_buckets)  * bucket_size,
            0, 0,
            (uint8_t *)malloc(pow(bucket_size, 2) * 3 * sizeof(char)),
            0
        };
        bucket.max[0] = bucket.min[0] + bucket_size;
        bucket.max[1] = bucket.min[1] + bucket_size;
        bucket.current_pixel = bucket.pixels;

        for (int y = bucket.min[1]; y < bucket.max[1]; ++y) {
            for (int x = bucket.min[0]; x < bucket.max[0]; ++x) {
                AtSamplerIterator *sampler_iter = AiSamplerIterator(sampler, sample_globals);
                float sample[2] = {};
                AtVector color = AtVector(0, 0, 0);

                while (AiSamplerGetSample(sampler_iter, sample)) {
                    AtVector direction = AtVector(
                        1 - (((float)x / w) * 2) + (sample[0] / w),
                        1 - (((float)y / h) * 2) + (sample[1] / h),
                        -1
                    );

                    AiV3Normalize(direction);
                    AtShaderGlobals *initial_globals = AiShaderGlobals();
                    AtRay ray = AiMakeRay(AI_RAY_CAMERA, origin, &direction, AI_BIG, initial_globals);
                    AtShaderGlobals *trace_globals = AiShaderGlobals();

                    bool hit = false;
                    hit = AiTraceProbe(ray, trace_globals);

                    color += trace_globals->N / samples;
                }

                *bucket.current_pixel++ = (char)(color.x * 255);
                *bucket.current_pixel++ = (char)(color.y * 255);
                *bucket.current_pixel++ = (char)(color.z * 255);
            }
        }
        AiMsgInfo("YAY");
        memcpy(buffer + bucket_index * (int)pow(bucket_size, 2), bucket.pixels, sizeof(char) * 3 * pow(bucket_size, 2));
    }

    stbi_write_jpg("test2.jpg", w, h, 3, buffer, 100);

    AiRenderEnd();
    AiEnd();
    return 0;
}

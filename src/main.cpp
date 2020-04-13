#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <ai.h>
#include <stb_image_write.h>

int main() {
    AiBegin();
    
    AiMsgInfo("Entered the Universe!!");
    AiASSLoad("scenes/cornell.ass");
    
    AtNode *options = AiUniverseGetOptions();
    AiNodeSetBool(options, "skip_license_check", true);

    AiRender(AI_RENDER_MODE_FREE);

    /*
    const char *driver_type_name = AiNodeEntryGetName(AiFindDriverType("jpg"));
    AtNode *driver = AiNode(driver_type_name, "simple_driver");
    AiDriverInitialize(driver, false);

    AiNodeSetStr(driver, "filename", "lambda.jpg");
    AtArray *outputs= AiArray(4, 1, AI_TYPE_STRING, "RGB", "RGB", "simple_filter", "simple_driver");

    const char *filter_name = "gaussian_filter";
    AtNode *filter = AiNode(filter_name, "simple_filter");
    AiNodeSetArray(options, "outputs", outputs);

    char *output_name = "RGB";
    int output_type = AI_TYPE_RGB;

    void *scanlines[480] = {};

    for (int i = 0; i < 480; ++i) {
        void *pixeldata = AiMalloc(640 * sizeof(float) * 3);
    }
    AiOutputIteratorGetNext(NULL, &output_name, &output_type, pixeldata);
    */

    // TODO: Use an actual camera to create proper rays
    // TODO: Use a sampler to generate better subpixel samples
    /*
    AtNode *cam = AiUniverseGetCamera();
    AtCameraInput in = {};
    AtCameraOutput out = {};

    AtCameraNodeMethods *methods = (AtCameraNodeMethods *)cam;
    (methods->CreateRay)(cam, in, out, 0);
    */


    AtVector origin = AtVector(0, 5, 10);
    bool hit = false;

    int w = 640;
    int h = 480;
    uint8_t *buffer = (uint8_t *)malloc(w * h * 3 *sizeof(char));
    uint8_t *curr = buffer;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            AtVector direction = AtVector(
                1 - (((float)x / w) * 2),
                1 - (((float)y / h) * 2),
                -1
            );
            AiV3Normalize(direction);
            AtShaderGlobals *initial_globals = AiShaderGlobals();
            AtRay ray = AiMakeRay(AI_RAY_CAMERA, origin, &direction, AI_BIG, initial_globals);
            AtShaderGlobals *trace_globals = AiShaderGlobals();
            hit = AiTraceProbe(ray, trace_globals);

            AtVector color = trace_globals->Nf;

            if (hit) {
                *curr = color.x * 255;
                *(curr + 1) = color.y * 255;
                *(curr + 2) = color.z * 255;
            } else {
                *curr = 0;
                *(curr + 1) = 0;
                *(curr + 2) = 0;
            }
            curr += 3;
        }
    }

    stbi_write_jpg("test.jpg", w, h, 3, buffer, 90);

    AiRenderEnd();
    AiEnd();
    return 0;
}

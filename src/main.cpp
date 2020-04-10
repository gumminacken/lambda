#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

//#include "../thirdparty/arnold_linux/include/ai.h"
#include "ai.h"
#include "../thirdparty/stb/stb_image_write.h"
#include "../thirdparty/stb/stb_image.h"

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

    AtVector origin = AtVector(0, 0, 8);
    AtVector direction = AtVector(0, -0.01, -1);
    AtShaderGlobals *initial_globals = AiShaderGlobals();

    AtRay ray = AiMakeRay(AI_RAY_CAMERA, origin, &direction, AI_BIG, initial_globals);
    
    AtShaderGlobals *trace_globals = AiShaderGlobals();
    bool hit = AiTraceProbe(ray, trace_globals);

    if (hit) {
        AiMsgInfo("We have a hit at %g %g %g", trace_globals->P.x, trace_globals->P.y, trace_globals->P.z );
        AtNode *hitobj = trace_globals->Op;
        AiMsgInfo("Object: %s", AiNodeGetName(hitobj));
        AiMsgInfo("Shader: %s", AiNodeGetName((AtNode *)AiNodeGetPtr(hitobj, "shader")));
    } else {
        AiMsgInfo("Oh vey - no hit. Try again!");
    }

    int w = 640;
    int h = 480;
    uint8_t *buffer = (uint8_t *)malloc(w * h * 3 *sizeof(char));
    uint8_t *curr = buffer;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            *curr = ((float)y / h) * 255;
            *(curr + 1) = ((float)x / w) * 255;
            *(curr + 2) = 0;
            curr += 3;
        }
    }

    stbi_write_jpg("test.jpg", 640, 480, 3, buffer, 90);

    AiRenderEnd();
    AiEnd();
    return 0;
}

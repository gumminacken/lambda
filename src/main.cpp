#include "../thirdparty/Arnold/include/ai.h"

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

    AiRenderEnd();
    AiEnd();
    return 0;
}

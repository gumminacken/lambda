#include "../thirdparty/Arnold/include/ai.h"

int main() {
    AiBegin();
    
    AiMsgInfo("Entered the Universe!!");
    AiASSLoad("scenes/cornell.ass");
    
    AtNode *options = AiUniverseGetOptions();
    AiNodeSetBool(options, "skip_license_check", true);


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

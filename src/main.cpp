#include "../thirdparty/Arnold/include/ai.h"

int main(int argc, char *argv[]) {
    AiBegin();


    AiASSLoad("scenes/cornell.ass");
    AtNode *options = AiUniverseGetOptions();

    AiRender(AI_RENDER_MODE_FREE);

    AtNode *cam = AiUniverseGetCamera();

    AtMatrix mat = AiNodeGetMatrix(cam, "matrix");

    AtVector origin = AtVector(0, 0, 8);
    AiM4PointByMatrixMult(mat, origin);

    AtVector direction = AtVector(-1, 0, -1);
    AiM4VectorByMatrixMult(mat, direction);

    AtShaderGlobals *startsg = AiShaderGlobals();
    AtRay ray = AiMakeRay(AI_RAY_CAMERA, origin, &direction, AI_BIG, startsg);

    AtShaderGlobals *sg = AiShaderGlobals();
    bool hit = AiTraceProbe(ray, sg);

    AiMsgInfo("Hit something? %d", hit);
    AiMsgInfo("Hit %s at position %g, %g, %g", AiNodeGetName(sg->Op), sg->P.x, sg->P.y, sg->P.z);

    AiRenderEnd();

    AiEnd();
    return 0;
}

#include "../../thirdparty/Arnold-6.0.2.0-linux/include/ai.h"

int main(int argc, char *argv[]) {
    AiBegin();
    AiMsgInfo("Hello, world.");

    AiRender(AI_RENDER_MODE_FREE);

    AtUniverse *universe = AiUniverse();
    AiASSLoad(universe, "/home/albusde/Downloads/cornell.ass");

    AtShaderGlobals * sg = AiShaderGlobals();

    AtNode *cam = AiUniverseGetCamera(universe);

    AtMatrix mat = AiNodeGetMatrix(cam, "matrix");

    AiMsgInfo("matrix: %g", mat.data[1][0]);

    AtVector origin = AtVector(0, 0, 0);
    AiM4PointByMatrixMult(mat, origin);

    AtVector direction = AtVector(0, 0, -1);
    AiM4VectorByMatrixMult(mat, direction);

    AtRay ray = AiMakeRay(AI_RAY_CAMERA, origin, &direction, 1000, sg);

    AtScrSample sample = {};
    AiTrace(ray, AtRGB(1, 1, 1), sample);

    AiMsgInfo("origin: %g, %g, %g", origin.x, origin.y, origin.z);
    AiMsgInfo("direction: %g, %g, %g", direction.x, direction.y, direction.z);

    AtString name = AiNodeGetStr(cam, "name");
    AiMsgInfo("%s", name.c_str());

    AiRenderEnd();

    AiEnd();
    return 0;
}

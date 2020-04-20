#include <rtcore.h>

int main() {

    RTCDevice device = rtcNewDevice("");
    RTCScene scene = rtcNewScene(device);

    RTCGeometry geo = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    return 0;
}
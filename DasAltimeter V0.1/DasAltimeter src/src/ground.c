#include <util.h>
#include <altimeter.h>

void updateGround(Altimeter *my_altimeter) {

    offsets.gravityOffsetBuffer = offsets.gravityOffsetBuffer * 0.8 + sample.accelZ * 0.2;
    offsets.groundOffsetBuffer = offsets.groundOffsetBuffer * 0.5 + altitude * 0.5;
    offsets.groundTemperatureBuffer = offsets.groundTemperatureBuffer * 0.8 + sample.temperatureCelcus * 0.2;

    if ((millis() - offsets.offsetBufferTime) > 2000) {
        //recursive filter
        offsets.groundOffset = offsets.groundOffsetBuffer;
        offsets.groundTemperature = offsets.groundTemperatureBuffer;
        offsets.gravityOffset = offsets.gravityOffsetBuffer;
        offsets.offsetBufferTime = millis();
    }
}

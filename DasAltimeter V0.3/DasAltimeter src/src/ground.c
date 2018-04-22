#include <util.h>
#include <altimeter.h>

void updateGround(Altimeter *my_altimeter) {



    offsets.gravityOffsetBuffer = offsets.gravityOffsetBuffer * 0.8 + sample.accelX * 0.2;
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

void isItPointingUp() {
    u8_t i;

    double pointingUpBuffer;

    pointingUpBuffer = sample.accelX;

    for ( i = 0; i < 10 ; ++i ) {
        delay_ms(10);
        sampleTick();
        pointingUpBuffer = pointingUpBuffer * 0.8 + sample.accelX * 0.2;
    }
    if ((pointingUpBuffer > 0.9) & (pointingUpBuffer <1.1)) {
        altimeter.pointingUp = true;
    } else if ((pointingUpBuffer < -0.9) & (pointingUpBuffer > -1.1)) {
        altimeter.pointingUp = false;
    } else {
        while(1) {

        }

    }

}
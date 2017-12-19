#include <util.h>

void updateGround(Altimeter *my_altimeter){
	
my_altimeter->offsets.gravityOffsetBuffer = my_altimeter->offsets.gravityOffsetBuffer * 0.8 + my_altimeter->myIMU.accelZ * 0.2;
my_altimeter->offsets.groundOffsetBuffer = my_altimeter->offsets.groundOffsetBuffer * 0.5 + my_altimeter->Altitude * 0.5;
my_altimeter->offsets.groundTemperatureBuffer = my_altimeter->offsets.groundTemperatureBuffer * 0.8 + my_altimeter->offsets.temperatureCelcus * 0.2;

if ((millis() - my_altimeter->myIMU.offsetBufferTime) > 2000) {
    //recursive filter
    my_altimeter->offsets.groundOffset = my_altimeter->offsets.groundOffsetBuffer;
    my_altimeter->offsets.groundTemperature = my_altimeter->offsets.groundTemperatureBuffer;
    my_altimeter->offsets.gravityOffset = my_altimeter->offsets.gravityOffsetBuffer;
    my_altimeter->offsets.offsetBufferTime = millis();
    }
}

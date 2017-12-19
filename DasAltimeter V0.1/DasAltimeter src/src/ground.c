#include <util.h>

	void updateGround(Altimeter *my_altimeter){
	
	   my_altimeter->myIMU.gravityOffsetBuffer = 	my_altimeter->myIMU.gravityOffsetBuffer * 0.8 + my_altimeter->myIMU.accelZ * 0.2;
        my_altimeter->myBarometer.groundOffsetBuffer = my_altimeter->myBarometer.groundOffsetBuffer * 0.5 + my_altimeter->Altitude * 0.5;
        my_altimeter->myBarometer.groundTemperatureBuffer = my_altimeter->myBarometer.groundTemperatureBuffer * 0.8 + my_altimeter->myBarometer.temperatureCelcus * 0.2;

        if ((millis() - my_altimeter->myIMU.offsetBufferTime) > 2000) {
            //recursive filter
            my_altimeter->myBarometer.groundOffset = my_altimeter->myBarometer.groundOffsetBuffer;
            my_altimeter->myBarometer.groundTemperature = my_altimeter->myBarometer.groundTemperatureBuffer;
            my_altimeter->myIMU.gravityOffset = my_altimeter->myIMU.gravityOffsetBuffer;
            my_altimeter->myIMU.offsetBufferTime = millis();
        }
	}

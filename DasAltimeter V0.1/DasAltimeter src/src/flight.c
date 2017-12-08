#include <util.h>
#include <boardDefines.h>



void flight(Altimeter *my_altimeter) {



    switch(my_altimeter->myFlightState) {
    case flightStatrup:

        my_altimeter->myIMU.gravityOffsetBuffer = 	my_altimeter->myIMU.gravityOffsetBuffer*0.5 + my_altimeter->myIMU.accelZ*0.5;
        my_altimeter->myIMU.gravityOffsetBufferRaw = 	my_altimeter->myIMU.gravityOffsetBufferRaw*0.5 + my_altimeter->myIMU.accelZRaw*0.5;
        my_altimeter->myBarometer.groundOffsetBuffer = my_altimeter->myBarometer.groundOffsetBuffer*0.5 + my_altimeter->myBarometer.heightCm*0.5;
        my_altimeter->myBarometer.groundTemperatureBuffer = my_altimeter->myBarometer.groundTemperatureBuffer*0.5 + my_altimeter->myBarometer.temperatureCelcus*0.5;


        if ((millis() - my_altimeter->myIMU.offsetBufferTime) > 2000) {
            //recursive filter
            my_altimeter->myBarometer.groundOffset = my_altimeter->myBarometer.groundOffsetBuffer;
            my_altimeter->myBarometer.groundTemperature = my_altimeter->myBarometer.groundTemperatureBuffer;
            my_altimeter->myIMU.gravityOffset = my_altimeter->myIMU.gravityOffsetBuffer;
            my_altimeter->myIMU.gravityOffsetRaw = my_altimeter->myIMU.gravityOffsetBufferRaw;
            my_altimeter->myIMU.offsetBufferTime = millis();
        }


        break;
    case flightIdle:
           TC4->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;
           TC5->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;



        break;
    case flightPad:
        /* from pad to boost
        *  initial trigger attempt will be
        *  Accel > 2g & vel > 5m/s
        *		 or
        *  baro alt > 40ft
        */





        break;
    case flightBoost:
        /*
        *	boost to fast
        *		while
        *	Accel > 1/4G
        */



        break;
    case flightFast:




        break;
    case flightCoast:

        /* Coast to drogue
        *		while
        *	   speed > 0
        *	   	 &
        *	altitude > baro_max_alt
        */


        break;
    case flightDrogue:



        break;
    case flightMain:



        break;
    case flightLanded:



        break;
    case flightTest:




        break;
    }


}

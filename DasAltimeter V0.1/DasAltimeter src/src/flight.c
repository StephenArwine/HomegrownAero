#include <util.h>
#include <boardDefines.h>



void flight(Altimeter *my_altimeter) {
	


    switch(my_altimeter->myFlightState) {
    case flightStatrup:

        my_altimeter->myIMU.gravityOffsetBuffer = 	my_altimeter->myIMU.gravityOffsetBuffer*0.5 + my_altimeter->myIMU.accelZ*0.5;

        if ((millis() - my_altimeter->myIMU.offsetBufferTime) > 2000) {
            //recursive filter
            my_altimeter->myIMU.gravityOffset = my_altimeter->myIMU.gravityOffsetBuffer; 
            my_altimeter->myIMU.offsetBufferTime = millis();
        }


        break;
    case flightIdle:


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
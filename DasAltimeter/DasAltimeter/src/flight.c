#include <flight.h>
#include <util.h>

enum flight_state my_flight_state; // current flight state.


void flight() {

    my_flight_state = flightStatrup;

    switch(my_flight_state) {
    case flightStatrup:


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
#include <util.h>
#include <boardDefines.h>



void flight(Altimeter *my_altimeter) {



    switch(my_altimeter->myFlightState) {
    case flightStatrup:

        beep(400);

        computeKalmanGains(&my_altimeter->myKalmanFilter);

        my_altimeter->myIMU.gravityOffset = my_altimeter->myIMU.accelZ;
        my_altimeter->myBarometer.groundOffset = my_altimeter->myBarometer.altitudefeet;

        my_altimeter->StartupTick = millis();

        my_altimeter->myFlightState = flightIdle;

        break;
    case flightIdle:



        my_altimeter->myIMU.gravityOffsetBuffer = 	my_altimeter->myIMU.gravityOffsetBuffer * 0.8 + my_altimeter->myIMU.accelZ * 0.2;
        my_altimeter->myBarometer.groundOffsetBuffer = my_altimeter->myBarometer.groundOffsetBuffer * 0.5 + my_altimeter->myBarometer.altitudefeet * 0.5;
        my_altimeter->myBarometer.groundTemperatureBuffer = my_altimeter->myBarometer.groundTemperatureBuffer * 0.8 + my_altimeter->myBarometer.temperatureCelcus * 0.2;

        if ((millis() - my_altimeter->myIMU.offsetBufferTime) > 3000) {
            //recursive filter
            my_altimeter->myBarometer.groundOffset = my_altimeter->myBarometer.groundOffsetBuffer;
            my_altimeter->myBarometer.groundTemperature = my_altimeter->myBarometer.groundTemperatureBuffer;
            my_altimeter->myIMU.gravityOffset = my_altimeter->myIMU.gravityOffsetBuffer;
            my_altimeter->myIMU.offsetBufferTime = millis();
        }

        logSensors(my_altimeter);


        if ((millis() - my_altimeter->StartupTick) > 10000) {
            findNewFlightStart(my_altimeter);
            logFlight(my_altimeter);
            startupJingle();
            my_altimeter->myFlightState = flightPad;
			break;
        }

        attemptConnection(my_altimeter);



        if (my_altimeter->batFloat < 3.5) {
            my_altimeter->myFlightState = flightTest;
            delay_ms(80);
            beep(300);
            delay_ms(80);
            beep(300);
            delay_ms(80);
            beep(300);
            delay_ms(80);
            pinLow(buzzerPin);
            pinLow(LedPin);
        }

        break;
    case flightPad:
        /* from pad to boost
        *  initial trigger attempt will be
        *  Accel > 2g & vel > 5m/s
        *		 or
        *  baro alt > 40ft
        */

        if (writeLog) {
            writeLog = false;



            logSensors(my_altimeter);

            if (my_altimeter->myFlashMemory.pageReady) {
                my_altimeter->myFlashMemory.pageReady = false;

                pinToggle(LedPin);
               
			    //AT25SFHoldTillReady();
                u8_t bytesWritten = AT25SEWritePage(my_altimeter->myFlashMemory.currentAddress,my_altimeter->myFlashMemory.pageToWrite);
                my_altimeter->myFlashMemory.currentAddress = (my_altimeter->myFlashMemory.currentAddress + 0x100);


            }
        }

        if (my_altimeter->batFloat < 3.5) {
            my_altimeter->myFlightState = flightTest;
            AT25SFHoldTillReady();
            writeFlightEndAddress(my_altimeter);
            delay_ms(80);
            beep(300);
            delay_ms(80);
            beep(300);
            delay_ms(80);
            beep(300);
            delay_ms(80);
            pinLow(buzzerPin);
            pinLow(LedPin);
        }

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

        TC4->COUNT8.CTRLA.reg = 0;
        TC5->COUNT8.CTRLA.reg = 0;



        break;
    }


}

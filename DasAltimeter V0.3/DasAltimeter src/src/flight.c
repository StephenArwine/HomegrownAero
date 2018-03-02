#include <util.h>
#include <boardDefines.h>


void flight() {

    switch(flightState) {
    case flightStatrup:

        updateGround( );

        if ((millis() - startupTick) > 10000) {
            findNewFlightStart( );
            logFlight( );
            startupJingle();
            startupTick = millis();
            flightState = flightTest;
            //flightState = flightPad;
            break;
        }

        attemptConnection();

        if (unplugged()) {
            flightState = flightIdle;
            unpluggedJingle();
        }

        break;
    case flightIdle:
        //TC4->COUNT8.CTRLA.reg = 0;
        //TC5->COUNT8.CTRLA.reg = 0;

        if (millis() - offsets.groundBeep > 2000) {
            offsets.groundBeep = millis();
            beep(400);
        }

        if (sercom(USART3)->SPI.INTFLAG.bit.RXC == 1) {
            u8_t possibleReset = usartDataIn(USART3);
            if (possibleReset == 0x52) {
                NVIC_SystemReset();
            }
        }

        break;
    case flightPad:
        /* from pad to boost
        *  initial trigger attempt will be
        *  Accel > 2g & vel > 15ft/s
        *		 or
        *  baro alt > 100ft
        */

        updateGround();

        if (writeLog) {
            //logSensors( );
            pinToggle(LedPin);
        }


        if ((( velocity > 15) && (accel > 2)) | (altitudeAGL() > 100)) {
            flightState = flightBoost;
            logEvent('L');
        }

        break;
    case flightBoost:
        /*
        *	boost to fast
        *		while
        *	Accel > 1/4G
        */
        if (writeLog) {
            logSensors( );
        }

        if (velocity < 0) {
            flightState = flightDrogue;
            igniteDrogue();
            logEvent('A');
            beep(100);
        }

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

        if (writeLog) {
            logSensors( );
        }


        if (sample.pressureAltitude < deploymentSettings.MAIN_DEPLOY) {
            flightState = flightMain;
            igniteMain();
            logEvent('M');
        }

        break;
    case flightMain:

        if (writeLog) {
            logSensors( );
        }

        if (velocity < 5) {
            flightState = flightLanded;
            finishFlight();
        }

        break;
    case flightLanded:

        if (millis() - offsets.groundBeep > 2000) {
            offsets.groundBeep = millis();
            beep(400);
        }

        break;
    case flightTest:
        //simple continuity test
        if ( (sample.voltage.senseA + sample.voltage.senseB +sample.voltage.senseC +sample.voltage.senseD) > 200) {
            unpluggedJingle();
            //igniteDrogue();

        }

//         if (altitudeAGL() > 15 | altitudeAGL() < -15) {
//             beep(100);
//         }

        if (writeLog) {
            logSensors( );
        }

        break;
    }



    if ((flightState != flightStatrup) & ((flightState != flightIdle) & unplugged())) {
        finishFlight();
    }

}
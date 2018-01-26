#include <util.h>
#include <boardDefines.h>

flightState_t flightState;


void flight() {


    switch(flightState) {
    case flightStatrup:


        updateGround( );

        //logSensors( );


        if ((millis() - startupTick) > 10000) {
            findNewFlightStart( );
            logFlight( );
            startupJingle();
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

        //delay_ms(1000);
        //pinToggle(LedPin);

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
        *  Accel > 2g & vel > 5m/s
        *		 or
        *  baro alt > 40ft
        */

        updateGround();

        if (writeLog) {
            writeLog = false;
            //logSensors( );
            pinToggle(LedPin);
        }

        if (( velocity > 0.05) && (altitudeAGL() > 5)) {
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
            writeLog = false;
            logSensors( );

        }


        if (velocity < 0) {
            flightState = flightDrogue;
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


        if (pageReady) {
            pageReady = false;
            pinToggle(LedPin);
            //AT25SFHoldTillReady();
            u8_t bytesWritten = AT25SEWritePage(currentAddress,pageToWrite);
            currentAddress = (currentAddress + 0x100);
        }



        break;
    case flightMain:



        break;
    case flightLanded:



        break;
    case flightTest:

        /*
           //simple continuity test
           if ( (sample.voltage.senseA + sample.voltage.senseB +sample.voltage.senseC +sample.voltage.senseD) > 200) {
               unpluggedJingle();
           }
        */



        if (writeLog) {
            writeLog = false;
            logSensors( );
           
        }


        break;
    }

    if (flightState != flightStatrup & flightState != flightIdle & unplugged()) {
        finishFlight();
    }



}

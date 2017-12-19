/*
 * DasAltimeter.c
 *
 * Created: 10/4/2017 5:28:21 PM
 * Author : ancar
 */


#include "sam.h"
#include <util.h>
#include <boardDefines.h>
#include "math.h"


void init() {

    /* Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet */
    NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;

    /* Turn on the digital interface clock */
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;

    SystemInit();
    GclkInit();
    RtcInit();
    delayInit();
    adcInit();
    dmaInit();

    NVIC_EnableIRQ(DMAC_IRQn);
    NVIC_SetPriority(DMAC_IRQn, 0x00);

    pinOut(LedPin);
    pinAnalog(senseBatPin);

    pinAnalog(senseAPin);
    pinMux(senseAPin);
    pinOut(fireAPin);
    pinLow(fireAPin);

    pinAnalog(senseBPin);
    pinMux(senseBPin);
    pinOut(fireBPin);
    pinLow(fireBPin);

    pinAnalog(senseCPin);
    pinOut(fireCPin);
    pinLow(fireCPin);

    pinAnalog(senseDPin);
    pinOut(fireDPin);
    pinLow(fireDPin);

    pinOut(spi0MOSI);
    pinOut(spi0SCK);
    pinIn(spi0MISO);
    pinMux(spi0MISO);
    pinMux(spi0SCK);
    pinMux(spi0MOSI);

    pinOut(cs_accel);
    pinHigh(cs_accel);

    pinOut(cs_gyro);
    pinHigh(cs_gyro);

    pinOut(spi1MOSI);
    pinOut(spi1SCK);
    pinIn(spi1MISO);
    pinMux(spi1MISO);
    pinMux(spi1SCK);
    pinMux(spi1MOSI);

    pinOut(cs_mem);
    pinHigh(cs_mem);

    pinOut(spi2MOSI);
    pinOut(spi2SCK);
    pinIn(spi2MISO);
    //  pinMux(spi2MISO);
    //  pinMux(spi2SCK);
    //  pinMux(spi2MOSI);

    pinOut(cs_baro);
    pinHigh(cs_baro);

    pinOut(buzzerPin);
    pinCfg(buzzerPin);

    pinOut(TxPo);
    pinMux(TxPo);
    pinIn(RxPo);
    pinMux(RxPo);

//    sercomClockEnable(SPI2, 3, 4);
//   sercomSpiMasterInit(SPI2, 3, 0, 0, 0, 0x00);

    sercomClockEnable(SPI0, 3, 4);
    sercomSpiMasterInit(SPI0, 3, 0, 0, 0, 0x00);

    sercomClockEnable(SPI1, 3, 4);
    sercomSpiMasterInit(SPI1, 3, 0, 0, 0, 0x00);

    sercomClockEnable(USART3, 4, 8);
    sercomUartInit(USART3,1,0,19200);

    TC4Init();
    TC5Init();
}


int main(void) {

    init();

    delay_ms(500);


    Altimeter my_altimeter;
    my_altimeter.myFlightState = flightStatrup;

    initMS5803Barometer(&my_altimeter.myBarometer);
    IMUinit();

    my_altimeter.myFlashMemory.pageLocation = 0x00;
    my_altimeter.myFlashMemory.pageReady = false;

    computeKalmanGains(&my_altimeter.myKalmanFilter);

    sampleTick(&my_altimeter);

    my_altimeter.myIMU.gravityOffset = my_altimeter.myIMU.accelZ;
    my_altimeter.myIMU.gravityOffsetBuffer = my_altimeter.myIMU.gravityOffset;
    my_altimeter.myBarometer.groundOffset = my_altimeter.myBarometer.altitudefeet;
    my_altimeter.myBarometer.groundOffsetBuffer = my_altimeter.myBarometer.groundOffset;


	

    beep(400);

    my_altimeter.StartupTick = millis();
	my_altimeter.myIMU.offsetBufferTime =  my_altimeter.StartupTick;
	
    while (1) {

        if (takeSample()) {
			if (my_altimeter.Altitude > 1000000)
			{
				    beep(400);

			}
            sampleTick(&my_altimeter);
            flight(&my_altimeter);
            computeKalmanStates(&my_altimeter);

        }


    }
}

/*
 * ParaBeepV0.1.c
 *
 * Created: 1/8/2021 7:02:10 PM
 * Author : Desktop
 */


#include "sam.h"
#include "util.h"

void init() {
    pinOut(BuzzerPin);

    pinOut(spi1MOSI);
    pinOut(spi1SCK);
    pinIn(spi1MISO);
    pinMux(spi1MISO);
    pinMux(spi1SCK);
    pinMux(spi1MOSI);

    pinOut(USART0TX);
    pinIn(USART0RX);
    pinMux(USART0TX);
    pinMux(USART0RX);

    pinOut(USART2TX);
    pinIn(USART2RX);
    pinMux(USART2TX);
    pinMux(USART2RX);
    pinOut(GPSINT);


    pinOut(cs_baro);
    pinHigh(cs_baro);


    delayInit();
    GclkInit(0);
    TC2Init();
    rtcInit();
    TC0Init();
    TC1Init();
    SPIinit(1);
    USART0init();
    USART2init();
}


void getAltitude() {

    pinLow(cs_baro);
    spiDataOut(SPI1, cmdAdcD2_4096_);
    pinHigh(cs_baro);
    delay_us(9040);
    volatile uint32_t tempRaw = readMS5803AdcResults();

    pinLow(cs_baro);
    spiDataOut(SPI1, cmdAdcD1_4096_);
    pinHigh(cs_baro);

    delay_us(9040);

    volatile u32_t PressureRaw = readMS5803AdcResults();
    ConvertPressureTemperature(PressureRaw, tempRaw, &sample.temperatureCelcus, &sample.pressureMbar);

    sample.lastSampleAltitude = sample.pressureAltitude;
    sample.lastSampleTime = sample.sampleTime;
    sample.sampleTime = millis();
    sample.dT = sample.sampleTime - sample.lastSampleTime;

    sample.pressureAltitude = paToFeetNOAA(sample.pressureMbar);
    sample.dX = sample.pressureAltitude - sample.lastSampleAltitude;

    sample.dxdt = sample.dX/sample.dT;

    sample.AVGdXdT = sample.AVGdXdT*0.75 + sample.dxdt*0.25;

    ++sample.sampleCount;
}

void warmUp() {
    //warm up baro
    while (sample.sampleCount < 10) {
        if (takeSample) {
            getAltitude();
        }
    }

}


u32_t samples = 0;
u32_t notsamples = 0;

int main(void) {
    /* Initialize the SAM system */
    SystemInit();
    init();
    initMS5803Barometer();



    //powerOffWithInterrupt();
    //delay_ms(10);
    volatile uint8_t data[255];
    if(sercom(2)->USART.INTFLAG.bit.RXC == 1) {
        for (u8_t i = 0; i < 254; i++) {
            data[i] = usartDataIn(2);
        }
    }

    buzzing = false;
    warmUp();

    //pinLow(GPSINT);
    pinHigh(GPSINT);


    long lasttime = millis();

    u32_t beep = 900;
    setBuzzerFreq(beep);
    beepms(90000/beep);
    TC2->COUNT16.CTRLA.reg = 0;


    //streamAlt();


    while (1) {

        //if ((millis() - lasttime) > 1000 ) {
        //    TC2->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV16 | TC_CTRLA_ENABLE;
        //    while(TC2->COUNT16.SYNCBUSY.bit.ENABLE);
        //    delay_ms(100);
        //    lasttime = millis();
        //    TC2->COUNT16.CTRLA.reg = 0;
        //}

        delay_us(1);

        if(sercom(2)->USART.INTFLAG.bit.RXC == 1) {
            /*u8_t message[255];
            volatile u8_t messageLength = 0;

            for (u8_t i = 0; i < 255; i++) {
                while(sercom(2)->SPI.INTFLAG.bit.RXC == 0);
                u8_t digit = usartDataIn(2);
                if (digit == 0x0a ) {
                    message[i] = digit;
                    messageLength++;
                    break ;
                }
                message[i] = digit;
                messageLength++;
            }

            for (u8_t j = 0; j < messageLength; j++) {
                usartDataOut(0, message[j]);
            }
			*/

            
            if(sercom(2)->USART.INTFLAG.bit.RXC == 1) {
                u8_t digit = usartDataIn(2);
                usartDataOut(0, digit);
            }

             if(sercom(0)->USART.INTFLAG.bit.RXC == 1) {
                u8_t digit = usartDataIn(0);
                 usartDataOut(2, digit);
             }
            

            // if (takeSample) {

            //getAltitude();
            //takeSample = false;



            parseGPSMessage();

            //if (sample.AVGdXdT > .1 || sample.AVGdXdT < -0.1  ) {

            //}
            //beepms(100);
            //}

        }

    }
}


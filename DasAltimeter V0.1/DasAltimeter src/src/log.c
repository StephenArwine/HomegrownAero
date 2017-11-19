#include <util.h>
#include <log.h>
#include <flight.h>
#include <boardDefines.h>




void logSensors(Altimeter *my_altimeter) {

    usartDataOut(USART3,SENSOR_LOG);
    usartDataOut(USART3,my_altimeter->sampleTick >> 0);
    usartDataOut(USART3,my_altimeter->sampleTick >> 8);
    usartDataOut(USART3,my_altimeter->sampleTick >> 16);
    usartDataOut(USART3,my_altimeter->sampleTick >> 24);
	
    usartDataOut(USART3,my_altimeter->myBarometer.heightCm >> 0);
    usartDataOut(USART3,my_altimeter->myBarometer.heightCm >> 8);
    usartDataOut(USART3,my_altimeter->myBarometer.heightCm >> 16);
    usartDataOut(USART3,my_altimeter->myBarometer.heightCm >> 24);
	
    usartDataOut(USART3,my_altimeter->myIMU.accelXRaw >> 0);
    usartDataOut(USART3,my_altimeter->myIMU.accelYRaw >> 8);
    usartDataOut(USART3,my_altimeter->myIMU.accelYRaw >> 0);
    usartDataOut(USART3,my_altimeter->myIMU.accelXRaw >> 8);
    usartDataOut(USART3,my_altimeter->myIMU.accelZRaw >> 0);
    usartDataOut(USART3,my_altimeter->myIMU.accelZRaw >> 8);
	
    usartDataOut(USART3,my_altimeter->myIMU.gyroXRaw >> 0);
    usartDataOut(USART3,my_altimeter->myIMU.gyroXRaw >> 8);
    usartDataOut(USART3,my_altimeter->myIMU.gyroYRaw >> 0);
    usartDataOut(USART3,my_altimeter->myIMU.gyroYRaw >> 8);
    usartDataOut(USART3,my_altimeter->myIMU.gyroZRaw >> 0);
    usartDataOut(USART3,my_altimeter->myIMU.gyroZRaw >> 8);

    usartDataOut(USART3,my_altimeter->myAnalogAccelerometer.analogRaw >> 0);
    usartDataOut(USART3,my_altimeter->myAnalogAccelerometer.analogRaw >> 8);


}




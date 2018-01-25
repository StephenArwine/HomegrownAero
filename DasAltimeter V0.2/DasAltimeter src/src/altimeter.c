#include <util.h>
#include <boardDefines.h>


void IMUinit() {

    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    pinLow(cs_accel);
    dummy_rx = spiDataTransfer(SPI0, BMI_ACCEL_PMU_RANGE);
    dummy_rx = spiDataTransfer(SPI0, BMI_ACCEL_16G_RANGE);
    pinHigh(cs_accel);

}

bool USARTconnectionAvaliable() {

    if (sercom(USART3)->SPI.INTFLAG.bit.RXC == 1) {
        u8_t connectAttempt = usartDataIn(USART3);
        if (connectAttempt == 0x48) {
            usartDataOut(USART3, 'H');
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void sendTheasePagesToComputer(u32_t firstPage, u32_t lastPage) {

    u16_t pagesToSend = (lastPage - firstPage) >> 8;

    u32_t pageToReadAddress = firstPage;

    for (u16_t page = 0; page <= pagesToSend; ++page) {

        u8_t data[256];
        AT25SEreadPage(pageToReadAddress , data);

        for (u16_t dataByte = 0; dataByte < 256; ++dataByte) {
            usartDataOut(USART3, data[dataByte]);
        }

        pageToReadAddress = pageToReadAddress + 0x100;
    }
}

int32_t altitudeAGL() {
    return altitude - offsets.groundOffset;
}


bool unplugged() {
    if (sample.voltage.batFloat < 3.5) {
        return true;
    } else {
        return false;
    }
}

void finishFlight() {
	
    flightState = flightIdle;
    AT25SFHoldTillReady();
    writeFlightEndAddress( );
    unpluggedJingle();
}

void POST(){
	uint8_t dummy_rx;
	uint8_t dummy_Tx = 0xFF;

	bool postFailed = false;
	u8_t failType = 0;


	//Baro post first
	u8_t crc = u8_t MS5803_CRC4();
	if (crc != coefficients_[7]){
		postFailed = true;
		failType = 1;
		
	}

	//Memory Post
	pinLow(cs_mem);
	dummy_rx = spiDataTransfer(SPI1,0x9f); // read id and mfg code
	u8_t mfgID = spiDataTransfer(SPI1,dummy_Tx);
	u8_t deviceID1 = spiDataTransfer(SPI1,dummy_Tx);
	u8_t deviceID2 = spiDataTransfer(SPI1,dummy_Tx);
	pinHigh(cs_mem);
	
	if ( mfgID != 0x1f & deviceID1 != 0x86 & deviceID2 != 0x01){
		postFailed = true;
		failType = 2;
	}
	
	
	//Accelerometer Post
	pinLow(cs_accel);
	dummy_rx = spiDataTransfer(SPI0, BMI055_BGW_CHIPID | BMI055_READ_REG);
	u8_t accelID = spiDataTransfer(SPI0,dummy_Tx);
	pinHigh(cs_accel);
	
	if (accelID != 0xFA){
		postFailed = true;
		failType = 3;
	}

	//Gyro Post
	pinLow(cs_gyro);
	dummy_rx = spiDataTransfer(SPI0, BMI055_BGW_CHIPID | BMI055_READ_REG);
	u8_t gyrolID = spiDataTransfer(SPI0,dummy_Tx);
	pinHigh(cs_gyro);
	
	if (gyroID != 0x0F){
		postFailed = true;
		failType = 4;
	}
	
	//Analog Accelerometer Post 


	if (postFailed){
		while (1){
			u8_t beepCount;
			
			for (beepCount = failType; beepCount > 0; beepCount--){
				beep(300);
				delayms(300);
			}	
delayms(1500);
		}
	}
}

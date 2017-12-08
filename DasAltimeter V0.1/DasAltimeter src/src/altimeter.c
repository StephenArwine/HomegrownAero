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

bool USARTconnectionAvaliable(){

	if (sercom(USART3)->SPI.INTFLAG.bit.RXC == 1) {
		u8_t connectAttempt = usartDataIn(USART3);
            if (connectAttempt == 0x48) {
				usartDataOut(USART3, 'H');
				return true;
			}
			else{
				return false;
			}
	}
		return false;
  }


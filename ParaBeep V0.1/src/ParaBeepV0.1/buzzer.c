#include <util.h>


void beep(u16_t duration) {

	long count = 0;

	while(count < duration) {
		pinToggle(BuzzerPin);
		delay_ms(1);
		count++;
	}
	pinLow(BuzzerPin);
}

void beepms(u16_t dur){
	
	//while (TC0->COUNT16.STATUS.reg != TC_STATUS_STOP ){}
	TC0->COUNT16.PER.reg = dur;
	//TC0->COUNT16.COUNT.reg = 0;
	//TC0->COUNT16.INTFLAG.reg = 0xFF;
	//TC0->COUNT16.INTENSET.reg = TC_INTENSET_OVF_Msk;
	TC0->COUNT16.CTRLBSET.reg = TC_CTRLBSET_CMD_RETRIGGER;
	
	//TC2->COUNT8.CTRLBSET.reg = TC_CTRLBSET_CMD_UPDATE;
	//buzzing = true;
}
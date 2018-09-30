

void testIgniters(){
	
	delay_ms(500);
	beepNumber(1);
	pinHigh(fireAPin);
	delay_ms(1500);
	pinLow(fireAPin);
	
	delay_ms(250);
	beepNumber(2);
	pinHigh(fireBPin);
	delay_ms(1500);
	pinLow(fireBPin);
		
	delay_ms(250);
	beepNumber(3);
	pinHigh(fireCPin);
	delay_ms(1500);
	pinLow(fireCPin);
	
	delay_ms(250);
	beepNumber(4);
	pinHigh(fireDPin);
	delay_ms(1500);
	pinLow(fireDPin);
}

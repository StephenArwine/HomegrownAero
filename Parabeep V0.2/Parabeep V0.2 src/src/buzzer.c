#include <util.h>
#include <boardDefines.h>
#include <math.h>

volatile bool buzzing = false;

void beep(u16_t duration) {


    long count = 0;

    long beepStop = duration*1000 / 122;

    pinToggle(LedPin);

    while(count < beepStop) {
        pinToggle(buzzerPin);
        delay_us(120);
        count++;
    }
    pinLow(buzzerPin);
    pinToggle(LedPin);
}

void setBuzzerFreq(u16_t freq){
	u8_t per = (8000000/256)/freq;
	TC4->COUNT8.PER.reg = per;
	
}

bool isBuzzing(){
	return buzzing;
}

void startBuzzing(){
	buzzing = true;
}

void stopBuzzing(){
	buzzing = false;
}

void beepDigit(u8_t digit) {
    if (digit == 0) {
        beep(600);
    } else {
        while (digit > 0) {
            beep(200);
            delay_ms(300);
            --digit;
        }
    }
}

void beepNumber(u32_t number) {

    if (number >=100000) {
        beepDigit(number/100000);
        number = number%100000;
        delay_ms(1000);
        beepDigit(number/10000);
        number = number%10000;
        delay_ms(1000);
        beepDigit(number/1000);
        number = number%1000;
        delay_ms(1000);
        beepDigit(number/100);
        number = number%100;
        delay_ms(1000);
        beepDigit(number/10);
        number = number%10;
        delay_ms(1000);
        beepDigit(number);
        return;
    }
    if (number >=10000) {
        beepDigit(number/10000);
        number = number%10000;
        delay_ms(1000);
        beepDigit(number/1000);
        number = number%1000;
        delay_ms(1000);
        beepDigit(number/100);
        number = number%100;
        delay_ms(1000);
        beepDigit(number/10);
        number = number%10;
        delay_ms(1000);
        beepDigit(number);
        return;
    }
    if (number >=1000) {
        beepDigit(number/1000);
        number = number%1000;
        delay_ms(1000);
        beepDigit(number/100);
        number = number%100;
        delay_ms(1000);
        beepDigit(number/10);
        number = number%10;
        delay_ms(1000);
        beepDigit(number);
        return;
    }
    if (number >=100) {
        beepDigit(number/100);
        number = number%100;
        delay_ms(1000);
        beepDigit(number/10);
        number = number%10;
        delay_ms(1000);
        beepDigit(number);
        return;
    }
    if (number >=10) {
        beepDigit(number/10);
        number = number%10;
        delay_ms(1000);
        beepDigit(number);
    }
    beepDigit(number);

}

void startupJingle() {
    beep(150);
    delay_ms(60);
    beep(150);
    delay_ms(250);
    beep(150);
    delay_ms(60);
    beep(150);
    delay_ms(500);
}

void unpluggedJingle() {
    delay_ms(60);
    beep(150);
    delay_ms(60);
    beep(150);
    delay_ms(60);
    beep(150);
    delay_ms(60);
    pinLow(buzzerPin);
    pinLow(LedPin);

}



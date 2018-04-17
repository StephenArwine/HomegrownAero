#include <util.h>
#include <boardDefines.h>
#include <math.h>


void beep(u16_t toggles) {

    pinToggle(LedPin);
    for (u16_t buzz = 0; buzz < toggles; ++buzz) {
        pinToggle(buzzerPin);
        delay_us(122);
    }
    pinLow(buzzerPin);
    pinToggle(LedPin);
}

void beepDigit(u8_t digit) {
    if (digit == 0) {
        beep(1000);
    } else {
        while (digit > 0) {
            beep(400);
            delay_ms(500);
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
    beep(300);
    delay_ms(80);
    beep(300);
    delay_ms(250);
    beep(300);
    delay_ms(80);
    beep(300);
    delay_ms(500);
}

void unpluggedJingle() {
    delay_ms(80);
    beep(300);
    delay_ms(80);
    beep(300);
    delay_ms(80);
    beep(300);
    delay_ms(80);
    pinLow(buzzerPin);
    pinLow(LedPin);

}



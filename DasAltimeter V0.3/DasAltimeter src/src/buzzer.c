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
            beep(500);
            delay_ms(500);
            --digit;
        }
    }
}

void beepNumber(u32_t number) {
	u8_t length = floor(log10(abs(number))) + 1;



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



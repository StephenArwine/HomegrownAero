#include <util.h>

asm(".global _printf_float");

u8_t RN4870CMDMODE() {

    usartDataOut(RN4871, '$');
    usartDataOut(RN4871, '$');
    usartDataOut(RN4871, '$');

    return 1;
}

u8_t RN4871Status() {
    u8_t message[255];
    for (u8_t i = 0; i < 255; i++) {
        while(sercom(RN4871)->USART.INTFLAG.bit.RXC == 0);
        message[i] = usartDataIn(RN4871);
        if (i != 0) {
            if (message[i] == 0x25 || message[i] == 0x0D) {
                return 1;
            }
        }
    }
    return 0;

}

void RN4871SetName() {

    u8_t cmd = RN4870CMDMODE();
    delay_ms(150);
    char newName[11];
    sprintf(newName,"S-,ParaBeep_");
    for (u8_t i = 0; i < 11 ; i++) {
        usartDataOut(RN4871,newName[i]);
    }
    usartDataOut(RN4871,0x0d);

    delay_ms(10);
    usartDataOut(RN4871,'-');
    usartDataOut(RN4871,'-');
    usartDataOut(RN4871,'-');
}

void RN4871SendLK8EX1() {
    /*
    LK8EX1,pressure,altitude,vario,temperature,battery,*checksum

    Field 0, raw pressure in hPascal:
    hPA*100 (example for 1013.25 becomes  101325)
    no padding (987.25 becomes 98725, NOT 098725)
    If no pressure available, send 999999 (6 times 9)
    If pressure is available, field 1 altitude will be ignored

    Field 1, altitude in meters, relative to QNH 1013.25
    If raw pressure is available, this value will be IGNORED (you can set it to 99999
    but not really needed)! (if you want to use this value, set raw pressure to 999999)

    Field 2, vario in cm/s
    If vario not available, send 9999  (4 times 9) Value can also be negative

    Field 3, temperature in C , can be also negative
    If not available, send 99

    Field 4, battery voltage or charge percentage Cannot be negative
    If not available, send 999 (3 times 9)
    Voltage is sent as float value like: 0.1 1.4 2.3  11.2
    To send percentage, add 1000. Example 0% = 1000
    14% = 1014 .  Do not send float values for percentages.
    Percentage should be 0 to 100, with no decimals, added by 1000!
    */

    char s[100];

    sprintf(s, "LK8EX1,%d,99999,9999,%d,%4.2f,", (int)(sample.pressureMbar * 100), (int)sample.temperatureCelcus, sample.battVoltage);

	//nmea checksum start
    u8_t c, XOR;
    XOR = 0;
    u8_t len = strlen(s);
    for (u8_t i = 0; i < len; i++) {
        c = s[i];
        XOR ^= c;
    }
    // Checksum berechnen

    sprintf(s,"%s*%x", s, XOR);

    usartDataOut(RN4871,0x24);
    u8_t len2 = strlen(s);
    for (u8_t i = 0 ; i < len2; i++) {
        usartDataOut(RN4871,s[i]);
    }
    usartDataOut(RN4871,0x0d);
    usartDataOut(RN4871,0x0a);
}
#include <util.h>
#include <boardDefines.h>


void igniterTick() {

    if (charges.igniterAHot) {
        if ((millis() - charges.igniterATick) > 2000) {
            pinLow(fireAPin);
            charges.igniterAHot = false;
        }
    }

    if (charges.igniterBHot) {
        if ((millis() - charges.igniterBTick) > 2000) {
            pinLow(fireBPin);
            charges.igniterBHot = false;
        }
    }

    if (charges.igniterCHot) {
        if ((millis() - charges.igniterCTick) > 2000) {
            pinLow(fireCPin);
            charges.igniterCHot = false;
        }
    }

    if (charges.igniterDHot) {
        if ((millis() - charges.igniterDTick) > 2000) {
            pinLow(fireDPin);
            charges.igniterDHot = false;
        }
    }
}

void igniteMain() {

    if ((charges.igniterAMain = true)) {
        lightIgniterA();
    }
    if ((charges.igniterBMain = true)) {
        lightIgniterB();
    }
    if ((charges.igniterCMain = true)) {
        lightIgniterC();
    }
    if ((charges.igniterDMain = true)) {
        lightIgniterD();
    }
}

void igniteDrogue() {

    if ((charges.igniterADrogue = true)) {
        lightIgniterA();
    }
    if ((charges.igniterBDrogue = true)) {
        lightIgniterB();
    }
    if ((charges.igniterCDrogue = true)) {
        lightIgniterC();
    }
    if ((charges.igniterDDrogue = true)) {
        lightIgniterD();
    }
}

void lightIgniterA() {
    pinHigh(fireAPin);
    charges.igniterAHot = true;
    charges.igniterATick = millis();
}

void lightIgniterB() {
    pinHigh(fireBPin);
    charges.igniterBHot = true;
    charges.igniterBTick = millis();
}

void lightIgniterC() {
    pinHigh(fireCPin);
    charges.igniterCHot = true;
    charges.igniterCTick = millis();
}

void lightIgniterD() {
    pinHigh(fireDPin);
    charges.igniterDHot = true;
    charges.igniterDTick = millis();
}
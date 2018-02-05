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

    if (deploymentSettings.channelASettings.ENABLED & (deploymentSettings.channelASettings.TYPE == MAIN_t)) {
        lightIgniterA();
    }
    if(deploymentSettings.channelBSettings.ENABLED & (deploymentSettings.channelBSettings.TYPE == MAIN_t)) {
        lightIgniterB();
    }
    if (deploymentSettings.channelCSettings.ENABLED & (deploymentSettings.channelCSettings.TYPE == MAIN_t)) {
        lightIgniterC();
    }
    if (deploymentSettings.channelDSettings.ENABLED & (deploymentSettings.channelDSettings.TYPE == MAIN_t)) {
        lightIgniterD();
    }
}

void igniteDrogue() {

    if (deploymentSettings.channelASettings.ENABLED & (deploymentSettings.channelASettings.TYPE == DROGUE_t)) {
        lightIgniterA();
    }
    if (deploymentSettings.channelBSettings.ENABLED & (deploymentSettings.channelBSettings.TYPE == DROGUE_t)) {
        lightIgniterB();
    }
    if (deploymentSettings.channelCSettings.ENABLED & (deploymentSettings.channelCSettings.TYPE == DROGUE_t)) {
        lightIgniterC();
    }
    if (deploymentSettings.channelDSettings.ENABLED & (deploymentSettings.channelDSettings.TYPE == DROGUE_t)) {
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
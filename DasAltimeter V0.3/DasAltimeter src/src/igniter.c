#include <util.h>
#include <boardDefines.h>


bool checkIfMain(deploymentChannel_t chan) {

    if (chan.ENABLED & (chan.TYPE == MAIN_t)) {
        return true;
    } else {
        return false;
    }

}

bool checkIfDrogue(deploymentChannel_t chan) {

    if (chan.ENABLED & (chan.TYPE == DROGUE_t)) {
        return true;
    } else {
        return false;
    }
}

void checkContinuity() {

    charges.mainFound = false;
    charges.drogueFound = false;

    if (checkIfDrogue(deploymentSettings.channelASettings)) {
        if (sample.voltage.senseA > 500) {
            charges.drogueFound = true;
        }
    }
    if (checkIfMain(deploymentSettings.channelBSettings)) {
        if (sample.voltage.senseB > 500) {
            charges.mainFound = true;
        }
    }


}

void continuityBeep() {

    checkContinuity();

    if ((charges.mainFound == false) & (charges.drogueFound == false)) {
        beepDigit(0);
    } else if ((charges.mainFound == false) & (charges.drogueFound == true)) {
        beepDigit(1);
    } else if ((charges.mainFound == true) & (charges.drogueFound == false)) {
        beepDigit(2);
    } else if ((charges.mainFound == true) & (charges.drogueFound == true)) {
        beepDigit(3);
    }


}

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
#include <util.h>
#include <boardDefines.h>


void findMainAltitude() {

    if (deploymentSettings.channelASettings.ENABLED & deploymentSettings.channelASettings.TYPE == MAIN_t) {
        deploymentSettings.MAIN_DEPLOY = deploymentSettings.channelASettings.MAIN_DEPLOY;
        return;
    }
    if ((deploymentSettings.channelBSettings.ENABLED & deploymentSettings.channelBSettings.TYPE == MAIN_t)) {
        deploymentSettings.MAIN_DEPLOY = deploymentSettings.channelBSettings.MAIN_DEPLOY;
        return;
    }
    if ((deploymentSettings.channelCSettings.ENABLED & deploymentSettings.channelCSettings.TYPE == MAIN_t)) {
        deploymentSettings.MAIN_DEPLOY = deploymentSettings.channelCSettings.MAIN_DEPLOY;
        return;
    }
    if ((deploymentSettings.channelDSettings.ENABLED & deploymentSettings.channelDSettings.TYPE == MAIN_t)) {
        deploymentSettings.MAIN_DEPLOY = deploymentSettings.channelDSettings.MAIN_DEPLOY;
        return;
    }


}

void applyDefaultSettings() {

    deploymentSettings.channelASettings.ENABLED = true;
    deploymentSettings.channelASettings.TYPE = DROGUE_t;
    deploymentSettings.channelASettings.MAIN_DEPLOY = 0;
    deploymentSettings.channelASettings.APPOGE_DELAY = 0;

    deploymentSettings.channelBSettings.ENABLED = true;
    deploymentSettings.channelBSettings.TYPE = MAIN_t;
    deploymentSettings.channelBSettings.MAIN_DEPLOY = 300;
    deploymentSettings.channelBSettings.APPOGE_DELAY = 0;

    deploymentSettings.channelCSettings.ENABLED = false;
    deploymentSettings.channelCSettings.TYPE = DROGUE_t;
    deploymentSettings.channelCSettings.MAIN_DEPLOY = 0;
    deploymentSettings.channelCSettings.APPOGE_DELAY = 0;

    deploymentSettings.channelDSettings.ENABLED = false;
    deploymentSettings.channelDSettings.TYPE = MAIN_t;
    deploymentSettings.channelDSettings.MAIN_DEPLOY = 300;
    deploymentSettings.channelDSettings.APPOGE_DELAY = 0;
}

void putSettings() {

    u8_t settings[256];

    settings[0] = deploymentSettings.channelASettings.ENABLED;
    settings[1] = deploymentSettings.channelASettings.TYPE;
    settings[2] = deploymentSettings.channelASettings.MAIN_DEPLOY >> 8;
    settings[3] = deploymentSettings.channelASettings.MAIN_DEPLOY >> 0;
    settings[4] = deploymentSettings.channelASettings.APPOGE_DELAY;

    settings[5] = deploymentSettings.channelBSettings.ENABLED;
    settings[6] = deploymentSettings.channelBSettings.TYPE;
    settings[7] = deploymentSettings.channelBSettings.MAIN_DEPLOY >> 8;
    settings[8] = deploymentSettings.channelBSettings.MAIN_DEPLOY >> 0;
    settings[9] = deploymentSettings.channelBSettings.APPOGE_DELAY;

    settings[10] = deploymentSettings.channelCSettings.ENABLED;
    settings[11] = deploymentSettings.channelCSettings.TYPE;
    settings[12] = deploymentSettings.channelCSettings.MAIN_DEPLOY >> 8;
    settings[13] = deploymentSettings.channelCSettings.MAIN_DEPLOY >> 0;
    settings[14] = deploymentSettings.channelCSettings.APPOGE_DELAY;

    settings[15] = deploymentSettings.channelDSettings.ENABLED;
    settings[16] = deploymentSettings.channelDSettings.TYPE;
    settings[17] = deploymentSettings.channelDSettings.MAIN_DEPLOY >> 8;
    settings[18] = deploymentSettings.channelDSettings.MAIN_DEPLOY >> 0;
    settings[19] = deploymentSettings.channelDSettings.APPOGE_DELAY;

    settings[20] = 0x00;

    AT25SEWritePage(0x00,settings);
}




void getSettings() {

    u8_t settings[256];

    AT25SEreadPage(0x00 , settings);

    deploymentSettings.channelASettings.ENABLED = settings[0];
    deploymentSettings.channelASettings.TYPE = settings[1];
    deploymentSettings.channelASettings.MAIN_DEPLOY = settings[2] << 8 | (settings[3]);
    deploymentSettings.channelASettings.APPOGE_DELAY = settings[4];

    deploymentSettings.channelBSettings.ENABLED = settings[5];
    deploymentSettings.channelBSettings.TYPE = settings[6];
    deploymentSettings.channelBSettings.MAIN_DEPLOY = settings[7] << 8 | (settings[8]);
    deploymentSettings.channelBSettings.APPOGE_DELAY = settings[9];

    deploymentSettings.channelCSettings.ENABLED = settings[10];
    deploymentSettings.channelCSettings.TYPE = settings[11];
    deploymentSettings.channelCSettings.MAIN_DEPLOY = settings[12] << 8 | (settings[13]);
    deploymentSettings.channelCSettings.APPOGE_DELAY = settings[14];

    deploymentSettings.channelDSettings.ENABLED = settings[15];
    deploymentSettings.channelDSettings.TYPE = settings[16];
    deploymentSettings.channelDSettings.MAIN_DEPLOY = settings[17] << 8 | (settings[18]);
    deploymentSettings.channelDSettings.APPOGE_DELAY = settings[19];


    if (settings[20] == 0xFF) {
        applyDefaultSettings();
        putSettings();
    }

    findMainAltitude();

}




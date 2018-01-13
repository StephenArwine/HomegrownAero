#include <util.h>
#include <boardDefines.h>
#include <cc1101.h>


#define cc1101_select()     pinLow(cs_tx)
#define cc1101_deselect()	pinHigh(cs_tx)
#define CC1101_wait_miso()      while(pinRead(spiMISO) == true)




struct cc1101_reg {
    u16_t addr;
    char *name;
};

static const struct cc1101_reg cc1101_reg[] = {
    { .addr = CC1101_IOCFG2,        .name = "IOCFG2"},
    { .addr = CC1101_IOCFG1,        .name = "IOCFG1"},
    { .addr = CC1101_IOCFG0,        .name = "IOCFG0"},
    { .addr = CC1101_FIFOTHR,       .name = "FIFOTHR"},
    { .addr = CC1101_SYNC1,         .name = "SYNC1"},
    { .addr = CC1101_SYNC0,         .name = "SYNC0"},
    { .addr = CC1101_PKTLEN,        .name = "PKTLEN"},
    { .addr = CC1101_PKTCTRL1,      .name = "PKTCTRL1"},
    { .addr = CC1101_PKTCTRL0,      .name = "PKTCTRL0"},
    { .addr = CC1101_ADDR,          .name = "ADDR"},
    { .addr = CC1101_CHANNR,        .name = "CHANNR"},
    { .addr = CC1101_FSCTRL1,       .name = "FSCTRL1"},
    { .addr = CC1101_FSCTRL0,       .name = "FSCTRL0"},
    { .addr = CC1101_FREQ2,         .name = "FREQ2"},
    { .addr = CC1101_FREQ1,         .name = "FREQ1"},
    { .addr = CC1101_FREQ0,         .name = "FREQ0"},
    { .addr = CC1101_MDMCFG4,       .name = "MDMCFG4"},
    { .addr = CC1101_MDMCFG3,       .name = "MDMCFG3"},
    { .addr = CC1101_MDMCFG2,       .name = "MDMCFG2"},
    { .addr = CC1101_MDMCFG1,       .name = "MDMCFG1"},
    { .addr = CC1101_MDMCFG0,       .name = "MDMCFG0"},
    { .addr = CC1101_DEVIATN,       .name = "DEVIATN"},
    { .addr = CC1101_MCSM2,         .name = "MCSM2"},
    { .addr = CC1101_MCSM1,         .name = "MCSM1"},
    { .addr = CC1101_MCSM0,         .name = "MCSM0"},
    { .addr = CC1101_FOCCFG,        .name = "FOCCFG"},
    { .addr = CC1101_BSCFG,         .name = "BSCFG"},
    { .addr = CC1101_AGCCTRL2,      .name = "AGCCTRL2"},
    { .addr = CC1101_AGCCTRL1,      .name = "AGCCTRL1"},
    { .addr = CC1101_AGCCTRL0,      .name = "AGCCTRL0"},
    { .addr = CC1101_WOREVT1,       .name = "WOREVT1"},
    { .addr = CC1101_WOREVT0,       .name = "WOREVT0"},
    { .addr = CC1101_WORCTRL,       .name = "WORCTRL"},
    { .addr = CC1101_FREND1,        .name = "FREND1"},
    { .addr = CC1101_FREND0,        .name = "FREND0"},
    { .addr = CC1101_FSCAL3,        .name = "FSCAL3"},
    { .addr = CC1101_FSCAL2,        .name = "FSCAL2"},
    { .addr = CC1101_FSCAL1,        .name = "FSCAL1"},
    { .addr = CC1101_FSCAL0,        .name = "FSCAL0"},
    { .addr = CC1101_RCCTRL1,       .name = "RCCTRL1"},
    { .addr = CC1101_RCCTRL0,       .name = "RCCTRL0"},
    { .addr = CC1101_FSTEST,        .name = "FSTEST"},
    { .addr = CC1101_PTEST,         .name = "PTEST"},
    { .addr = CC1101_AGCTEST,       .name = "AGCTEST"},
    { .addr = CC1101_TEST2,         .name = "TEST2"},
    { .addr = CC1101_TEST1,         .name = "TEST1"},
    { .addr = CC1101_TEST0,         .name = "TEST0"},
    { .addr = CC1101_PARTNUM,       .name = "PARTNUM"},
    { .addr = CC1101_VERSION,       .name = "VERSION"},
    { .addr = CC1101_FREQEST,       .name = "FREQEST"},
    { .addr = CC1101_LQI,           .name = "LQI"},
    { .addr = CC1101_RSSI,          .name = "RSSI"},
    { .addr = CC1101_MARCSTATE,     .name = "MARCSTATE"},
    { .addr = CC1101_WORTIME1,      .name = "WORTIME1"},
    { .addr = CC1101_WORTIME0,      .name = "WORTIME0"},
    { .addr = CC1101_PKTSTATUS,     .name = "PKTSTATUS"},
    { .addr = CC1101_VCO_VC_DAC,    .name = "VCO_VC_DAC"},
    { .addr = CC1101_TXBYTES,       .name = "TXBYTES"},
    { .addr = CC1101_RXBYTES,       .name = "RXBYTES"},
    { .addr = CC1101_RCCTRL1_STATUS,.name = "RCCTRL1_STATUS"},
    { .addr = CC1101_RCCTRL0_STATUS,.name = "RCCTRL0_STATUS"}

};

u8_t cc1101_num_reg = (sizeof(cc1101_reg) / sizeof(cc1101_reg[0]));




void CC1101_cmd_strobe(u8_t reg) {

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, reg);
    cc1101_deselect();

}

void CC1101_set_RX_state() {

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_SRX);
    cc1101_deselect();
}

void CC1101_set_TX_state() {

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_STX);
    cc1101_deselect();
}

u8_t cc1101_read_reg(u8_t regester) {
    u8_t reg = 0;

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, regester);
    reg = byteIn(spiSCK, spiMISO);
    cc1101_deselect();

    return reg;
}



void CC1101_write_burst_reg(u8_t reg, u8_t* buffer, u8_t length) {

    u8_t i = 0;
    u8_t add = reg | WRITE_BURST;

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, add);
    for(i = 0; i < length; i++) {
        byteOut(spiSCK, spiMOSI, buffer[i]);
    }
    cc1101_deselect();

}

void cc1101_write_reg(u8_t reg, u8_t value) {

    cc1101_select();
    CC1101_wait_miso();
    byteOut(spiSCK, spiMOSI, reg | WRITE_BURST);
    byteOut(spiSCK, spiMOSI, value);
    cc1101_deselect();

}


void sendreg() {

    volatile u8_t regirsterssss[cc1101_num_reg];

    volatile u8_t i;

    for (i = 0; i <= cc1101_num_reg ; i++) {

        cc1101_select();
        while(pinRead(spiMISO) == true);
        byteOut(spiSCK, spiMOSI, i | 0x80);
        volatile u8_t reg_data = byteIn(spiSCK,spiMISO);
        cc1101_deselect();

        regirsterssss[i] = reg_data;
    }
    cc1101_deselect();
}

bool CC1101_tx_data(u8_t *packet, u8_t packenlen) {

    u8_t marcstate;
    bool res = false;

    //Enter RX state
    CC1101_set_RX_state();

    u16_t tries = 0;

    // Check that the RX state has been entered
    while (((marcstate = CC1101_read_status_reg(CC1101_MARCSTATE))) != 0x0D) {
        if (marcstate == 0x11) {     // RX_OVERFLOW

            SendUSART("RX overflow", strlen("RX overflow"));
            SendUSART(0x15, 1);
            cc1101_select();
            while(pinRead(spiMISO) == true);
            byteOut(spiSCK, spiMOSI, CC1101_SFRX);	// Flush RXFIFO
            cc1101_deselect();
        }
        if (tries >= 1000) {
            // TODO: MarcState sometimes never enters the expected state; this is a hack workaround.
            return false;
        }
    }



    delay_us(500);

    cc1101_write_reg(CC1101_TXFIFO, packenlen);
    CC1101_write_burst_reg(CC1101_TXFIFO, packet, packenlen);

    delay_us(500);

    //

    CC1101_set_TX_state();

    delay_us(500);


    // Check that TX state is being entered (state = RXTX_SETTLING)
    marcstate =  CC1101_read_status_reg(CC1101_MARCSTATE) & 0x1F;
    if ((marcstate != 0x13) && (marcstate != 0x14) && (marcstate != 0x15)) {

        SendUSART("TX not entered", strlen("TX not entered"));
        SendUSART(0x15, 1);

        cc1101_select();
        while(pinRead(spiMISO) == true);
        byteOut(spiSCK, spiMOSI, CC1101_SIDLE);	// Enter IDLE state
        cc1101_deselect();

        cc1101_select();
        while(pinRead(spiMISO) == true);
        byteOut(spiSCK, spiMOSI, CC1101_SFTX);	// Flush Tx FIFO
        cc1101_deselect();

        cc1101_select();
        while(pinRead(spiMISO) == true);
        byteOut(spiSCK, spiMOSI, CC1101_SFRX);	// Back to RX state
        cc1101_deselect();

        return false;
    }

    while (CC1101_read_status_reg(CC1101_TXBYTES) != 0) {
        delay_ms(10);
    };

    if ((CC1101_read_status_reg(CC1101_TXBYTES)) == 0) {
        cc1101_select();
        while(pinRead(spiMISO) == true);
        byteOut(spiSCK, spiMOSI, CC1101_SIDLE);	// Enter IDLE state
        cc1101_deselect();

        res = true;
    }

    return res;
}

void write_cc1101_status_regersters() {

    cc1101_write_reg(CC1101_IOCFG2,RF_IOCFG2);
    cc1101_write_reg(CC1101_IOCFG1,RF_IOCFG1);
    cc1101_write_reg(CC1101_IOCFG0,RF_IOCFG0);
    cc1101_write_reg(CC1101_FIFOTHR,RF_FIFOTHR);
    cc1101_write_reg(CC1101_SYNC1,RF_SYNC1);
    cc1101_write_reg(CC1101_SYNC0,RF_SYNC0);
    cc1101_write_reg(CC1101_PKTLEN,RF_PKTLEN);
    cc1101_write_reg(CC1101_PKTCTRL1,RF_PKTCTRL1);
    cc1101_write_reg(CC1101_PKTCTRL0,RF_PKTCTRL0);
    cc1101_write_reg(CC1101_ADDR,RF_ADDR);
    cc1101_write_reg(CC1101_CHANNR,RF_CHANNR);
    cc1101_write_reg(CC1101_FSCTRL1,RF_FSCTRL1);
    cc1101_write_reg(CC1101_FSCTRL0,RF_FSCTRL0);
    cc1101_write_reg(CC1101_FREQ2,RF_FREQ2);
    cc1101_write_reg(CC1101_FREQ1,RF_FREQ1);
    cc1101_write_reg(CC1101_FREQ0,RF_FREQ0);
    cc1101_write_reg(CC1101_MDMCFG4,RF_MDMCFG4);
    cc1101_write_reg(CC1101_MDMCFG3,RF_MDMCFG3);
    cc1101_write_reg(CC1101_MDMCFG2,RF_MDMCFG2);
    cc1101_write_reg(CC1101_MDMCFG1,RF_MDMCFG1);
    cc1101_write_reg(CC1101_MDMCFG0,RF_MDMCFG0);
    cc1101_write_reg(CC1101_DEVIATN,RF_DEVIATN);
    cc1101_write_reg(CC1101_MCSM2,RF_MCSM2);
    cc1101_write_reg(CC1101_MCSM1,RF_MCSM1);
    cc1101_write_reg(CC1101_MCSM0,RF_MCSM0);
    cc1101_write_reg(CC1101_FOCCFG,RF_FOCCFG);
    cc1101_write_reg(CC1101_BSCFG,RF_BSCFG);
    cc1101_write_reg(CC1101_AGCCTRL2,RF_AGCCTRL2);
    cc1101_write_reg(CC1101_AGCCTRL1,RF_AGCCTRL1);
    cc1101_write_reg(CC1101_AGCCTRL0,RF_AGCCTRL0);
    cc1101_write_reg(CC1101_WOREVT1,RF_WOREVT1);
    cc1101_write_reg(CC1101_WOREVT0,RF_WOREVT0);
    cc1101_write_reg(CC1101_WORCTRL,RF_WORCTRL);
    cc1101_write_reg(CC1101_FREND1,RF_FREND1);
    cc1101_write_reg(CC1101_FREND0,RF_FREND0);
    cc1101_write_reg(CC1101_FSCAL3,RF_FSCAL3);
    cc1101_write_reg(CC1101_FSCAL2,RF_FSCAL2);
    cc1101_write_reg(CC1101_FSCAL1,RF_FSCAL1);
    cc1101_write_reg(CC1101_FSCAL0,RF_FSCAL0);
    cc1101_write_reg(CC1101_RCCTRL1,RF_RCCTRL1);
    cc1101_write_reg(CC1101_RCCTRL0,RF_RCCTRL0);
    cc1101_write_reg(CC1101_FSTEST,RF_FSTEST);
    cc1101_write_reg(CC1101_PTEST,RF_PTEST);
    cc1101_write_reg(CC1101_AGCTEST,RF_AGCTEST);
    cc1101_write_reg(CC1101_TEST2,RF_TEST2);
    cc1101_write_reg(CC1101_TEST1,RF_TEST1);
    cc1101_write_reg(CC1101_TEST0,RF_TEST0);
    cc1101_write_reg(CC1101_PARTNUM,RF_PARTNUM);
    cc1101_write_reg(CC1101_VERSION,RF_VERSION);
    cc1101_write_reg(CC1101_FREQEST,RF_FREQEST);
    cc1101_write_reg(CC1101_LQI,RF_LQI);
    cc1101_write_reg(CC1101_RSSI,RF_RSSI);
    cc1101_write_reg(CC1101_MARCSTATE,RF_MARCSTATE);
    cc1101_write_reg(CC1101_WORTIME1,RF_WORTIME1);
    cc1101_write_reg(CC1101_WORTIME0,RF_WORTIME0);
    cc1101_write_reg(CC1101_PKTSTATUS,RF_PKTSTATUS);
    cc1101_write_reg(CC1101_VCO_VC_DAC,RF_VCO_VC_DAC);
    cc1101_write_reg(CC1101_TXBYTES,RF_TXBYTES);
    cc1101_write_reg(CC1101_RXBYTES,RF_RXBYTES);
    cc1101_write_reg(CC1101_RCCTRL1_STATUS,RF_RCCTRL1_STATUS);
    cc1101_write_reg(CC1101_RCCTRL0_STATUS,RF_RCCTRL0_STATUS);

    cc1101_select();
    while(pinRead(spiMISO) == true);
    cc1101_deselect();
}

void CC1101_reset_chip() {

    cc1101_deselect();
    delay_us(30);
    cc1101_select();
    delay_us(30);
    cc1101_deselect();
    delay_us(45);

    //send Reset
    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_SRES);
    while(pinRead(spiMISO) == true);
    cc1101_deselect();

    //configure reg
    write_cc1101_status_regersters();

}

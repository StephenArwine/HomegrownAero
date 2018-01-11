#include <util.h>
#include <boardDefines.h>
#include <cc1101.h>


#define cc1101_select()     pinLow(cs_tx)
#define cc1101_deselect()	pinHigh(cs_tx)




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


void CC1101_set_RX_state(){

	cc1101_select();
    while(pinRead(spiMISO) == true);
	byteOut(spiSCK, spiMOSI, CC1101_SRX);
	cc1101_deselect();
}

void CC1101_set_TX_state(){

	cc1101_select();
    while(pinRead(spiMISO) == true);
	byteOut(spiSCK, spiMOSI, CC1101_STX);
	cc1101_deselect();
}

u8_t cc1101_read_reg(u8_t reg){

    cc1101_select();
    while(pinRead(spiMISO) == true);
	byteOut(spiSCK, spiMOSI, reg);
    u8_t reg = byteIn(spiSCK, spiMISO);
	cc1101_deselect();

	return reg;
}

void CC1101_write_reg(u8_t reg, u8_t value){

    cc1101_select();
    while(pinRead(spiMISO) == true);
	byteOut(spiSCK, spiMOSI, reg);
	byteOut(spiSCK, spiMOSI, value);
	cc1101_deselect();

}

void CC1101_write_burst_reg(u8_t reg, u8_t* buffer, u8_t length){

	u8_t i = 0;
	u8_t add = reg | WRITE_BURST;

    cc1101_select();
    while(pinRead(spiMISO) == true);
	byteOut(spiSCK, spiMOSI, reg);
	for(i = 0; i < length; i++){
		byteOut(spiSCK, spiMOSI, buffer[i]);
	}
	cc1101_deselect();

}


void sendreg() {

    volatile u8_t regirsterssss[200];

    volatile u8_t i;

    for (i = 0; i < cc1101_num_reg ; i++) {

        cc1101_select();
        while(pinRead(spiMISO) == true);
        byteOut(spiSCK, spiMOSI, i | 0x80);
        volatile u8_t reg_data = byteIn(spiSCK,spiMISO);
        cc1101_deselect();

        regirsterssss[i] = reg_data;

    }
    cc1101_deselect();
}

bool CC1101_tx_data(u8_t* packet, u8_t packenlen){

	u8_t marcstate;
	bool res = false;
	
	//Enter RX state
	CC1101_set_RX_state();
	
   u16_t tries = 0
   // Check that the RX state has been entered 
   while (tries++ < 1000 && ((marcState = CC1101_read_status_reg(CC1101_MARCSTATE)) & 0x1F) != 0x0D) 
   { 
    if (marcState == 0x11){        // RX_OVERFLOW 
    //  flushRxFifo();     // flush receive queue 
		} 
	if (tries >= 1000) { 
		// TODO: MarcState sometimes never enters the expected state; this is a hack workaround. 
		return false; 
		}
   } 

   delay_us(500);
   
   CC1101_write_reg(CC1101_TXFIFO, packenlen);
   CC1101_write_burst_reg(CC1101_TXFIFO, packet, packenlen);
   
   
   CC1101_set_TX_state();
   
   
   
   // Check that TX state is being entered (state = RXTX_SETTLING) 
   marcState = readStatusReg(CC1101_MARCSTATE) & 0x1F; 
   if ((marcState != 0x13) && (marcState != 0x14) && (marcState != 0x15)) 
   { 
   
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
		flushTxFifo();         
		setRxState();          
 
		return false; 
   } 

   delay_ms(200);

   if (CC1101_read_status_reg(CC1101_TXBYTES) &0x70) == 0){
   		cc1101_select();
		while(pinRead(spiMISO) == true);
		byteOut(spiSCK, spiMOSI, CC1101_SIDLE);	// Enter IDLE state 
		cc1101_deselect(); 
		
		res = true;
   
   
   }

return res;
}

void write_cc1101_status_regersters() {

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_IOCFG0);
    byteOut(spiSCK, spiMOSI, RF_IOCFG0);
    cc1101_deselect();


    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FIFOTHR);
    byteOut(spiSCK, spiMOSI, RF_FIFOTHR);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_PKTCTRL0);
    byteOut(spiSCK, spiMOSI, RF_PKTCTRL0);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FSCTRL1);
    byteOut(spiSCK, spiMOSI, RF_FSCTRL1);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FREQ2);
    byteOut(spiSCK, spiMOSI, RF_FREQ2);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FREQ1);
    byteOut(spiSCK, spiMOSI, RF_FREQ1);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FREQ0);
    byteOut(spiSCK, spiMOSI, RF_FREQ0);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_MDMCFG4);
    byteOut(spiSCK, spiMOSI, RF_MDMCFG4);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_MDMCFG3);
    byteOut(spiSCK, spiMOSI, RF_MDMCFG3);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_MDMCFG2);
    byteOut(spiSCK, spiMOSI, RF_MDMCFG2);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_DEVIATN);
    byteOut(spiSCK, spiMOSI, RF_DEVIATN);
    cc1101_deselect();
	
    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_MCSM1);
    byteOut(spiSCK, spiMOSI, RF_MCSM0);
    cc1101_deselect();


    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_MCSM0);
    byteOut(spiSCK, spiMOSI, RF_MCSM0);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FOCCFG);
    byteOut(spiSCK, spiMOSI, RF_FOCCFG);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_WORCTRL);
    byteOut(spiSCK, spiMOSI, RF_WORCTRL);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FSCAL3);
    byteOut(spiSCK, spiMOSI, RF_FSCAL3);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FSCAL2);
    byteOut(spiSCK, spiMOSI, RF_FSCAL2);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FSCAL1);
    byteOut(spiSCK, spiMOSI, RF_FSCAL1);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_FSCAL0);
    byteOut(spiSCK, spiMOSI, RF_FSCAL0);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_TEST2);
    byteOut(spiSCK, spiMOSI, RF_TEST2);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_TEST1);
    byteOut(spiSCK, spiMOSI, RF_TEST1);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, CC1101_TEST0);
    byteOut(spiSCK, spiMOSI, RF_TEST0);
    cc1101_deselect();

    cc1101_select();
    while(pinRead(spiMISO) == true);
    byteOut(spiSCK, spiMOSI, 0x3E);
    byteOut(spiSCK, spiMOSI, 0xC0);
    cc1101_deselect();


}

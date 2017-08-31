#include <services.h>


void USART_init(uint32_t baud) {

    uint32_t USART_CLKGEN_F = 8000000UL;
    uint64_t br = (uint64_t)65536 * (USART_CLKGEN_F - 16 * baud) / USART_CLKGEN_F;

    //enable GPS pins
    //  SaLPinMode(MTK3339_RX_PIN,INPUT);
    //  SaLPinMode(MTK3339_TX_PIN,OUTPUT);
    SYSCTRL->OSC8M.reg -= SYSCTRL_OSC8M_ENABLE;
    SYSCTRL->OSC8M.reg -= SYSCTRL_OSC8M_PRESC_3;
    SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ENABLE;



    //portB22->PINCFG->reg = 0x44;
    // portB23->PINCFG->reg = 0x44;

    // GPS pin configs
    ((Port *)PORT)->Group[1].PINCFG[22].reg = 0x41;
    ((Port *)PORT)->Group[1].PINCFG[23].reg = 0x41;
    ((Port *)PORT)->Group[1].PMUX[11].reg = 0x32;


    // usb port configs
    //  ((Port *)PORT)->Group[0].PINCFG[24].reg = 0x41;
    // ((Port *)PORT)->Group[0].PINCFG[25].reg = 0x41;
    // ((Port *)PORT)->Group[0].PMUX[12].reg = 0x24;


    //enable power to sercom 5 module
    PM->APBCMASK.reg |= PM_APBCMASK_SERCOM5;
    //enable and configure the sercom clock
    GCLK->GENDIV.reg =  GCLK_GENDIV_ID(3) |
                        GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(3) |
                        GCLK_GENCTRL_SRC_OSC8M |
                        GCLK_GENCTRL_IDC |
                        GCLK_GENCTRL_RUNSTDBY |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM5_CORE |
                        GCLK_CLKCTRL_GEN_GCLK3 |
                        GCLK_CLKCTRL_CLKEN;
    //     GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOMX_SLOW |
    //                         GCLK_CLKCTRL_GEN_GCLK3 |
    //                         GCLK_CLKCTRL_CLKEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM3_CORE |
                        GCLK_CLKCTRL_GEN_GCLK3 |
                        GCLK_CLKCTRL_CLKEN;

    //configure the sercom module for the gps (sercom 5)
    SERCOM5->USART.CTRLA.reg = SERCOM_USART_CTRLA_DORD |
                               SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
                               SERCOM_USART_CTRLA_RXPO(3) |
                               SERCOM_USART_CTRLA_TXPO(1);
    USART_sync(SERCOM5);
    SERCOM5->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN |
                               SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);
    // SERCOM_USART_CTRLB_SFDE;
    USART_sync(SERCOM5);
    SERCOM5->USART.BAUD.reg = (uint16_t)br;
    USART_sync(SERCOM5);
    SERCOM5->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
    USART_sync(SERCOM5);

    // initUSART(&USART_0,SERCOM5);


}


int32_t IoWrite(struct IoDescriptor *const IoDescr,
const uint8_t *const buf,
const uint16_t length) {

	return IoDescr->write(IoDescr,buf,length);
};

int32_t IoRead(struct IoDescriptor *const IoDescr,
uint8_t *const buf,
const uint16_t length) {

	return IoDescr->read(IoDescr,buf,length);
};





int32_t initUSART(struct USARTDescriptor *const descr,
                  void *const hw) {

    descr->device.hw = hw;
    descr->io.read = USARTDataRead;
    descr->io.write = USARTDataWrite;
    return (int32_t)NULL;
}


void syncUSARTIo(struct USARTDescriptor *const descr,
                 struct IoDescriptor **Io) {
    *Io = &descr->io;
}

bool _USARTByteRecieved(const struct _USARTDevice *const device) {
    return _USARTGetInteruptRCX(device->hw);
}

uint8_t _USARTGetData(const struct _USARTDevice *const device) {
    return _USARTGetDataReg(device->hw);
}

bool _USARTGetDreInterupt(const struct _USARTDevice *const device) {
    return _USARTGetDreInteruptReg(device->hw);
}

void _USARTSetData(const struct _USARTDevice *const device, uint8_t data) {
    _USARTSetDataReg(device->hw,data);
}




static int32_t USARTDataRead(struct IoDescriptor *const ioDescr,
                             uint8_t *const buf,
                             const uint16_t length) {

    int32_t offset = 0;
    struct USARTDescriptor *descr = CONTAINER_OF(ioDescr,struct USARTDescriptor, io);

    do {
        while (!_USARTByteRecieved(&descr->device));
        buf[offset] = _USARTGetData(&descr->device);
    } while (++offset < length);

    return (int32_t)offset;
}

static int32_t USARTDataWrite(struct IoDescriptor *const ioDescr,
                              const uint8_t *const buf,
                              const uint16_t length) {

    int32_t offset = 0;
    struct USARTDescriptor *descr = CONTAINER_OF(ioDescr,struct USARTDescriptor, io);

    while (!_USARTGetDreInterupt(&descr->device));
    do {
        _USARTSetDataReg(&descr->device,buf[offset]);
        while (!_USARTGetDreInterupt(&descr->device));
    } while (++offset < length);
    return offset;

}


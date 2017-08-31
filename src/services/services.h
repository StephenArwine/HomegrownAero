#include <util.h>

/// USART.c Stuff
#ifndef _SALUSART_H_
#define _SALUSART_H_


//usart.c

static inline void USART_sync(sercomId id) {while (sercom(id)->USART.SYNCBUSY.bit.CTRLB)};
static inline void _USARTSetDataReg(const void *const hw, uint8_t data ) {
    ATOMIC_SECTION_ENTER
    ((Sercom *)hw)->USART.DATA.reg = data;
    ATOMIC_SECTION_LEAVE
};

bool _usartByteRecieved(sercomId id);
uint8_t _usartGetData(sercomId id);
static int32_t usartDataRead(sercomId id, uint8_t *const buf, const uint16_t len); 
void sercomUSARTInit(sercomId id, u32 buad);





#endif

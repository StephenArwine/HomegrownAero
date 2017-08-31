#include <util.h>

/// USART.c Stuff
#ifndef _SALUSART_H_
#define _SALUSART_H_

static inline void USART_sync(sercomId id) {
    while (sercom(id)->USART.SYNCBUSY.bit.CTRLB);
};




static inline void _USARTSetDataReg(const void *const hw, uint8_t data ) {
    ATOMIC_SECTION_ENTER
    ((Sercom *)hw)->USART.DATA.reg = data;
    ATOMIC_SECTION_LEAVE
}

void sercomUSARTInit(sercomId id, u32 buad);

#endif

#include <util.h>

/// USART.c Stuff
#ifndef _SALUSART_H_
#define _SALUSART_H_

static inline void USART_sync(const void *const hw) {
    while (((Sercom *)hw)->USART.SYNCBUSY.bit.CTRLB);
};

static inline bool _USARTGetInteruptRCX(const void *const hw) {
    return (((Sercom *)hw)->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC) >> SERCOM_USART_INTFLAG_RXC_Pos;

};

static inline sercomUSARTDataReg_t _USARTGetDataReg(const void *const hw) {
    return ((Sercom *)hw)->USART.DATA.reg;
};

static inline bool _USARTGetDreInteruptReg(const void *const hw) {
    return (((Sercom *)hw)->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE) >> SERCOM_USART_INTFLAG_DRE_Pos;
};

static inline void _USARTSetDataReg(const void *const hw, uint8_t data ) {
    ATOMIC_SECTION_ENTER
    ((Sercom *)hw)->USART.DATA.reg = data;
    ATOMIC_SECTION_LEAVE
}

void sercomUSARTInit(sercomPort descr);

#endif

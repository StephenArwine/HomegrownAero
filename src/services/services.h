#include <util.h>

/// USART.c Stuff
#ifndef _SALUSART_H_
#define _SALUSART_H_


//usart.c



static inline void USART_sync(const void *const hw) {
    while  (SERCOM5->USART.SYNCBUSY.bit.CTRLB);
};
static inline void _USARTSetDataReg(const void *const hw, uint8_t data ) {
    ATOMIC_SECTION_ENTER
    ((Sercom *)hw)->USART.DATA.reg = data;
    ATOMIC_SECTION_LEAVE
};

static inline bool _usartGetInteruptDre(const void *const hw) {
	return (((Sercom *)hw)->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE) >> SERCOM_USART_INTFLAG_DRE_Pos;
};

static inline bool _usartGetInteruptRCX(const void *const hw) {
    return (((Sercom *)hw)->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC) >> SERCOM_USART_INTFLAG_RXC_Pos;

};


bool _usartByteRecieved(const void *const hw);
uint8_t _usartGetData(const void *const hw);
int32_t usartDataRead(const void *const hw, uint8_t *const buf, const uint16_t len);
void sercomUSARTInit(const void *const hw, u32_t buad);





#endif

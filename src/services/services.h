#include <util.h>

/// USART.c Stuff
#ifndef _SALUSART_H_
#define _SALUSART_H_

static inline void USART_sync(const void *const hw) {
    while (((Sercom *)hw)->USART.SYNCBUSY.bit.CTRLB);
};

struct IoDescriptor;
typedef uint16_t SercomUSARTDataReg_t;


typedef int32_t (* IoWrite_t)(struct IoDescriptor *const IoDescr,
                              const uint8_t *const buf,
                              const uint16_t length);

typedef int32_t (* IoRead_t)(struct IoDescriptor *const IoDescr,
                             uint8_t *const buf,
                             const uint16_t length);

struct IoDescriptor {
    IoWrite_t write;
    IoRead_t read;
};





struct _USARTDevice {
    void *hw;
};




struct USARTDescriptor {
    struct IoDescriptor io;
    struct _USARTDevice device;
};



struct USARTDescriptor USART_0;

static int32_t USARTDataRead(struct IoDescriptor *const IoDescr,
                             uint8_t *const buf,
                             const uint16_t length);
							 
static int32_t USARTDataWrite(struct IoDescriptor *const ioDescr,
                              const uint8_t *const buf,
                              const uint16_t length);


static inline bool _USARTGetInteruptRCX(const void *const hw) {
    return (((Sercom *)hw)->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC) >> SERCOM_USART_INTFLAG_RXC_Pos;

};

static inline SercomUSARTDataReg_t _USARTGetDataReg(const void *const hw) {
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

void USART_init(uint32_t baud);

#endif
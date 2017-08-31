#ifndef PORT_H_
#define PORT_H_

#include <util.h>


#define INPUT 0
#define OUTPUT 1
#define OUTSTRONG 3

typedef struct Pin {
    u8_t  mux;
    u8_t  cfg;
    u8_t  group;
    u8_t  pin;
    u8_t  chan;
} Pin;

//sercom port define

#define UART_RX_SIZE 32

typedef struct _uartBuf {
u8 head;
u8 tail;
u8 bufLen;
u8 rx[UART_RX_SIZE];
} uartBuf;


typedef struct sercomPort{
// USART buffer
_uartBuf uartBuf;

//port info
Pin txpo;
Pin rxpo;
u32 buad;


} sercomPort;




// general IO port stuff
static inline PortGroup* getPort(
       const u8_t gpioPin) {
u8_t port_index = (gpioPin / 128);
u8_t group_index = (gpioPin / 32);
/* Array of available ports */ 
    Port *const ports[PORT_INST_NUM] = PORT_INSTS; 
 
     if (port_index < PORT_INST_NUM) { 
       return &(ports[port_index]->Group[group_index]); 
     } else { 
         return NULL; 
     } 
 }; 

inline static void pinAnalog(Pin p) { 
     if (p.pin & 1) { 
         PORT->Group[p.group].PMUX[p.pin/2].bit.PMUXO = 0x1; 
     } else { 
         PORT->Group[p.group].PMUX[p.pin/2].bit.PMUXE = 0x1; 
     } 
 
     PORT->Group[p.group].PINCFG[p.pin].bit.PMUXEN = 1; 
 } 
 

inline static bool pinRead(Pin p) { 
     return (PORT->Group[p.group].IN.reg & (1<<p.pin)) != 0; 
 } 

inline static void pinMux(Pin p) { 
if (p.pin & 1) { 
PORT->Group[p.group].PMUX[p.pin/2].bit.PMUXO = p.mux; 
} else { 
PORT->Group[p.group].PMUX[p.pin/2].bit.PMUXE = p.mux; 
} 
PORT->Group[p.group].PINCFG[p.pin].bit.PMUXEN = 1; 
} 

 
inline static void pinGpio(Pin p) { 
PORT->Group[p.group].PINCFG[p.pin].bit.PMUXEN = 0; 
} 

 
inline static void pinCfg(Pin p) { 
PORT->Group[p.group].PINCFG[p.pin].reg |= p.cfg; 
} 

 
inline static void pinIn(Pin p) { 
pinGpio(p); 
    PORT->Group[p.group].PINCFG[p.pin].bit.INEN = 1; 
     PORT->Group[p.group].DIRCLR.reg = (1<<p.pin); 
 } 
 
 
 inline static void pinOut(Pin p) { 
     pinGpio(p);   
   PORT->Group[p.group].DIRSET.reg = (1<<p.pin);  
} 
 
 
 
 
 inline static void pinHigh(Pin p) { 
     PORT->Group[p.group].OUTSET.reg = (1<<p.pin); 
 } 
 
 
 inline static void pinLow(Pin p) { 
     PORT->Group[p.group].OUTCLR.reg = (1<<p.pin); 
 } 
 
 
 inline static void pinToggle(Pin p) { 
     PORT->Group[p.group].OUTTGL.reg = (1<<p.pin); 
 } 

#endif

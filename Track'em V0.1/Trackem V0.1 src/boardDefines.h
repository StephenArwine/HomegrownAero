//#include <util.h>

#define  USART0 0
#define  USART1 1



//Track'em V0.1



//const static Pin cs_mem = {.group = 0, .pin = 2};
const static Pin cs_tx = {.group = 0, .pin = 16};


//const static Pin GPSTxPo = {.group = 0, .pin = 14, .mux = MUX_PA14C_SERCOM0_PAD0};
//const static Pin GPSRxPo = {.group = 0, .pin = 15, .mux = MUX_PA15C_SERCOM0_PAD1};

//const static Pin ComRxPo = {.group = 0, .pin = 25, .mux = MUX_PA25C_SERCOM1_PAD3};
//const static Pin ComTxPo = {.group = 0, .pin = 24, .mux = MUX_PA24C_SERCOM1_PAD2};

const static Pin spiMOSI = {.group = 0, .pin = 23};
const static Pin spiMISO = {.group = 0, .pin = 21};
const static Pin spiSCK = {.group = 0, .pin = 22};




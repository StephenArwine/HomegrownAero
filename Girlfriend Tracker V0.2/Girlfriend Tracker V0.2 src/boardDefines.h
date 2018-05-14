//#include <util.h>

#define  USART0 0
#define  USART1 1



//Track'em V0.1



//const static Pin cs_mem = {.group = 0, .pin = 2};
const static Pin cs_tx = {.group = 0, .pin = 16};
const static Pin Pin_GDO0 = {.group = 0, .pin = 17};
	
const static Pin AMP_EN = {.group = 0, .pin = 24}; 



const static Pin GPSTxPo = {.group = 0, .pin = 0, .mux = MUX_PA00D_SERCOM1_PAD0};
const static Pin GPSRxPo = {.group = 0, .pin = 1, .mux = MUX_PA01D_SERCOM1_PAD1};

//const static Pin ComRxPo = {.group = 0, .pin = 25, .mux = MUX_PA25C_SERCOM1_PAD3};
//const static Pin ComTxPo = {.group = 0, .pin = 24, .mux = MUX_PA24C_SERCOM1_PAD2};

const static Pin LEDPin = {.group = 1, .pin = 9};

const static Pin spiMOSI = {.group = 0, .pin = 23, .mux = MUX_PA23D_SERCOM5_PAD1};
const static Pin spiMISO = {.group = 0, .pin = 21, .mux = MUX_PA21C_SERCOM5_PAD3};
const static Pin spiSCK = {.group = 0, .pin = 22, .mux = MUX_PA22D_SERCOM5_PAD0};




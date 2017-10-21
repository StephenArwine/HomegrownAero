//#include <util.h>

#define SPI1 2

//frostburn
const static Pin cs_mem = {.group = 0, .pin = 20 };
const static Pin cs_baro = {.group = 0, .pin = 14 };
const static Pin cs_imu = {.group = 0, .pin = 20 };


const static Pin spi1MISO = {.group = 0, .pin = 15, .mux = MUX_PA15C_SERCOM2_PAD3 };
const static Pin spi1SCK = {.group = 0, .pin = 13, .mux = MUX_PA13C_SERCOM2_PAD1};
const static Pin spi1MOSI = {.group = 0, .pin = 12, .mux = MUX_PA12C_SERCOM2_PAD0 };

// mux is not accurate to produced board
//const static Pin spi2MISO = {.group = 0, .pin = 13, .mux = MUX_PA13C_SERCOM2_PAD1 };
//const static Pin spi2SCK = {.group = 0, .pin = 15, .mux = MUX_PA15C_SERCOM2_PAD3 };
//const static Pin spi2MOSI = {.group = 0, .pin = 14, .mux = MUX_PA14C_SERCOM2_PAD2 };

// "salaltimeter
//    const static Pin cs_mem = {.group = 0, .pin = 8 };
//    const static Pin spi1MISO = {.group = 0, .pin = 14 };
//    const static Pin spi1SCK = {.group = 0, .pin = 15};
//    const static Pin spi1MOSI = {.group = 0, .pin = 13 };

const static Pin LedPin = {.group = 0, .pin = 28 };
const static Pin senseBat = {.group = 0, .pin = 5, .chan = 5 };

#define DMA_SERCOM1_TX 1
#define DMA_SERCOM1_RX 2

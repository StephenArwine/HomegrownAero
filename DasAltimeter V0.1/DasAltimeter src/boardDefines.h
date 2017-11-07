//#include <util.h>

#define SPI1 1
#define SPI2 2

//dasAltimeter
const static Pin cs_mem = {.group = 0, .pin = 18};
const static Pin cs_baro = {.group = 0, .pin = 15};
//const static Pin cs_imu = {.group = 0, .pin = 20};

//const static Pin spi1MOSI = {.group = 0, .pin = 12, .mux = MUX_PA12C_SERCOM2_PAD0};
//const static Pin spi1SCK = {.group = 0, .pin = 13, .mux = MUX_PA13C_SERCOM2_PAD1};
//const static Pin spi1MISO = {.group = 0, .pin = 15, .mux = MUX_PA15C_SERCOM2_PAD3};

const static Pin spi2MOSI = {.group = 0, .pin = 12, .mux = MUX_PA12C_SERCOM2_PAD0};
const static Pin spi2MISO = {.group = 0, .pin = 13, .mux = MUX_PA13C_SERCOM2_PAD1};
const static Pin spi2SCK = {.group = 0, .pin = 14, .mux = MUX_PA14C_SERCOM2_PAD2};

const static Pin LedPin = {.group = 0, .pin = 20};
const static Pin buzzerPin = {.group = 0, .pin = 22, .cfg = 0x40};
const static Pin senseBatPin = {.group = 0, .pin = 5, .chan = 5};

const static Pin senseAPin = {.group = 0, .pin = 8, .chan = 8};
const static Pin fireAPin = {.group = 0, .pin = 24};

const static Pin analogAccelPin = {.group = 0, .pin = 7, .chan = 7};


#define DMA_SERCOM1_TX 1
#define DMA_SERCOM1_RX 2

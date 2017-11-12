//#include <util.h>

#define SPI0 0
#define SPI1 1
#define SPI2 2

//dasAltimeter
const static Pin cs_mem = {.group = 0, .pin = 18};
const static Pin cs_baro = {.group = 0, .pin = 15};
const static Pin cs_gyro = {.group = 0, .pin = 10};
const static Pin cs_accel = {.group = 1, .pin = 10};




const static Pin spi0MOSI = {.group = 0, .pin = 8, .mux = MUX_PA08C_SERCOM0_PAD0};
const static Pin spi0MISO = {.group = 0, .pin = 11, .mux = MUX_PA11C_SERCOM0_PAD3};
const static Pin spi0SCK = {.group = 0, .pin = 9, .mux = MUX_PA09C_SERCOM0_PAD1};

const static Pin spi1MOSI = {.group = 0, .pin = 16, .mux = MUX_PA16C_SERCOM1_PAD0};
const static Pin spi1MISO = {.group = 0, .pin = 19, .mux = MUX_PA19C_SERCOM1_PAD3};
const static Pin spi1SCK = {.group = 0, .pin = 17, .mux = MUX_PA17C_SERCOM1_PAD1};

const static Pin spi2MOSI = {.group = 0, .pin = 12, .mux = MUX_PA12C_SERCOM2_PAD0};
const static Pin spi2MISO = {.group = 0, .pin = 13, .mux = MUX_PA13C_SERCOM2_PAD1};
const static Pin spi2SCK = {.group = 0, .pin = 14, .mux = MUX_PA15C_SERCOM2_PAD3};

const static Pin LedPin = {.group = 0, .pin = 20};
const static Pin buzzerPin = {.group = 0, .pin = 21, .cfg = 0x40};
const static Pin senseBatPin = {.group = 0, .pin = 6, .chan = 6};

const static Pin senseAPin = {.group = 0, .pin = 8, .chan = 8};
const static Pin fireAPin = {.group = 0, .pin = 24};

const static Pin analogAccelPin = {.group = 0, .pin = 7, .chan = 7};


#define DMA_SERCOM1_TX 1
#define DMA_SERCOM1_RX 2

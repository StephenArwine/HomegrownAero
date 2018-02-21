
#include <AT25SF.h>
#include <boardDefines.h>


void AT25SFWriteEnable() {

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);
}

void AT25SFHoldTillReady() {
    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_READSTATUS);
    while (spiDataIn(MEM_SPI) & 0x01);
    pinHigh(cs_mem);
}
void AT25SFChipErase() {

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_CHIP_ERASE);
    pinHigh(cs_mem);
}

uint8_t AT25SFGetByte(uint32_t address) {

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_SLOWREAD_ARRAY);
    spiDataOut(MEM_SPI,((address & 0xFF0000) >> 16));
    spiDataOut(MEM_SPI,((address & 0x00FF00) >>  8));
    spiDataOut(MEM_SPI,((address & 0x0000FF) >>  0));

    uint8_t _byte = spiDataIn(MEM_SPI);
    pinHigh(cs_mem);

    return _byte;
}

void AT25SFWriteBytes(uint32_t address, u8_t len, uint8_t *bytes) {


    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_PROGRAM);
    spiDataOut(MEM_SPI,((address & 0xFF0000) >> 16));
    spiDataOut(MEM_SPI,((address & 0x00FF00) >>  8));
    spiDataOut(MEM_SPI,((address & 0x0000FF) >>  0));

    for (u8_t pos = 0; pos < len; ++pos) {
        spiDataOut(MEM_SPI, bytes[pos]);
    }

    pinHigh(cs_mem);

}

void AT25SFErace32KBlock(uint32_t address ) {

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_32K_ERASE);
    spiDataOut(MEM_SPI,((address & 0xFF0000) >> 16));
    spiDataOut(MEM_SPI,((address & 0x00FF00) >>  8));
    spiDataOut(MEM_SPI,((address & 0x0000FF) >>  0));
    pinHigh(cs_mem);

    delay_ms(300);
}

void AT25SFErace4KBlock(uint32_t address) {

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_04K_ERASE);
    spiDataOut(MEM_SPI,((address & 0xFF0000) >> 16));
    spiDataOut(MEM_SPI,((address & 0x00FF00) >>  8));
    spiDataOut(MEM_SPI,((address & 0x0000FF) >>  0));
    pinHigh(cs_mem);

    delay_ms(70);

}


u8_t AT25SEWritePage(u32_t startingAddress, u8_t *data) {

    u8_t bytesSent = 0;

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_PROGRAM);
    spiDataOut(MEM_SPI,((startingAddress & 0xFF0000) >> 16));
    spiDataOut(MEM_SPI,((startingAddress & 0x00FF00) >>  8));
    spiDataOut(MEM_SPI,((startingAddress & 0x0000FF) >>  0));


    for (u16_t pos = 0; pos < 256; ++pos) {
        ++bytesSent;
        spiDataOut(SPI1,data[pos]);
    }
    pinHigh(cs_mem);


    return bytesSent;
}

void AT25SEreadSample(u32_t startingAddress, u8_t len, u8_t *data) {

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_FASTREAD_ARRAY);
    spiDataOut(MEM_SPI,((startingAddress & 0xFF0000) >> 16));
    spiDataOut(MEM_SPI,((startingAddress & 0x00FF00) >>  8));
    spiDataOut(MEM_SPI,((startingAddress & 0x0000FF) >>  0));
    spiDataOut(MEM_SPI, 0xFF);


    for (u8_t pos = 0; pos < len; ++pos) {
        data[pos] = spiDataIn(MEM_SPI);
    }
    pinHigh(cs_mem);

}

void AT25SEreadPage(u32_t startingAddress, u8_t *data) {

    pinLow(cs_mem);
    spiDataOut(MEM_SPI,OPCODE_FASTREAD_ARRAY);
    spiDataOut(MEM_SPI,((startingAddress & 0xFF0000) >> 16));
    spiDataOut(MEM_SPI,((startingAddress & 0x00FF00) >>  8));
    spiDataOut(MEM_SPI,((startingAddress & 0x0000FF) >>  0));
    spiDataOut(MEM_SPI, 0xFF);

    for (u16_t pos = 0; pos < 256; ++pos) {
        data[pos] = spiDataIn(MEM_SPI);
    }
    pinHigh(cs_mem);

}

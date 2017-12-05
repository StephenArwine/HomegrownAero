
#include <AT25SF.h>
#include <boardDefines.h>


uint8_t dummy_Tx = 0xFF;
uint8_t dummy_rx;

void AT25SFWriteEnable() {

    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);
}

uint8_t AT25SFGetByte(uint32_t address) {

    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_SLOWREAD_ARRAY);
    dummy_rx = spiDataTransfer(SPI1,((address & 0xFF0000) >> 16));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x00FF00) >>  8));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x0000FF) >>  0));

    uint8_t _byte = spiDataTransfer(SPI1,dummy_rx);
    pinHigh(cs_mem);

    return _byte;
}

void AT25SFWriteBytes(uint32_t address, u8_t len, uint8_t *bytes) {


    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);

    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_PROGRAM);
    dummy_rx = spiDataTransfer(SPI1,((address & 0xFF0000) >> 16));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x00FF00) >>  8));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x0000FF) >>  0));

    for (u8_t pos = 0; pos < len; ++pos) {
        dummy_rx = spiDataTransfer(SPI1, bytes[pos]);
    }

    pinHigh(cs_mem);

}

void AT25SFErace32KBlock(uint8_t blockToErace) {

    uint32_t address = 0x008000*blockToErace;

    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);

    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_32K_ERASE);
    dummy_rx = spiDataTransfer(SPI1,((address & 0xFF0000) >> 16));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x00FF00) >>  8));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x0000FF) >>  0));
    pinHigh(cs_mem);

    delay_ms(300);
}

void AT25SFErace4KBlock(uint8_t blockToErace) {

    uint32_t address = 0x001000*blockToErace;

    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);

    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_04K_ERASE);
    dummy_rx = spiDataTransfer(SPI1,((address & 0xFF0000) >> 16));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x00FF00) >>  8));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x0000FF) >>  0));
    pinHigh(cs_mem);

    delay_ms(70);

}


u8_t AT25SEWritePage(u32_t startingAddress, u8_t *data) {

    u8_t bytesSent = 0;


    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);


    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_PROGRAM);
    dummy_rx = spiDataTransfer(SPI1,((startingAddress & 0xFF0000) >> 16));
    dummy_rx = spiDataTransfer(SPI1,((startingAddress & 0x00FF00) >>  8));
    dummy_rx = spiDataTransfer(SPI1,((startingAddress & 0x0000FF) >>  0));


    for (u8_t pos = 0; pos < 256; ++pos) {
        ++bytesSent;
        dummy_rx = spiDataTransfer(SPI1,data[pos]);
    }
    pinHigh(cs_mem);


    return bytesSent;
}

void AT25SEreadSample(u32_t startingAddress, u8_t len, u8_t *data) {

    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_FASTREAD_ARRAY);
    dummy_rx = spiDataTransfer(SPI1,((startingAddress & 0xFF0000) >> 16));
    dummy_rx = spiDataTransfer(SPI1,((startingAddress & 0x00FF00) >>  8));
    dummy_rx = spiDataTransfer(SPI1,((startingAddress & 0x0000FF) >>  0));
    dummy_rx = spiDataTransfer(SPI1, 0xFF);


    for (u8_t pos = 0; pos < len; ++pos) {
        data[pos] = spiDataTransfer(SPI1,dummy_rx);
    }
    pinHigh(cs_mem);

}

void AT25SEreadPage(u32_t startingAddress, u8_t *data) {


    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_FASTREAD_ARRAY);
    dummy_rx = spiDataTransfer(SPI1,((startingAddress & 0xFF0000) >> 16));
    dummy_rx = spiDataTransfer(SPI1,((startingAddress & 0x00FF00) >>  8));
    dummy_rx = spiDataTransfer(SPI1,((startingAddress & 0x0000FF) >>  0));
    dummy_rx = spiDataTransfer(SPI1, 0xFF);


    for (u8_t pos = 0; pos < 255; ++pos) {
        data[pos] = spiDataTransfer(SPI1,dummy_rx);
    }
    pinHigh(cs_mem);

}

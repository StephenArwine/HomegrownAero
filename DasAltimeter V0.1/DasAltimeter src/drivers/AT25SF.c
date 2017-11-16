
#include <AT25SF.h>
#include <boardDefines.h>


uint8_t dummy_Tx = 0xFF;
uint8_t dummy_rx;


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

void AT25SFWriteByte(uint32_t address, uint8_t byteToWrite) {


    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_WRITEENABLE);
    pinHigh(cs_mem);

    pinLow(cs_mem);
    dummy_rx = spiDataTransfer(SPI1,OPCODE_PROGRAM);
    dummy_rx = spiDataTransfer(SPI1,((address & 0xFF0000) >> 16));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x00FF00) >>  8));
    dummy_rx = spiDataTransfer(SPI1,((address & 0x0000FF) >>  0));

    dummy_rx = spiDataTransfer(SPI1,byteToWrite);
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

    delay_ms(60);

}
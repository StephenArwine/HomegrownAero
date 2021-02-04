/*
 * USART.c
 *
 * Created: 2/1/2021 5:22:54 PM
 *  Author: Desktop
 */ 

#include "util.h"


void streamAlt() {
	            float fractionalAlt = sample.pressureAltitude - (int16_t)(sample.pressureAltitude);
	            int16_t fractAltPart = fractionalAlt * 1000;
	            u8_t dataToSend[5];
	            dataToSend[0] = (uint32_t)(sample.pressureAltitude) >> 0;
	            dataToSend[1] = (uint32_t)(sample.pressureAltitude) >> 8;
	            dataToSend[2] = (uint32_t)(sample.pressureAltitude) >> 16;
	            dataToSend[3] = (uint32_t)(sample.pressureAltitude) >> 24;
	            dataToSend[4] = fractAltPart >> 0;
	            dataToSend[5] = fractAltPart >> 8;

	            dataToSend[6] = sample.sampleCount >> 0;
	            dataToSend[7] = sample.sampleCount >> 8;
	            dataToSend[8] = sample.sampleCount >> 16;
	            dataToSend[9] = sample.sampleCount >> 24;

	            usartDataOut(0, dataToSend[0]);
	            usartDataOut(0, dataToSend[1]);
	            usartDataOut(0, dataToSend[2]);
	            usartDataOut(0, dataToSend[3]);
	            usartDataOut(0, dataToSend[4]);
	            usartDataOut(0, dataToSend[5]);
	            usartDataOut(0, dataToSend[6]);
	            usartDataOut(0, dataToSend[7]);
	            usartDataOut(0, dataToSend[8]);
	            usartDataOut(0, dataToSend[9]);
	
}
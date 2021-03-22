#include <util.h>

asm(".global _printf_float");


float altSmoothing = 0.0150;
float deltaSmoothing = 0.0050;

void vario() {


    sample.lastSampleTime = sample.sampleTime;
    sample.sampleTime = millis();


    sample.lastSampleMeters = sample.sampleMeters;
    sample.sampleMeters = paToFeetNOAA(sample.pressureMbar) * 0.3048;

    sample.dX = sample.altitudeMeters -  sample.sampleMeters;
    sample.dT = sample.sampleTime - sample.lastSampleTime;
    sample.dT = sample.dT/1000;

    sample.dxdt = sample.dX/sample.dT;

    double altSigma = sample.sampleMeters - sample.altitudeMeters;
    sample.altitudeMeters = sample.altitudeMeters + altSmoothing*altSigma;

    sample.altitudeVar = (1 - altSmoothing)*(sample.altitudeVar + altSmoothing*altSigma*altSigma);
    sample.altitudeStdDev = sqrt(sample.altitudeVar);

    double deltaSigma = sample.dxdt - sample.AVGdXdT;
    sample.AVGdXdT = sample.AVGdXdT + deltaSmoothing*deltaSigma;


}

void sendDebugData() {

    char debugString[150];

    sprintf(debugString, "%9.4f %9.4f %9.4f %9.4f %9.4f", sample.altitudeMeters, sample.sampleMeters, sample.dX, sample.dxdt, sample.AVGdXdT);
    //sprintf(debugString, "%9.4f %9.4f", sample.dxdt, sample.AVGdXdT);



    u8_t len2 = strlen(debugString);
    for (u8_t i = 0 ; i < len2; i++) {
        usartDataOut(3,debugString[i]);
    }
    usartDataOut(3,0x0d);
    usartDataOut(3,0x0a);
}
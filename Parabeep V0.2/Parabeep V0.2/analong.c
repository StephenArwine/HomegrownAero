

#include <util.h>



void adcInit() {

// set up clock
    MCLK->APBDMASK.reg |= MCLK_APBDMASK_ADC0;

// divide prescaler by 512 (93.75KHz), max adc freq is 2.1MHz
    ADC0->CTRLB.reg = ADC_CTRLB_RESSEL_12BIT ;

// enable clock adc channel
    connect_gclk_to_peripheral(1, 40);

    ADC0->CALIB.reg =ADC_CALIB_BIASREFBUF(ADC1_FUSES_BIASREFBUF_Pos) |
                     ADC_CALIB_BIASR2R(ADC0_FUSES_BIASR2R_Pos)|
                     ADC_CALIB_BIASCOMP(ADC0_FUSES_BIASCOMP_Pos);

    ADC0->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTVCC0;
	
	ADC0->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1;

    ADC0->CTRLA.reg = ADC_CTRLA_ENABLE |
                      ADC_CTRLA_PRESCALER_DIV256; // enable
    while(ADC0->STATUS.reg & ADC_STATUS_ADCBUSY);
}



u16_t adcSample() {
    ADC0->SWTRIG.reg = ADC_SWTRIG_START;
    while(!(ADC0->INTFLAG.reg = ADC_INTFLAG_RESRDY)); // wait until result is ready
    return ADC0->RESULT.reg;
}


u16_t adc_read(Pin p) {
    // switch pin mux to analog in
    pinAnalog(p);

    ADC0->INPUTCTRL.reg = ADC_INPUTCTRL_MUXPOS(p.chan) // select from proper pin
                          | ADC_INPUTCTRL_MUXNEG_GND; // 0 = gnd
    return adcSample();
}


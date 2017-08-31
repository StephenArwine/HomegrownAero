#include <services.h>

void sercomUSARTInit(SercomId id, u32 buad){

uint32_t USART_CLKGEN_F = 8000000UL; 
uint64_t br = (uint64_t)65536 * (USART_CLKGEN_F - 16 * baud) / USART_CLKGEN_F; 

 SYSCTRL->OSC8M.reg -= SYSCTRL_OSC8M_ENABLE; 
 SYSCTRL->OSC8M.reg -= SYSCTRL_OSC8M_PRESC_3; 
 SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ENABLE; 

     // GPS pin configs 
    ((Port *)PORT)->Group[1].PINCFG[22].reg = 0x41; 
     ((Port *)PORT)->Group[1].PINCFG[23].reg = 0x41; 
    ((Port *)PORT)->Group[1].PMUX[11].reg = 0x32; 


     //enable power to sercom module 
     PM->APBCMASK.reg |= (PM_APBCMASK_SERCOM0_Pos + id); 
     //enable and configure the sercom clock 
     GCLK->GENDIV.reg =  GCLK_GENDIV_ID(3) | 
                         GCLK_GENDIV_DIV(1); 
     GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(3) | 
                         GCLK_GENCTRL_SRC_OSC8M | 
                         GCLK_GENCTRL_IDC | 
                         GCLK_GENCTRL_RUNSTDBY | 
                         GCLK_GENCTRL_GENEN; 
     GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_ID_SERCOM0_CORE_Pos + id) | 
                         GCLK_CLKCTRL_GEN_GCLK3 | 
                         GCLK_CLKCTRL_CLKEN; 

 
 
     //configure the sercom module for the gps (sercom 5) 
     sercom(id)->USART.CTRLA.reg = SERCOM_USART_CTRLA_DORD | 
                                SERCOM_USART_CTRLA_MODE_USART_INT_CLK | 
                                SERCOM_USART_CTRLA_RXPO(3) | 
                                SERCOM_USART_CTRLA_TXPO(1); 
     USART_sync(sercom(id)); 
     sercom(id)->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN 
                                   | SERCOM_USART_CTRLB_TXEN 
                                   | SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/); 
    USART_sync(sercom(id));  
     sercom(id)->USART.BAUD.reg = baud; 
     USART_sync(sercom(id));  
     sercom(id)->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE; 
     USART_sync(sercom(id));  
}


 inline void sercom_reset(SercomId id) { 
     sercom(id)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST; 
     while(sercom(id)->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_SWRST); 
} 

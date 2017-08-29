
 
 
 void SalGclkInit() { 

 
     SYSCTRL->INTFLAG.reg = SYSCTRL_INTFLAG_BOD33RDY | SYSCTRL_INTFLAG_BOD33DET | 
                            SYSCTRL_INTFLAG_DFLLRDY; 
     NVMCTRL->CTRLB.reg |= NVMCTRL_CTRLB_RWS_HALF; 
 
 
 // start and enable external 32k crystal 
     SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ENABLE | 
                            SYSCTRL_XOSC32K_XTALEN | 
                            SYSCTRL_XOSC32K_EN32K | 
                            ( 6 << SYSCTRL_XOSC32K_STARTUP_Pos); 
 
  //wait for crystal to warm up 
     while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_XOSC32KRDY)) == 0); 
 
 

 
 


 
 //Configure the FDLL48MHz FLL, we will use this to provide a clock to the CPU 
 //Set the course and fine step sizes, these should be less than 50% of the values used for the course and fine values (P150) 
 
 
 #define NVM_DFLL_COARSE_POS    58 
 #define NVM_DFLL_COARSE_SIZE   6 
 #define NVM_DFLL_FINE_POS      64 
 #define NVM_DFLL_FINE_SIZE     10 
    uint32_t coarse =( *((uint32_t *)(NVMCTRL_OTP4) 
                          + (NVM_DFLL_COARSE_POS / 32)) 
                        >> (NVM_DFLL_COARSE_POS % 32)) 
                      & ((1 << NVM_DFLL_COARSE_SIZE) - 1); 
     if (coarse == 0x3f) { 
         coarse = 0x1f; 
     }; 
     uint32_t fine =( *((uint32_t *)(NVMCTRL_OTP4) 
                        + (NVM_DFLL_FINE_POS / 32)) 
                      >> (NVM_DFLL_FINE_POS % 32)) 
                    & ((1 << NVM_DFLL_FINE_SIZE) - 1); 
     if (fine == 0x3ff) { 
         fine = 0x1ff; 
     }; 


     // Disable ONDEMAND mode while writing configurations (errata 9905) 
     SYSCTRL->DFLLCTRL.reg = dfll_ctrl_usb & ~SYSCTRL_DFLLCTRL_ONDEMAND; 
     while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_DFLLRDY)) == 0); 
     SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(coarse) | SYSCTRL_DFLLVAL_FINE(fine); 
 
 
     //Wait and see if the DFLL output is good . . . 
     while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_DFLLRDY)) == 0); 
 
 
     SYSCTRL->DFLLCTRL.reg = dfll_ctrl_usb; 
 
 
     //For generic clock generator 0, select the DFLL48 Clock as input 
     GCLK->GENDIV.reg  = (GCLK_GENDIV_DIV(1)  | GCLK_GENDIV_ID(0)); 
     GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(0)  | (GCLK_GENCTRL_SRC_DFLL48M) | (GCLK_GENCTRL_GENEN)); 
}

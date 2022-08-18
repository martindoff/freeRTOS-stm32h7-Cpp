#ifndef __STARm_REGMACROS_H
#define __STARm_REGMACROS_H


#define RCC_AHB4ENR   (__IO uint32_t*) (RCC_BASE   + 0x0E0)  // clock enable register for AHB4
#define RCC_AHB4RSTR  (__IO uint32_t*) (RCC_BASE   + 0x088)  // reset register for AHB4

#endif

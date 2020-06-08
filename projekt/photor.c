#include "photor.h"
#include "delay.h"

void init_photor(void);
void start_measuring_light(void);

int photoValue = 0;

void init_photor(void)
{
  
  *((volatile unsigned int *) 0x400E0E14U) = 0x8;              //PIOB_ODR - PA3,PA4 = input

  *((volatile unsigned int *) 0x400E0700U) = 1 << 5;            //ADC interrupt
  
  *((volatile unsigned int *) 0x400C0004U) = 1 << 9;            //ADC_MR - prescal 2
  
}

void start_measuring_light(void){
 
  *((volatile unsigned int *) 0x400C0010U) = 0x2;              //ADC_CHER
  
  *((volatile unsigned int *) 0x400C0000U) = 0x2;               //ADC_CR - start conversion

  delay_ms(1);
  
  volatile unsigned int *conv = &(*((volatile unsigned int *)0x400C0020U));
  
  unsigned int ADCvalue = *conv;
  
  photoValue = ADCvalue;

  *((volatile unsigned int *) 0x400C0000U) = 0x2;               //ADC_CR - software trigger
  
}


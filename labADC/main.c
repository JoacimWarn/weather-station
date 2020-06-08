#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"


//ADC_MR 0x400C0004
//ADC_CHER 0x400C0010
//ADC_IER 0x400C0024  bit 24 = DRDY
//ADC_LCDR 0x400C0020
//ADC_CR 0x400C0000
//ADC_ISR 0x400C0030
//ADC_CHSR 0x400C0018

//ADC_SR 0x400C0030



//PIOA_OER 0x400E0E10
//PIOA_ODR 0x400E0E14
//PIOA_CODR 0x400E0E34
//PIOA_PDSR 0x400E0E3C
//PIOA_PUDR 0x400E0E60


void delay(int);

void main(void){
  
  SystemInit();
  
  *((volatile unsigned int *) 0x400E0610U) = 0xFFFFFFFC;        //PMC_PCER=0
  
  
  
  *((volatile unsigned int *) 0x400C0004U) = 0x200;        //ADC_MR
  
  *((volatile unsigned int *) 0x400E0E10U) = 0x18;             //PIOA_OER
  *((volatile unsigned int *) 0x400E0E34U) = 0x18;             //PIOA_CODR
  
  delay(233333);
  
  //Start measuring
  
  //*((volatile unsigned int *) 0x400C0024U) = 0x1000000;             //ADC_IER
  
  
  *((volatile unsigned int *) 0x400E0E14U) = 0x18;             //PIOA_ODR
  *((volatile unsigned int *) 0x400E0E60U) = 0x18;             //PIOA_PUDR
  
  
  *((volatile unsigned int *) 0x400C0010U) = 0xFFFF;        //ADC_CHER
  
  
  *((volatile unsigned int *) 0x400C0000U) = 0x2;        //ADC_CR - start conversion - sw_reset
  
  
  volatile unsigned int *flag = &(*((volatile unsigned int *) 0x400C0030U));
  unsigned int flaggan = *flag & 0x1000000;
  
  
  while(flaggan != 0x1000000){
    
    volatile unsigned int *flag = &(*((volatile unsigned int *) 0x400C0030U));
    unsigned int flaggan = *flag & 0x1000000;
    
  }
  
  
  //volatile unsigned int *CounterValue = &(*((volatile unsigned int *) 0x40080010U));
  volatile unsigned int *Converted = &(*((volatile unsigned int *) 0x400C0020U));
  double conv = *Converted;
  
  
  printf("%f\n",conv);
  
  while(1){
    
    
  }
  
}

void delay(int value){
  
  int i = 0;
  
  for(; i < value; i++){
    
    asm("nop");
    
  }
  
}
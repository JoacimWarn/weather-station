#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "Delay.h"


void PIOD_Handler(void);
void SysTick_Handler(void);
void Delay(void);


void Delay(void){
  
  unsigned int hej = SysTick_Config(0xFFFF);
  
}


void SysTick_Handler(void){
  
  
  *((volatile unsigned int *) 0x400E144CU);//PIOD_ISR
  
  
  
  
}

void PIOD_Handler(void){}
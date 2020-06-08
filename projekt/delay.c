#include "delay.h"

//3st NOP är cirka 1 nanosekund

  
  int systick_counter = 0;
  
  
  void delay_ns(int tid){
    
      *((volatile unsigned int *) 0xE000E014U) = 0xFF;               //systick-load, 84000 = 84mhz / 8400000 = 10 ms en gång varje sekund = interrupt
      *((volatile unsigned int *) 0xE000E010U) = 0x7;                   //systick-ctrl
      
      
      systick_counter = 0;
    
     
      while((systick_counter) < 1 * tid);
    
     
     *((volatile unsigned int *) 0xE000E010U) = 0x0;                   //systick-ctrl - slå av interrupt
      
  }
  
  
  void delay_ms(int tid){
    
    
      *((volatile unsigned int *) 0xE000E014U) = 0x14820;               //systick-load, 84000 = 84mhz / 84000 = 1 ms en gång varje sekund = interrupt
      *((volatile unsigned int *) 0xE000E010U) = 0x7;                   //systick-ctrl
    
      
     systick_counter = 0;
    
     
     while((systick_counter) < 1 * tid);
    
     
     *((volatile unsigned int *) 0xE000E010U) = 0x0;                   //systick-ctrl - slå av interrupt
      
     
  }


void SysTick_Handler(void){
  
  systick_counter++;
  
}
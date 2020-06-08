#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "Screen.h"


void TC1_Handler(void);
void TC0_Handler(void);
void PIOB_Handler(void);

//0x400E103C piob_pdsr

unsigned int Ra,Rb;
double Global;


void main(void)
{
  
  SystemInit();
  
  *((volatile unsigned int *) 0x400E0610U) = 0xFFFFFFFC;        //PMC_PCER=0
  
  *((volatile unsigned int *) 0x400E1014U) = 0x2000000;    //0x400E1014 PIOB_ODR
  
  *((volatile unsigned int *) 0x400E1064U) = 0x2000000;      //PIOB_PUER 0x400E1064
  
      volatile unsigned int *CounterValue = &(*((volatile unsigned int *) 0x40080010U));
    Ra = *CounterValue;
  
  *((volatile unsigned int *) 0x400800E4U) = 0x0;               //TC_WPMR
  
  unsigned int i = 0;
  
  //*((volatile unsigned int *) 0x400E1000U) = 0x2000000;         //PIOB_PER - enable pin
  
  //init();
  
  unsigned short int j = 0;
  *((volatile unsigned int *) 0x400E1000U) = 0x2000000;         //PIOB_PER 
  
  for(; j < 33; j++) asm("nop");
  
  *((volatile unsigned int *) 0x400E1004U) = 0x2000000;         //PIOB_PDR
  //*((volatile unsigned int *) 0x400E1004U) = 0x2000000;         //PIOB_PDR
  
  
  *((volatile unsigned int *) 0x40080004) = 0x50000;         //Välj timer_clock1
  
  *((volatile unsigned int *) 0x40080000U) = 0x5;         //TC_CCR0 - enable timer and make sw_reset
  
  
  
  //Load counters
  //&(*((volatile unsigned int *) 0x400E123CU));
  
  //volatile unsigned int *Ra = &(*((volatile unsigned int *) 0x40080010U));        //0x40080010 counter value
  
  //volatile unsigned int *RisingB = &(*((volatile unsigned int *) 0x40080010U));        //0x40080004
  //unsigned int risingb = *RisingB;
  
  volatile unsigned int *FallingA = &(*((volatile unsigned int *) 0x40080010U));       //0x40080004
  unsigned int fallinga = *FallingA;
  
  
   if((fallinga & 0x20000) == 0x20000){
    
    volatile unsigned int *CounterValue = &(*((volatile unsigned int *) 0x40080010U));
    Ra = *CounterValue;
    
  }
  /*
  if((risingb & 0x40000) == 0x40000){
    
    volatile unsigned int *CounterValue = &(*((volatile unsigned int *) 0x40080010U));
    Rb = *CounterValue;
    
  }
  */
  
  //*((volatile unsigned int *) 0x400E1000U) = 0x2000000;         //PIOB_PER - enable pin
  

  //*((volatile unsigned int *) 0x400E1040U) = 0x2000000;          //PIOB_IER 0x400E1040
  
  //*((volatile unsigned int *) 0x400E1004U) = 0x2000000;         //PIOB_PDR
  j=0;
  for(; j < 34; j++) asm("nop");
  
  //start measurement
  
  for(; i < 3460000; i++){
  
  NVIC_ClearPendingIRQ(27);
  NVIC_SetPriority(27,3);
  NVIC_EnableIRQ(27);
  
  NVIC_ClearPendingIRQ(28);
  NVIC_SetPriority(28,2);
  NVIC_EnableIRQ(28);
  
  NVIC_ClearPendingIRQ(12);
  NVIC_SetPriority(12,1);
  NVIC_EnableIRQ(12);
  
  //NVIC_SetPendingIRQ(27);
  
  *((volatile unsigned int *) 0x40080024U) = 0x60;              //TC_IER_LDRBS
  
  //*((volatile unsigned int *) 0x400E1040U) = 0x2000000;          //PIOB_IER 0x400E1040
  
  unsigned int i = 0;
  
  *((volatile unsigned int *) 0x40080000U) = 0x4;               //sw_reset
  *((volatile unsigned int *) 0x400E1004U) = 0x2000000;         //PIOB_PDR
  
  NVIC_SetPendingIRQ(27);
  
  for(; i < 54; i++) asm("nop");
  
  //*((volatile unsigned int *) 0x400E1000U) = 0x2000000;         //PIOB_PER - enable pin
  *((volatile unsigned int *) 0x400E1040U) = 0x2000000;          //PIOB_IER 0x400E1040
  
  //Startpuls = 34st i
  //*((volatile unsigned int *) 0x400E1000U) = 0x2000000;         //PIOB_PER - enable pin
  //*((volatile unsigned int *) 0x400E1040U) = 0x2000000;          //PIOB_IER 0x400E1040
  }
}
  /*
  while(1){
    
    //volatile unsigned char *temp;
    //temp = &(*((volatile unsigned char *) 0x400E123CU));
    
    unsigned short int i = 0;
    
     for(; i < 3000; i++){
      asm("nop");
    }
    
    //tal =  &(*((volatile unsigned int *) 0x400E103CU));
    //unsigned int hej = *tal & 0xFFFFFFFF;
    
    //printf("%d\n",hej);
    
    
  }
  */


void PIOB_Handler(void){
  
    //volatile unsigned int *RisingB = &(*((volatile unsigned int *) 0x40080010U));        //0x40080004
  //unsigned int risingb = *RisingB;
  
    volatile unsigned int *CounterValue = &(*((volatile unsigned int *) 0x40080010U));
    Rb = *CounterValue;
  
  *((volatile unsigned int *) 0x400E104CU);                     //0x400E104C PIOB_ISR
  
  //*((volatile unsigned int *) 0x400E1044U) = 0x2000000;         //0x400E1044 PIOB_IDR
  
  //*((volatile unsigned int *) 0x40080028U) = 0x60;              //TC_IDR_LDRBS
  Global = (((Rb - Ra)/5.0)) - 273.15;
  printf("%f\n",Global);
  
}


void TC1_Handler(void){
  
    //volatile unsigned int *FallingA = &(*((volatile unsigned int *) 0x40080010U));       //0x40080004
    //unsigned int fallinga = *FallingA;
    
    volatile unsigned int *CounterValue = &(*((volatile unsigned int *) 0x40080010U));
    Ra = *CounterValue;
  
}


void TC0_Handler(void){
  
    //volatile unsigned int *FallingA = &(*((volatile unsigned int *) 0x40080010U));       //0x40080004
    //unsigned int fallinga = *FallingA;
    
    volatile unsigned int *CounterValue = &(*((volatile unsigned int *) 0x40080010U));
    Ra = *CounterValue;
  
}

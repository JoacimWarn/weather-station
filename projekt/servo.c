#include "servo.h"
#include "photor.h"
#include "delay.h"

void init_servo(void);
void startSearching(void);

void init_servo(){
  
 *((volatile unsigned int *) 0x400E1210U) = 0x3000;             //PIOC_OER - PC12
 *((volatile unsigned int *) 0x400E1230U) = 0x1000;             //PIOC_SODR - PC12 - slår av chip till skärmen
 *((volatile unsigned int *) 0x400E1234U) = 0x2000;             //PIOC_CODR - PC13
  
  //Initiering av servo 
  *((volatile unsigned int *) 0x400E0610U) = 1 << 13;            //PMC_PCER0 - PIOC
  *((volatile unsigned int *) 0x400E0610U) = 1 << 14;            //PMC_PCER0 - PIOD
  *((volatile unsigned int *) 0x400E0700U) = 1 << 4;           //PMC_PCER1 - PWM
  *((volatile unsigned int *) 0x400E1004U) = 1 << 17;           //PIOB_PDR - PB17
  *((volatile unsigned int *) 0x400E1070U) = 1 << 17;           //PIOB_ABSR - PB17
  *((volatile unsigned int *) 0x40094004U) = 0x2;               //PWM_ENA1
  *((volatile unsigned int *) 0x40094220U) = 0x5;               //PWM_CMR[1] - prescalar = MCK/32, center aligned
  *((volatile unsigned int *) 0x4009422CU) = 0x8000;
  //Slut initiering av servo
  
  *((volatile unsigned int *) 0x40094224U) = 0x1900;             //90 grader
  
  delay_ms(100);
  
}

void startSearching(void){
  
  *((volatile unsigned int *) 0x40094224U) = 0x1900;            //+90 grader
  
  delay_ms(500);
  
  start_measuring_light();
  
  int counter = 1;
  
  int values[40];
  int i = 0;
  
  do{
    
    delay_ms(25);
    
    start_measuring_light();
    
    values[i] = photoValue;
    
    *((volatile unsigned int *) 0x40094224U) = 0x1900 - counter*100;
    
    i++;
    counter++;
    
  }while(i < 40);
  
  i = 0;
  int position = 0;
  int varde = values[0];
  
  for(; i < 40; i++){
    
    if(varde > values[i]){
      
      varde = values[i];
      position = i;
      
    }
    
  }
  
  delay_ms(1000);
  
  *((volatile unsigned int *) 0x40094224U) = 0x1900 - position*100;
  
}

#include "keypad.h"

int func(void); // returnerar tryckt knapp

int func(void){
  
 *((volatile unsigned int *) 0x400E1210U) = 0x3000;             //PIOC_OER - PC12
 *((volatile unsigned int *) 0x400E1230U) = 0x1000;             //PIOC_SODR - PC12 - slår av chip till skärmen
 *((volatile unsigned int *) 0x400E1234U) = 0x2000;             //PIOC_CODR - PC13
   
 *((volatile unsigned int *) 0x400E1200U) = 0x38;       //Row pin PIOC_PER 

 *((volatile unsigned int *) 0x400E1400U) = 0x4;        //PIOD_PER bit 2
 *((volatile unsigned int *) 0x400E1410U) = 0x4;        //PIOD_OER
 *((volatile unsigned int *) 0x400E1434U) = 0x4;        //Clear OE key bus bit 2 PIOD_CODR
 
 *((volatile unsigned int *) 0x400E1200U) = 0x380;      //PIOC_PER bit 7 8 9
 *((volatile unsigned int *) 0x400E1210U) = 0x380;      //Make output bit 7 8 9 PIOC_OER
 *((volatile unsigned int *) 0x400E1230U) = 0x380;      //PIOC_SODR bit 7 8 9 as high
   
 unsigned int value = 20;
 int i=0,j=0;
 
 unsigned mask = (int)0x100;
 
 volatile unsigned int *adress;
 adress = &(*((volatile unsigned int *) 0x400E123CU));          //PIOC_PDSR
 
 for(; i < 3; i++){
   
   unsigned int rowCounter = (int)0x20;
   
   *((volatile unsigned int *) 0x400E1234U) = mask;  //PIOC_CODR
   
   for(; j < 4; j++){
     
     if((*adress & rowCounter) == 0){

       *((volatile unsigned int *) 0x400E1214U) = 0x380; //Column pin input
       *((volatile unsigned int *) 0x400E1430U) = 0x4;        //OE key bus PIOD_SODR
       value = j*3 + i + 1;
       return value; 
     }
     
     if(j==0){
       rowCounter = rowCounter / 8;
     }
     if(j==1){
       rowCounter = rowCounter * 8;
       rowCounter = rowCounter / 4;
     }
     if(j==2){
       rowCounter = rowCounter * 4;
       rowCounter = rowCounter / 2;
     }   
   }
   
   *((volatile unsigned int *) 0x400E1230U) = mask;     //PIOC_SODR
   
   if(i==0){
     mask = mask*2;
   }
   if(i==1){
     mask = mask/4;
   }
   j = 0;
   
 }
 *((volatile unsigned int *) 0x400E1214U) = 0x380;       //Column pin input
 *((volatile unsigned int *) 0x400E1430U) = 0x4;        //OE key bus PIOD_SODR
  
 return value;
}
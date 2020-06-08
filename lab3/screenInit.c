#include "Screen.h"
#include "Delay.h"


unsigned char Read_Status_Display(void);
void Write_Data_2_Display(unsigned char Data);
void Write_Command_2_Display(unsigned char Command);


void init(){
  
    *((volatile unsigned int *) 0x400E1410U) = 0x1;       //PIOD_OER
  *((volatile unsigned int *) 0x400E1234U) = 0x1;       //PIOD_CODR - clear reset - PD0
  
  //*((volatile unsigned int *) 0x400E1410U) = 0xFFFFFFFF;           //PIOC_OER
  
  *((volatile unsigned int *) 0x400E1200U) = 0x3C000;       //PIOC_PER
  *((volatile unsigned int *) 0x400E1210U) = 0x3C000;       //PIOC_OER PC17-PC14
  
  *((volatile unsigned int *) 0x400E1200U) = 0xC0000;       //PIOC_PER
  *((volatile unsigned int *) 0x400E1210U) = 0xC0000;       //PIOC_OER PC19-PC18 Pin44-pin45
  
  *((volatile unsigned int *) 0x400E1200U) = 0x3000;        //PIOC_PER
  *((volatile unsigned int *) 0x400E1210U) = 0x3000;        //PIOC_OER 0x3000 = Pin51 och Pin50
  
  
  unsigned int i = 0;
  for(; i < 60000000; i++);     //Delay ca 10 sekunder
  
  
  printf("Delay done...\n");
  *((volatile unsigned int *) 0x400E1430U) = 0x1;       //PIOD_SODR - set reset
  printf("Screen resetted...\n");
  
  
  *((volatile unsigned int *) 0x400E1230U) = 0x40000;    //FS = 1: 6x8 font PIOC_SODR
  *((volatile unsigned int *) 0x400E1234U) = 0x80000;    //RV = 0: normal PIOC_CODR
  
  
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40);//Set text home address
  
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); //Set graphic home address
  
  Write_Data_2_Display(0x28);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); // Set text area
  
  
  //Write_Data_2_Display(0x1e);
  Write_Data_2_Display(0x28);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); // Set graphic area
  
  
  Write_Command_2_Display(0x80); // text mode
  Write_Command_2_Display(0x94); // Text on graphic off
  
  
  //Write_Command_2_Display(0x93);        //Curson on, blink on
  
  //Write_Command_2_Display(0x90);        //Display off
  
  //0x21 = cursor pointer
  
  
  //Write_Data_2_Display(0x28);
  
  i = 0;
  
  for(; i < 20000; i++){
    
    Write_Data_2_Display(0x21);
    Write_Command_2_Display(0xC0);
    
  }
  
}


unsigned char Read_Status_Display(void){
  
  //Chip enable(CE) = Chip select
  
  volatile unsigned char *temp;
  volatile unsigned int *pekare;
  
  //3FC = PC9 - PC2
  
  //DIR - pin 50 - PC13
  //0x2000 = PC13
  
  //OE - pin 51 - PC12
  //0x1000 = PC12
  
  //C/D - pin 46 - PC17
  //0x20000 = PC17
  
  //PIOC_PER 0x400E1200
  //PIOC_ODR 0x400E1214
  //PIOC_OER 0x400E1210
  //PIOC_SODR 0x400E1230
  //PIOC_CODR 0x400E1234
  //PIOC_PDSR 0x400E123C
  
  
  //*((volatile unsigned int *) 0x400E1200U) = 0x3FC;     //PIOC_PER
  *((volatile unsigned int *) 0x400E1214U) = 0x3FC;     //PIOC_ODR PC9-PC2
  
  
  //*((volatile unsigned int *) 0x400E1200U) = 0x2000;    //PIOC_PER PC13
  *((volatile unsigned int *) 0x400E1230U) = 0x2000;    //PIOC_SODR PC13 - dir input
  *((volatile unsigned int *) 0x400E1234U) = 0x1000;    //PIOC_CODR PC12 - clear/enable output - enable
  *((volatile unsigned int *) 0x400E1230U) = 0x20000;   //PIOC_SODR PC17 Set C/D
  *((volatile unsigned int *) 0x400E1234U) = 0x10000;   //PIOC_CODR - clear chip select display - PC16 - clear chip select
  *((volatile unsigned int *) 0x400E1234U) = 0x8000;   //PIOC_CODR - clear read display - PC15 0x8000
  
  Delay();
  
  
  temp = &(*((volatile unsigned char *) 0x400E123CU));  //PIOC_PDSR - read databus save it in temp
  pekare = &(*((volatile unsigned int *) 0x400E123CU));  //PIOC_PDSR - read databus save it in temp
  
  *((volatile unsigned int *) 0x400E1230U) = 0x10000;   //PIOC_SODR - set chip select
  *((volatile unsigned int *) 0x400E1230U) = 0x8000;    //PIOC_SODR - set read display
  *((volatile unsigned int *) 0x400E1230U) = 0x1000;    //PIOC_SODR - disable output
  *((volatile unsigned int *) 0x400E1234U) = 0x2000;    //PIOC_CODR - dir output
  
  //printf("%d",(*pekare & 0xC));
  
  char hej = *temp & 0xC;
  //char hej = *temp & 0x3;
  //char hej = *temp & 0x6;
  
  //printf("%d\n",hej);
  *temp = *temp & 0x3FC;
  
   //adress = &(*((volatile unsigned int *) 0x400E123CU));          //PIOC_PDSR
  
  
  return hej;
  
}


void Write_Data_2_Display(unsigned char Data){
  
    //*((volatile unsigned int *) 0x400E1200U) = 0x3FC;     //PIOC_PER
  //*((volatile unsigned int *) 0x400E1214U) = 0x3FC;     //PIOC_ODR PC9-PC2 - databus
  
  while((Read_Status_Display()) != (char)12);
  //while((Read_Status_Display()) != (char)6);
  
  
  unsigned int insertData = Data;
  
  
  *((volatile unsigned int *) 0x400E1234U) = 0x3FC;     //PIOC_CODR - clear databus
  //*((volatile unsigned int *) 0x400E1230U) = (0x3FC & (Data*4));     //PIOC_SODR - set data to databus
  *((volatile unsigned int *) 0x400E1230U) = (0x3FC & (insertData*4));     //PIOC_SODR - set data to databus
  //*((volatile unsigned int *) 0x400E1230U) = (insertData);     //PIOC_SODR - set data to databus
  
  *((volatile unsigned int *) 0x400E1234U) = 0x2000;    //PIOC_CODR - dir output
  
  *((volatile unsigned int *) 0x400E1234U) = 0x1000;    //PIOC_CODR PC12 enable output
  *((volatile unsigned int *) 0x400E1210U) = 0x3FC;     //PIOC_OER - Set databus as output
  *((volatile unsigned int *) 0x400E1234U) = 0x20000;   //PIOC_CODR PC17 clear C/D
  *((volatile unsigned int *) 0x400E1234U) = 0x10000;   //PIOC_CODR - clear chip select display - PC16 - clear chip select
  *((volatile unsigned int *) 0x400E1234U) = 0x4000;    //PIOC_CODR - clear WR display
  
  Delay();
  
  *((volatile unsigned int *) 0x400E1230U) = 0x10000;   //PIOC_SODR - set chip select - set chip enable
  *((volatile unsigned int *) 0x400E1230U) = 0x4000;    //PIOC_SODR - set WR display
  *((volatile unsigned int *) 0x400E1230U) = 0x1000;    //PIOC_SODR - disable output
  *((volatile unsigned int *) 0x400E1214U) = 0x3FC;     //PIOC_ODR - Set databus as input
  
  
}


void Write_Command_2_Display(unsigned char Command){
  
  
  //PIOC_PER = 0x400E1200
  
  
  while((Read_Status_Display()) != (char)12);
  //while((Read_Status_Display()) != (char)6);
  
  
  unsigned int insertCommand = Command;
  
  
  *((volatile unsigned int *) 0x400E1234U) = 0x3FC;     //PIOC_CODR - clear databus
  //*((volatile unsigned int *) 0x400E1230U) = (0x3FC & (Command*4));     //PIOC_SODR - set command to databus
  *((volatile unsigned int *) 0x400E1230U) = (0x3FC & (insertCommand*4));     //PIOC_SODR - set command to databus
  //*((volatile unsigned int *) 0x400E1230U) = (insertCommand);     //PIOC_SODR - set command to databus
  
  
  *((volatile unsigned int *) 0x400E1234U) = 0x2000;    //PIOC_CODR - dir output
  
  
  *((volatile unsigned int *) 0x400E1234U) = 0x1000;    //PIOC_CODR PC12 enable output
  *((volatile unsigned int *) 0x400E1210U) = 0x3FC;     //PIOC_OER - Set databus as output
  *((volatile unsigned int *) 0x400E1230U) = 0x20000;   //PIOC_SODR PC17 set C/D High
  *((volatile unsigned int *) 0x400E1234U) = 0x10000;   //PIOC_CODR - clear chip select display - PC16 - clear chip select
  *((volatile unsigned int *) 0x400E1234U) = 0x4000;    //PIOC_CODR - clear WR display
  
  Delay();
  
  *((volatile unsigned int *) 0x400E1230U) = 0x10000;   //PIOC_SODR - set chip select - set chip enable
  *((volatile unsigned int *) 0x400E1230U) = 0x4000;    //PIOC_SODR - set WR display
  *((volatile unsigned int *) 0x400E1230U) = 0x1000;    //PIOC_SODR - disable output
  *((volatile unsigned int *) 0x400E1214U) = 0x3FC;     //PIOC_ODR - Set databus as input
  
  
}
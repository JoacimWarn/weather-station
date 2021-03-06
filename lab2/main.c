#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "stdio.h"
#include "time.h"


//PIOC_PER 0x400E1200
//PIOD_PER 0x400E1400


unsigned char Read_Status_Display(void);
void Write_Data_2_Display(unsigned char);
void Write_Command_2_Display(unsigned char);
void Delay(int);
void Init_Display(void);
int func(void);


void main(void){
  
  
  SystemInit(); // Disables the Watchdog and setup the MCK
  
  Delay(5);
  
  //unsigned char hej = Read_Status_Display();
  
  
  *((volatile unsigned int *) 0x400E0610U) = 0xFFFFFFFF; //PMC_PCER0
  *((volatile unsigned int *) 0x400E0700U) = 0xFFFF;     //PMC_PCER1
  
  
  *((volatile unsigned int *) 0x400E1410U) = 0x4;       //PIOD_OER 0x400E1410   Pin 27
  *((volatile unsigned int *) 0x400E1430U) = 0x4;       //PIOD_SODR 0x400E1430
  
  
  Init_Display();
  
  
  //PIOD_PER 0x400E1400
  //PIOD_SODR 0x400E1430
  //PIOD_CODR 0x400E1434
  //PIOD_OER 0x400E1410
  
  
  //PIOC_PER 0x400E1200
  //PIOC_ODR 0x400E1214
  //PIOC_SODR 0x400E1230
  //PIOC_CODR 0x400E1234
  
  
  
  
  
  /*
  time_t start,end;
  time(&start);
  Delay(150000);
  time(&end);
  double theTime = difftime(end,start);
  printf("It took:%5.24lf seconds\n",theTime);
  */
  
  /*
  clock_t begin = clock();
  //Delay(30000000);    ca 3.4 seconds
  Delay(200000);
  clock_t end = clock();
  double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time spent was:%.20lf seconds\n",(timeSpent)/100);
  */
  
  while(1){
    
    
    
    
    
 }// while(1)
} // main


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
  
  Delay(150);
  
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
  
  Delay(150);
  
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
  
  Delay(150);
  
  *((volatile unsigned int *) 0x400E1230U) = 0x10000;   //PIOC_SODR - set chip select - set chip enable
  *((volatile unsigned int *) 0x400E1230U) = 0x4000;    //PIOC_SODR - set WR display
  *((volatile unsigned int *) 0x400E1230U) = 0x1000;    //PIOC_SODR - disable output
  *((volatile unsigned int *) 0x400E1214U) = 0x3FC;     //PIOC_ODR - Set databus as input
  
  
}


void Init_Display(void){
  
  //PIOD_OER = 0x400E1410
  //PIOC_OER = 0x400E1210 
  //PIOC_PER = 0x400E1200
  //PIOD_PER = 0x400E1400
  
  
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
  
  
  /*
  Write_Data_2_Display(0xFF);
  Write_Data_2_Display(0xFF);
  Write_Command_2_Display(0x24);
  
  Delay(3000);
  Write_Data_2_Display(0x24);
  Write_Command_2_Display(0xC0);
  Delay(3000);
  
  Write_Data_2_Display(0x21);
  Write_Command_2_Display(0xC0);
  Delay(3000);
  */
  
  
  i = 0;
  for(; i < 30000; i++){
  Delay(300);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0xC0);
  }
  
  Write_Data_2_Display(0xFF);
  Write_Data_2_Display(0xFF);
  Write_Command_2_Display(0x24);
  
  Delay(3000);
  Write_Data_2_Display(0x24);
  Write_Command_2_Display(0xC0);
  Delay(3000);
  
  /*
  Delay(3000);
  Write_Data_2_Display(0x24);
  Write_Command_2_Display(0xC0);
  Delay(3000);
  */
  
  
  *((volatile unsigned int *) 0x400E1230U) = 0x1000;    //PIOC_SODR - disable output till sk�rmen
  
  *((volatile unsigned int *) 0x400E1434U) = 0x4;       //PIOD_CODR 0x400E1430 - enable output till keypad
  
  unsigned int retur;
  
  /*
  while(func()==0){
    
    
    
  }
  
  retur = func();
  */
  
  while(1){
    
    Delay(900000);
    
  switch(func()){
    
    //Delay(900000);
    
  case 1:
      Write_Data_2_Display(0x11);
      Write_Command_2_Display(0xC0);
    break;
    
  case 2:
          Write_Data_2_Display(0x12);
      Write_Command_2_Display(0xC0);
    break;
    
  case 3:
          Write_Data_2_Display(0x13);
      Write_Command_2_Display(0xC0);
    break;
    
  case 4:
          Write_Data_2_Display(0x14);
      Write_Command_2_Display(0xC0);
    break;
    
      case 5:
          Write_Data_2_Display(0x15);
      Write_Command_2_Display(0xC0);
    break;
    
      case 6:
          Write_Data_2_Display(0x16);
      Write_Command_2_Display(0xC0);
    break;
    
      case 7:
          Write_Data_2_Display(0x17);
      Write_Command_2_Display(0xC0);
    break;
    
      case 8:
          Write_Data_2_Display(0x18);
      Write_Command_2_Display(0xC0);
    break;
    
      case 9:
          Write_Data_2_Display(0x19);
      Write_Command_2_Display(0xC0);
    break;
    
      case 10:
          Write_Data_2_Display(0x0A);
      Write_Command_2_Display(0xC0);
    break;
    
      case 11:
          Write_Data_2_Display(0x10);
      Write_Command_2_Display(0xC0);
    break;
    
      case 12:
          Write_Data_2_Display(0x03);
      Write_Command_2_Display(0xC0);
    break;
    
  case 0:
  break;
    
  }
  }
  
}


void Delay(int value){
  
  int i = 0;
  
  for(; i < value; i++){
    asm("nop");
  }
  
}


//PMC_PCER0 0x400E0610
//PMC_PCER1 0x400E0700


int func(void){
  
   *((volatile unsigned int *) 0x400E1200U) = 0x38;       //Row pin PIOC_PER 
 //*((volatile unsigned int *) 0x400E1264U) = 0x38;     //PIOC_PUER
 //*((volatile unsigned int *) 0x400E1260U) = 0x38;       //PIOC_PUDR
 //*((volatile unsigned int *) 0x400E1214U) = 0x38;     //Row pin as inputs PIOC_ODR
 
 
 *((volatile unsigned int *) 0x400E1400U) = 0x4;        //PIOD_PER bit 2
 *((volatile unsigned int *) 0x400E1410U) = 0x4;        //PIOD_OER
 *((volatile unsigned int *) 0x400E1434U) = 0x4;        //Clear OE key bus bit 2 PIOD_CODR
 //*((volatile unsigned int *) 0x400E1430U) = 0x4;        //OE key bus PIOD_SODR
 
 
 *((volatile unsigned int *) 0x400E1200U) = 0x380;      //PIOC_PER bit 7 8 9
 *((volatile unsigned int *) 0x400E1210U) = 0x380;      //Make output bit 7 8 9 PIOC_OER
 *((volatile unsigned int *) 0x400E1230U) = 0x380;      //PIOC_SODR bit 7 8 9 as high
 
 
 //PIOC_PDSR 0x400E123C
   
 unsigned int value = 0;
 int i=0,j=0;
 
 
 //unsigned mask = (int)0x200;
 unsigned mask = (int)0x100;
 //unsigned mask = (int)0x80;
 
 
 volatile unsigned int *adress;
 adress = &(*((volatile unsigned int *) 0x400E123CU));          //PIOC_PDSR
 
 for(; i < 3; i++){
   
   unsigned int rowCounter = (int)0x20;
   //unsigned rowCounter = (int)0x4;
   
   *((volatile unsigned int *) 0x400E1234U) = mask;  //PIOC_CODR
   
   int delay = 0;
   
   for(; delay < 100; delay++);
   
   for(; j < 4; j++){
     
     //*((volatile unsigned int *) 0x400E123CU) = 0x40; //PIOC_PDSR
     
     if((*adress & rowCounter) == 0){
       
       //printf("\nValue is: %d",(j*3+i+1));
       value = j*3 + i + 1;
       *((volatile unsigned int *) 0x400E1214U) = 0x380; //Column pin input
       *((volatile unsigned int *) 0x400E1430U) = 0x4;        //OE key bus PIOD_SODR
       return value;
       
     }
     
     //rowCounter = rowCounter / 2;       //Logic shift right
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
   
   //rowCounter = rowCounter/2;
   //--->rowCounter = rowCounter*2;
   
   if(i==0){
     mask = mask*2;
   }
   if(i==1){
     mask = mask/4;
   }
   j = 0;
   
 }
 
 
 *((volatile unsigned int *) 0x400E1214U) = 0x380; //Column pin input
 *((volatile unsigned int *) 0x400E1430U) = 0x4;        //OE key bus PIOD_SODR
  
 return value;
 
}


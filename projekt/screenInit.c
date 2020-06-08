#include "screen.h"
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "delay.h"


void Clearscreen();                                     //Ränsar skärmen ifrån skräptext
unsigned char Read_Status_Display(void);                //Kollar om skärmen är redo för utskrift
void Write_Data_2_Display(unsigned char Data);          //skickar tecken till skärmen
void Write_Command_2_Display(unsigned char Command);    //skriver komando till skärmen
void printTemperature(int);                             //omvandlar rå tempdata till utskrift
void Delay(int);                                        //nop delayen (supersnabb)
void newRow(int);                                       //skapar mellanrum
void printDays(void);                                   //skriver ut dagarna på rätt platts på skärmen
void printCurrentDay(int);                              //väljer vilken dag och sätter den redo för utskrift på rätt position på skärmn
void printColumns(void);                                //skriver ut namn kolumnerna
void syncPointer(void);                                 //sätter pekaran högs uppe i högra hörnet
void setupDisplay(int);                                 //skriver ut valt tecken samt nästa possition 
void printMenu(void);                                   //skriver ut sun,tes och l&h på skärmen
void printLowAndUpTemp(void);                           //rensar och ändrar (printMenu) om användaren interagerar med skärmen
void printLower(int);                                   //används för att skriva ut värde från keypaden
void printWarning(int,int,int,int);                     //Skriver ut en varning om inmatad temp är överstigen eller understigen
void printUpAndLow(int,int);                            //skriver ut vald u/l temp på skärmen
void turboPrint(int);                                   //skriver ut On/off på skärmen


void init(){
  
  *((volatile unsigned int *) 0x400E1410U) = 0x1;       //PIOD_OER
  *((volatile unsigned int *) 0x400E1234U) = 0x1;       //PIOD_CODR - clear reset - PD0
  
  *((volatile unsigned int *) 0x400E1200U) = 0x3C000;       //PIOC_PER
  *((volatile unsigned int *) 0x400E1210U) = 0x3C000;       //PIOC_OER PC17-PC14
  
  *((volatile unsigned int *) 0x400E1200U) = 0xC0000;       //PIOC_PER
  *((volatile unsigned int *) 0x400E1210U) = 0xC0000;       //PIOC_OER PC19-PC18 Pin44-pin45
  
  *((volatile unsigned int *) 0x400E1200U) = 0x3000;        //PIOC_PER
  *((volatile unsigned int *) 0x400E1210U) = 0x3000;        //PIOC_OER 0x3000 = Pin51 och Pin50
  
  
  unsigned int i = 0;
  
  delay_ms(5000);               //Delay ca 5 sekunder
  
  
  printf("Delay done...\n");
  *((volatile unsigned int *) 0x400E1430U) = 0x1;       //PIOD_SODR - set reset
  printf("Screen resetted...\n");
  
  
  *((volatile unsigned int *) 0x400E1230U) = 0x40000;    //FS = 1: 6x8 font PIOC_SODR
  *((volatile unsigned int *) 0x400E1234U) = 0x80000;    //RV = 0: normal PIOC_CODR
  
  
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40);                         //Set text home address
  
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); //Set graphic home address
  
  Write_Data_2_Display(0x28);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); // Set text area
  
  Write_Data_2_Display(0x28);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); // Set graphic area
  
  Write_Command_2_Display(0x80); // text mode
  Write_Command_2_Display(0x94); // Text on graphic off
  
  Clearscreen();
  
  //Write_Command_2_Display(0x93);        //Curson on, blink on
  //Write_Command_2_Display(0x90);        //Display off
  //0x21 = cursor pointer
}

//Skriver ut om turbomode är på eller av
//Value = 1 - turbomode är på
//Value = 0 - turbomode är av
void turboPrint(int value){
  
  syncPointer();
  setupDisplay(0x00);
  
  newRow(31);
  
  setupDisplay(0x34);
  setupDisplay(0x35);
  setupDisplay(0x32);
  setupDisplay(0x22);
  setupDisplay(0x2F);
  setupDisplay(0x00);
  //Skriver ut "TURBO "
  
  if(value == 1){
    
    setupDisplay(0x2F);
    setupDisplay(0x2E);
    setupDisplay(0x00);
    //Skriver ut "ON "
    
  }
  else{
    
    setupDisplay(0x2F);
    setupDisplay(0x26);
    setupDisplay(0x26);
    
  }
  
}


void printUpAndLow(int upperLimit, int lowerLimit){
  
  syncPointer();
  setupDisplay(0x00);
  newRow(400);
  
  int i = 0;
  
  for(; i < 40; i++){
    
    setupDisplay(0x00);
    
  }
  
  syncPointer();
  setupDisplay(0x00);
  newRow(400);
  
  setupDisplay(0x35);
  setupDisplay(0x00);
  
  int rTemp = upperLimit;
  int lTemp = upperLimit / 100;
  int talen[4];
  
  i=0;
  
  while(i<4){
    talen[i] = upperLimit % 10;
    upperLimit = upperLimit / 10;
    i++;
  }
  
  i=3;
  for(; i >= 0; i--){
    
      Write_Data_2_Display(0x10 + talen[i]);
      Write_Command_2_Display(0xC0);
      
      if(i==2){
        
        setupDisplay(0x20E);
        
      }
    
  }
  
  
  //Skriv ut ett C
  
  setupDisplay(0x23);
  
  setupDisplay(0x00);
  
  setupDisplay(0x2C);
  setupDisplay(0x00);
  
  rTemp = lowerLimit;
  lTemp = lowerLimit / 100;
  talen[4];
  
  i=0;
  
  while(i<4){
    talen[i] = lowerLimit % 10;
    lowerLimit = lowerLimit / 10;
    i++;
  }
  
  i=3;
  for(; i >= 0; i--){
    
      Write_Data_2_Display(0x10 + talen[i]);
      Write_Command_2_Display(0xC0);
      
      if(i==2){
        
        setupDisplay(0x20E);
        
      }
    
  }
  
  //Skriv ut ett C
  
  setupDisplay(0x00);
  
  setupDisplay(0x23);
  
  setupDisplay(0x00);
  
}


void printWarning(int dailyMin, int dailyMax, int upperLimit, int lowerLimit){
  
  syncPointer();
  setupDisplay(0x00);
  newRow(440);          //Hamnar 2 rader under söndag
  
  int i = 0;
  
  for(; i < 40; i++){
    
    setupDisplay(0x00);
    
  }
  
  syncPointer();
  setupDisplay(0x00);
  newRow(440);          //Hamnar 2 rader under söndag
  
  if(dailyMin < lowerLimit && dailyMax > upperLimit){
    //Lower and upper limit breached
    setupDisplay(0x2C);
    setupDisplay(0x2f);
    setupDisplay(0x37);
    setupDisplay(0x25);
    setupDisplay(0x32);
    setupDisplay(0x00);
    
    setupDisplay(0x21);
    setupDisplay(0x2e);
    setupDisplay(0x24);
    setupDisplay(0x00);
    
    setupDisplay(0x35);
    setupDisplay(0x30);
    setupDisplay(0x30);
    setupDisplay(0x25);
    setupDisplay(0x32);
    setupDisplay(0x00);
    
    setupDisplay(0x2c);
    setupDisplay(0x29);
    setupDisplay(0x2d);
    setupDisplay(0x29);
    setupDisplay(0x34);
    setupDisplay(0x00);
    
    setupDisplay(0x22);
    setupDisplay(0x32);
    setupDisplay(0x25);
    setupDisplay(0x21);
    setupDisplay(0x23);
    setupDisplay(0x28);
    setupDisplay(0x25);
    setupDisplay(0x24);
    
  }
  else if(dailyMax > upperLimit){
    //Upper limit breached
    setupDisplay(0x35);
    setupDisplay(0x30);
    setupDisplay(0x30);
    setupDisplay(0x25);
    setupDisplay(0x32);
    
    setupDisplay(0x00);
    
    setupDisplay(0x2c);
    setupDisplay(0x29);
    setupDisplay(0x2d);
    setupDisplay(0x29);
    setupDisplay(0x34);
    setupDisplay(0x00);
    
    setupDisplay(0x22);
    setupDisplay(0x32);
    setupDisplay(0x25);
    setupDisplay(0x21);
    setupDisplay(0x23);
    setupDisplay(0x28);
    setupDisplay(0x25);
    setupDisplay(0x24);
    
  }
  else if(lowerLimit > dailyMin){
    //Lower limit breached
    setupDisplay(0x2C);
    setupDisplay(0x2f);
    setupDisplay(0x37);
    setupDisplay(0x25);
    setupDisplay(0x32);
    setupDisplay(0x00);
    
    setupDisplay(0x2c);
    setupDisplay(0x29);
    setupDisplay(0x2d);
    setupDisplay(0x29);
    setupDisplay(0x34);
    setupDisplay(0x00);
    
    setupDisplay(0x22);
    setupDisplay(0x32);
    setupDisplay(0x25);
    setupDisplay(0x21);
    setupDisplay(0x23);
    setupDisplay(0x28);
    setupDisplay(0x25);
    setupDisplay(0x24);
  } 
}

void printLower(int value){
  
  setupDisplay(0x10 + value);
  
}


void printLowAndUpTemp(void){
  
  syncPointer();
  setupDisplay(0x00);
  
  newRow(520);          //Hamnar 4 rader under söndag
  
  int i = 0;
  
  for(; i < 100; i++){
    
    setupDisplay(0x00);
    
  }
  //Rensar bort menyn
  
  syncPointer();
  setupDisplay(0x00);
  
  newRow(520);          //Hamnar 4 rader under söndag
  
  setupDisplay(0x2C);
  setupDisplay(0x2F);
  setupDisplay(0x37);
  setupDisplay(0x25);
  setupDisplay(0x32);
  setupDisplay(0x1A);
  setupDisplay(0x00);
  //Skriver ut "LOWER: "
  
  setupDisplay(0x10);
  setupDisplay(0x10);
  setupDisplay(0x0E);
  setupDisplay(0x10);
  setupDisplay(0x10);
  setupDisplay(0x00);
  setupDisplay(0x23);
  //Skriver ut "00.00 C"
  
  syncPointer();
  setupDisplay(0x00);
  newRow(560);          //Hamnar 5 rader under söndag
  
  setupDisplay(0x35);
  setupDisplay(0x30);
  setupDisplay(0x30);
  setupDisplay(0x25);
  setupDisplay(0x32);
  setupDisplay(0x1A);
  setupDisplay(0x00);
  //Skriver ut "UPPER: "
  
  setupDisplay(0x18);
  setupDisplay(0x10);
  setupDisplay(0x0E);
  setupDisplay(0x10);
  setupDisplay(0x10);
  setupDisplay(0x00);
  setupDisplay(0x23);
  //Skriver ut "80.00 C"
  
  syncPointer();
  setupDisplay(0x00);
  newRow(527);          //Hamnar 4 rader under söndag
  
  
}


void printMenu(void){
  
  syncPointer();
  setupDisplay(0x00);
  newRow(520);          //Hamnar 4 rader under söndag
  
  setupDisplay(0x11);   //skriver ut 1
  setupDisplay(0x0E);
  setupDisplay(0x00);
  
  setupDisplay(0x33);
  setupDisplay(0x35);
  setupDisplay(0x2E);
  //skriver ut 1. SUN
  
  syncPointer();
  setupDisplay(0x00);
  newRow(560);          //Hamnar 1 rader under val 1
  
  setupDisplay(0x12);   //skriver ut 2
  setupDisplay(0x0E);
  setupDisplay(0x00);
  
  setupDisplay(0x34);
  setupDisplay(0x25);
  setupDisplay(0x33);
  setupDisplay(0x34);
  setupDisplay(0x00);
  
  setupDisplay(0x2F);
  setupDisplay(0x2E);
  setupDisplay(0x0F);
  setupDisplay(0x2F);
  setupDisplay(0x26);
  setupDisplay(0x26);
  //Skriver ut 2. TEST ON/OFF
  
  syncPointer();
  setupDisplay(0x00);
  newRow(600);          //Hamnar 2 rader under val 1
  
  setupDisplay(0x13);   //skriver ut 3
  setupDisplay(0x0E);
  setupDisplay(0x00);
  
  setupDisplay(0x2C);
  setupDisplay(0x2F);
  setupDisplay(0x37);
  setupDisplay(0x00);
  setupDisplay(0x06);
  setupDisplay(0x00);
  setupDisplay(0x28);
  setupDisplay(0x29);
  setupDisplay(0x27);
  setupDisplay(0x28);
  
  setupDisplay(0x00);
  
  setupDisplay(0x34);
  setupDisplay(0x25);
  setupDisplay(0x2D);
  setupDisplay(0x30);
  //skriver ut 3. LOW & HIGH TEMP
  
}


void setupDisplay(int value){
  
  Write_Data_2_Display(value);
  Write_Command_2_Display(0xC0);
  
}


void newRow(int value){         // skapar mellanrum, (40 är en rads mellanrum)
  
  int i = 0;
  
  for(;i < value; i++){
    
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xC1);
    
  }
  
}


void printColumns(void){
  
  Write_Data_2_Display(0xFF);
  Write_Data_2_Display(0xFF);
  Write_Command_2_Display(0x21);
  
  
  setupDisplay(0x24);
  setupDisplay(0x21);
  setupDisplay(0x39);
  //day
  int i = 0;
  for(; i < 3; i++){
    
    setupDisplay(0x00);
    
  }
  
  setupDisplay(0x2D);
  setupDisplay(0x29);
  setupDisplay(0x2E);
  //min
  i = 0;
  for(; i < 4; i++){
    
    setupDisplay(0x00);
    
  }
  
  setupDisplay(0x2D);
  setupDisplay(0x21);
  setupDisplay(0x38);
  //max
  i = 0;
  for(; i < 4; i++){
    
    setupDisplay(0x00);
    
  }

  setupDisplay(0x21);
  setupDisplay(0x36);
  setupDisplay(0x27);
  //avg
  newRow(19);
  
  i = 0;
  for(; i < 21; i++){
    
    setupDisplay(0x0D);
    
  }

  syncPointer();
  setupDisplay(0x00);
  newRow(360);          //Hamnar 1 rad under söndag
  
  i = 0;
  for(; i < 21; i++){
    
    setupDisplay(0x0D);
    
  }
  
}


void printDays(void){
  
    syncPointer();
    setupDisplay(0x00);
    
    newRow(80);
 
    setupDisplay(0x2D);
    setupDisplay(0x2F);
    setupDisplay(0x2E);
  
  //skriv ut måndag
 
  newRow(37);
  
  setupDisplay(0x34);
  setupDisplay(0x35);
  setupDisplay(0x25);
  
  //Skriver ut tisdag
  
    newRow(37);
  
    
    setupDisplay(0x37);
    setupDisplay(0x25);
    setupDisplay(0x24);
  //skriver ut onsdag
    
      newRow(37);
  
    setupDisplay(0x34);
    setupDisplay(0x28);
    setupDisplay(0x35);
    //skriver ut torsdag
  
    newRow(37);
  
    setupDisplay(0x26);
    setupDisplay(0x32);
    setupDisplay(0x29);
    //skriver ut fredag
    
    newRow(37);
  
    setupDisplay(0x33);
    setupDisplay(0x21);
    setupDisplay(0x34);
    //skriver ut lördag
    
    newRow(37);
  
    setupDisplay(0x33);
    setupDisplay(0x35);
    setupDisplay(0x2E);
    //skriver ut söndag  
}

void syncPointer(void){
  
  Write_Data_2_Display(0xFF);// (x,y)kord
  Write_Data_2_Display(0xFF);
  Write_Command_2_Display(0x24);// sätter pekaren till  (x,y) kord
  
}


void printCurrentDay(int value){
  
  switch(value){
    case 0:             //Skriver ut måndag - 85
      newRow(85); 
    break;
    
    case 3:             //Skriver ut tisdag - 125
      newRow(125);
    break;

    case 6:             //Skriver ut onsdag - 165
      newRow(165);
    break;
    
    case 9:             //Skriver ut torsdag - 205
      newRow(205);
    break;
    
    case 12:            //Skriver ut fredag - 245
      newRow(245);
    break;
    
    case 15:            //Skriver ut lördag - 285
      newRow(285);
    break;
    
    case 18:            //Skriver ut söndag - 325
      newRow(325);
    break;
    
    default:
    break;
  }
}


void printTemperature(int temperature){
  
  static int val = 0;
  
  printCurrentDay(val);  // väljer dag och possition
  
  // metod för att extrahera en siffra åt gången för att
  // sedan kunna skicka motsvarande kommando till skärmen
  
  int rTemp = temperature;
  int lTemp = temperature / 100;
  int talen[4];
  
  int i=0;
  
  while(i<4){
    talen[i] = temperature % 10;
    temperature = temperature / 10;
    i++;
  }
  
  
  i=3;
  for(; i >= 0; i--){
    
      Write_Data_2_Display(0x10 + talen[i]);
      Write_Command_2_Display(0xC0);
      
      if(i==2){
        
        setupDisplay(0x20E);
        
      }
    
  }
  
  
  //Skriv ut ett C
  
  setupDisplay(0x23);
  
  //Skriv ut ett mellanslag
  
  setupDisplay(0x00);
  
  val++;
  
  if(val >= 21) val = 0;                //Börja skriva från måndag igen
  
}


void Clearscreen(void){
  
  int i = 0;
  
  Write_Data_2_Display(0xFF); // (x,y) kord uppe i högra hörnet
  Write_Data_2_Display(0xFF);
  Write_Command_2_Display(0x24);// pointer
  
  for(; i < 2000; i++){
    
    delay_ns(1);
    
    setupDisplay(0x00);// blankt tecken
    
  }
  
  
  Write_Data_2_Display(0xFF);
  Write_Data_2_Display(0xFF);
  Write_Command_2_Display(0x24);
  
  
  setupDisplay(0x00);
  
}


unsigned char Read_Status_Display(void){
  
  volatile unsigned char *temp;
  volatile unsigned int *pekare;
  
  *((volatile unsigned int *) 0x400E1214U) = 0x3FC;     //PIOC_ODR PC9-PC2
  
  *((volatile unsigned int *) 0x400E1230U) = 0x2000;    //PIOC_SODR PC13 - dir input
  *((volatile unsigned int *) 0x400E1234U) = 0x1000;    //PIOC_CODR PC12 - clear/enable output - enable
  *((volatile unsigned int *) 0x400E1230U) = 0x20000;   //PIOC_SODR PC17 Set C/D
  *((volatile unsigned int *) 0x400E1234U) = 0x10000;   //PIOC_CODR - clear chip select display - PC16 - clear chip select
  *((volatile unsigned int *) 0x400E1234U) = 0x8000;   //PIOC_CODR - clear read display - PC15 0x8000
  
  Delay(150);
  
  temp = &(*((volatile unsigned char *) 0x400E123CU));  //PIOC_PDSR - read databus save it in temp
  
  *((volatile unsigned int *) 0x400E1230U) = 0x10000;   //PIOC_SODR - set chip select
  *((volatile unsigned int *) 0x400E1230U) = 0x8000;    //PIOC_SODR - set read display
  *((volatile unsigned int *) 0x400E1230U) = 0x1000;    //PIOC_SODR - disable output
  *((volatile unsigned int *) 0x400E1234U) = 0x2000;    //PIOC_CODR - dir output

  char readyStatus = *temp & 0xC;

  *temp = *temp & 0x3FC;
  
  return readyStatus;
  
}


void Write_Data_2_Display(unsigned char Data){
  
  while((Read_Status_Display()) != (char)12); // kollar om skärmen är redo
  
  unsigned int insertData = Data;       // teckenkod
   
  *((volatile unsigned int *) 0x400E1234U) = 0x3FC;     //PIOC_CODR - clear databus
  *((volatile unsigned int *) 0x400E1230U) = (0x3FC & (insertData*4));     //PIOC_SODR - set data to databus // shiftar två steg
  
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
  
  while((Read_Status_Display()) != (char)12); // kollar så skärmen är redo
  
  unsigned int insertCommand = Command;
  
  *((volatile unsigned int *) 0x400E1234U) = 0x3FC;     //PIOC_CODR - clear databus
  *((volatile unsigned int *) 0x400E1230U) = (0x3FC & (insertCommand*4));     //PIOC_SODR - set command to databus
  
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

void Delay(int value){
  
  int i = 0;
  
  for(; i < value; i++){
    
    asm("nop");
    
  }
  
}
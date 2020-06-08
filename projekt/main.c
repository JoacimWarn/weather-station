#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "screen.h"
#include "delay.h"
#include "tempsensor.h"
#include "servo.h"
#include "keypad.h"


#define DayInMinutes 1440
#define oneWeek 7

struct DayData average(int[]);

struct DayData{
  
  double avg;
  int max;
  int min;
  
};

int temperatur[DayInMinutes];           //en plats varje minut
struct DayData weekStats[oneWeek];
void inputLowerAndUpperTemp(void);      //tar lower och uppertemp ifrån användaren
void turboFunc(void);                   //Sätter igång eller slår av turboläge

int turboMode = 60;
int lowerTemp = 0;
int upperTemp = 8000;

void main(void)
{
  
  SystemInit();         //Initiera system
  
  *((volatile unsigned int *) 0x400E1430U) = 0x4;        //Set OE key bus bit 2 PIOD_SODR - Stänger av keypad
  *((volatile unsigned int *) 0x400E0610U) = 1 << 13;       //PMC_PCER0 - PIOC
  *((volatile unsigned int *) 0x400E0610U) = 1 << 14;       //PMC_PCER0 - PIOD
  
  ////////intitieringar och utskrivning av gränssnitt/////////////
  init();               //Initiera skärmen
  printColumns();       //skriver ut namn kolumnerna
  printDays();          //Skriver ut dagar
  printMenu();          //skriver ut meny
  turboPrint(turboMode);        //Skriver ut om turbo är på eller av
  init_temp();          //Initiera tempsensor
  init_photor();        //Initiera photosensor
  init_servo();         //Initiera servo
  /////////////////////////////////////////////////////////////////
  
  int i = 0;  //1440 ticks en dag - lagra värderna
  int counter = 0;
  int keyPressed = 0;
  
  while(1){
      /////////////////////Den lägre menyn//////////////////////////////////////
      keyPressed = func();
      if(keyPressed == 1){
        startSearching();               //Börjar leta efter sol
      }
      else if(keyPressed == 2){
        turboFunc();                    //Sätter igång eller slår av turboläge
      }
      else if(keyPressed == 3){
        printLowAndUpTemp();
        inputLowerAndUpperTemp();
        printUpAndLow(upperTemp,lowerTemp);
        //Användaren får mata in lägre gränsen samt övre gränsen för temperaturen.  
      }
      //////////////////////////////////////////////////////////////////////////
      
      start_measuring_temp();
      
      delay_ms(5);                      //För kort delay = inget värde på temperatur, läser inte in värde
     
      int globalTemp = tempSensor;      //temperatur[i] = globalTemp;
      
      
      //////////////////////ändring av Turbo////////////////////////////////////
      if(turboMode == 60){              //Långsamt läge - turboMode = 60
        
        temperatur[i] = globalTemp;
        delay_ms(38);                   //Experimenterat fram, 38 = 60 sekunder, det blir inte 60 eftersom vi har delayer runtom i programmet som är obligatoriska
        
      }
      else{
        
        temperatur[i] = globalTemp;
        
      }
      //////////////////////////////////////////////////////////////////////////
      
      
      //////////////////////////en dag är klar//////////////////////////////////
      if(i == 1439){
        
        struct DayData hej = average(temperatur);
        
        syncPointer();
        
        printTemperature(hej.min);
        printTemperature(hej.max);
        printTemperature(hej.avg);
        
        weekStats[counter] = hej;
        
        if((lowerTemp > weekStats[counter].min) || (upperTemp < weekStats[counter].max)){
          
          printWarning(weekStats[counter].min,weekStats[counter].max,upperTemp,lowerTemp);
          
        }
        
        counter++;
        
        i = 0;
        
        if(counter == 7){
          counter = 0;
        }
        
        
        
      }
      i++;
      
  }
  
  
}


void turboFunc(void){
  
  delay_ms(500);                //Tar bort elastisk studs från knapp
  
  if(turboMode == 1){
    
    turboMode = 60;
    turboPrint(0);
    
  }
  else{
    
    turboMode = 1;
    turboPrint(1);
    
  }
  
}


void inputLowerAndUpperTemp(void){
  
  int lowerTempLimits[4];
  int upperTempLimits[4];
  
  delay_ms(1000);               //Tar bort elastisk studs från knapp
  
  int keyPressed = 0;
  int i = 0;
  //Lägre gränsen
  while(i <= 3){
    
    keyPressed = func();
    
    if(keyPressed != 20){
      
      delay_ms(300);
      
      if(i == 2) setupDisplay(0x0E);
      
      if(keyPressed == 11){
        
        keyPressed = 0;
        printLower(keyPressed);
  
      }
      
      else printLower(keyPressed);
      
      lowerTempLimits[i] = keyPressed;
      
      i++;
      
    }
    
  }
  
  syncPointer();
  setupDisplay(0x00);
  newRow(567);          //Hamnar 5 rader under söndag
  
  i= 0;
  //Övre gränsen
  while(i <= 3){
    
    keyPressed = func();
    
    if(keyPressed != 20){
      
      delay_ms(300);
      
      if(i == 2) setupDisplay(0x0E);
      
        if(keyPressed == 11){
        
        keyPressed = 0;
        printLower(keyPressed);
        
      }
      else printLower(keyPressed);
      
      upperTempLimits[i] = keyPressed;
      
      i++;
      
    }
    
  }
  
  delay_ms(2000);
  
  
  syncPointer();
  setupDisplay(0x00);
  newRow(520);          //Hamnar 4 rader under söndag
  
  i = 0;
  
  for(; i < 100; i++){
    
    setupDisplay(0x00);
    
  }
  //Rensar bort text och skriver sedan ut menyn igen
  
  
  i = 1000;
  int counter = 0;
  lowerTemp = 0;
  upperTemp = 0;
  //Undre gräns
  for(; i >= 1;){
    
    lowerTemp = lowerTemp + (lowerTempLimits[counter] * i);
    
    i = (i /10);
    counter++;
    
  }
  
  counter = 0;
  i = 1000;
  //Övre gräns
  for(; i >= 1;){
    
    upperTemp = upperTemp + (upperTempLimits[counter] * i);
    
    i = (i /10);
    counter++;
    
  }
  
  printf("%d %d\n",lowerTemp,upperTemp);
  
  printMenu();          //skriver ut meny
  
}


struct DayData average(int temp[]){
  
    int i = 0;
    
    struct DayData retur;
    
    retur.avg = 0.0;
    retur.max = temp[i];
    retur.min = temp[i];
    
    double avgSum = 0.0;
    
    for(; i < DayInMinutes; i++){
      
      avgSum = avgSum + temp[i];
      
      if(retur.max < temp[i]){
        
        retur.max = temp[i];
        
      }
      
      if(retur.min > temp[i]){
        
        retur.min = temp[i];
        
      }
      
    }
    
    avgSum = avgSum / DayInMinutes;
    
    retur.avg = avgSum;
  
    return retur;
  
}


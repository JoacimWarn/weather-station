#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "screen.h"
#include "delay.h"
#include "tempsensor.h"

int tempSensor;

void init_temp(void);
void start_measuring_temp(void);
void TC0_Handler(void);

void start_measuring_temp(void){
  
    *AT91C_TC0_IER = 1 << 6;             // interrupt LDRBS 

    // create a startpuls with a Delay(25); sw_reset in TC0_CCR0.

    *AT91C_PIOB_OER   = AT91C_PIO_PB25;
    *AT91C_PIOB_CODR  = AT91C_PIO_PB25;
    
    delay_ms(1);
    
    *AT91C_PIOB_ODR = AT91C_PIO_PB25;

    *AT91C_TC0_CCR = 1 << 2; 
}

void init_temp(void){
  
    *AT91C_PMC_PCER= 1 << 27;           // init TC0

    // Enable counter and make a sw_reset in TC0_CCR0
    *AT91C_TC0_CCR = 0x5;               // SWTRG | CLKEN

    *AT91C_TC0_CMR = 0x60000;           // TIOA (A - falling - 10b), (B - rising - 01b) and TCCLK1

    //Enable the pin
    *AT91C_PIOB_PER = AT91C_PIO_PB25;
    *AT91C_PIOB_ODR = AT91C_PIO_PB25;

    NVIC_ClearPendingIRQ(TC0_IRQn);
    NVIC_SetPriority(TC0_IRQn, 1);
    NVIC_EnableIRQ(TC0_IRQn);
    
}


void TC0_Handler(void){
  
    *AT91C_TC0_IDR = 1 << 6;    // disable interrupt
  
    // global_variable = REG_TC0_RB0 - REG_TC0_RA0 or use a flag
    int32_t rb = (*AT91C_TC0_RB); //Register B
    int32_t ra = (*AT91C_TC0_RA); //Register A 

    *AT91C_TC0_SR;  // status register
    *AT91C_TC0_SR;  // status register

    float temp = ((rb - ra) / (5.0f * 42.0f)) - 273.15f; // divide by 42
    
    int32_t temperatur = temp * 100;
    
    tempSensor = temperatur;
    
}
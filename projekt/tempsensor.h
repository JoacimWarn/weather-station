#ifndef TEMPSENSOR_H_INCLUDED
#define TEMPSENSOR_H_INCLUDED

extern int tempSensor;

void start_measuring_temp(void);
void init_temp(void);
void TC0_Handler(void);

#endif
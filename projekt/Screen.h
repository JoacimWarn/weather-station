#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

void Clearscreen();
unsigned char Read_Status_Display(void);
void Write_Data_2_Display(unsigned char Data);
void Write_Command_2_Display(unsigned char Command);
void printTemperature(int);
void Delay(int);
void newRow(int);
void printDays(void);
void printColumns(void);
void printWarning(int,int,int,int);
void turboPrint(int);

#endif
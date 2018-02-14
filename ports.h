/*
 * ports.h
 *
 * Created: 10/02/2018 23:06:26
 *  Author: Graham
 */ 


#ifndef PORTS_H_
#define PORTS_H_

#include "custom_types.h"

void PRT_Init(void);
void PRT_Set_OC0A_as_output(void);
void PRT_Set_ADC1_as_intput(void);
void PRT_Set_INT0_as_input(void);
void PRT_Set_PB4_as_input(void);
void PRT_Set_PB3_as_output(void);
void PRT_Set_PB3_level(CST_LOW_HIGH level);
CST_LOW_HIGH PRT_Get_PB4_level(void);


#endif /* PORTS_H_ */
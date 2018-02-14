/*
 * port.c
 *
 * Created: 10/02/2018 23:06:37
 *  Author: Graham
 */ 

#include <avr/io.h>

#include "ports.h"

#define ADC1_INPUT	0x00
#define OC0A_OUTPUT	0x01
#define INT0_INPUT	0x00
#define PB4_INPUT	0x00
#define PB3_OUTPUT	0x08

#define PRT_GET_PB4_LEVEL()		(PINB & (1 << 4)) ? CST_HIGH : CST_LOW
#define PRT_SET_PB3_LEVEL(x)	(x == CST_HIGH) ? (1 << 4) : 0

//
//	Name: PRT_Init
//	Desc: Initialises the ports hardware and variables
//
void PRT_Init(void)
{
	DDRB = 0x00;
}

//
//	Name: PRT_Set_OC0A_as_output
//	Desc: Sets the OCR0A pin as an output
//
void PRT_Set_OC0A_as_output(void)
{
	DDRB |= OC0A_OUTPUT;
}

//
//	Name: PRT_Set_ADC1_as_intput
//	Desc: sets the ADC1 pin as an output
//
void PRT_Set_ADC1_as_intput(void)
{
	DDRB |= ADC1_INPUT;
}

//
//	Name: PRT_Set_INT0_as_input
//	Desc: sets the INT0 pin as an output
//
void PRT_Set_INT0_as_input(void)
{
	DDRB |= INT0_INPUT;
}

//
//	Name: PRT_Set_PB4_as_input
//	Desc: sets the PB4 pin as an input
//
void PRT_Set_PB4_as_input(void)
{
	DDRB |= PB4_INPUT;
}

//
//	Name: PRT_Set_PB3_as_output
//	Desc: sets the PB3 pin as an output
//
void PRT_Set_PB3_as_output(void)
{
	DDRB |= PB3_OUTPUT;
}

//
//	Name: PRT_Get_PB4_level
//	Desc: returns CST_HIGH if pin is high else returns low
//
CST_LOW_HIGH PRT_Get_PB4_level(void)
{
	CST_LOW_HIGH pin_state;
	
	pin_state = PRT_GET_PB4_LEVEL();
	
	return pin_state;
}

//
//	Name: PRT_Set_PB3_level
//	Desc: sets port B pin 3 level
//
void PRT_Set_PB3_level(CST_LOW_HIGH level)
{
	PORTB |= PRT_SET_PB3_LEVEL(level);
}


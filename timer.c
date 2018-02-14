/*
 * timer.c
 *
 * Created: 10/02/2018 23:29:48
 *  Author: Graham
 */ 

#include <avr/interrupt.h>

#include "timer.h"
#include "schedular.h"

#define TMR_MAX_OCR_VALUE		0xFF

#define TMR_FAST_PWM			0x03
#define TMR_SET_THEN_CLEAR		0xC0
#define TMR_CLEAR_THEN_SET		0x80

#define TMR_CLK_DIV_1			0x01
#define TMR_CLK_DIV_8			0x02
#define TMR_CLK_DIV_64			0x03
#define TMR_CLK_DIV_256			0x04
#define TMR_CLK_DIV_1024		0x05

#define TMR_INTERRUPT_OVF_EN	0x02
#define TMR_INTERRUPT_COMPA_EN	0x04

//
//	Name: TMR_Init
//	Desc: Initialises the timer hardware and variables
//
void TMR_Init(void)
{
	TCCR0A = TMR_SET_THEN_CLEAR | TMR_FAST_PWM;
	TCCR0B = TMR_CLK_DIV_8;	// 9.6MHZ / 8 / TMR_MAX_OCR_VALUE = 4705Hz 
	OCR0A = TMR_MAX_OCR_VALUE;
	TIMSK0 = TMR_INTERRUPT_OVF_EN;
}

//
//	Name: TMR_Set_pwm_off_period
//	Desc: sets the off period of the pwm signal
//
void TMR_Set_pwm_off_period(uint8_t off_period)
{
	OCR0A = off_period;
}

//
//	Name: TMR_Synchronise_timer
//	Desc: sets TCNT0 to 0 to synchronise the timer.
//
void TMR_Synchronise_timer(void)
{
	TCNT0 = 0;
}

//
//	Name: ISR(TIM0_OVF_vect)
//	Desc: timer interrupt service routine
//
ISR(TIM0_OVF_vect)
{
	SCH_process_timers();
}
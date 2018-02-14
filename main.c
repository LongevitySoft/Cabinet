/*
 * Cabinet.c
 *
 * Created: 10/02/2018 23:00:34
 * Author : Graham
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"
#include "ports.h"
#include "fan.h"
#include "schedular.h"
#include "infrared.h"

int main(void)
{
	PRT_Init();
	
	SCH_Init();
	
	ADC_Init();
		
	FAN_Init();
	
	IFR_Init();
	
	sei();
	
	// background tasks run in main loop
    while (1) 
    {
		SCH_run_bg_tasks();
    }
}


/*
 * fan.c
 *
 * Created: 11/02/2018 00:23:55
 *  Author: Graham
 */ 

#include <avr/io.h>

#include "adc.h"
#include "fan.h"
#include "timer.h"
#include "ports.h"
#include "schedular.h"

#define FAN_TURN_ON_ADC_COUNT	200
#define FAN_TURN_OFF_ADC_COUNT	100

static void fan_set_fan_pwm_output(void);

static SCH_TASK_STRUCTURE adc_task = {fan_set_fan_pwm_output, CST_NULL, CST_FALSE, SCH_1S_TIMEOUT, SCH_1S_TIMEOUT};

static CST_BOOLEAN fan_is_on;
	
//
//	Name: FAN_Init
//	Desc: Initialises the fan control module variables
//
void FAN_Init(void)
{
	//
	fan_is_on = CST_FALSE;
	//
	// set up the timer to output the PWM
	TMR_Init();
	//
	PRT_Set_OC0A_as_output();
	//
	SCH_add_task(&adc_task);
}

//
//	Name: fan_set_fan_pwm_output
//	Desc: Sets the fan pwm output based on the current adc count
//
void fan_set_fan_pwm_output(void)
{
	uint8_t adc_value;
	//
	// get the adc count
	adc_value = ADC_Get_adc_count();
	//
	// check for a change in fan on state 
	if(fan_is_on == CST_FALSE)
	{
		// fan is off so test for condition to turn on 
		if(adc_value >= FAN_TURN_ON_ADC_COUNT)
		{
			fan_is_on = CST_TRUE;	
		}
	}
	else
	{
		// fan is on so test for condition to turn off
		if(adc_value < FAN_TURN_OFF_ADC_COUNT)
		{
			fan_is_on = CST_FALSE;	
		}
	}
	//
	// set adc value to max - adc count if fan is on else set it to max
	adc_value = (fan_is_on == CST_TRUE) ? ADC_MAX_ADC_COUNT - adc_value : ADC_MAX_ADC_COUNT;
	//
	TMR_Set_pwm_off_period(adc_value);
	//
}
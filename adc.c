/*
 * adc.c
 *
 * Created: 10/02/2018 23:01:20
 *  Author: Graham
 */ 

#include <avr/interrupt.h>

#include "adc.h"
#include "ports.h"
#include "custom_types.h"

#define ADC_INTERNAL_REFERENCE		0x40
#define ADC_LEFT_ADJ_RESULT			0x20
#define ADC_1_MUX					0x01

#define ADC_ENABLE					0x80
#define ADC_START_CONVERSION		0x40
#define ADC_AUTO_TRIGGER_EN			0x20
#define ADC_INTERRUPT_EN			0x08
#define ADC_CLOCK_DIV_128			0x07

#define ADC_TIMER_COMPA_TRIG		0x03
#define ADC_TIMER_OVF_TRIG			0x04

#define ADC_1_DIGITAL_IN_DISABLE	0x04

#define ADC_MAX_ADC_SAMPLES			16

typedef struct adc_average_struct{
		uint16_t samples_sum;
		uint8_t last_samples[ADC_MAX_ADC_SAMPLES];
		uint8_t next_sample_index;
		CST_BOOLEAN filter_full;
}ADC_AVERAGE_STRUCT;

static volatile ADC_AVERAGE_STRUCT adc_average;

//
//	Name: ADC_Init
//	Desc: Initialises the ADC hardware and variables
//
void ADC_Init(void)
{
	int i;
	
	// set up the ADC peripheral
	ADMUX = ADC_INTERNAL_REFERENCE | ADC_LEFT_ADJ_RESULT | ADC_1_MUX;
	ADCSRB = ADC_TIMER_OVF_TRIG;
	DIDR0 = ADC_1_DIGITAL_IN_DISABLE;
	ADCSRA = ADC_ENABLE | ADC_START_CONVERSION | 
				ADC_AUTO_TRIGGER_EN | ADC_INTERRUPT_EN | 
				ADC_CLOCK_DIV_128;
				
	// clear the ADC average structure
	adc_average.filter_full = CST_FALSE;
	adc_average.samples_sum = 0;
	adc_average.next_sample_index = 0;
	//
	for(i = 0; i < ADC_MAX_ADC_SAMPLES; i++)
	{
		adc_average.last_samples[i] = 0;
	}
	
	// set adc 1 pin as input.
	PRT_Set_ADC1_as_intput();
}

//
//	Name: ADC_Get_adc_count
//	Desc: returns the current average adc count if filter 
//			is full else returns 0	
//
uint8_t ADC_Get_adc_count(void)
{
	uint8_t adc_count;
	
	cli();
	//
	adc_count = (adc_average.filter_full == CST_TRUE) ? (uint8_t)(adc_average.samples_sum / ADC_MAX_ADC_SAMPLES) : 0;
	//
	sei();
	
	return adc_count;
}

//
//	Name: ISR(ADC_vect)
//	Desc: ADC interrupt service routine 
//			updates the adc average structure.
//
ISR(ADC_vect)
{
	uint8_t value;
	//
	value = ADCH;
	//
	// subtract oldest sample
	adc_average.samples_sum -= adc_average.last_samples[adc_average.next_sample_index];
	//
	// add newest sample
	adc_average.samples_sum += value;
	//
	// store this sample
	adc_average.last_samples[adc_average.next_sample_index] = value;
	//
	if(++adc_average.next_sample_index >= ADC_MAX_ADC_SAMPLES)
	{
		adc_average.next_sample_index = 0;
		//
		adc_average.filter_full = CST_TRUE;
	}
}
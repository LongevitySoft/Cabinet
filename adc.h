/*
 * adc.h
 *
 * Created: 10/02/2018 23:01:36
 *  Author: Graham
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

#define ADC_MAX_ADC_COUNT	0xFF

void ADC_Init(void);
uint8_t ADC_Get_adc_count(void);


#endif /* ADC_H_ */
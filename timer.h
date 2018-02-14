/*
 * timer.h
 *
 * Created: 10/02/2018 23:30:00
 *  Author: Graham
 */ 


#ifndef TIMER_H_
#define TIMER_H_

void TMR_Init(void);
void TMR_Set_pwm_off_period(uint8_t off_period);
void TMR_Synchronise_timer(void);


#endif /* TIMER_H_ */
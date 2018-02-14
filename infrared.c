/*
 * infrared.c
 *
 * Created: 11/02/2018 11:46:56
 *  Author: Graham
 */ 

#include <avr/interrupt.h>

#include "infrared.h"
#include "schedular.h"
#include "ports.h"
#include "timer.h"

#define IFR_INTERRUPT_BOTH_EDGES	0x01
#define IFR_INTERRUPT_FALLING_EDGE	0x02
#define IFR_INTERRUPT_RISING_EDGE	0x03

#define IFR_ENABLE_INT0_INTERRUPT	0x40

#define IFR_NO_PATTERN				0x00000000
#define IFR_START_PATTERN			0x0000003F
#define IFR_START_INDEX				6
#define IFR_LAST_INDEX				10

static void receive_transmit_ir_signal(void);

static SCH_TASK_STRUCTURE ifr_task = {receive_transmit_ir_signal, CST_NULL, SCH_TASK_STOPPED, SCH_425US_TIMEOUT, SCH_NO_TIMER};

static CST_BOOLEAN start_receiving;
static volatile uint32_t received_pattern;
static volatile uint8_t received_index;
static volatile uint8_t transmit_index;

//
//	Name: IFR_Init
//	Desc: Initialises the infra red module
//
void IFR_Init(void)
{
	start_receiving = CST_FALSE;
	received_pattern = IFR_NO_PATTERN;
	received_index = 0;
	transmit_index = 0;
	//
	PRT_Set_INT0_as_input();
	PRT_Set_PB4_as_input();
	PRT_Set_PB3_as_output();
	//
	MCUCR = IFR_INTERRUPT_FALLING_EDGE;
	GIMSK = IFR_ENABLE_INT0_INTERRUPT;
	//
	SCH_add_task(&ifr_task);
}

//
//	Name: transmit_ir_signal
//	Desc: background task to transmit Infrared signal
//
static void receive_transmit_ir_signal(void)
{
	if(start_receiving == CST_TRUE)
	{
		if(received_index != IFR_LAST_INDEX)
		{
			if(PRT_Get_PB4_level() == CST_HIGH)
			{
				received_pattern |= received_index;
			}
			
			received_index = (received_index << 1);
		}
		
		if(transmit_index != IFR_LAST_INDEX)
		{
			if(received_pattern & transmit_index)
			{
				PRT_Set_PB3_level(CST_HIGH);		
			}
			else
			{
				PRT_Set_PB3_level(CST_HIGH);
			}
			
			transmit_index = (transmit_index << 1);
			
			ifr_task.timeout = SCH_425US_TIMEOUT;
			ifr_task.state = SCH_TASK_IDLE;
		}
		else
		{
			start_receiving = CST_FALSE;
		}
	}
}

//
//	Name: ISR(INT0_vect)
//	Desc: external interrupt service routine
//
ISR(INT0_vect)
{
	if(start_receiving == CST_TRUE)
	{
		// set the initial values for transfering ir signal
		received_pattern = IFR_START_PATTERN;
		received_index = IFR_START_INDEX;
		transmit_index = 0;
		start_receiving = CST_TRUE;
	
		// start transfer	
		receive_transmit_ir_signal();
		
		// synchronise the timer isr's 
		TMR_Synchronise_timer();
	}
}
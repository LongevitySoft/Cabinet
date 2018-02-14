/*
 * schedular.h
 *
 * Created: 11/02/2018 09:39:14
 *  Author: Graham
 */ 


#ifndef SCHEDULAR_H_
#define SCHEDULAR_H_

#include <avr/io.h>
#include "custom_types.h"

#define SCH_1S_TIMEOUT		4705
#define SCH_100MS_TIMEOUT	471
#define SCH_10MS_TIMEOUT	47
#define SCH_425US_TIMEOUT	2
#define SCH_NO_TIMER		0

typedef enum schedular_task_states{SCH_TASK_IDLE, SCH_TASK_RUN, SCH_TASK_STOPPED}SCH_TASK_STATES;

typedef struct schedular_task_structure SCH_TASK_STRUCTURE;

struct schedular_task_structure{
	void (*task_function_ptr)(void);
	SCH_TASK_STRUCTURE *next_task_ptr;
	volatile SCH_TASK_STATES state;
	volatile uint16_t timeout;
	uint16_t reload;
};

void SCH_Init(void);
void SCH_run_bg_tasks(void);
void SCH_add_task(SCH_TASK_STRUCTURE *task_struct);
void SCH_process_timers(void);

#endif /* SCHEDULAR_H_ */
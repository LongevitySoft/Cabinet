/*
 * schedular.c
 *
 * Created: 11/02/2018 09:38:57
 *  Author: Graham
 */ 

#include "schedular.h"

static SCH_TASK_STRUCTURE *task_list_head_ptr;
static SCH_TASK_STRUCTURE *next_task_ptr;

//
//	Name: SCH_Init
//	Desc: Initialises the schedular variables
//
void SCH_Init(void)
{
	task_list_head_ptr = CST_NULL;
	next_task_ptr = task_list_head_ptr;
}

//
//	Name: SCH_run_bg_tasks
//	Desc: runs the next background task which is ready
//
void SCH_run_bg_tasks(void)
{
	// has task got to the end of the list
	if(next_task_ptr == CST_NULL)
	{
		next_task_ptr = task_list_head_ptr;
	}
	else
	{
		if(next_task_ptr->state == SCH_TASK_RUN)
		{
			// reset the task variables
			if(next_task_ptr->reload != 0)
			{
				next_task_ptr->timeout = next_task_ptr->reload;
				next_task_ptr->state = SCH_TASK_IDLE;
			}
			else
			{
				next_task_ptr->state = SCH_TASK_STOPPED;
			}
			
			// run the task
			next_task_ptr->task_function_ptr();	
		}
			
		next_task_ptr = next_task_ptr->next_task_ptr;
	}
}

//
//	Name: SCH_add_task
//	Desc: Add's a task to the schedular
//
void SCH_add_task(SCH_TASK_STRUCTURE *task_struct)
{
	// point this task to the head
	task_struct->next_task_ptr = task_list_head_ptr;
	task_list_head_ptr = task_struct;
}

//
//	Name: SCH_process_timers
//	Desc: process the task timers
//
void SCH_process_timers(void)
{
	SCH_TASK_STRUCTURE *tasks_ptr;
	
	for(tasks_ptr = task_list_head_ptr; tasks_ptr != CST_NULL; tasks_ptr = tasks_ptr->next_task_ptr)
	{
		if(tasks_ptr->state == SCH_TASK_IDLE)
		{
			if(--tasks_ptr->timeout == 0)
			{
				tasks_ptr->state = SCH_TASK_RUN;
			}
		}
	}
}
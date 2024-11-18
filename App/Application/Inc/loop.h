#ifndef __LOOP_H__
#define __LOOP_H__

/******************************************************************************/

#include "loop_types.h"
#include <stdbool.h>

extern volatile uint32_t loop_execution_rate_1ms_timer;

#define loop_pexecution_rate_1ms_timer 	            (&loop_execution_rate_1ms_timer)

/******************************************************************************/

#define LOOP_EXECUTION_RATE_1MS_TIME    	        0

/******************************************************************************/

void piezo_turn_on(uint8_t index);
void piezo_turn_off(uint8_t index);

/******************************************************************************//******************************************************************************/
// AlissonGOE

traffic_status_t transit_state_group_loop_1(uint32_t time_between_loops_t, uint32_t time_in_loop_t, uint32_t gap, uint32_t time_spent_in_the_bonds_t);
traffic_status_t transit_state_group_loop_2(uint32_t time_between_loops_t, uint32_t time_in_loop_t, uint32_t gap, uint32_t time_spent_in_the_bonds_t);
void current_status_group_loop_1(traffic_status_t state);
void current_status_group_loop_2(traffic_status_t state);

// AlissonGOE
/******************************************************************************/

void loop_turn_on(uint8_t index);
void loop_turn_off(uint8_t index);
void loop_1ms_clock(void);              
void loop_init(void);                   
void loop_update(void);                  
void loop_deinit(void);                  

/******************************************************************************/

#endif
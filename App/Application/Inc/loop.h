#ifndef __LOOP_H__
#define __LOOP_H__

/******************************************************************************/

#include "loop_types.h"
#include "app_types.h"
#include <stdbool.h>

extern volatile uint32_t loop_execution_rate_1ms_timer;

#define loop_pexecution_rate_1ms_timer 	            (&loop_execution_rate_1ms_timer)

/******************************************************************************/

#define LOOP_EXECUTION_RATE_1MS_TIME    	        0

/******************************************************************************/

void piezo_turn_on(uint8_t index);
void piezo_turn_off(uint8_t index);

void loop_update_state(loop_state_update_t state, uint8_t index, loop_mode_t mode);
/******************************************************************************/

void loop_turn_on(uint8_t index);
void loop_turn_off(uint8_t index);
void loop_1ms_clock(void);              
void loop_init(void);                   
void loop_update(void);                  
void loop_deinit(void);                  

/******************************************************************************/

#endif
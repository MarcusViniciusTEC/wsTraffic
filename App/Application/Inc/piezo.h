
#ifndef __PIEZO_H__
#define __PIEZO_H__

/******************************************************************************/

#include "piezo_types.h"
#include "loop_types.h"

/******************************************************************************/
extern volatile uint32_t piezo_execution_rate_1ms_timer;

#define piezo_pexecution_rate_1ms_timer 	            (&piezo_execution_rate_1ms_timer)

#define PIEZO_EXECUTION_RATE_1MS_TIME    	        0

/******************************************************************************/

void piezo_turn_on(uint8_t index);
void piezo_turn_off(uint8_t index);
loop_status_update_t piezo_update_state(uint16_t time_between_loops_t, uint16_t gap_t, uint16_t time_spent_in_the_bonds_t, uint16_t time_start_piezo_t, uint8_t group_index);
void init_axles(uint8_t group_index, uint16_t loop_execution_time, uint16_t time_start_piezo_t);
void loop_update_state(loop_status_update_t state, uint8_t index);

void piezo_1ms_clock(void);    
void piezo_init(void);                 
void piezo_update(void);
void piezo_deinit(void);


#endif

#ifndef __PIEZO_H__
#define __PIEZO_H__

/******************************************************************************/

#include "piezo_types.h"

/******************************************************************************/
extern volatile uint32_t piezo_execution_rate_1ms_timer;

#define piezo_pexecution_rate_1ms_timer 	            (&piezo_execution_rate_1ms_timer)

#define PIEZO_EXECUTION_RATE_1MS_TIME    	        0

/******************************************************************************/

void piezo_turn_on(uint8_t index);
void piezo_turn_off(uint8_t index);

uint8_t piezo_group_received_parameters(piezo_groups_t piezo_group, piezo_pin_data_t piezo_enter, piezo_pin_data_t piezo_exit, piezo_mode_t piezo_mode);


void piezo_1ms_clock(void);    
void piezo_init(void);                 
void piezo_update(void);
void piezo_deinit(void);


#endif
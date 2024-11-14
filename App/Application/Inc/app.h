#ifndef __APP_H__
#define __APP_H__

/******************************************************************************/

#include "app_types.h"

/******************************************************************************/

extern volatile uint32_t app_execution_rate_1ms_timer;
#define app_pexecution_rate_1ms_timer (&app_execution_rate_1ms_timer)
#define APP_EXECUTION_RATE_1MS_TIME    100

/******************************************************************************/


/******************************************************************************/


/******************************************************************************/
// AlissonGOE

void transit_state(uint16_t time_between_loops_t, uint16_t time_in_loop_t, uint16_t gap, uint16_t time_spent_in_the_bonds_t);
void current_status(traffic_status_t state);
void traffic_calculation_app(void);

// AlissonGOE
/******************************************************************************/

void app_1ms_clock(void);
void app_init(void);
void app_update(void);
void app_deinit(void);

/******************************************************************************/

#endif/*__APP_H__*/

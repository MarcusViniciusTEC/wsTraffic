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

void app_1us_clock(void);

void app_1ms_clock(void);
void app_init(void);
void app_update(void);
void app_deinit(void);

/******************************************************************************/

#endif/*__APP_H__*/

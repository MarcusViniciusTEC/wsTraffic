#ifndef __APP_TYPES_H__
#define __APP_TYPES_H__

/******************************************************************************/

#include "app_cfg.h"

/******************************************************************************/

#define TIME_ZERO 0


typedef enum
{
  KEY_ON = 0,
  KEY_OFF
}key_state_t;

typedef enum
{
  MODE_PE = 0,
  MODE_CONV
}traffic_mode_t;

typedef enum
{
  CALC_ON = 0,
  CALC_OF
}calc_status_t;




/******************************************************************************/

#endif/*__APP_TYPES_H__*/

#ifndef __APP_TYPES_H__
#define __APP_TYPES_H__

/******************************************************************************/

#include "app_cfg.h"

/******************************************************************************/

#define TIME_ZERO 0


typedef enum
{
    LANE_0 = 0U,
    LANE_1,
    NUMBER_OF_LANES
}lane_t;



typedef enum
{
  LANE_LOOP_INIT = 0U,
  LANE_LOOP_START,
}loop_lane_state_t;


typedef struct 
{
    loop_lane_state_t state;
}lane_loop_data_t;


typedef struct  
{
  lane_loop_data_t lane_loop[NUMBER_OF_LANES];
}traffic_t;


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

/******************************************************************************/

#endif/*__APP_TYPES_H__*/

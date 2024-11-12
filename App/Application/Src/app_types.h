#ifndef __APP_TYPES_H__
#define __APP_TYPES_H__

/******************************************************************************/

#include "app_cfg.h"

/******************************************************************************/

#define TIME_ZERO 0

typedef enum
{
  AXLE_1 = 0U,
  AXLE_2,
  AXLE_3,
  AXLE_4,
  AXLE_5,
  AXLE_7,
  AXLE_8,
  AXLE_9,
  NUMBER_OF_AXLES_MAX
}axles_id_t;

typedef enum
{
  LANE_1 = 0,
  LANE_2,
  NUMBER_OF_LANES
}number_of_lanes_t;

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

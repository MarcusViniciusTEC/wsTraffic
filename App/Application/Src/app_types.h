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
    AXLE_6,
    AXLE_7,
    AXLE_8, 
    AXLE_9,
    NUMBER_OF_AXLES
}axles_types_t;

typedef struct 
{   
    uint8_t       qnt_axles;
    uint32_t      axles_time[NUMBER_OF_AXLES];
    axles_types_t next_axle;
}lane_axles_data_t;


typedef enum
{
  PIEZO_TURN_ON = 0U,
  PIEZO_WAIT_ON,
  PIEZO_TURN_OFF
}piezo_pulse_state_t;

typedef struct
{
  uint32_t time;
  piezo_pulse_state_t state;
}piezo_pulse_data_t;

typedef struct
{
  piezo_pulse_data_t piezo_pin[4];
}piezo_pulse_t;



typedef enum
{
    LANE_0 = 0U,
    LANE_1,
    NUMBER_OF_LANES
}lane_t;


typedef struct 
{
    lane_axles_data_t lane[NUMBER_OF_LANES];
}traffic_axles_t;




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

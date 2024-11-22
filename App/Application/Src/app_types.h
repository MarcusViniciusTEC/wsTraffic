#ifndef __APP_TYPES_H__
#define __APP_TYPES_H__

/******************************************************************************/

#include "app_cfg.h"
#include "loop.h"

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
  AXLE_INACTIVE = 0U,
  AXLE_ACTIVE,
  AXLE_NUMBER_OF_STATES
}axle_state_t;

typedef enum
{
  CHANNEL_DISABLE = 0U,
  CHANNEL_ENABLE 
}vehicle_states_t;

typedef enum
{
  GROUP_1 = 0,
  GROUP_2,
  NUMBER_OF_GROUPS
} number_groups_t;

typedef enum 
{
  STOP_PIEZO = 0U,
  START_PIEZO,
  PIEZO_STATES
}piezo_state_t;

typedef enum
{
  GROUP_ENABLE = 0,
  GROUP_DISABLED
} state_group_t;


typedef enum
{
  SET_TIMER = 0U,
  RESET_TIMER,
}timer_state_t;

typedef struct 
{
  uint16_t time_between_loops;
  uint16_t gap;
  uint16_t start_piezo;
  uint16_t loop_execution_time;
  state_group_t state;
}app_loop_data_t;

typedef struct 
{
  traffic_mode_t mode;
  uint8_t next_traffic;
  uint8_t traffic_id;
  uint8_t last_traffic;
}app_traffic_ctrl_t;

typedef struct
{
  timer_state_t timer_state;
  piezo_state_t state_piezo_enter;
  piezo_state_t state_piezo_exit;
}app_piezo_ctrl_t;

typedef struct 
{
    uint32_t      delay_time;  
    uint8_t       piezo_index;    
    axle_state_t  state;       
} axle_t;

typedef struct 
{
    axle_t*   axles;       
    uint8_t   num_axles;
    uint32_t  weight_ms; 
    vehicle_states_t channel_state
} app_piezo_data_t;


/******************************************************************************/

#endif/*__APP_TYPES_H__*/

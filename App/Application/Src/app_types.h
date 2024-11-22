#ifndef __APP_TYPES_H__
#define __APP_TYPES_H__

/******************************************************************************/

#include "app_cfg.h"
//#include "loop.h"

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
  SIMU_ENABLE = 0,
  SIMU_DISABLE
}state_sumulation_t;

typedef enum 
{
  AXLE_INACTIVE = 0U,
  AXLE_ACTIVE,
  AXLE_NUMBER_OF_STATES
}axle_state_t;

typedef enum
{
  CALC_ENABLE = 0,
  CALC_DISABLED
} state_calc_t;

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

typedef enum
{              
  VEHICLES_CLASS_2C = 0,   // 2 AXLES  |FRONT| 0.5 |EX-1-S| 4.5 |EX-2-D| 1.0  |BACK|                                                                                              6m - 16T
  VEHICLES_CLASS_3C,       // 3 AXLES  |FRONT| 0.5 |EX-1-S| 7.5 |EX-2-D| 1.2  |EX-3-D| 0.8  |BACK|                                                                               10m - 23T
  VEHICLES_CLASS_4C,       // 4 AXLES  |FRONT| 0.5 |EX-1-S| 8.1 |EX-2-D| 1.2  |EX-3-D| 1.2  |EX-4-D| 1.0 |BACK|                                                                  12m - 31T
  VEHICLES_CLASS_2S3,      // 5 AXLES  |FRONT| 0.5 |EX-1-S| 3.0 |EX-2-D| 11.2 |EX-3-D| 1.2  |EX-4-D| 1.2 |EX-5-D| 1.0 |BACK|                                                     18m - 41T
  VEHICLES_CLASS_3S3,      // 6 AXLES  |FRONT| 0.5 |EX-1-S| 3.0 |EX-2-D| 1.2  |EX-3-D| 11.2 |EX-4-D| 1.2 |EX-5-D| 1.2 |EX-6-D| 1.0 |BACK|                                        20m - 45T
  VEHICLES_CLASS_3D3,      // 6 AXLES  |FRONT| 0.5 |EX-1-S| 3.0 |EX-2-D| 1.2  |EX-3-D| 14.2 |EX-4-D| 1.2 |EX-5-D| 1.2 |EX-6-D| 1.0 |BACK|                                        23m - 49T
  VEHICLES_CLASS_3C2,      // 5 AXLES  |FRONT| 0.5 |EX-1-S| 4.5 |EX-2-D| 1.2  |EX-3-D| 5.4  |EX-4-D| 6.4 |EX-5-D| 1.0 |BACK|                                                     19m - 43T
  VEHICLES_CLASS_2J4,      // 6 AXLES  |FRONT| 0.5 |EX-1-S| 3.0 |EX-2-D| 5.0  |EX-3-D| 3.3  |EX-4-D| 5.0 |EX-5-D| 1.2 |EX-6-D| 1.0 |BACK|                                        19m - 45T
  VEHICLES_CLASS_2D4,      // 6 AXLES  |FRONT| 0.5 |EX-1-S| 3.1 |EX-2-D| 6.0  |EX-3-D| 1.2  |EX-4-D| 6.0 |EX-5-D| 1.2 |EX-6-D| 1.0 |BACK|                                        19m - 45T
  VEHICLES_CLASS_35D,      // 8 AXLES  |FRONT| 0.5 |EX-1-S| 3.3 |EX-2-D| 1.2  |EX-3-D| 7.2  |EX-4-D| 7.2 |EX-5-D| 1.2 |EX-6-D| 7.2 |EX-7-D| 1.2 |EX-8-D| 1.0 |BACK|              30m - 67T
  VEHICLES_CLASS_3D6,      // 9 AXLES  |FRONT| 0.5 |EX-1-S| 3.1 |EX-2-D| 1.2  |EX-3-D| 5.0  |EX-4-S| 5.0 |EX-5-S| 5.0 |EX-6-D| 1.2 |EX-7-D| 5.0 |EX-8-S| 5.0 |EX-9-S| 1.0 |BACK| 32m - 80T
  NUMBER_OF_VEHICLES
} vehicle_class_t;

typedef enum
{
  TRAFFIC_INIT = 0U,
  TRAFFIC_CALC,
  TRAFFIC_RUNNING
}traffic_state_t;

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
  traffic_state_t state;
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
  vehicle_states_t  channel_state;
}app_piezo_data_t;


/******************************************************************************/

#endif/*__APP_TYPES_H__*/

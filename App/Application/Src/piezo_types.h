#ifndef __PIEZO_TYPES_H__
#define __PIEZO_TYPES_H__


#include "piezo_cfg.h"
#include "app_types.h"
/******************************************************************************/

#define LED_PIEZO_AXILE_CONV_CH1      1
#define LED_PIEZO_AXILE_CONV_CH2      4

#define LED_PIEZO_AXILE_PE_CH1        1
#define LED_PIEZO_AXILE_PE_CH2        3

#define LED_PIEZO_DUALLYWEELS_CH1     4
#define LED_PIEZO_DUALLYWEELS_CH2     5


#define NUMBER_OF_CARS 11

/******************************************************************************/

typedef struct
{
  GPIO_TypeDef *GPIO;
      uint32_t  PinMask;
} piezo_pininfo_t;


typedef enum
{
  GROUP_1 = 0,
  GROUP_2,
  NUMBER_OF_GROUPS
} number_groups_t;

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
}axle_types_t;

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
} traffic_t;

typedef enum
{   PIEZO_PULSE_INIT = 0U,
    PIEZO_PULSE_TURN_ON,
    PIEZO_PULSE_PERIOD_TURN_ON,
    PIEZO_PULSE_TURN_OFF
}led_pulse_state_t;

typedef struct 
{
    uint16_t delay;
    led_pulse_state_t state;
    traffic_mode_t mode;
}piezo_pulse_data_t;

typedef enum
{
  INITIAL_TRANSIT_GAP = 0,
  INPUT_LOOP_ACTIVATION,
  OUTPUT_LOOP_ACTIVATION,
  INPUT_LOOP_DISABLED,
  OUTPUT_LOOP_DISABLED
} loop_state_update_t;


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


typedef struct 
{
  state_group_t state;
  uint16_t gap;
  uint16_t time_between_loops;
  uint16_t loop_execution_time;
  uint16_t start_piezo;
}app_loop_data_t;

typedef struct 
{
 
  traffic_mode_t mode;
  uint8_t       index;
  uint8_t next_traffic;
}app_loop_ctrl_t;





/******************************************************************************/


/******************************************************************************/

#endif
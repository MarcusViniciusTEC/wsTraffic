#ifndef __PIEZO_TYPES_H__
#define __PIEZO_TYPES_H__


#include "piezo_cfg.h"
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
  VEHICLE_INACTIVE = 0U,
  VEHICLE_ACTIVE
}vehicle_states_t;

typedef struct 
{   uint8_t       piezo_index;
    uint32_t      delay_time;   
    axle_state_t  state;       
} axle_t;


typedef struct 
{
    axle_t*   axles;       
    uint8_t   num_axles;
    uint32_t  weight_ms; 
    vehicle_states_t vehicle_state
} traffic_t;




/******************************************************************************/


/******************************************************************************/

#endif
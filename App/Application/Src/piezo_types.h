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

/******************************************************************************/


/******************************************************************************/

#endif
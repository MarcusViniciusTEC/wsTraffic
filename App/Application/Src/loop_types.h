#ifndef __LOOP_TYPES_H__
#define __LOOP_TYPES_H__

#include "loop_cfg.h"

#define LOOP_LED_LOOP_0 0
#define LOOP_LED_LOOP_1 2
#define LOOP_LED_LOOP_2 3
#define LOOP_LED_LOOP_3 5

#define DISTANCE_BETWEEN_LOOPS_MTS 1
#define LENGHT_LOOP_MTS 2

typedef struct
{
  GPIO_TypeDef *GPIO;
  uint32_t PinMask;
} loop_pininfo_t;

typedef enum
{
  LOOP_MODE_PE = 0,
  LOOP_MODE_CONV
}loop_mode_t;


typedef enum
{
  INITIAL_TRANSIT_GAP = 0,
  INPUT_LOOP_ACTIVATION,
  OUTPUT_LOOP_ACTIVATION,
  INPUT_LOOP_DISABLED,
  OUTPUT_LOOP_DISABLED
} loop_state_update_t;

#endif
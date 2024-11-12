#ifndef __LOOP_TYPES_H__
#define __LOOP_TYPES_H__

#include "loop_cfg.h"

#define LOOP_LED_LOOP_0                         0
#define LOOP_LED_LOOP_1                         2
#define LOOP_LED_LOOP_2                         3
#define LOOP_LED_LOOP_3                         5

#define DISTANCE_BETWEEN_LOOPS_MTS              1
#define LENGHT_LOOP_MTS                         2

typedef struct
{
  GPIO_TypeDef *GPIO;
      uint32_t  PinMask;
} loop_pininfo_t;


#endif
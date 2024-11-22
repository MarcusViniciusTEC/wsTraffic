#ifndef __LOOP_CFG_H__
#define __LOOP_CFG_H__

#include "main.h"

#define LOOP_NUMBER_OF_OUTPUTS  4

#define LED_LOOP_ENTER_GROUP_1  0
#define LED_LOOP_EXIT_GROUP_1   2

#define LED_LOOP_ENTER_GROUP_2  3
#define LED_LOOP_EXIT_GROUP_2   5

#define UPDATE_NUMBER_OF_LEDS          2
#define UPDATE_NUMBER_OF_LOOPS         2

#define LOOP_ENTER_GROUP_1      0
#define LOOP_EXIT_GROUP_1       1

#define LOOP_ENTER_GROUP_2      2
#define LOOP_EXIT_GROUP_2       3

#define loop_pininfo_vector_default_value \
{                                         \
  {/*LOOP_IN_1*/                          \
    .GPIO    = LP_1_DET_GPIO_Port,        \
    .PinMask = LP_1_DET_Pin,              \
  }                                       \
  ,                                       \
  {/*LOOP_OUT_1*/                         \
    .GPIO    = LP_2_DET_GPIO_Port,        \
    .PinMask = LP_2_DET_Pin,              \
  }                                       \
  ,                                       \
  {/*LOOP_IN_2*/                          \
    .GPIO    = LP_3_DET_GPIO_Port,        \
    .PinMask = LP_3_DET_Pin,              \
  }                                       \
  ,                                       \
  {/*LOOP_OUT_2*/                         \
    .GPIO    = LP_4_DET_GPIO_Port,        \
    .PinMask = LP_4_DET_Pin               \
  }                                       \
}



#endif
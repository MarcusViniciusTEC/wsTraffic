#ifndef __PIEZO_CFG_H__
#define __PIEZO_CFG_H__


#include "main.h"

#define PIEZO_NUMBER_OF_OUTPUTS 4

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


#define piezo_pininfo_vector_default_value  \
{                                           \
  {/*PIEZO_IN_ROD_1*/                       \
    .GPIO    = DWPZ_1_GPIO_Port,            \
    .PinMask = DWPZ_1_Pin,                  \
  }                                         \
  ,                                         \
  {/*PIEZO_OUT_ROD_1*/                      \
    .GPIO    = DWPZ_2_GPIO_Port,            \
    .PinMask = DWPZ_2_Pin,                  \
  }                                         \
  ,                                         \
  {/*PIEZO_IN_EIXO_2*/                      \
    .GPIO    = DXPZ_1_GPIO_Port,            \
    .PinMask = DXPZ_1_Pin,                  \
  }                                         \
  ,                                         \
  {/*PEIZO_OUT_EIXO_2*/                     \
    .GPIO    = DXPZ_2_GPIO_Port,            \
    .PinMask = DXPZ_2_Pin,                  \
  }                                         \
}


#endif
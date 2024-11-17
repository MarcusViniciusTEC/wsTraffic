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
  INITIAL_TRANSIT_GAP = 0,
  INPUT_LOOP_ACTIVATION,
  OUTPUT_LOOP_ACTIVATION,
  INPUT_LOOP_DISABLED,
  OUTPUT_LOOP_DISABLED
} traffic_status_t;

typedef enum
{
  GROUP_ACTIVE = 0,
  GROUP_DISABLED
} state_group_t;

typedef enum
{
  GROUP_1 = 0,
  GROUP_2,
  NUMBER_OF_GROUPS_INDEX
} group_loop_index;

typedef struct
{
  uint32_t speed_traffic;
  uint8_t vehicle_length;
  uint16_t gap_traffic_in_second;
  state_group_t state_group_loop;

} traffic_loop_t;

typedef struct 
{
  uint32_t speed_in_meters_per_second;
  uint32_t time_in_loop;
  uint32_t time_between_loops;
  uint32_t time_spent_in_the_loops;
  uint16_t time_gap_in_ms;
} calc_group_loop_t;

#endif
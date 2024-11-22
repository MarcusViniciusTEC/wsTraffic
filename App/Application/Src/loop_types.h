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

typedef struct 
{
  uint16_t time_in_loop;
  uint16_t time_between_loops;
  uint16_t time_execution_loops;
  uint16_t time_start_piezo_ms;
  uint16_t piezo_trigger_time[11];
  uint16_t piezo_firing_window;
  uint16_t time_trigger_for_axle[9];
} calc_group_loop_t;

typedef struct
{
  uint8_t vehicle_length;
  uint8_t vehicle_axles[9];
  uint8_t vehicle_number_axles;

} vehicle_t;


typedef struct
{
  calc_group_loop_t vehicle[11];
  uint16_t time_gap_in_ms;
  uint16_t speed_in_meters_per_second;
//state_group_t state_group_loop;

} traffic_loop_group_t;


typedef enum
{
  INITIAL_TRANSIT_GAP = 0,
  INPUT_LOOP_ACTIVATION,
  OUTPUT_LOOP_ACTIVATION,
  INPUT_LOOP_DISABLED,
  OUTPUT_LOOP_DISABLED
} loop_state_update_t;

#endif
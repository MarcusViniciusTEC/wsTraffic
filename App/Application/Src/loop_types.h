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
  PIEZO_START,
  OUTPUT_LOOP_ACTIVATION,
  INPUT_LOOP_DISABLED,
  OUTPUT_LOOP_DISABLED
} loop_status_update_t;

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
  uint16_t speed_in_meters_per_second;
  uint16_t time_in_loop;
  uint16_t time_between_loops;
  uint16_t time_spent_in_the_loops;
  uint16_t time_gap_in_ms;
  uint16_t time_start_piezo;
} calc_group_loop_t;

typedef struct
{
  uint8_t speed_traffic;
  uint8_t vehicle_length;
  uint8_t gap_traffic_in_second;

} vehicle_t;

typedef struct
{
  calc_group_loop_t calc_vehicle[11];
  vehicle_t vehicle[11];
  state_group_t state_group_loop;

} traffic_loop_group_t;



typedef enum
{
  VEHICLES_CLASS_2C = 0,  // 2 AXLES
  VEHICLES_CLASS_3C,       // 3 AXLES
  VEHICLES_CLASS_4C,       // 4 AXLES
  VEHICLES_CLASS_2S3,      // 5 AXLES
  VEHICLES_CLASS_3S3,      // 6 AXLES
  VEHICLES_CLASS_3D3,      // 6 AXLES
  VEHICLES_CLASS_3C2,      // 5 AXLES
  VEHICLES_CLASS_2J4,      // 6 AXLES
  VEHICLES_CLASS_2D4,      // 6 AXLES
  VEHICLES_CLASS_35D,      // 8 AXLES
  VEHICLES_CLASS_3D6,       // 9 AXLES
  NUMBER_OF_VEHICLES_INDEX
} vehicle_class_t;

#endif
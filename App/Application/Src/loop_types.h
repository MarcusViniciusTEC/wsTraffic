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




typedef struct 
{

  uint16_t time_in_loop;
  uint16_t time_between_loops;
  uint16_t time_execution_loops;
  uint16_t time_start_piezo_ms;
  uint16_t piezo_trigger_time[11];
} calc_group_loop_t;

typedef struct
{
  uint8_t vehicle_length;
  uint8_t vehicle_axles[9];

} vehicle_t;

typedef struct
{
  vehicle_t vehicle_data[11];
  calc_group_loop_t vehicle[11];
  uint16_t time_gap_in_ms;
  uint16_t speed_in_meters_per_second;
//  //state_group_t state_group_loop;

} traffic_loop_group_t;



typedef enum
{              
  VEHICLES_CLASS_2C = 0,   // 2 AXLES  |FRONT| 0.5 |EX-1-S| 4.5 |EX-2-D| 1.0  |BACK|                                                                                              6m - 16T
  VEHICLES_CLASS_3C,       // 3 AXLES  |FRONT| 0.5 |EX-1-S| 7.5 |EX-2-D| 1.2  |EX-3-D| 0.8  |BACK|                                                                               10m - 23T
  VEHICLES_CLASS_4C,       // 4 AXLES  |FRONT| 0.5 |EX-1-S| 8.1 |EX-2-D| 1.2  |EX-3-D| 1.2  |EX-4-D| 1.0 |BACK|                                                                  12m - 31T
  VEHICLES_CLASS_2S3,      // 5 AXLES  |FRONT| 0.5 |EX-1-S| 3.0 |EX-2-D| 11.2 |EX-3-D| 1.2  |EX-4-D| 1.2 |EX-5-D| 1.0 |BACK|                                                     18m - 41T
  VEHICLES_CLASS_3S3,      // 6 AXLES  |FRONT| 0.5 |EX-1-S| 3.0 |EX-2-D| 1.2  |EX-3-D| 11.2 |EX-4-D| 1.2 |EX-5-D| 1.2 |EX-6-D| 1.0 |BACK|                                        20m - 45T
  VEHICLES_CLASS_3D3,      // 6 AXLES  |FRONT| 0.5 |EX-1-S| 3.0 |EX-2-D| 1.2  |EX-3-D| 14.2 |EX-4-D| 1.2 |EX-5-D| 1.2 |EX-6-D| 1.0 |BACK|                                        23m - 49T
  VEHICLES_CLASS_3C2,      // 5 AXLES  |FRONT| 0.5 |EX-1-S| 4.5 |EX-2-D| 1.2  |EX-3-D| 5.4  |EX-4-D| 6.4 |EX-5-D| 1.0 |BACK|                                                     19m - 43T
  VEHICLES_CLASS_2J4,      // 6 AXLES  |FRONT| 0.5 |EX-1-S| 3.0 |EX-2-D| 5.0  |EX-3-D| 3.3  |EX-4-D| 5.0 |EX-5-D| 1.2 |EX-6-D| 1.0 |BACK|                                        19m - 45T
  VEHICLES_CLASS_2D4,      // 6 AXLES  |FRONT| 0.5 |EX-1-S| 3.1 |EX-2-D| 6.0  |EX-3-D| 1.2  |EX-4-D| 6.0 |EX-5-D| 1.2 |EX-6-D| 1.0 |BACK|                                        19m - 45T
  VEHICLES_CLASS_35D,      // 8 AXLES  |FRONT| 0.5 |EX-1-S| 3.3 |EX-2-D| 1.2  |EX-3-D| 7.2  |EX-4-D| 7.2 |EX-5-D| 1.2 |EX-6-D| 7.2 |EX-7-D| 1.2 |EX-8-D| 1.0 |BACK|              30m - 67T
  VEHICLES_CLASS_3D6,      // 9 AXLES  |FRONT| 0.5 |EX-1-S| 3.1 |EX-2-D| 1.2  |EX-3-D| 5.0  |EX-4-S| 5.0 |EX-5-S| 5.0 |EX-6-D| 1.2 |EX-7-D| 5.0 |EX-8-S| 5.0 |EX-9-S| 1.0 |BACK| 32m - 80T
  NUMBER_OF_VEHICLES
} vehicle_class_t;

#endif
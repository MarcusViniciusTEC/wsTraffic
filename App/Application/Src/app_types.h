#ifndef __APP_TYPES_H__
#define __APP_TYPES_H__

/******************************************************************************/

#include "app_cfg.h"

/******************************************************************************/

#define TIME_ZERO 0


typedef enum
{
  KEY_ON = 0,
  KEY_OFF
}key_state_t;

typedef enum
{
  MODE_PE = 0,
  MODE_CONV
}traffic_mode_t;


typedef struct 
{
    uint32_t delay_time;  
    uint8_t led_index;    
    uint8_t active;       
} axle_t;


typedef struct {
    axle_t* axles;       
    uint8_t num_axles;    
} traffic_t;


/******************************************************************************/

#endif/*__APP_TYPES_H__*/

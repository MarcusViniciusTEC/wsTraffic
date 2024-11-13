#include "sl.h"
#include "loop.h"
#include "loop_cfg.h"
#include "hmi.h"

#include <stdbool.h>

/******************************************************************************/

volatile uint32_t loop_execution_rate_1ms_timer;

/******************************************************************************/

static const loop_pininfo_t loop_pininfo_vector[LOOP_NUMBER_OF_OUTPUTS] = loop_pininfo_vector_default_value;

/******************************************************************************/

void loop_turn_on(uint8_t index)
{
  loop_pininfo_t loop_pininfo;
  loop_pininfo = loop_pininfo_vector[index];
  LL_GPIO_SetOutputPin(loop_pininfo.GPIO, loop_pininfo.PinMask);
}

/******************************************************************************/

void loop_turn_off(uint8_t index)
{
  loop_pininfo_t loop_pininfo;
  loop_pininfo = loop_pininfo_vector[index];
  LL_GPIO_ResetOutputPin(loop_pininfo.GPIO, loop_pininfo.PinMask);
}

/******************************************************************************/

void loop_init_apply(void)
{
  for(uint8_t loop_index = 0; loop_index < LOOP_NUMBER_OF_OUTPUTS; loop_index++ )
  {
    loop_turn_off(loop_index);
  }
}

/******************************************************************************/

void loop_1ms_clock(void)
{


}  

/******************************************************************************/

void loop_init(void)
{

}              

/******************************************************************************/

void loop_update(void)
{


}            

/******************************************************************************/

void loop_deinit(void)
{


}


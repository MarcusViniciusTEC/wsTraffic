#include "sl.h"
#include "loop.h"
#include "loop_cfg.h"
#include "loop_types.h"
#include "hmi.h"
#include "app.h"

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
// AlissonGOE

traffic_status_t transit_state_group_loop_1(uint16_t time_between_loops_t, uint16_t gap_t, uint16_t time_spent_in_the_bonds_t, uint8_t group_index)
{
  uint16_t time_between_loop[2];
  uint16_t gap[2];
  uint16_t time_spent_in_the_bonds[2];
  static uint32_t count_loop_1[2] = {0,0};

  time_between_loop[group_index] = time_between_loops_t;
  gap[group_index] = gap_t;
  time_spent_in_the_bonds[group_index] = time_spent_in_the_bonds_t;
  count_loop_1[group_index]++;
  
  if (count_loop_1[group_index] <= gap[group_index])
  {
    current_status_group_loop_1(INITIAL_TRANSIT_GAP, group_index); // off off
    return INITIAL_TRANSIT_GAP;
  }
  else if (count_loop_1[group_index] <= (gap[group_index] + time_between_loop[group_index]))
  {
    current_status_group_loop_1(INPUT_LOOP_ACTIVATION, group_index); // on off
    return INPUT_LOOP_ACTIVATION;
  }
  else if (count_loop_1[group_index] <= (gap[group_index] + time_spent_in_the_bonds[group_index]))
  {
    current_status_group_loop_1(OUTPUT_LOOP_ACTIVATION, group_index); // on on
    return OUTPUT_LOOP_ACTIVATION;
  }
  else if (count_loop_1[group_index] <= (gap[group_index] + time_spent_in_the_bonds[group_index] + time_between_loop[group_index]))
  {
    current_status_group_loop_1(INPUT_LOOP_DISABLED, group_index); // off on
    return INPUT_LOOP_DISABLED;
  }
  else
  {
    count_loop_1[group_index] = 0;
    time_between_loop[group_index] = 0;
    gap[group_index] = 0;
    time_spent_in_the_bonds[group_index] = 0;
    current_status_group_loop_1(OUTPUT_LOOP_DISABLED, group_index); // off off
    return OUTPUT_LOOP_DISABLED;
  }
  
}

void current_status_group_loop_1(traffic_status_t state, uint8_t group_index)
{

  uint8_t led_input;
  uint8_t led_output;
  uint8_t loop_input;
  uint8_t loop_output;

  if(group_index == GROUP_1)
  {
    led_input = 0;
    loop_input = 0;

    led_output = 2;
    loop_output = 1;
  }
  else
  {
    led_input = 3;
    loop_input = 2;

    led_output = 5;
    loop_output = 3;
  }

  switch (state)
  {
  case INITIAL_TRANSIT_GAP:
    hmi_led_turn_off(led_input);
    loop_turn_off(loop_input);

    hmi_led_turn_off(led_output);
    loop_turn_off(loop_output);
    break;

  case INPUT_LOOP_ACTIVATION:
    hmi_led_turn_on(led_input);
    loop_turn_on(loop_input);

    hmi_led_turn_off(led_output);
    loop_turn_off(loop_output);
    break;

  case OUTPUT_LOOP_ACTIVATION:
    hmi_led_turn_on(led_input);
    loop_turn_on(loop_input);

    hmi_led_turn_on(led_output);
    loop_turn_on(loop_output);
    break;

  case INPUT_LOOP_DISABLED:
    hmi_led_turn_off(led_input);
    loop_turn_off(loop_input);

    hmi_led_turn_on(led_output);
    loop_turn_on(loop_output);
    break;

  case OUTPUT_LOOP_DISABLED:
    hmi_led_turn_off(led_input);
    loop_turn_off(loop_input);

    hmi_led_turn_off(led_output);
    loop_turn_off(loop_output);
    break;

  default:
    break;
  }
}


// AlissonGOE
/******************************************************************************/

void loop_init_apply(void)
{
  for (uint8_t loop_index = 0; loop_index < LOOP_NUMBER_OF_OUTPUTS; loop_index++)
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

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

void transit_state_group_loop_1(uint32_t time_between_loops_t, uint32_t time_in_loop_t, uint32_t gap, uint32_t time_spent_in_the_bonds_t)
{
  static uint32_t count_loop_1 = 0;

  count_loop_1++;

  if (count_loop_1 <= gap)
  {
    current_status_group_loop_1(INITIAL_TRANSIT_GAP); // off off
  }
  else if (count_loop_1 <= (gap + time_between_loops_t))
  {
    current_status_group_loop_1(INPUT_LOOP_ACTIVATION); // on off
  }
  else if (count_loop_1 <= (gap + time_spent_in_the_bonds_t))
  {
    current_status_group_loop_1(OUTPUT_LOOP_ACTIVATION); // on on
  }
  else if (count_loop_1 <= (gap + time_spent_in_the_bonds_t + time_between_loops_t))
  {
    current_status_group_loop_1(INPUT_LOOP_DISABLED); // on on
  }
  else
  {
    count_loop_1 = 0;
    current_status_group_loop_1(OUTPUT_LOOP_DISABLED); // off off
  }
}

void transit_state_group_loop_2(uint32_t time_between_loops_t, uint32_t time_in_loop_t, uint32_t gap, uint32_t time_spent_in_the_bonds_t)
{
  static uint32_t count_loop_2 = 0;

  count_loop_2++;

  if (count_loop_2 <= gap)
  {
    current_status_group_loop_2(INITIAL_TRANSIT_GAP); // off off
  }
  else if (count_loop_2 <= (gap + time_between_loops_t))
  {
    current_status_group_loop_2(INPUT_LOOP_ACTIVATION); // on off
  }
  else if (count_loop_2 <= (gap + time_spent_in_the_bonds_t))
  {
    current_status_group_loop_2(OUTPUT_LOOP_ACTIVATION); // on on
  }
  else if (count_loop_2 <= (gap + time_spent_in_the_bonds_t + time_between_loops_t))
  {
    current_status_group_loop_2(INPUT_LOOP_DISABLED); // on on
  }
  else
  {
    count_loop_2 = 0;
    current_status_group_loop_2(OUTPUT_LOOP_DISABLED); // off off
  }
}


void current_status_group_loop_1(traffic_status_t state)
{
  switch (state)
  {
  case INITIAL_TRANSIT_GAP:
    hmi_led_turn_off(0);
    loop_turn_off(0);

    hmi_led_turn_off(2);
    loop_turn_off(1);
    break;

  case INPUT_LOOP_ACTIVATION:
    hmi_led_turn_on(0);
    loop_turn_on(0);

    hmi_led_turn_off(2);
    loop_turn_off(1);
    break;

  case OUTPUT_LOOP_ACTIVATION:
    hmi_led_turn_on(0);
    loop_turn_on(0);

    hmi_led_turn_on(2);
    loop_turn_on(1);
    break;

  case INPUT_LOOP_DISABLED:
    hmi_led_turn_off(0);
    loop_turn_off(0);

    hmi_led_turn_on(2);
    loop_turn_on(1);
    break;

  case OUTPUT_LOOP_DISABLED:
    hmi_led_turn_off(0);
    loop_turn_off(0);

    hmi_led_turn_off(2);
    loop_turn_off(1);
    break;

  default:
    break;
  }
}

void current_status_group_loop_2(traffic_status_t state)
{
  switch (state)
  {
  case INITIAL_TRANSIT_GAP:
    hmi_led_turn_off(3);
    loop_turn_off(2);

    hmi_led_turn_off(5);
    loop_turn_off(3);
    break;

  case INPUT_LOOP_ACTIVATION:
    hmi_led_turn_on(3);
    loop_turn_on(2);

    hmi_led_turn_off(5);
    loop_turn_off(3);
    break;

  case OUTPUT_LOOP_ACTIVATION:
    hmi_led_turn_on(3);
    loop_turn_on(2);

    hmi_led_turn_on(5);
    loop_turn_on(3);
    break;

  case INPUT_LOOP_DISABLED:
    hmi_led_turn_off(3);
    loop_turn_off(2);

    hmi_led_turn_on(5);
    loop_turn_on(3);
    break;

  case OUTPUT_LOOP_DISABLED:
    hmi_led_turn_off(3);
    loop_turn_off(2);

    hmi_led_turn_off(5);
    loop_turn_off(3);
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

  traffic_calculation_app();

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

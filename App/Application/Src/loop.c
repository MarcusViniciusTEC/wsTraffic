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

loop_status_update_t transit_state_group_loop_1(uint16_t time_between_loops_t, uint16_t gap_t, uint16_t time_spent_in_the_loops_t, uint16_t time_start_piezo_t, uint8_t group_index)
{
  uint16_t time_between_loop[2];
  uint16_t gap[2];
  uint16_t time_spent_in_the_loops[2];
  uint16_t time_start_piezo[2];
  static uint32_t count_loop_1[2] = {0, 0};

  time_between_loop[group_index] = time_between_loops_t;
  gap[group_index] = gap_t;
  time_spent_in_the_loops[group_index] = time_spent_in_the_loops_t;
  time_start_piezo[group_index] = time_start_piezo_t;
  count_loop_1[group_index]++;

  if (count_loop_1[group_index] <= gap[group_index])      // 1000                           432                 360                 1728
  {
    loop_update_state(INITIAL_TRANSIT_GAP, group_index); // off off
    return INITIAL_TRANSIT_GAP;
  }


  else if (count_loop_1[group_index] <= (gap[group_index] + time_between_loop[group_index]))
  {
    loop_update_state(INPUT_LOOP_ACTIVATION, group_index); // on off
    if (count_loop_1[group_index] <= (gap[group_index] + time_start_piezo[group_index]))
    {
      loop_update_state(PIEZO_START, group_index); // on off on_pz
    }
  }



  else if (count_loop_1[group_index] <= (gap[group_index] + time_spent_in_the_loops[group_index]))
  {
    loop_update_state(OUTPUT_LOOP_ACTIVATION, group_index); // on on
    return OUTPUT_LOOP_ACTIVATION;
  }



  else if (count_loop_1[group_index] <= (gap[group_index] + time_spent_in_the_loops[group_index] + time_between_loop[group_index]))
  {
    loop_update_state(INPUT_LOOP_DISABLED, group_index); // off on
    return INPUT_LOOP_DISABLED;
  }



  else
  {
    count_loop_1[group_index] = 0;
    time_between_loop[group_index] = 0;
    gap[group_index] = 0;
    time_spent_in_the_loops[group_index] = 0;
    loop_update_state(OUTPUT_LOOP_DISABLED, group_index); // off off
    return OUTPUT_LOOP_DISABLED;
  }
}

void loop_update_state(loop_status_update_t state, uint8_t index)
{
  const uint8_t LED_ENTER_LOOP[UPDATE_NUMBER_OF_LOOPS] = {LED_LOOP_ENTER_GROUP_1, LED_LOOP_ENTER_GROUP_2};
  const uint8_t LED_EXIT_LOOP[UPDATE_NUMBER_OF_LEDS] = {LED_LOOP_EXIT_GROUP_1, LED_LOOP_EXIT_GROUP_2};
  const uint8_t ENTER_LOOP_PIN[UPDATE_NUMBER_OF_LOOPS] = {LOOP_ENTER_GROUP_1, LOOP_ENTER_GROUP_2};
  const uint8_t EXIT_LOOP_PIN[UPDATE_NUMBER_OF_LOOPS] = {LOOP_EXIT_GROUP_1, LOOP_EXIT_GROUP_2};
  switch (state)
  {
  case INITIAL_TRANSIT_GAP:
    hmi_led_turn_off(LED_ENTER_LOOP[index]);
    loop_turn_off(ENTER_LOOP_PIN[index]);
    hmi_led_turn_off(LED_EXIT_LOOP[index]);
    loop_turn_off(EXIT_LOOP_PIN[index]);
    break;
  case PIEZO_START:
    hmi_led_turn_on(LED_ENTER_LOOP[index]);
    loop_turn_on(ENTER_LOOP_PIN[index]);
    hmi_led_turn_off(LED_EXIT_LOOP[index]);
    loop_turn_off(EXIT_LOOP_PIN[index]);
    break;
  case INPUT_LOOP_ACTIVATION:
    hmi_led_turn_on(1);
    hmi_led_turn_on(4);
    break;
  case OUTPUT_LOOP_ACTIVATION:
    hmi_led_turn_on(LED_ENTER_LOOP[index]);
    loop_turn_on(ENTER_LOOP_PIN[index]);
    hmi_led_turn_on(LED_EXIT_LOOP[index]);
    loop_turn_on(EXIT_LOOP_PIN[index]);
    break;
  case INPUT_LOOP_DISABLED:
    hmi_led_turn_off(LED_ENTER_LOOP[index]);
    loop_turn_off(ENTER_LOOP_PIN[index]);
    hmi_led_turn_on(LED_EXIT_LOOP[index]);
    loop_turn_on(EXIT_LOOP_PIN[index]);
    break;
  case OUTPUT_LOOP_DISABLED:
    hmi_led_turn_off(LED_ENTER_LOOP[index]);
    loop_turn_off(ENTER_LOOP_PIN[index]);
    hmi_led_turn_off(LED_EXIT_LOOP[index]);
    loop_turn_off(EXIT_LOOP_PIN[index]);
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

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

void loop_init_apply(void)
{
  for (uint8_t loop_index = 0; loop_index < LOOP_NUMBER_OF_OUTPUTS; loop_index++)
  {
    loop_turn_off(loop_index);
  }
}

/******************************************************************************/

void loop_update_state(loop_state_update_t state, uint8_t index, traffic_mode_t mode)
{
    const uint8_t LED_EXIT_LOOP [UPDATE_NUMBER_OF_LOOPS]=   {LED_LOOP_EXIT_GROUP_1, LED_LOOP_EXIT_GROUP_2};
    const uint8_t LED_ENTER_LOOP[UPDATE_NUMBER_OF_LOOPS] = {LED_LOOP_ENTER_GROUP_1, LED_LOOP_ENTER_GROUP_2};
    const uint8_t ENTER_LOOP_PIN[UPDATE_NUMBER_OF_LOOPS] = {LOOP_ENTER_GROUP_1, LOOP_ENTER_GROUP_2};
    const uint8_t EXIT_LOOP_PIN[UPDATE_NUMBER_OF_LOOPS] =  {LOOP_EXIT_GROUP_1, LOOP_EXIT_GROUP_2};
    switch (state)
    {
    case INITIAL_TRANSIT_GAP:
        hmi_led_turn_off(LED_ENTER_LOOP[index]);
        loop_turn_off   (ENTER_LOOP_PIN[index]);
        hmi_led_turn_off(LED_EXIT_LOOP [index]);
        loop_turn_off   (EXIT_LOOP_PIN [index]);
        break;
    case INPUT_LOOP_ACTIVATION:
        hmi_led_turn_on (LED_ENTER_LOOP[index]);
        loop_turn_on    (ENTER_LOOP_PIN[index]);
        hmi_led_turn_off(LED_EXIT_LOOP [index]);
        loop_turn_off   (EXIT_LOOP_PIN [index]);
        break;
    case OUTPUT_LOOP_ACTIVATION:
        hmi_led_turn_on (LED_ENTER_LOOP[index]);
        loop_turn_on    (ENTER_LOOP_PIN[index]);
        hmi_led_turn_on (LED_EXIT_LOOP [index]);
        loop_turn_on    (EXIT_LOOP_PIN [index]);
        break;
    case INPUT_LOOP_DISABLED:
        hmi_led_turn_off(LED_ENTER_LOOP[index]);
        loop_turn_off   (ENTER_LOOP_PIN[index]);
        hmi_led_turn_on (LED_EXIT_LOOP [index]);
        loop_turn_on    (EXIT_LOOP_PIN [index]);
        break;
    case OUTPUT_LOOP_DISABLED:
        hmi_led_turn_off(LED_ENTER_LOOP[index]);
        loop_turn_off   (ENTER_LOOP_PIN[index]);
        hmi_led_turn_off(LED_EXIT_LOOP [index]);
        loop_turn_off   (EXIT_LOOP_PIN [index]);
        break;
      default:
        break;
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

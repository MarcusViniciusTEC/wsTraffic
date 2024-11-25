#include "sl.h"
#include "piezo.h"
#include "piezo_cfg.h"
#include "hmi.h"
#include "app.h"
#include "stdbool.h"

/******************************************************************************/

volatile uint32_t piezo_execution_rate_1ms_timer;

/******************************************************************************/

static const piezo_pininfo_t piezo_pininfo_vector[PIEZO_NUMBER_OF_OUTPUTS] = piezo_pininfo_vector_default_value;
static piezo_pulse_data_t   piezo_pulse_data[NUMBER_OF_GROUPS] = {0};

uint8_t mode_sat = MODE_CONV;

/******************************************************************************/

void piezo_turn_on(uint8_t index)
{
    piezo_pininfo_t piezo_pininfo;
    piezo_pininfo = piezo_pininfo_vector[index];
    LL_GPIO_SetOutputPin(piezo_pininfo.GPIO, piezo_pininfo.PinMask);
}

/******************************************************************************/

void piezo_turn_off(uint8_t index)
{
    piezo_pininfo_t piezo_pininfo;
    piezo_pininfo = piezo_pininfo_vector[index];
    LL_GPIO_ResetOutputPin(piezo_pininfo.GPIO, piezo_pininfo.PinMask);
}

/******************************************************************************/

void piezo_pulse_1us(void)
{
    LL_GPIO_TogglePin(MCU_STATUS_GPIO_Port, MCU_STATUS_Pin);
    for(uint8_t index_delay = 0; index_delay < NUMBER_OF_GROUPS; index_delay++)
    {
        if(piezo_pulse_data[index_delay].delay > 0 && piezo_pulse_data[index_delay].state == PIEZO_PULSE_PERIOD_TURN_ON)
        {
            piezo_pulse_data[index_delay].delay--;
        }
    }
}

/******************************************************************************/

void piezo_pulse_received_parameters(uint8_t index, piezo_pulse_data_t piezo_pulse_data_par)
{
    piezo_pulse_data[index] = piezo_pulse_data_par;
}

/******************************************************************************/

static void piezo_pulse_update_state (uint8_t index) /*microseconds*/
{
    const uint8_t LEDS_PIEZO_CONV  [NUMBER_OF_GROUPS] = {LED_PIEZO_AXILE_CONV_CH1, LED_PIEZO_AXILE_CONV_CH2};
    const uint8_t LEDS_PIEZO_PE    [NUMBER_OF_GROUPS] = {LED_PIEZO_AXILE_PE_CH1  ,   LED_PIEZO_AXILE_PE_CH2};
    switch (piezo_pulse_data[index].state)
    {
    case PIEZO_PULSE_INIT:
        piezo_pulse_data[index].state = PIEZO_PULSE_TURN_ON;
        break;
    case PIEZO_PULSE_TURN_ON:
        if(piezo_pulse_data[index].mode == MODE_CONV)
        {
            hmi_led_turn_on(LEDS_PIEZO_CONV[index]);
        }
        else if(piezo_pulse_data[index].mode == MODE_PE)
        {
            hmi_led_turn_on(LEDS_PIEZO_PE[index]);
        }  
        piezo_turn_on(index);
        piezo_pulse_data[index].state = PIEZO_PULSE_PERIOD_TURN_ON;
        break;
    case PIEZO_PULSE_PERIOD_TURN_ON:
        if(piezo_pulse_data[index].delay == 0)
        {
            piezo_pulse_data[index].state = PIEZO_PULSE_TURN_OFF;
        }
        break;
    case PIEZO_PULSE_TURN_OFF:
        if(piezo_pulse_data[index].mode == MODE_CONV)
        {
            hmi_led_turn_off(LEDS_PIEZO_CONV[index]);
        }
        else if(piezo_pulse_data[index].mode == MODE_PE)
        {
            hmi_led_turn_off(LEDS_PIEZO_PE[index]);
        }
        piezo_turn_off(index);
        break;
    default:
        break;
    }
}

/******************************************************************************/

void piezo_1us_clock(void) /*microseconds*/
{
    piezo_pulse_1us();
    for(uint8_t index = 0; index < NUMBER_OF_GROUPS; index ++)
    {
        piezo_pulse_update_state(index);
    }
}

/******************************************************************************/

void piezo_1ms_clock(void)
{

}

/******************************************************************************/

void piezo_init(void)
{
    for(uint8_t index = 0; index < NUMBER_OF_GROUPS; index ++)
    {
        piezo_pulse_data[index].state = PIEZO_PULSE_INIT;
    }
}                  

/******************************************************************************/

void piezo_update(void)
{
 
}                

/******************************************************************************/

void piezo_deinit(void)
{

}
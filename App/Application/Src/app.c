#include "sl.h"
#include "app.h"
#include "app_cfg.h"

/******************************************************************************/

#include "hmi.h"
#include "loop.h"
#include "piezo.h"
#include "loop_types.h"
// #include "wlog.h"

/******************************************************************************/

// #include "gpio.h"

#include <stdint.h>

#define DELAY_BETWEEN_CARS 1000 // 1000ms de espera entre carros

/******************************************************************************/

volatile uint32_t app_execution_rate_1ms_timer;

static traffic_loop_t traffic_loop[1];
static calc_group_loop_t calc_group_loop[2];

/******************************************************************************/



uint32_t car_delay_timer = 0;
uint8_t current_car = 0;
traffic_t traffic[1];
axle_t axles[9];

void init_axles(void)
{

    traffic[0].axles = axles;
    traffic[0].num_axles = 3;
    traffic[0].axles[0].delay_time = 1;
    traffic[0].axles[1].delay_time = 15;
    traffic[0].axles[2].delay_time = 25;

    for (int i = 0; i < traffic[current_car].num_axles; i++)
    {
        traffic[current_car].axles[i].active = 1;
    }
}

void app_1ms_clock(void)
{

    static uint32_t timer = 0;

    for (uint8_t i = 0; i < traffic[current_car].num_axles; i++)
    {
        if (traffic[current_car].axles[i].active && timer == traffic[current_car].axles[i].delay_time)
        {
            hmi_led_turn_on(traffic[current_car].axles[i].led_index);
            hmi_led_turn_off(traffic[current_car].axles[i].led_index);
            traffic[current_car].axles[i].active = 0;
        }
    }

    timer++;
}

void app_1us_clock() /*10uS*/
{
}

void update_state_loop(void)
{
}
void app_set_address_and_mode(uint8_t addres, uint8_t mode)
{
}

/******************************************************************************/

void app_read_address_and_mode(void)
{
    uint8_t address = 0;
    uint8_t mode = 0;
    if (LL_GPIO_IsInputPinSet(APP_ID_MODE_GPIO_PORT, APP_ID_MODE_GPIO_PIN) == MODE_PE)
    {
        mode = MODE_PE;
    }
    else
    {
        mode = MODE_CONV;
    }
    if (LL_GPIO_IsInputPinSet(APP_ID_BIT1_GPIO_PORT, APP_ID_BIT1_GPIO_PIN) == KEY_OFF)
    {
        address |= (1 << 0);
    }
    if (LL_GPIO_IsInputPinSet(APP_ID_BIT2_GPIO_PORT, APP_ID_BIT2_GPIO_PIN) == KEY_OFF)
    {
        address |= (1 << 1);
    }
    if (LL_GPIO_IsInputPinSet(APP_ID_BIT3_GPIO_PORT, APP_ID_BIT3_GPIO_PIN) == KEY_OFF)
    {
        address |= (1 << 2);
    }
    app_set_address_and_mode(address, mode);
}

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
// AlissonGOE

void traffic_calculation_app(void)
{
    traffic_loop[GROUP_LOOP_INDEX_1].gap_traffic = 1000;
    traffic_loop[GROUP_LOOP_INDEX_1].speed_traffic = 120;
    traffic_loop[GROUP_LOOP_INDEX_1].vehicle_length = 4;
    traffic_loop[GROUP_LOOP_INDEX_1].state_group_loop = GROUP_ACTIVE;

    traffic_loop[GROUP_LOOP_INDEX_2].gap_traffic = 1000;
    traffic_loop[GROUP_LOOP_INDEX_2].speed_traffic = 80;
    traffic_loop[GROUP_LOOP_INDEX_2].vehicle_length = 22;
    traffic_loop[GROUP_LOOP_INDEX_2].state_group_loop = GROUP_ACTIVE;

    
    for (uint8_t calc_index = 0; calc_index < NUMBER_OF_GROUPS_INDEX; calc_index++)
    {
        calc_group_loop[calc_index].speed_in_meters_per_second = (traffic_loop[calc_index].speed_traffic * 1000) / 3.6;

        calc_group_loop[calc_index].time_between_loops = (((LENGHT_LOOP_MTS + DISTANCE_BETWEEN_LOOPS_MTS) * 1000000)
        / calc_group_loop[calc_index].speed_in_meters_per_second);

        calc_group_loop[calc_index].time_in_loop = ((LENGHT_LOOP_MTS * 1000000) / calc_group_loop[calc_index].speed_in_meters_per_second);

        calc_group_loop[calc_index].time_spent_in_the_bonds = ((traffic_loop[calc_index].vehicle_length * 1000000)
        / calc_group_loop[calc_index].speed_in_meters_per_second) + calc_group_loop[calc_index].time_in_loop;

    }

    if(traffic_loop[GROUP_LOOP_INDEX_1].state_group_loop == GROUP_ACTIVE)
    {
        transit_state_group_loop_1(calc_group_loop[GROUP_LOOP_INDEX_1].time_between_loops, calc_group_loop[GROUP_LOOP_INDEX_1].time_in_loop, traffic_loop[GROUP_LOOP_INDEX_1].gap_traffic, calc_group_loop[GROUP_LOOP_INDEX_1].time_spent_in_the_bonds);
    }
    
    if(traffic_loop[GROUP_LOOP_INDEX_2].state_group_loop == GROUP_ACTIVE)
    {
        transit_state_group_loop_2(calc_group_loop[GROUP_LOOP_INDEX_2].time_between_loops, calc_group_loop[GROUP_LOOP_INDEX_2].time_in_loop, traffic_loop[GROUP_LOOP_INDEX_2].gap_traffic, calc_group_loop[GROUP_LOOP_INDEX_2].time_spent_in_the_bonds);
    }

}

// AlissonGOE
/******************************************************************************/

/******************************************************************************/

void app_init(void)
{
    //init_axles();
}

/******************************************************************************/

void app_update(void)
{
}

/******************************************************************************/

void app_deinit(void)
{
}

/******************************************************************************/

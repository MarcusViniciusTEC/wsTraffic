#include "sl.h"
#include "app.h"
#include "app_cfg.h"
#include <stdint.h>

/******************************************************************************/

#include "hmi.h"
#include "loop.h"
#include "piezo.h"
#include "loop_types.h"
#include "piezo_types.h"

// #include "wlog.h"

/******************************************************************************/

volatile uint32_t app_execution_rate_1ms_timer;

traffic_loop_group_t calc_group_loop[2];
vehicle_t vehicle_data[10];

/******************************************************************************/

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
// AlissonGOE

void traffic_calculation_app(void)
{

    uint16_t gap_traffic_in_second = 1000;
    uint8_t speed_traffic = 40;
    static uint8_t group_index = 0;
    static uint8_t vehicle_index = 0;
    static uint8_t axle_index = 0;

    calc_group_loop[group_index].time_gap_in_ms = gap_traffic_in_second * 1000;
    calc_group_loop[group_index].speed_in_meters_per_second = (speed_traffic * 1000) / 3.6;

    for (vehicle_index; vehicle_index < NUMBER_OF_VEHICLES; vehicle_index++)
    {
        calc_group_loop[group_index].vehicle[vehicle_index].time_between_loops = (((LENGHT_LOOP_MTS + DISTANCE_BETWEEN_LOOPS_MTS) * 1000000) 
        / calc_group_loop[group_index].speed_in_meters_per_second);

        calc_group_loop[group_index].vehicle[vehicle_index].time_in_loop = ((LENGHT_LOOP_MTS * 1000000) 
        / calc_group_loop[group_index].speed_in_meters_per_second);

        calc_group_loop[group_index].vehicle[vehicle_index].time_execution_loops = ((vehicle_data[vehicle_index].vehicle_length * 1000000) 
        / calc_group_loop[group_index].speed_in_meters_per_second) + calc_group_loop[group_index].vehicle[vehicle_index].time_in_loop;

        calc_group_loop[group_index].vehicle[vehicle_index].time_start_piezo_ms = (((LENGHT_LOOP_MTS * 1000000) + 500000) 
        / calc_group_loop[group_index].speed_in_meters_per_second);

        calc_group_loop[group_index].vehicle[vehicle_index].piezo_firing_window = (calc_group_loop[group_index].vehicle[vehicle_index].time_execution_loops 
        - (2 * calc_group_loop[group_index].vehicle[vehicle_index].time_start_piezo_ms));

        for (axle_index; axle_index == vehicle_data[vehicle_index].vehicle_number_axles; axle_index++)
        {
            calc_group_loop[group_index].vehicle[vehicle_index].time_trigger_for_axle[axle_index] = (calc_group_loop[group_index].vehicle[vehicle_index].piezo_firing_window 
            * vehicle_data[vehicle_index].vehicle_axles[axle_index]) / 100;
        }   
    }
}

void value_traffic(void)
{
    vehicle_data[VEHICLES_CLASS_2C].vehicle_length        =  6;
    vehicle_data[VEHICLES_CLASS_2C].vehicle_number_axles  =  2;
    vehicle_data[VEHICLES_CLASS_2C].vehicle_axles[AXLE_1] =  8;
    vehicle_data[VEHICLES_CLASS_2C].vehicle_axles[AXLE_2] = 83;


    vehicle_data[VEHICLES_CLASS_3C].vehicle_length        = 10;
    vehicle_data[VEHICLES_CLASS_3C].vehicle_number_axles  =  3;
    vehicle_data[VEHICLES_CLASS_3C].vehicle_axles[AXLE_1] =  5;
    vehicle_data[VEHICLES_CLASS_3C].vehicle_axles[AXLE_2] = 80;
    vehicle_data[VEHICLES_CLASS_3C].vehicle_axles[AXLE_3] = 92;

    vehicle_data[VEHICLES_CLASS_4C].vehicle_length        = 12;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_number_axles  =  4;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_1] =  4;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_2] = 71;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_3] = 73;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_4] = 83;


    vehicle_data[VEHICLES_CLASS_2S3].vehicle_length        = 18;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_number_axles  =  5;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_1] =  3;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_2] = 19;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_3] = 81;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_4] = 88;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_5] = 95;

    vehicle_data[VEHICLES_CLASS_3S3].vehicle_length        = 20;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_number_axles  =  6;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_1] =  3;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_2] = 17;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_3] = 23;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_4] = 80;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_5] = 85;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_6] = 91;

    vehicle_data[VEHICLES_CLASS_3D3].vehicle_length        = 23;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_number_axles  =  6;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_1] =  2;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_2] = 15;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_3] = 20;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_4] = 82;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_5] = 87;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_6] = 92;

    vehicle_data[VEHICLES_CLASS_3C2].vehicle_length        = 19;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_number_axles  =  5;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_1] =  3;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_2] = 26;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_3] = 32;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_4] = 61;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_5] = 94;

    vehicle_data[VEHICLES_CLASS_2J4].vehicle_length        = 19;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_number_axles  =  6;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_1] =  3;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_2] = 18;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_3] = 44;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_4] = 62;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_5] = 88;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_6] = 94;

    vehicle_data[VEHICLES_CLASS_2D4].vehicle_length        = 19;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_number_axles  =  6;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_1] =  3;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_2] = 19;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_3] = 50;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_4] = 57;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_5] = 88;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_6] = 95;

    vehicle_data[VEHICLES_CLASS_35D].vehicle_length        = 30;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_number_axles  =  8;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_1] =  2;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_2] = 13;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_3] = 17;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_4] = 41;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_5] = 65;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_6] = 69;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_7] = 93;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_8] = 97;

    vehicle_data[VEHICLES_CLASS_3D6].vehicle_length        = 32;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_number_axles  =  9;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_1] =  2;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_2] = 11;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_3] = 15;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_4] = 31;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_5] = 46;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_6] = 62;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_7] = 66;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_8] = 81;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_9] = 97;

}

// AlissonGOE
/******************************************************************************/

void app_1ms_clock(void)
{
}

/******************************************************************************/

void app_init(void)
{
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

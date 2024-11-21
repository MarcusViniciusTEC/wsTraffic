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
traffic_loop_group_t group_loop;

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

    calc_group_loop[GROUP_1].vehicle[VEHICLES_CLASS_2C].time_between_loops;

    // for (group_index; group_index < NUMBER_OF_GROUPS; group_index++)
    // {
    // }
    calc_group_loop[group_index].time_gap_in_ms = gap_traffic_in_second * 1000;
    calc_group_loop[group_index].speed_in_meters_per_second = (speed_traffic * 1000) / 3.6;

    for (vehicle_index; vehicle_index < NUMBER_OF_VEHICLES; vehicle_index++)
    {
        calc_group_loop[group_index].vehicle[vehicle_index].time_between_loops = (((LENGHT_LOOP_MTS + DISTANCE_BETWEEN_LOOPS_MTS) * 1000000) 
        / calc_group_loop[group_index].speed_in_meters_per_second);

        calc_group_loop[group_index].vehicle[vehicle_index].time_in_loop = ((LENGHT_LOOP_MTS * 1000000) 
        / calc_group_loop[group_index].speed_in_meters_per_second);

        calc_group_loop[group_index].vehicle[vehicle_index].time_execution_loops = ((group_loop[group_index].vehicle_data[vehicle_index].vehicle_length * 1000000) 
        / calc_group_loop[group_index].speed_in_meters_per_second) + calc_group_loop[group_index].vehicle[vehicle_index].time_in_loop;

        calc_group_loop[group_index].vehicle[vehicle_index].time_start_piezo_ms = (((LENGHT_LOOP_MTS * 1000000) + 500000) 
        / calc_group_loop[group_index].speed_in_meters_per_second);
    }

    // if (group_loop[GROUP_1].state_group_loop == GROUP_ACTIVE)
    // {
    //     traffic_status_t return_transit_state_group_1;
    //     static uint8_t calc_vehicle_index_group_1 = 0;

    //     return_transit_state_group_1 = transit_state_group_loop_1(
    //     calc_group_loop[GROUP_1].calc_vehicle[calc_vehicle_index_group_1].time_between_loops,
    //     calc_group_loop[GROUP_1].calc_vehicle[calc_vehicle_index_group_1].time_in_loop,
    //     calc_group_loop[GROUP_1].calc_vehicle[calc_vehicle_index_group_1].time_gap_in_ms,
    //     calc_group_loop[GROUP_1].calc_vehicle[calc_vehicle_index_group_1].time_spent_in_the_loops);
    //     if(return_transit_state_group_1 == OUTPUT_LOOP_DISABLED)
    //     {
    //         calc_vehicle_index_group_1++;
    //         if (calc_vehicle_index_group_1 >= 11)
    //         {
    //             calc_vehicle_index_group_1 = 0;
    //         }
    //     }
    // }

    // if (group_loop[GROUP_2].state_group_loop == GROUP_ACTIVE)
    // {
    //    // transit_state_group_loop_1(calc_group_loop[GROUP_1].time_between_loops, calc_group_loop[GROUP_1].time_in_loop, traffic_loop[GROUP_1].gap_traffic, calc_group_loop[GROUP_1].time_spent_in_the_bonds);
    // }
}

void value_traffic(void)
{
    group_loop.vehicle_data[VEHICLES_CLASS_2C].vehicle_length = 6;
    group_loop.vehicle_data[VEHICLES_CLASS_2C].vehicle_axles[AXLE_1] = 8;
    group_loop.vehicle_data[VEHICLES_CLASS_2C].vehicle_axles[AXLE_2] = 83;

    group_loop.vehicle_data[VEHICLES_CLASS_3C].vehicle_length = 10;
    group_loop.vehicle_data[VEHICLES_CLASS_3C].vehicle_axles[AXLE_1] = 5;
    group_loop.vehicle_data[VEHICLES_CLASS_3C].vehicle_axles[AXLE_2] = 80;
    group_loop.vehicle_data[VEHICLES_CLASS_3C].vehicle_axles[AXLE_3] = 92;

    group_loop.vehicle_data[VEHICLES_CLASS_4C].vehicle_length = 12;
    group_loop.vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_1] = 4;
    group_loop.vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_2] = 71;
    group_loop.vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_3] = 73;
    group_loop.vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_4] = 83;

    group_loop.vehicle_data[VEHICLES_CLASS_2S3].vehicle_length = 18;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_1] = 3;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_2] = 19;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_3] = 81;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_4] = 88;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_5] = 95;

    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3S3].vehicle_length = 20;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_1] = 3;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_2] = 17;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_3] = 23;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_4] = 80;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_5] = 85;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_6] = 91;

    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D3].vehicle_length = 23;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_1] = 2;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_2] = 15;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_3] = 20;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_4] = 82;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_5] = 87;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_6] = 92;

    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3C2].vehicle_length = 19;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_1] = 0;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_2] = 0;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_3] = 0;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_4] = 0;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_5] = 0;

    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2J4].vehicle_length = 19;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_1] = 3;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_2] = 18;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_3] = 44;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_4] = 62;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_5] = 88;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_6] = 94;

    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2D4].vehicle_length = 19;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_1] = 3;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_2] = 19;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_3] = 50;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_4] = 57;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_5] = 88;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_6] = 95;

    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_35D].vehicle_length = 30;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_1] = 2;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_2] = 13;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_3] = 17;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_4] = 41;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_5] = 65;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_6] = 69;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_7] = 93;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_8] = 97;

    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_length = 32;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_1] = 2;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_2] = 11;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_3] = 15;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_4] = 31;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_5] = 46;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_6] = 62;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_7] = 66;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_8] = 81;
    group_loop[GROUP_1].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_9] = 97;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2C ].vehicle_length = 6;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2C ].vehicle_axles[AXLE_1] = 8;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2C ].vehicle_axles[AXLE_2] = 83;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C ].vehicle_length = 10;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C ].vehicle_axles[AXLE_1] = 5;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C ].vehicle_axles[AXLE_2] = 80;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C ].vehicle_axles[AXLE_3] = 92;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_4C ].vehicle_length = 12;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_4C ].vehicle_axles[AXLE_1] = 4;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_4C ].vehicle_axles[AXLE_2] = 71;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_4C ].vehicle_axles[AXLE_3] = 73;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_4C ].vehicle_axles[AXLE_4] = 83;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2S3].vehicle_length = 18;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_1] = 3;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_2] = 19;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_3] = 81;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_4] = 88;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_5] = 95;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3S3].vehicle_length = 20;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_1] = 3;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_2] = 17;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_3] = 23;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_4] = 80;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_5] = 85;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_6] = 91;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D3].vehicle_length = 23;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_1] = 2;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_2] = 15;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_3] = 20;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_4] = 82;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_5] = 87;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_6] = 92;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C2].vehicle_length = 19;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_1] = 0;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_2] = 0;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_3] = 0;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_4] = 0;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_5] = 0;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2J4].vehicle_length = 19;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_1] = 3;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_2] = 18;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_3] = 44;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_4] = 62;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_5] = 88;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_6] = 94;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2D4].vehicle_length = 19;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_1] = 3;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_2] = 19;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_3] = 50;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_4] = 57;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_5] = 88;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_6] = 95;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_35D].vehicle_length = 30;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_1] = 2;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_2] = 13;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_3] = 17;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_4] = 41;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_5] = 65;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_6] = 69;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_7] = 93;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_8] = 97;

    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_length = 32;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_1] = 2;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_2] = 11;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_3] = 15;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_4] = 31;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_5] = 46;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_6] = 62;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_7] = 66;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_8] = 81;
    // group_loop[GROUP_2].vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_9] = 97;
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

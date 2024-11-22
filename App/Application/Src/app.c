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

/******************************************************************************/

volatile uint32_t app_execution_rate_1ms_timer;

/******************************************************************************/

static app_piezo_data_t         app_piezo_data  [NUMBER_OF_CARS];
static axle_t                   axles           [NUMBER_OF_CARS]   [NUMBER_OF_AXLES];
static app_loop_data_t          app_loop_data   [NUMBER_OF_GROUPS];
static app_traffic_ctrl_t       app_traffic_ctrl;
static app_piezo_ctrl_t         app_piezo_ctrl  [NUMBER_OF_GROUPS];
static traffic_loop_group_t     calc_group_loop [NUMBER_OF_GROUPS];
static vehicle_t                vehicle_data    [NUMBER_OF_VEHICLES];

uint16_t gap_traffic_in_second = 1000;
uint8_t speed_traffic = 40;

/******************************************************************************/

void traffic_calculation_app(void)
{
    static uint8_t group_index = 0;
    static vehicle_class_t vehicle_index = VEHICLES_CLASS_2C;
    static axle_types_t axle_index = 0;

    calc_group_loop[group_index].time_gap_in_ms = gap_traffic_in_second * 1000;
    calc_group_loop[group_index].speed_in_meters_per_second = ((speed_traffic * 1000) / 3.6);

    for (vehicle_index; vehicle_index <= NUMBER_OF_VEHICLES; vehicle_index++)
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

        for (axle_index; axle_index <= vehicle_data[vehicle_index].vehicle_number_axles; axle_index++)
        {
            calc_group_loop[group_index].vehicle[vehicle_index].time_trigger_for_axle[axle_index] = ((calc_group_loop[group_index].vehicle[vehicle_index].piezo_firing_window 
            * vehicle_data[vehicle_index].vehicle_axles[axle_index]) / 100) ;
        }
        axle_index = 0;
    }
}

/******************************************************************************/

static void traffic_ctrl_increment_id(void)
{
    if(app_traffic_ctrl.traffic_id < NUMBER_OF_VEHICLES)
    {
        app_traffic_ctrl.traffic_id ++;
        if(app_traffic_ctrl.traffic_id == NUMBER_OF_VEHICLES)
        {
            app_traffic_ctrl.traffic_id = 0;
        }
    }
}

/******************************************************************************/

void vehicle_update_data(void) 
{
    app_traffic_ctrl.mode = MODE_CONV;
    traffic_ctrl_increment_id();
    for(uint8_t index_group = 0; index_group < 2; index_group++)
    {
        app_loop_data [index_group].gap                  = calc_group_loop[CHANNEL_DEFAULT].time_gap_in_ms;
        app_loop_data [index_group].loop_execution_time  = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_execution_loops;
        app_loop_data [index_group].start_piezo          = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_start_piezo_ms;
        app_loop_data [index_group].time_between_loops   = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_between_loops;
        app_loop_data [index_group].state                = CHANNEL_ENABLE;
        app_piezo_data[index_group].axles = axles[index_group];
        app_piezo_data[index_group].num_axles = vehicle_data[app_traffic_ctrl.traffic_id].vehicle_number_axles;
        app_piezo_data[index_group].weight_ms = 80;
        app_piezo_data[index_group].axles[AXLE_1].delay_time = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_trigger_for_axle[AXLE_1];
        app_piezo_data[index_group].axles[AXLE_2].delay_time = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_trigger_for_axle[AXLE_2];
        app_piezo_data[index_group].axles[AXLE_3].delay_time = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_trigger_for_axle[AXLE_3];
        app_piezo_data[index_group].axles[AXLE_4].delay_time = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_trigger_for_axle[AXLE_4];
        app_piezo_data[index_group].axles[AXLE_5].delay_time = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_trigger_for_axle[AXLE_5];
        app_piezo_data[index_group].axles[AXLE_6].delay_time = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_trigger_for_axle[AXLE_6];
        app_piezo_data[index_group].axles[AXLE_7].delay_time = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_trigger_for_axle[AXLE_7];
        app_piezo_data[index_group].axles[AXLE_8].delay_time = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_trigger_for_axle[AXLE_8];
        app_piezo_data[index_group].axles[AXLE_9].delay_time = calc_group_loop[CHANNEL_DEFAULT].vehicle[app_traffic_ctrl.traffic_id].time_trigger_for_axle[AXLE_9];
        app_piezo_data[index_group].channel_state = CHANNEL_ENABLE; 

        for (int index_axles = 0; index_axles < app_piezo_data[index_group].num_axles; index_axles++) 
        {
                app_piezo_data[index_group].axles[index_axles].state = AXLE_ACTIVE;
                app_piezo_data[index_group].axles[index_axles].piezo_index = index_group /*OUTPUT: GRUPO 0 E GRUPO 1*/;
        }
    }
}

/******************************************************************************/

void app_update_1ms_state_loop(void)
{
    static uint32_t TIMER_LOOP[NUMBER_OF_GROUPS] = {0};
    uint8_t index_channel_loop = 0;
    for(index_channel_loop = 0; index_channel_loop < NUMBER_OF_GROUPS; index_channel_loop++)
    {
        if(app_loop_data[index_channel_loop].state == CHANNEL_ENABLE)
        {
            if (TIMER_LOOP[index_channel_loop] <= app_loop_data[index_channel_loop].gap)
            {
                loop_update_state(INITIAL_TRANSIT_GAP, index_channel_loop, app_traffic_ctrl.mode); 
            }
            else if (TIMER_LOOP[index_channel_loop] <= (app_loop_data[index_channel_loop].gap + app_loop_data[index_channel_loop].time_between_loops))
            {
                loop_update_state(INPUT_LOOP_ACTIVATION, index_channel_loop, app_traffic_ctrl.mode); 
                if(TIMER_LOOP[index_channel_loop] == app_loop_data[index_channel_loop].gap + app_loop_data[index_channel_loop].start_piezo)
                {   /*-------------------------- SAT MODO PESAGEM/CONVENCIONAL, TRIGGER PARA PIEZO DE ENTRADA --------------------------*/     
                    app_piezo_ctrl[index_channel_loop].state_piezo_enter = START_PIEZO;
                    app_piezo_ctrl[index_channel_loop].timer_state = RESET_TIMER;
                }
            }
            else if (TIMER_LOOP[index_channel_loop] <= (app_loop_data[index_channel_loop].gap + app_loop_data[index_channel_loop].loop_execution_time))
            {
                loop_update_state(OUTPUT_LOOP_ACTIVATION, index_channel_loop, app_traffic_ctrl.mode); 
                if(TIMER_LOOP[index_channel_loop] == (app_loop_data[index_channel_loop].gap+app_loop_data[index_channel_loop].time_between_loops + app_loop_data[index_channel_loop].start_piezo))
                {    /*-------------------------- SAT MODO PESAGEM, TRIGGER PARA PIEZO DE SAIDA --------------------------*/        
                    
                }
            }
            else if (TIMER_LOOP[index_channel_loop] <= (app_loop_data[index_channel_loop].loop_execution_time+app_loop_data[index_channel_loop].time_between_loops + app_loop_data[index_channel_loop].gap))
            {
                loop_update_state(INPUT_LOOP_DISABLED, index_channel_loop, app_traffic_ctrl.mode); 
            }
            else
            {
                TIMER_LOOP[index_channel_loop] = 0;
                app_loop_data[index_channel_loop].time_between_loops   = 0;
                app_loop_data[index_channel_loop].gap                  = 0;
                app_loop_data[index_channel_loop].loop_execution_time  = 0;
                loop_update_state(OUTPUT_LOOP_DISABLED, index_channel_loop, app_traffic_ctrl.mode);
                if(index_channel_loop == GROUP_2) 
                {
                    vehicle_update_data();
                }
            }
            TIMER_LOOP[index_channel_loop]++;
        }
    }
}

/****************************************************************************/

void app_update_1ms_state_piezo(void)
{
    static uint32_t TIMER_PIEZO[NUMBER_OF_CARS]  = {0};
    static piezo_pulse_data_t piezo_pulse_data = {0};
    for (uint8_t index_channel = 0; index_channel < NUMBER_OF_GROUPS; index_channel++)
    {
        if(app_piezo_ctrl[index_channel].timer_state == RESET_TIMER)
        {
            TIMER_PIEZO[index_channel] = 0;
            app_piezo_ctrl[index_channel].timer_state = SET_TIMER;
        }
        if(app_piezo_ctrl[index_channel].state_piezo_enter == START_PIEZO)
        {
            if(app_piezo_data[index_channel].channel_state == CHANNEL_ENABLE)
            {
                for (uint8_t index_axles = 0; index_axles < app_piezo_data[index_channel].num_axles; index_axles++) 
                {
                    if (app_piezo_data[index_channel].axles[index_axles].state == AXLE_ACTIVE && TIMER_PIEZO[index_channel] == app_piezo_data[index_channel].axles[index_axles].delay_time) 
                    {    
                        piezo_pulse_data.state = PIEZO_PULSE_INIT;
                        piezo_pulse_data.delay = app_piezo_data[index_channel].weight_ms;
                        app_piezo_data[index_channel].axles[index_axles].state = AXLE_INACTIVE;  
                        piezo_pulse_received_parameters(index_channel, app_traffic_ctrl.mode, piezo_pulse_data);
                    }  
                }
            }
           TIMER_PIEZO[index_channel]++;
        }
    }
}

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

void app_1ms_clock(void)
{
    app_update_1ms_state_loop();
    app_update_1ms_state_piezo();
}


/******************************************************************************/

void app_init(void)
{   
    traffic_data();
    vehicle_update_data();
    app_traffic_ctrl.traffic_id = VEHICLES_CLASS_2C;
    app_traffic_ctrl.state = TRAFFIC_INIT;
}

/******************************************************************************/

void app_update(void)
{
    switch (app_traffic_ctrl.state)
    {
    case TRAFFIC_INIT:
        app_traffic_ctrl.state = TRAFFIC_CALC;
        break;
    case TRAFFIC_CALC:
        traffic_calculation_app();
        app_traffic_ctrl.state = TRAFFIC_RUNNING;
        break;
    case TRAFFIC_RUNNING:

        break;
    default:
        break;
    }

}

/******************************************************************************/

void app_deinit(void)
{
}

/******************************************************************************/

void traffic_data(void)
{
    vehicle_data[VEHICLES_CLASS_2C].vehicle_length = 6;
    vehicle_data[VEHICLES_CLASS_2C].vehicle_number_axles = 2;
    vehicle_data[VEHICLES_CLASS_2C].vehicle_axles[AXLE_1] = 8;
    vehicle_data[VEHICLES_CLASS_2C].vehicle_axles[AXLE_2] = 83;

    vehicle_data[VEHICLES_CLASS_3C].vehicle_length = 10;
    vehicle_data[VEHICLES_CLASS_3C].vehicle_number_axles = 3;
    vehicle_data[VEHICLES_CLASS_3C].vehicle_axles[AXLE_1] = 5;
    vehicle_data[VEHICLES_CLASS_3C].vehicle_axles[AXLE_2] = 80;
    vehicle_data[VEHICLES_CLASS_3C].vehicle_axles[AXLE_3] = 92;

    vehicle_data[VEHICLES_CLASS_4C].vehicle_length = 12;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_number_axles = 4;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_1] = 4;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_2] = 71;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_3] = 73;
    vehicle_data[VEHICLES_CLASS_4C].vehicle_axles[AXLE_4] = 83;

    vehicle_data[VEHICLES_CLASS_2S3].vehicle_length = 18;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_number_axles = 5;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_1] = 3;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_2] = 19;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_3] = 81;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_4] = 88;
    vehicle_data[VEHICLES_CLASS_2S3].vehicle_axles[AXLE_5] = 95;

    vehicle_data[VEHICLES_CLASS_3S3].vehicle_length = 20;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_number_axles = 6;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_1] = 3;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_2] = 17;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_3] = 23;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_4] = 80;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_5] = 85;
    vehicle_data[VEHICLES_CLASS_3S3].vehicle_axles[AXLE_6] = 91;

    vehicle_data[VEHICLES_CLASS_3D3].vehicle_length = 23;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_number_axles = 6;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_1] = 2;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_2] = 15;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_3] = 20;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_4] = 82;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_5] = 87;
    vehicle_data[VEHICLES_CLASS_3D3].vehicle_axles[AXLE_6] = 92;

    vehicle_data[VEHICLES_CLASS_3C2].vehicle_length = 19;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_number_axles = 5;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_1] = 3;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_2] = 26;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_3] = 32;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_4] = 61;
    vehicle_data[VEHICLES_CLASS_3C2].vehicle_axles[AXLE_5] = 94;

    vehicle_data[VEHICLES_CLASS_2J4].vehicle_length = 19;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_number_axles = 6;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_1] = 3;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_2] = 18;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_3] = 44;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_4] = 62;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_5] = 88;
    vehicle_data[VEHICLES_CLASS_2J4].vehicle_axles[AXLE_6] = 94;

    vehicle_data[VEHICLES_CLASS_2D4].vehicle_length = 19;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_number_axles = 6;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_1] = 3;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_2] = 19;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_3] = 50;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_4] = 57;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_5] = 88;
    vehicle_data[VEHICLES_CLASS_2D4].vehicle_axles[AXLE_6] = 95;

    vehicle_data[VEHICLES_CLASS_35D].vehicle_length = 30;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_number_axles = 8;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_1] = 2;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_2] = 13;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_3] = 17;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_4] = 41;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_5] = 65;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_6] = 69;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_7] = 93;
    vehicle_data[VEHICLES_CLASS_35D].vehicle_axles[AXLE_8] = 97;

    vehicle_data[VEHICLES_CLASS_3D6].vehicle_length = 32;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_number_axles = 9;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_1] = 2;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_2] = 11;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_3] = 15;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_4] = 31;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_5] = 46;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_6] = 62;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_7] = 66;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_8] = 81;
    vehicle_data[VEHICLES_CLASS_3D6].vehicle_axles[AXLE_9] = 97;
}

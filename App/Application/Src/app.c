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

static traffic_t            traffic         [NUMBER_OF_CARS];
static axle_t               axles           [NUMBER_OF_CARS]   [NUMBER_OF_AXLES];
static app_loop_data_t      app_loop_data   [NUMBER_OF_GROUPS];
static app_loop_ctrl_t      app_loop_ctrl;
static app_piezo_ctrl_t     app_piezo_ctrl  [NUMBER_OF_GROUPS];

/******************************************************************************/

void init_axles(uint8_t index) 
{
    app_loop_ctrl.mode = MODE_CONV;
    uint16_t traffic_time_between_loops  [11] = {0};
    uint16_t traffic_gap                 [11] = {0};
    uint16_t traffic_start_piezo         [11] = {0};
    uint16_t traffic_loop_execution_time [11] = {0};
    uint16_t traffic_state               [11] = {0};

    traffic_time_between_loops      [0] = 270;
    traffic_gap                     [0] = 1000;
    traffic_start_piezo             [0] = 225;
    traffic_loop_execution_time     [0] = 1080;
    traffic_state                   [0] = CHANNEL_ENABLE;

    traffic_time_between_loops      [1] = 216;
    traffic_gap                     [1] = 1000;
    traffic_start_piezo             [1] = 180;
    traffic_loop_execution_time     [1] = 864;
    traffic_state                   [1] = CHANNEL_ENABLE;

    traffic_time_between_loops      [2] = 180;  
    traffic_gap                     [2] = 1000;
    traffic_start_piezo             [2] = 150;
    traffic_loop_execution_time     [2] = 720;
    traffic_state                   [2] = CHANNEL_ENABLE;

    traffic_time_between_loops      [3] = 135;
    traffic_gap                     [3] = 1000;  
    traffic_start_piezo             [3] = 112;
    traffic_loop_execution_time     [3] = 540;
    traffic_state                   [3] = CHANNEL_ENABLE;

    traffic_time_between_loops      [4] = 216;
    traffic_gap                     [4] = 1000;  
    traffic_start_piezo             [4] = 180;
    traffic_loop_execution_time     [4] = 864;
    traffic_state                   [4] = CHANNEL_ENABLE;

    app_loop_data[0].time_between_loops         =   traffic_time_between_loops  [index];
    app_loop_data[0].gap                        =   traffic_gap                 [index];
    app_loop_data[0].start_piezo                =   traffic_start_piezo         [index];
    app_loop_data[0].loop_execution_time        =   traffic_loop_execution_time [index];
    app_loop_data[0].state                      =   traffic_state               [index];

    app_loop_data[1].time_between_loops         =   traffic_time_between_loops  [index];
    app_loop_data[1].gap                        =   traffic_gap                 [index];
    app_loop_data[1].start_piezo                =   traffic_start_piezo         [index];
    app_loop_data[1].loop_execution_time        =   traffic_loop_execution_time [index];
    app_loop_data[1].state                      =   traffic_state               [index];

    traffic[0].axles = axles[0];
    traffic[0].num_axles = 6;
    traffic[0].weight_ms = 80;
    traffic[0].axles[AXLE_1].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*2)/100);
    traffic[0].axles[AXLE_2].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*11)/100);
    traffic[0].axles[AXLE_3].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*15)/100);
    traffic[0].axles[AXLE_4].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*31)/100);
    traffic[0].axles[AXLE_5].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*46)/100);
    traffic[0].axles[AXLE_6].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*62)/100);
    traffic[0].axles[AXLE_7].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*66)/100);
    traffic[0].axles[AXLE_8].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*81)/100);
    traffic[0].axles[AXLE_9].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*97)/100);
    traffic[0].channel_state = CHANNEL_DISABLE;

    for (int i = 0; i < traffic[0].num_axles; i++) 
    {
        traffic[0].axles[i].state = AXLE_ACTIVE;
        traffic[0].axles[i].piezo_index = GROUP_1;
    }

    traffic[1].axles = axles[1];
    traffic[1].num_axles = 9;
    traffic[1].weight_ms = 80;
    traffic[1].axles[AXLE_1].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*2)/100);
    traffic[1].axles[AXLE_2].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*11)/100);
    traffic[1].axles[AXLE_3].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*15)/100);
    traffic[1].axles[AXLE_4].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*31)/100);
    traffic[1].axles[AXLE_5].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*46)/100);
    traffic[1].axles[AXLE_6].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*62)/100);
    traffic[1].axles[AXLE_7].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*66)/100);
    traffic[1].axles[AXLE_8].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*81)/100);
    traffic[1].axles[AXLE_9].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*97)/100);

    traffic[1].channel_state = CHANNEL_ENABLE;

    for (int i = 0; i < traffic[1].num_axles; i++) 
    {
        traffic[1].axles[i].state = AXLE_ACTIVE;
        traffic[1].axles[i].piezo_index = GROUP_2;
    }
}

/******************************************************************************/

void app_update_1ms_state_loop(void)
{

    static uint32_t TIMER_LOOP[NUMBER_OF_GROUPS] = {0};
    static uint8_t  state_piezo[NUMBER_OF_GROUPS] = {0};
    static uint8_t  index[NUMBER_OF_GROUPS] = {0};
    

    uint8_t index_channel_loop = 0;
    for(index_channel_loop = 0; index_channel_loop < NUMBER_OF_GROUPS; index_channel_loop++)
    {
        if(app_loop_data[index_channel_loop].state == CHANNEL_ENABLE)
        {
            if (TIMER_LOOP[index_channel_loop] <= app_loop_data[index_channel_loop].gap)
            {
                loop_update_state(INITIAL_TRANSIT_GAP, index_channel_loop, app_loop_ctrl.mode); 
            }
            else if (TIMER_LOOP[index_channel_loop] <= (app_loop_data[index_channel_loop].gap + app_loop_data[index_channel_loop].time_between_loops))
            {
                loop_update_state(INPUT_LOOP_ACTIVATION, index_channel_loop, app_loop_ctrl.mode); 
                if(TIMER_LOOP[index_channel_loop] == app_loop_data[index_channel_loop].gap + app_loop_data[index_channel_loop].start_piezo)
                {   /*-------------------------- SAT MODO PESAGEM/CONVENCIONAL, TRIGGER PARA PIEZO DE ENTRADA --------------------------*/     
                    
                    app_piezo_ctrl[index_channel_loop].stape_piezo_enter = START_PIEZO;
                    traffic     [index_channel_loop].channel_state = CHANNEL_ENABLE;
                    // if(app_loop_ctrl.mode == MODE_PE)
                    // {
                    //     //TIMER_PIEZO [GROUP_1] = 0; 
                    //     state_piezo [GROUP_1] = START_PIEZO;
                    //     traffic     [GROUP_1].channel_state = CHANNEL_ENABLE;
                    // }
                    // else if(app_loop_ctrl.mode == MODE_CONV)
                    // {
                    //     //TIMER_PIEZO [index_channel_loop] = 0; 
                    //     state_piezo [index_channel_loop] = START_PIEZO;
                    //     traffic     [index_channel_loop].channel_state = CHANNEL_ENABLE;
                    // }
                }
            }
            else if (TIMER_LOOP[index_channel_loop] <= (app_loop_data[index_channel_loop].gap + app_loop_data[index_channel_loop].loop_execution_time))
            {
                loop_update_state(OUTPUT_LOOP_ACTIVATION, index_channel_loop, app_loop_ctrl.mode); 
                if(TIMER_LOOP[index_channel_loop] == (app_loop_data[index_channel_loop].gap 
                +app_loop_data[index_channel_loop].time_between_loops + app_loop_data[index_channel_loop].start_piezo))
                {    /*-------------------------- SAT MODO PESAGEM, TRIGGER PARA PIEZO DE SAIDA --------------------------*/        
                    
                    //app_piezo_ctrl[index_channel_loop].stape_piezo_exit = START_PIEZO;
                    // if(app_loop_ctrl.mode == MODE_PE)
                    // {
                    //     //TIMER_PIEZO [GROUP_2] = 0; 
                    //     state_piezo [GROUP_2] = START_PIEZO;
                    //     traffic     [GROUP_2].channel_state = CHANNEL_ENABLE;
                    // }
                }
            }
            else if (TIMER_LOOP[index_channel_loop] <= (app_loop_data[index_channel_loop].loop_execution_time+app_loop_data[index_channel_loop].time_between_loops + app_loop_data[index_channel_loop].gap))
            {
                loop_update_state(INPUT_LOOP_DISABLED, index_channel_loop, app_loop_ctrl.mode); 
            }
            else
            {
                TIMER_LOOP[index_channel_loop]                         = 0;
                app_loop_data[index_channel_loop].time_between_loops   = 0;
                app_loop_data[index_channel_loop].gap                  = 0;
                app_loop_data[index_channel_loop].loop_execution_time  = 0;
                loop_update_state(OUTPUT_LOOP_DISABLED, index_channel_loop, app_loop_ctrl.mode);
                if(index_channel_loop == GROUP_2) 
                {
                    if(index[index_channel_loop] > 4)
                    {
                        index[index_channel_loop] = 0;
                    }
                    init_axles(index[index_channel_loop]);
                    index[index_channel_loop]++;
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
        if(app_piezo_ctrl[index_channel].stape_piezo_enter == START_PIEZO)
        {
            if(traffic[index_channel].channel_state == CHANNEL_ENABLE)
            {
                for (uint8_t index_axles = 0; index_axles < traffic[index_channel].num_axles; index_axles++) 
                {
                    if (traffic[index_channel].axles[index_axles].state == AXLE_ACTIVE && TIMER_PIEZO[index_channel] == traffic[index_channel].axles[index_axles].delay_time) 
                    {    
                        piezo_pulse_data.state = PIEZO_PULSE_INIT;
                        piezo_pulse_data.delay = traffic[index_channel].weight_ms;
                        traffic[index_channel].axles[index_axles].state = AXLE_INACTIVE;  
                        piezo_pulse_received_parameters(index_channel, app_loop_ctrl.mode, piezo_pulse_data);
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
    init_axles(0);
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

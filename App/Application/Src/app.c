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

#define transito_teste                             \
{                                                  \
    {                                              \ 
        .time_between_loops  =   270,              \
        .gap                 =   1000,             \
        .start_piezo         =   225,              \
        .loop_execution_time =   1080,             \
        .state               =   CHANNEL_ENABLE    \
    }                                              \                                    
    ,                                              \ 
    {                                              \ 
        .time_between_loops  =   216,              \
        .gap                 =   1000,             \
        .start_piezo         =   180,              \  
        .loop_execution_time =   1864,             \
        .state               =   CHANNEL_ENABLE    \
    }                                              \                                                                                              
}   

volatile uint32_t app_execution_rate_1ms_timer;

static app_piezo_data_t                app_piezo_data  [NUMBER_OF_CARS];
static axle_t                   axles           [NUMBER_OF_CARS]   [NUMBER_OF_AXLES];
static app_loop_data_t          app_loop_data   [NUMBER_OF_GROUPS];
static app_traffic_ctrl_t       app_traffic_ctrl;
static app_piezo_ctrl_t         app_piezo_ctrl  [NUMBER_OF_GROUPS];

static app_loop_data_t app_piezo_data_vector[2] = transito_teste;

/******************************************************************************/

void init_axles() 
{
    app_traffic_ctrl.mode = MODE_CONV;

    static uint8_t index = 0;

    if(index >= 3)
    {
        index = 0;
    }
    else 
    {
        index++;
    }

    app_loop_data[0]=  app_piezo_data_vector[0];
    app_loop_data[1] = app_piezo_data_vector[0];

    app_piezo_data[0].axles = axles[0];
    app_piezo_data[0].num_axles = 6;
    app_piezo_data[0].weight_ms = 80;
    app_piezo_data[0].axles[AXLE_1].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*2)/100);
    app_piezo_data[0].axles[AXLE_2].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*11)/100);
    app_piezo_data[0].axles[AXLE_3].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*15)/100);
    app_piezo_data[0].axles[AXLE_4].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*31)/100);
    app_piezo_data[0].axles[AXLE_5].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*46)/100);
    app_piezo_data[0].axles[AXLE_6].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*62)/100);
    app_piezo_data[0].axles[AXLE_7].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*66)/100);
    app_piezo_data[0].axles[AXLE_8].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*81)/100);
    app_piezo_data[0].axles[AXLE_9].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*97)/100);
    app_piezo_data[0].channel_state = CHANNEL_ENABLE;

    for (int i = 0; i < app_piezo_data[0].num_axles; i++) 
    {
        app_piezo_data[0].axles[i].state = AXLE_ACTIVE;
        app_piezo_data[0].axles[i].piezo_index = GROUP_1;
    }

    app_piezo_data[1].axles = axles[1];
    app_piezo_data[1].num_axles = 9;
    app_piezo_data[1].weight_ms = 80;
    app_piezo_data[1].axles[AXLE_1].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*2)/100);
    app_piezo_data[1].axles[AXLE_2].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*11)/100);
    app_piezo_data[1].axles[AXLE_3].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*15)/100);
    app_piezo_data[1].axles[AXLE_4].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*31)/100);
    app_piezo_data[1].axles[AXLE_5].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*46)/100);
    app_piezo_data[1].axles[AXLE_6].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*62)/100);
    app_piezo_data[1].axles[AXLE_7].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*66)/100);
    app_piezo_data[1].axles[AXLE_8].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*81)/100);
    app_piezo_data[1].axles[AXLE_9].delay_time = (((app_loop_data[0].loop_execution_time-(app_loop_data[0].start_piezo*2))*97)/100);

    app_piezo_data[1].channel_state = CHANNEL_ENABLE;

    for (int i = 0; i < app_piezo_data[1].num_axles; i++) 
    {
        app_piezo_data[1].axles[i].state = AXLE_ACTIVE;
        app_piezo_data[1].axles[i].piezo_index = GROUP_2;
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
                    init_axles();
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
    init_axles();
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

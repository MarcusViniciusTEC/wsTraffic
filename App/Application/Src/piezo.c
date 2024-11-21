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

static traffic_t            traffic         [NUMBER_OF_CARS];
static axle_t               axles           [NUMBER_OF_CARS]   [NUMBER_OF_AXLES];
static app_loop_data_t      app_loop_data   [NUMBER_OF_GROUPS];
static app_loop_ctrl_t      app_loop_ctrl;
static piezo_pulse_data_t   piezo_pulse_data[NUMBER_OF_GROUPS] = {0};

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

static void piezo_pulse_update_state (uint8_t index)
{
    const uint8_t LEDS_PIEZO_CONV  [NUMBER_OF_GROUPS] = {LED_PIEZO_AXILE_CONV_CH1, LED_PIEZO_AXILE_CONV_CH2};
    const uint8_t LEDS_PIEZO_PE    [NUMBER_OF_GROUPS] = {LED_PIEZO_AXILE_PE_CH1  ,   LED_PIEZO_AXILE_PE_CH2};
    switch (piezo_pulse_data[index].state)
    {
    case PIEZO_PULSE_INIT:
        piezo_pulse_data[index].state = PIEZO_PULSE_TURN_ON;
        break;
    case PIEZO_PULSE_TURN_ON:
        if(app_loop_ctrl.mode == MODE_CONV)
        {
            hmi_led_turn_on(LEDS_PIEZO_CONV[index]);
        }
        else if(app_loop_ctrl.mode == MODE_PE)
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
        if(app_loop_ctrl.mode == MODE_CONV)
        {
            hmi_led_turn_off(LEDS_PIEZO_CONV[index]);
        }
        else if(app_loop_ctrl.mode == MODE_PE)
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


void init_axles(uint8_t index) 
{
    app_loop_ctrl.mode = MODE_CONV;
    static uint16_t traffic_time_between_loops  [11] = {0};
    static uint16_t traffic_gap                 [11] = {0};
    static uint16_t traffic_start_piezo         [11] = {0};
    static uint16_t traffic_loop_execution_time [11] = {0};
    static uint16_t traffic_state               [11] = {0};

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
    traffic[0].num_axles = 9;
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

void piezo_update_state(void)
{
    static uint32_t TIMER_PIEZO[NUMBER_OF_CARS]  = {0};
    static uint32_t TIMER_LOOP[NUMBER_OF_GROUPS] = {0};
    static uint8_t  state_piezo[NUMBER_OF_GROUPS] = {0};
    uint8_t index_channel_loop = 0;
    static uint8_t index[2] = {0};
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
                    if(app_loop_ctrl.mode == MODE_PE)
                    {
                        TIMER_PIEZO [GROUP_1] = 0; 
                        state_piezo [GROUP_1] = START_PIEZO;
                        traffic     [GROUP_1].channel_state = CHANNEL_ENABLE;
                    }
                    else if(app_loop_ctrl.mode == MODE_CONV)
                    {
                        TIMER_PIEZO [index_channel_loop] = 0; 
                        state_piezo [index_channel_loop] = START_PIEZO;
                        traffic     [index_channel_loop].channel_state = CHANNEL_ENABLE;
                    }
                }
            }
            else if (TIMER_LOOP[index_channel_loop] <= (app_loop_data[index_channel_loop].gap + app_loop_data[index_channel_loop].loop_execution_time))
            {
                loop_update_state(OUTPUT_LOOP_ACTIVATION, index_channel_loop, app_loop_ctrl.mode); 
                if(TIMER_LOOP[index_channel_loop] == (app_loop_data[index_channel_loop].gap 
                +app_loop_data[index_channel_loop].time_between_loops + app_loop_data[index_channel_loop].start_piezo))
                {    /*-------------------------- SAT MODO PESAGEM, TRIGGER PARA PIEZO DE SAIDA --------------------------*/        
                    if(app_loop_ctrl.mode == MODE_PE)
                    {
                        TIMER_PIEZO [GROUP_2] = 0; 
                        state_piezo [GROUP_2] = START_PIEZO;
                        traffic     [GROUP_2].channel_state = CHANNEL_ENABLE;
                    }
                    else if (app_loop_ctrl.mode == MODE_CONV)
                    {
                    }
                }
            }
            else if (TIMER_LOOP[index_channel_loop] <= (app_loop_data[index_channel_loop].loop_execution_time 
            +app_loop_data[index_channel_loop].time_between_loops + app_loop_data[index_channel_loop].gap))
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
                    if(index[index_channel_loop] > 3)
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
    for (uint8_t index_channel = 0; index_channel < 2; index_channel++)
    {
        if(state_piezo[index_channel] == START_PIEZO)
        {
            if(traffic[index_channel].channel_state == CHANNEL_ENABLE)
            {
                for (uint8_t index_axles = 0; index_axles < traffic[index_channel].num_axles; index_axles++) 
                {
                    if (traffic[index_channel].axles[index_axles].state == AXLE_ACTIVE && TIMER_PIEZO[index_channel] == traffic[index_channel].axles[index_axles].delay_time) 
                    {    
                        piezo_pulse_data[index_channel].state = PIEZO_PULSE_INIT;
                        piezo_pulse_data[index_channel].delay = traffic[index_channel].weight_ms;
                        traffic[index_channel].axles[index_axles].state = AXLE_INACTIVE;  
                    }  
                }
            }
           TIMER_PIEZO[index_channel]++;
        } 
    }
}

/******************************************************************************/

void loop_update_state(loop_state_update_t state, uint8_t index, traffic_mode_t mode)
{
    const uint8_t LED_ENTER_LOOP[UPDATE_NUMBER_OF_LOOPS] = {LED_LOOP_ENTER_GROUP_1, LED_LOOP_ENTER_GROUP_2};
    const uint8_t LED_EXIT_LOOP[UPDATE_NUMBER_OF_LEDS] =  {LED_LOOP_EXIT_GROUP_1, LED_LOOP_EXIT_GROUP_2};
    const uint8_t ENTER_LOOP_PIN[UPDATE_NUMBER_OF_LOOPS] = {LOOP_ENTER_GROUP_1, LOOP_ENTER_GROUP_2};
    const uint8_t EXIT_LOOP_PIN[UPDATE_NUMBER_OF_LOOPS] = {LOOP_EXIT_GROUP_1, LOOP_EXIT_GROUP_2};

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

void piezo_1us_clock(void)
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
    piezo_update_state();
}

/******************************************************************************/

void piezo_init(void)
{
    init_axles(0);
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
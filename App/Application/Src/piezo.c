#include "sl.h"
#include "piezo.h"
#include "piezo_cfg.h"
#include "hmi.h"
#include "stdbool.h"

/******************************************************************************/

volatile uint32_t piezo_execution_rate_1ms_timer;

/******************************************************************************/

static const piezo_pininfo_t piezo_pininfo_vector[PIEZO_NUMBER_OF_OUTPUTS] = piezo_pininfo_vector_default_value;

static traffic_t    traffic  [NUMBER_OF_CARS];
static axle_t       axles    [NUMBER_OF_CARS]  [NUMBER_OF_AXLES];
static app_loop_data_t  app_loop_data[NUMBER_OF_GROUPS];
static app_loop_ctrl_t  app_loop_ctrl;

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

static void piezo_pulse (uint8_t index, uint32_t time_us)
{
    hmi_led_turn_on(index);
    for(uint32_t i = 0 ; i < time_us; i++);
    hmi_led_turn_off(index);
}

/******************************************************************************/




void init_axles(void) 
{

    traffic[0].axles = axles[0];
    traffic[0].num_axles = 9;
    traffic[0].weight_ms = 2000;
    traffic[0].axles[AXLE_1].delay_time = 1;
    traffic[0].axles[AXLE_2].delay_time = 10;
    traffic[0].axles[AXLE_3].delay_time = 15;
    traffic[0].axles[AXLE_4].delay_time = 20;
    traffic[0].axles[AXLE_5].delay_time = 21;
    traffic[0].axles[AXLE_6].delay_time = 22;
    traffic[0].axles[AXLE_7].delay_time = 25;
    traffic[0].axles[AXLE_8].delay_time = 28;
    traffic[0].axles[AXLE_9].delay_time = 30;
    traffic[0].vehicle_state = VEHICLE_ACTIVE;

    for (int i = 0; i < traffic[0].num_axles; i++) 
    {
        traffic[0].axles[i].state = AXLE_ACTIVE;
        traffic[0].axles[i].piezo_index = 1;
    
    }

    traffic[1].axles = axles[1];
    traffic[1].num_axles = 3;
    traffic[1].weight_ms = 2000;
    traffic[1].axles[AXLE_1].delay_time = 1;
    traffic[1].axles[AXLE_2].delay_time = 10;
    traffic[1].axles[AXLE_3].delay_time = 15;
    traffic[1].vehicle_state = VEHICLE_ACTIVE;

    for (int i = 0; i < traffic[1].num_axles; i++) 
    {
        traffic[1].axles[i].state = AXLE_ACTIVE;
        traffic[1].axles[i].piezo_index = 4;
    }
}

/******************************************************************************/


    uint16_t time_between_loop[2];
    uint16_t gap[2];
    uint16_t loop_execution_time[2];
    uint8_t  index;

void piezo_update_state(void)
{
    static uint32_t TIMER_PIEZO[NUMBER_OF_CARS]  = {0};
    static uint32_t TIMER_LOOP[2] = {0};
    uint8_t index_vehicle;
    for(index_vehicle = 0; index_vehicle < NUMBER_OF_CARS; index_vehicle ++)
    {
        if(traffic[index_vehicle].vehicle_state == VEHICLE_ACTIVE)
        {
            for (uint8_t index_axles = 0; index_axles < traffic[index_vehicle].num_axles; index_axles++) 
            {
                if (traffic[index_vehicle].axles[index_axles].state == AXLE_ACTIVE && TIMER_PIEZO[index_vehicle] == traffic[index_vehicle].axles[index_axles].delay_time) 
                {                 
                    piezo_pulse(traffic[index_vehicle].axles[index_axles].piezo_index, traffic[index_vehicle].weight_ms);
                    traffic[index_vehicle].axles[index_axles].state = AXLE_INACTIVE;  
                }
            }
        }
        for (uint8_t index_state = 0; index_state < traffic[index_vehicle].num_axles; index_state++) 
        {
            if (traffic[index_vehicle].axles[index_state].state == AXLE_INACTIVE) 
            {

            }
        }
        TIMER_PIEZO[index_vehicle]++;
    }
    TIMER_LOOP[app_loop_ctrl.index]++;
    if (TIMER_LOOP[index] <= app_loop_data[index].gap)
    {
        loop_update_state(INITIAL_TRANSIT_GAP, app_loop_ctrl.index); 
    }
    else if (TIMER_LOOP[app_loop_ctrl.index] <= (app_loop_data[app_loop_ctrl.index].gap + app_loop_data[app_loop_ctrl.index].time_between_loops))
    {
        loop_update_state(INPUT_LOOP_ACTIVATION, app_loop_ctrl.index); 
    }
    else if (TIMER_LOOP[app_loop_ctrl.index] <= (app_loop_data[index].gap + app_loop_data[app_loop_ctrl.index].loop_execution_time))
    {
        loop_update_state(OUTPUT_LOOP_ACTIVATION, app_loop_ctrl.index); 
    }
    else if (TIMER_LOOP[app_loop_ctrl.index] <= (app_loop_data[app_loop_ctrl.index].gap+ app_loop_data[app_loop_ctrl.index].loop_execution_time + app_loop_data[app_loop_ctrl.index].time_between_loops))
    {
        loop_update_state(INPUT_LOOP_DISABLED, app_loop_ctrl.index); 
    }
    else
    {
        TIMER_LOOP[app_loop_ctrl.index]                         = 0;
        app_loop_data[app_loop_ctrl.index].time_between_loops   = 0;
        app_loop_data[app_loop_ctrl.index].gap                  = 0;
        app_loop_data[app_loop_ctrl.index].loop_execution_time  = 0;
        loop_update_state(OUTPUT_LOOP_DISABLED, app_loop_ctrl.index); 
    }
}

/******************************************************************************/

void loop_update_state(loop_state_update_t state, uint8_t index)
{
    const uint8_t LED_ENTER_LOOP     [UPDATE_NUMBER_OF_LOOPS] = {LED_LOOP_ENTER_GROUP_1, LED_LOOP_ENTER_GROUP_2};
    const uint8_t LED_EXIT_LOOP      [UPDATE_NUMBER_OF_LEDS ] = {LED_LOOP_EXIT_GROUP_1,   LED_LOOP_EXIT_GROUP_2};
    const uint8_t ENTER_LOOP_PIN     [UPDATE_NUMBER_OF_LOOPS] = {LOOP_ENTER_GROUP_1,         LOOP_ENTER_GROUP_2};
    const uint8_t EXIT_LOOP_PIN      [UPDATE_NUMBER_OF_LOOPS] = {LOOP_EXIT_GROUP_1,           LOOP_EXIT_GROUP_2};
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

void piezo_1ms_clock(void)
{
    piezo_update_state();
}


/******************************************************************************/

void piezo_init(void)
{
    init_axles();
}                  

/******************************************************************************/

void piezo_update(void)
{

}                


void piezo_deinit(void)
{

}
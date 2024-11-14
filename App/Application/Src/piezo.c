#include "sl.h"
#include "piezo.h"
#include "piezo_cfg.h"
#include "hmi.h"
#include "stdbool.h"

/******************************************************************************/

volatile uint32_t piezo_execution_rate_1ms_timer;

static const piezo_pininfo_t piezo_pininfo_vector[PIEZO_NUMBER_OF_OUTPUTS] = piezo_pininfo_vector_default_value;

static traffic_t    traffic [NUMBER_OF_CARS];
static axle_t       axles   [NUMBER_OF_AXLES];


/******************************************************************************/

static uint32_t ticks_us = 0;
static uint32_t delay_led = 10;

/******************************************************************************/






/******************************************************************************/

/******************************************************************************/



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

static void piezo_pulse(piezo_index)
{
   // hmi_led_turn_on (piezo_index);
   // hmi_led_turn_off(piezo_index);
}

/******************************************************************************/

uint8_t init_decrement = 0;

void app_1us_clock() /*10uS*/
{
    ticks_us++;
}


uint32_t get_tick_us()
{
    return ticks_us;
}


void traffic_delay(uint32_t time_us)
{
	uint32_t i = get_tick_us();
	while((get_tick_us() - i) < time_us) {}
}


/******************************************************************************/

static void piezo_led_pulse (uint8_t led_index, uint32_t time_us)
{
    hmi_led_turn_on(led_index);
    for(uint32_t i = 0 ; i < time_us; i++);
    hmi_led_turn_off(led_index);
}

/******************************************************************************/

void init_axles(void) 
{
    traffic[0].axles = axles;
    traffic[0].num_axles = 3;
    traffic[0].weight_ms = 2000;
    traffic[0].axles[AXLE_1].delay_time = 1;
    traffic[0].axles[AXLE_2].delay_time = 10;
    traffic[0].axles[AXLE_3].delay_time = 15;

    traffic[0].vehicle_state = VEHICLE_ACTIVE;

    for (int i = 0; i < traffic[0].num_axles; i++) 
    {
        traffic[0].axles[i].state = AXLE_ACTIVE;
    }
}

/******************************************************************************/

void piezo_update_state(void)
{
    static uint32_t TIMER = 0;
    for(uint8_t index_vehicle = 0; index_vehicle <= NUMBER_OF_CARS; index_vehicle ++)
    {
        if(traffic[index_vehicle].vehicle_state == VEHICLE_ACTIVE)
        {
            for (uint8_t index_axles = 0; index_axles < traffic[index_vehicle].num_axles; index_axles++) 
            {
                if (traffic[index_vehicle].axles[index_axles].state == AXLE_ACTIVE && TIMER == traffic[index_vehicle].axles[index_axles].delay_time) 
                {                 
                    piezo_led_pulse(traffic[index_vehicle].axles[index_vehicle].piezo_index, traffic[index_vehicle].weight_ms);
                    traffic[index_vehicle].axles[index_vehicle].state = AXLE_INACTIVE;  
                }
            }
        }
        for (uint8_t index_state = 0; index_state < traffic[index_vehicle].num_axles; index_state++) 
        {
            if (traffic[index_vehicle].axles[index_state].state == AXLE_INACTIVE) 
            {

            }
        }

    }
    TIMER++;
}

/******************************************************************************/

                    // hmi_led_turn_on    (traffic[index_vehicle].axles[index_axles].piezo_index);
                    // hmi_led_turn_off (traffic[index_vehicle].axles[index_axles].piezo_index);

void piezo_1ms_clock(void)
{
    piezo_update_state();
    //tick_1ms();
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
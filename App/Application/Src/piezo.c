#include "sl.h"
#include "piezo.h"
#include "piezo_cfg.h"
#include "hmi.h"
#include "stdbool.h"

/******************************************************************************/

volatile uint32_t piezo_execution_rate_1ms_timer;

/******************************************************************************/

static const piezo_pininfo_t piezo_pininfo_vector[PIEZO_NUMBER_OF_OUTPUTS] = piezo_pininfo_vector_default_value;

static traffic_t    traffic [NUMBER_OF_CARS];
static axle_t       axles[NUMBER_OF_CARS]  [NUMBER_OF_AXLES];


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
    piezo_turn_on(index);
    for(uint32_t i = 0 ; i < time_us; i++);
    piezo_turn_off(index);
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
    traffic[0].axles[AXLE_5].delay_time = 25;
    traffic[0].axles[AXLE_6].delay_time = 40;
    traffic[0].axles[AXLE_7].delay_time = 42;
    traffic[0].axles[AXLE_8].delay_time = 46;
    traffic[0].axles[AXLE_9].delay_time = 50;

    traffic[0].vehicle_state = VEHICLE_ACTIVE;

    for (int i = 0; i < traffic[0].num_axles; i++) 
    {
        traffic[0].axles[i].state = AXLE_ACTIVE;
        traffic[0].axles[i].piezo_index = 1;
    }


    traffic[1].axles = axles[1];
    
    traffic[1].num_axles = 2;
    traffic[1].weight_ms = 2000;
    traffic[1].axles[AXLE_1].delay_time = 1;
    traffic[1].axles[AXLE_2].delay_time = 10;
    traffic[1].axles[AXLE_3].delay_time = 15;

    traffic[1].vehicle_state = VEHICLE_ACTIVE;

    for (int i = 0; i < traffic[1].num_axles; i++) 
    {
        traffic[1].axles[i].state = AXLE_ACTIVE;
        traffic[1].axles[i].piezo_index = 3;
    }
}

/******************************************************************************/

void piezo_update_state(void)
{
    static uint32_t TIMER  = 0;
    uint8_t index_vehicle;
    uint8_t index_axles;
    for(index_vehicle = 0; index_vehicle < NUMBER_OF_CARS; index_vehicle ++)
    {
        if(traffic[index_vehicle].vehicle_state == VEHICLE_ACTIVE)
        {
            for (index_axles = 0; index_axles < traffic[index_vehicle].num_axles; index_axles++) 
            {
                if (traffic[index_vehicle].axles[index_axles].state == AXLE_ACTIVE && TIMER == traffic[index_vehicle].axles[index_axles].delay_time) 
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
        
    }
    TIMER++;
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
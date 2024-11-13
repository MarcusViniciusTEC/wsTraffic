#include "sl.h"
#include "app.h"
#include "app_cfg.h"

/******************************************************************************/

#include "hmi.h"
#include "loop.h"
#include "piezo.h"
//#include "wlog.h"


/******************************************************************************/

//#include "gpio.h"

/******************************************************************************/

volatile uint32_t app_execution_rate_1ms_timer;

/******************************************************************************/

static uint32_t ticks = 0;
static piezo_pulse_t piezo_pulse;

static traffic_axles_t traffic_axles;


void app_1us_clock() /*10uS*/
{
    
    
}

/******************************************************************************/



/******************************************************************************/



void app_1ms_clock(void)
{
    uint8_t lane_index = 0;

    static uint8_t flag = 0;

    if(flag == 0)
    {
        traffic_axles.lane[lane_index].next_axle = 0;
        flag = 1;
    }

    static uint32_t counter1 = 0;
    static uint32_t counter2 = 0;
    uint32_t counter3 = 0;
    uint32_t counter4 = 0;

    counter1++;
    counter2++;
    switch (traffic_axles.lane[lane_index].next_axle)
    {
    case AXLE_1:
        
        if(counter1 == 10)
        {   
            //traffic_axles.lane[lane_index].next_axle = AXLE_2;
            hmi_led_turn_on(0);
            hmi_led_turn_off(0);
            
            //counter1 = 0;
        }  

        
        else if(counter1 == 60)
        {
            hmi_led_turn_on(0);
            hmi_led_turn_off(0);
            //traffic_axles.lane[lane_index].next_axle = 2;
            //counter2 = 0;
        }  

        //counter3++;           
        else if(counter1 == 80)
        {
            hmi_led_turn_on(0);
            hmi_led_turn_off(0);
            //traffic_axles.lane[lane_index].next_axle = 6;
            counter1 = 0;
            return 0;
        }
        else
        {
            
        } 

        

        break;
    case AXLE_2:


        break;
    case AXLE_3:

        break;
    case AXLE_4:


        //traffic_axles.lane[lane_index].next_axle = AXLE_5;
        break; 
    case AXLE_5:

        traffic_axles.lane[lane_index].next_axle = AXLE_6;
        break;
    case AXLE_6:

        traffic_axles.lane[lane_index].next_axle = AXLE_7;
        break;
    case AXLE_7:

        traffic_axles.lane[lane_index].next_axle = AXLE_8;
        break;
    case AXLE_8:

        traffic_axles.lane[lane_index].next_axle = AXLE_9;
        break;
    case AXLE_9:

        traffic_axles.lane[lane_index].next_axle = AXLE_1;
        break;
    default:
        break;
    }

    // //if(piezo_pulse.piezo_pin[0].time > 0 )
    // {
    //     piezo_pulse.piezo_pin[0].time --;
    // }
}

/******************************************************************************/


void piezo_pulse_update_state(uint8_t piezo_index, uint32_t time_turn_on)
{   
    switch (piezo_pulse.piezo_pin[piezo_index].state)
    {
    case PIEZO_TURN_ON:
        hmi_led_turn_on(piezo_index);
        piezo_pulse.piezo_pin[piezo_index].state = PIEZO_TURN_OFF;
        break;
    case PIEZO_TURN_OFF:
        hmi_led_turn_off(piezo_index);
        piezo_pulse.piezo_pin[piezo_index].state = PIEZO_TURN_ON ;
        return 0;
        break;
    default:
        break;
    }
}

void piezo_update_state(lane_t lane_index, traffic_axles_t traffic_axles)
{

}

void app_set_address_and_mode (uint8_t addres, uint8_t mode)
{
    
}

/******************************************************************************/

void app_read_address_and_mode(void)
{
    uint8_t address =0; 
    uint8_t mode = 0;
    if(LL_GPIO_IsInputPinSet(APP_ID_MODE_GPIO_PORT, APP_ID_MODE_GPIO_PIN) == MODE_PE)
    {
       mode = MODE_PE;
    }
    else 
    {
        mode = MODE_CONV;
    }
    if(LL_GPIO_IsInputPinSet(APP_ID_BIT1_GPIO_PORT, APP_ID_BIT1_GPIO_PIN) == KEY_OFF)
    {
        address |= (1 << 0);
    }
    if(LL_GPIO_IsInputPinSet(APP_ID_BIT2_GPIO_PORT, APP_ID_BIT2_GPIO_PIN) == KEY_OFF)
    {
        address |= (1 << 1);
    }
    if(LL_GPIO_IsInputPinSet(APP_ID_BIT3_GPIO_PORT, APP_ID_BIT3_GPIO_PIN) == KEY_OFF)
    {
        address |= (1 << 2);
    }
    app_set_address_and_mode(address, mode);
}

/******************************************************************************/


/******************************************************************************/

void app_init(void)
{
    piezo_pulse.piezo_pin[3].state = 0;
         
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


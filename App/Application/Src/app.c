#include "sl.h"
#include "app.h"
#include "app_cfg.h"

/******************************************************************************/

#include "hmi.h"
#include "loop.h"
#include "piezo.h"
// #include "wlog.h"

/******************************************************************************/

// #include "gpio.h"

/******************************************************************************/

volatile uint32_t app_execution_rate_1ms_timer;

uint32_t input_loop_gap = 0;        //ms
uint32_t output_loop_gap = 0;
uint32_t entry_loop_stay_time = 0;
uint32_t exit_loop_stay_time = 0;

uint16_t speed_traffic = 0;        //Km/h
uint8_t vehicle_length = 0;
uint16_t gap_traffic = 0;

/******************************************************************************/

#include <stdint.h>

#define DELAY_BETWEEN_CARS 1000  // 1000ms de espera entre carros


typedef struct {
    uint32_t delay_time;  
    uint8_t led_index;    
    uint8_t active;       
} axle_t;


typedef struct {
    axle_t* axles;       
    uint8_t num_axles;    
} traffic_t;


axle_t car1_axles[9];   

traffic_t traffic[1];          

uint32_t car_delay_timer = 0;  
uint8_t current_car = 0;       


void init_axles(void) {
    traffic[0].axles = car1_axles;
    traffic[0].num_axles = 9;
    traffic[0].axles[0].delay_time = 1;
    traffic[0].axles[1].delay_time = 5;
    traffic[0].axles[2].delay_time = 6;
    traffic[0].axles[3].delay_time = 10;
    traffic[0].axles[4].delay_time = 11;
    traffic[0].axles[5].delay_time = 15;
    traffic[0].axles[6].delay_time = 16;
    traffic[0].axles[7].delay_time = 21;
    traffic[0].axles[8].delay_time = 22;

    for (int i = 0; i < traffic[current_car].num_axles; i++) 
    {
        traffic[current_car].axles[i].active = 1;
    }
}


void app_1ms_clock(void) {
    static uint32_t timer = 0;

    if (car_delay_timer > 0) 
    {
        car_delay_timer--;
        return;  
    }

    for (int i = 0; i < traffic[current_car].num_axles; i++) 
    {
        if (traffic[current_car].axles[i].active && timer >= traffic[current_car].axles[i].delay_time) 
        {
            hmi_led_turn_on(traffic[current_car].axles[i].led_index);
            hmi_led_turn_off(traffic[current_car].axles[i].led_index);
            traffic[current_car].axles[i].active = 0;  
        }
    }
    uint8_t all_axles_done = 1;
    for (int i = 0; i < traffic[current_car].num_axles; i++) 
    {
        if (traffic[current_car].axles[i].active) 
        {
            all_axles_done = 0;
            break;
        }
    }
    if (all_axles_done) 
    {
        car_delay_timer = DELAY_BETWEEN_CARS;  
        current_car++;
        if (current_car >= 1) 
        {
            current_car = 0;  
        }
        init_axles();  
    }
    timer++;
}


void app_1us_clock() /*10uS*/
{



}

void update_state_loop(void)
{

}
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

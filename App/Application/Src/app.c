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





         
uint32_t car_delay_timer = 0;  
uint8_t current_car = 0; 
traffic_t traffic[1];
axle_t axles[9];         

void init_axles(void) 
{
    
     

    traffic[0].axles = axles;
    traffic[0].num_axles = 3;
    traffic[0].axles[0].delay_time = 1;
    traffic[0].axles[1].delay_time = 15;
    traffic[0].axles[2].delay_time = 25;

    for (int i = 0; i < traffic[current_car].num_axles; i++) 
    {
        traffic[current_car].axles[i].active = 1;
    }
}


void app_1ms_clock(void) {

    static uint32_t timer = 0;

    for (uint8_t i = 0; i < traffic[current_car].num_axles; i++) 
    {
        if (traffic[current_car].axles[i].active && timer == traffic[current_car].axles[i].delay_time) 
        {
            hmi_led_turn_on(traffic[current_car].axles[i].led_index);
            hmi_led_turn_off(traffic[current_car].axles[i].led_index);
            traffic[current_car].axles[i].active = 0;  
        }
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


/******************************************************************************/
// AlissonGOE

void traffic_calculation_app(void)
{
    static uint32_t speed_in_meters_per_second = 0;
    static uint32_t time_in_loop = 0;
    static uint32_t time_between_loops = 0;
    static uint32_t time_spent_in_the_bonds = 0;

    speed_traffic = 50;
    vehicle_length = 12;
    gap_traffic = 3000;

    /* Area dos calculos*/
    speed_in_meters_per_second = ((speed_traffic * 1000)/ 3.6);

    time_between_loops = (((LENGHT_LOOP_MTS + DISTANCE_BETWEEN_LOOPS_MTS) * 1000000) / speed_in_meters_per_second);

    time_in_loop = ((LENGHT_LOOP_MTS * 1000000) / speed_in_meters_per_second);

    time_spent_in_the_bonds = ((vehicle_length * 1000000)/ speed_in_meters_per_second) + time_in_loop;
    /* Area dos calculos*/

    transit_state(time_between_loops, time_in_loop, gap_traffic, time_spent_in_the_bonds);
}

void transit_state(uint16_t time_between_loops_t, uint16_t time_in_loop_t, uint16_t gap, uint16_t time_spent_in_the_bonds_t)
{
    static uint16_t count_loop_1 = 0; 

    count_loop_1++;

    if (count_loop_1 <= gap) {
        current_status(INITIAL_TRANSIT_GAP) ; // off off
    }
    else if (count_loop_1 <= (gap + time_between_loops_t)) {
        current_status (INPUT_LOOP_ACTIVATION); // on off
    }
    else if (count_loop_1 <= (gap + time_between_loops_t + time_in_loop_t)) {
        current_status (OUTPUT_LOOP_ACTIVATION); // on on
    }
    else if (count_loop_1 <= (gap + (2 * time_between_loops_t) + time_in_loop_t)) {
        current_status (OUTPUT_LOOP_ACTIVATION); // on on
    }
    else if (count_loop_1 <= (gap + (3 * time_between_loops_t) + time_in_loop_t)) {
        current_status (INPUT_LOOP_DISABLED); // on on
    }
    else{
        count_loop_1 = 0;
        current_status (OUTPUT_LOOP_DISABLED); // off off
    }
}

void current_status(traffic_status_t state)
{

    switch (state) {
        case INITIAL_TRANSIT_GAP:
            hmi_led_turn_off(0);
            loop_turn_off(0);

            hmi_led_turn_off(2);
            loop_turn_off(1);
            break;

        case INPUT_LOOP_ACTIVATION:
            hmi_led_turn_on(0);
            loop_turn_on(0);

            hmi_led_turn_off(2);
            loop_turn_off(1);
            break;

        case OUTPUT_LOOP_ACTIVATION:
            hmi_led_turn_on(0);
            loop_turn_on(0);

            hmi_led_turn_on(2);
            loop_turn_on(1);
            break;

        case INPUT_LOOP_DISABLED:
            hmi_led_turn_off(0);
            loop_turn_off(0);

            hmi_led_turn_on(2);
            loop_turn_on(1);
            break;

        case OUTPUT_LOOP_DISABLED:
            hmi_led_turn_off(0);
            loop_turn_off(0);

            hmi_led_turn_off(2);
            loop_turn_off(1);
            break;

        default:
            break;
    }
}

// AlissonGOE
/******************************************************************************/


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

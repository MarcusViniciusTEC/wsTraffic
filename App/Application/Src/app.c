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

uint32_t sequency[6][18] = {0};

#define LOOP_ENTER_CH0          0
#define LOOP_EXIT_CH0           1

#define PIEZO_ENTER_CH0         2 


void app_1us_clock() /*10uS*/
{
    static uint32_t counter = 0;

    if(counter == 0)
    {
        hmi_led_turn_on(0);
    }
    if(counter  == 200)
    {
        hmi_led_turn_on(1);
        counter = 0;
    }

    counter++;
    



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

void app_1ms_clock(void)
{
  
}

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

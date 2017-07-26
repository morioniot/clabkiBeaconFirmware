

#include <stdbool.h>
#include "pca10028.h"
#include "ble_gap.h"
#include "app_button.h"
#include "app_timer.h"


#include "advertising_functions.h"



#define NUM_OF_BUTTONS                   1                                 /**< Number of buttons available */
#define APP_TIMER_PRESCALER             15                                 /**< Value of the RTC1 PRESCALER register. */
#define CONNECTABLE_MODE_FLAG            1                                 /**< Flag that indicates the adversiting connectable status */
/**
 * @brief Function to handle button trigger handler.
 */

static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{

	uint32_t err_code;
	err_code = app_button_disable();
    APP_ERROR_CHECK(err_code);
	if(button_action){
		non_connection_time_counter = 0;
		change_advertising_mode(CONNECTABLE_MODE_FLAG);
	}
}

/**
 * @brief Function that configures the button with pullup and detection low state.
 */
static const app_button_cfg_t app_buttons[NUM_OF_BUTTONS] =
{
    {BUTTON_1, false, BUTTON_PULL, button_event_handler}

};

void button_init(void){

	uint32_t err_code;
    err_code = app_button_init((app_button_cfg_t *)app_buttons, NUM_OF_BUTTONS, APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER));
    err_code = app_button_enable();
    APP_ERROR_CHECK(err_code);

};

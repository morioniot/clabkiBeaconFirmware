/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup clabki_configurable_modes main.c
 * @{
 * @ingroup clabki_configurable_modes
 * @brief Clabki Application main file.
 *
 *
 */

#include "app_util_platform.h"
#include "app_scheduler.h"
#include "app_timer_appsh.h"
#include "bsp.h"

// Application Resources
#include "adc.h"
#include "button.h"
#include "timer.h"
#include "advertising_functions.h"
#include "ble_functions.h"
#include "global_services_definitions.h"
#include "beacon_parameters_update_service.h"
#include "password_service.h"
#include "memory_definitions.h"
#include "connection.h"

#define STATUS_LED            		    BSP_LED_3_MASK
#define DEAD_BEEF                       0xDEADBEEF                        /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define NON_CONNECTABLE_MODE_FLAG       0                                 /**< Flag that indicates the adversiting connectable status */

/////******* TIMER CONFIGURATION **************************

#define APP_TIMER_PRESCALER             15                                 /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         3                                 /**< Size of timer operation queues. */

// Scheduler settings
#define SCHED_MAX_EVENT_DATA_SIZE       2
#define SCHED_QUEUE_SIZE                2

//// *****************************************************////

/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/**@brief Function for doing power management.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}



/**
 * @brief Setting Status_Led as an output.
 */
static void leds_init(void)
{
	LEDS_CONFIGURE(STATUS_LED);
	LEDS_ON(STATUS_LED);           /// 1 is OFF 0 is ON
}

/**
 * @brief Set P02 as ground (Needed to configure the ADC Hardware).
 */
static void p02_config(void){
	nrf_gpio_cfg_output(ARDUINO_A1_PIN);
	nrf_gpio_pin_clear(ARDUINO_A1_PIN);
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    uint32_t err_code;

    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
    // Initialize the application timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);

    err_code = bsp_init(BSP_INIT_LED, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), NULL);
    APP_ERROR_CHECK(err_code);

	leds_init();
	p02_config();
	button_init();
	// Initialize.
    ble_stack_init();
    gap_params_init();
    services_init();
    advertising_init(NON_CONNECTABLE_MODE_FLAG);
    conn_params_init();

    // Start execution.
    sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);                  /**< Enabling DCDC converter in order to save more power */
    advertising_start(NON_CONNECTABLE_MODE_FLAG);
    m_conn_handle.conn_handle = BLE_CONN_HANDLE_INVALID;            /**< Setting Default Handle of the current connection. */

    timer_init();
    // Configure ADC.
    adc_config();

    // Enter main loop.
    for (;;)
    {
        power_manage();
        app_sched_execute();
    }
}

/**
 * @}
 */

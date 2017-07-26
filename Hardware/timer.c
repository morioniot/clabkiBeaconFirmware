

#include "app_timer.h"
#include "app_scheduler.h"
#include "adc.h"
#include "advertising_functions.h"
#include "timer.h"


#define APP_TIMER_PRESCALER                    15                                 /**< Value of the RTC1 PRESCALER register. */

APP_TIMER_DEF(m_timer_id);


static void timer_functions_update(void)
{

	app_sched_event_put(0,0,(app_sched_event_handler_t)adc_sample);              									//Put adc_sample function into the scheduler queue, which will then be executed in the main context (lowest priority) when app_sched_execute is called in the main loop
	app_sched_event_put(0,0,(app_sched_event_handler_t)no_connection_time_counter);   //Put no_connection_time_counter function into the scheduler queue, which will then be executed in the main context (lowest priority) when app_sched_execute is called in the main loop
}

/**@brief Function for handling the Battery measurement timer timeout.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *
 * @param[in] p_context  Pointer used for passing some arbitrary information (context) from the
 *                       app_start_timer() call to the timeout handler.
 */
static void timeout_handler(void * p_context)
{
	UNUSED_PARAMETER(p_context);
    timer_functions_update();
}

// Create timers
static void create_timers()
{
    uint32_t err_code;
    // Create timers
    err_code = app_timer_create(&m_timer_id,
                                APP_TIMER_MODE_REPEATED,
								timeout_handler);
    APP_ERROR_CHECK(err_code);
}


void timer_init(void){

    uint32_t err_code;
	// Create application timers.
    create_timers();

    err_code = app_timer_start(m_timer_id, APP_TIMER_TICKS(TIMER_INTERVAL, APP_TIMER_PRESCALER), NULL);
    APP_ERROR_CHECK(err_code);

}


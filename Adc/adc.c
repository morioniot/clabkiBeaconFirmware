
#include "app_util_platform.h"
#include "nrf_drv_adc.h"
#include "bsp.h"

#define STATUS_LED            		    BSP_LED_3_MASK

// ***** ADC DEFINITIONS ***** ////

static nrf_drv_adc_channel_t m_channel_config = NRF_DRV_ADC_DEFAULT_CHANNEL(NRF_ADC_CONFIG_INPUT_2); /**< Channel instance. Default configuration used. */
#define ADC_REF_VOLTAGE_IN_MILLIVOLTS        1200
#define ADC_RESULT_IN_MILLI_VOLTS(ADC_VALUE)\
				((((ADC_VALUE) * ADC_REF_VOLTAGE_IN_MILLIVOLTS) / 1023))

//****************************/////

void adc_event_handler(nrf_drv_adc_evt_t const * p_event)
{
	//LEDS_INVERT(STATUS_LED);
	uint16_t adc_result_millivolts;
    int16_t adc_value;
	adc_value = p_event->data.sample.sample;
	adc_result_millivolts = ADC_RESULT_IN_MILLI_VOLTS(adc_value);
	if( adc_result_millivolts >= 800 && adc_result_millivolts <= 830){
		////**** EXECUTE SOMETHING  WHEN BATTERY LEVEL GETS IN RANGE**** ////
		//LEDS_INVERT(STATUS_LED);
	}
}

/**
 * @brief ADC initialization.
 */
void adc_config(void)
{
    ret_code_t ret_code;
    nrf_drv_adc_config_t config = NRF_DRV_ADC_DEFAULT_CONFIG;
    ret_code = nrf_drv_adc_init(&config, adc_event_handler);
    APP_ERROR_CHECK(ret_code);
}


void adc_sample(void)
{
	ret_code_t ret_code;
	ret_code = nrf_drv_adc_sample_convert(&m_channel_config, NULL);              // Allocate buffer for ADC
	APP_ERROR_CHECK(ret_code);
}

#ifndef ADC_H__
#define ADC_H__


#include "nrf_drv_adc.h"


void adc_event_handler(nrf_drv_adc_evt_t const * p_event);

/**
 * @brief ADC initialization.
 */
void adc_config(void);


void adc_sample(void);

#endif  /* ADC_H__ */

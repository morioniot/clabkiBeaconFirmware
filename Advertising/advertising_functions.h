#ifndef ADVERTISING_FUNCTIONS_H__
#define ADVERTISING_FUNCTIONS_H__


#include "ble_gap.h"
#include "beacon_parameters_update_service.h"

#define    MAX_TIME_WITHOUT_CONNECTION      20000                  /**< Maximium number of millisecond before the advertising mode changes back to NON-CONNECTABLE after the button has been pressed */

ble_gap_adv_params_t m_adv_params;                                 /**< Parameters to be passed to the stack when starting advertising. */

bool       advertising_type_status_flag;                       	   /**< Connectable or  Non-Connectable Advertising flag  */
uint8_t    non_connection_time_counter;                        /**< Time (seconds) without connection after the button has been pressed */


void advertising_data_array_updating(beacon_parameters_t new_beacon_parameters);

/** @brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
void advertising_init(uint8_t connection_configuration_mode);

/**@brief Function for starting advertising.
 */
void advertising_start(uint8_t connection_configuration_mode);

/**@brief Function for changing advertising mode (connectable or not).
 */
void change_advertising_mode(uint8_t connection_configuration_mode);

/**@brief Function for counting time without connection after the button has been pressed
 */
void no_connection_time_counter(void);

#endif  /* ADVERTISING_FUNCTIONS_H__ */

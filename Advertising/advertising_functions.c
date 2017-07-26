

#include "ble_advdata.h"
#include "ble_gap.h"
#include "ble_conn_params.h"
#include "bsp.h"

#include "memory_definitions.h"
#include "beacon_parameters_update_service.h"
#include "advertising_functions.h"
#include "timer.h"

#define STATUS_LED            		    BSP_LED_3_MASK
#define APP_CFG_NON_CONN_ADV_TIMEOUT    0                                 /**< Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. */
#define ADVERTISING_TIME_INTERVAL       MSEC_TO_UNITS(1000, UNIT_0_625_MS) /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). */
#define NON_CONNECTABLE_MODE_FLAG       0                                 /**< Flag that indicates the adversiting connectable status */

#define MAJ_VAL_OFFSET_IN_BEACON_INFO   4                                /**< Position of the MSB of the Major Value in clabki_advertising_data_array array. */
#define APP_ADV_DATA_LENGTH             0x0D                              /**< Length of manufacturer specific data in the advertisement. */
#define APP_MEASURED_RSSI               0xC3                              /**< The Beacon's measured RSSI at 1 meter distance in dBm. */
#define APP_BATTERY_LEVEL               0x00                              /**< Battery Beacon Level */

// Clabki common data
#define APP_CLABKI_UUID                 0xCABF                               /**< UUID for Clabki beacons according to specification. */

#define APP_MAJOR_VALUE                 0x01, 0x02                        /**< Major value used to identify Beacons. */
#define APP_MINOR_VALUE                 0x03, 0x04                        /**< Minor value used to identify Beacons. */
#define APP_BEACON_UUID                 0x01, 0x12, 0x23, 0x34            /**< Proprietary UUID for Beacon. */




static uint8_t clabki_advertising_data_array[] =                    /**< Information advertised by the Beacon. */
{
    APP_BEACON_UUID,     // 32 bit UUID value.
    APP_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
    APP_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
    APP_MEASURED_RSSI,    // Manufacturer specific information. The Beacon's measured TX power in
                         // this implementation.
	APP_BATTERY_LEVEL    // Battery Level information
};
/**@brief Function for updating the data array to be advertised in the future.
 *
 * @param[in] beacon_parameters_t new_beacon_parameters.
 */
void advertising_data_array_updating(beacon_parameters_t new_beacon_parameters){
	uint8_t index = MAJ_VAL_OFFSET_IN_BEACON_INFO;
	clabki_advertising_data_array[index++] = MSB_16(new_beacon_parameters.major);
	clabki_advertising_data_array[index++] = LSB_16(new_beacon_parameters.major);
    clabki_advertising_data_array[index++] = MSB_16(new_beacon_parameters.minor);
    clabki_advertising_data_array[index++] = LSB_16(new_beacon_parameters.minor);
}

/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
void advertising_init(uint8_t connection_configuration_mode)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    ble_uuid_t    adv_uuids[] = {{APP_CLABKI_UUID, BLE_UUID_TYPE_BLE}};

    //*** Reading Major and Minor values from Flash memory ***///

    pg_size = NRF_FICR->CODEPAGESIZE;
    pg_num  = NRF_FICR->CODESIZE - 1;  // Use last page in flash
    major_minor_addr = (uint32_t *)(pg_size * pg_num);

    uint16_t major_value = ((*(uint32_t *)major_minor_addr) & 0xFFFF0000) >> 16;
    uint16_t minor_value = ((*(uint32_t *)major_minor_addr) & 0x0000FFFF);

	uint8_t index = MAJ_VAL_OFFSET_IN_BEACON_INFO;
	clabki_advertising_data_array[index++] = MSB_16(major_value);
	clabki_advertising_data_array[index++] = LSB_16(major_value);
    clabki_advertising_data_array[index++] = MSB_16(minor_value);
    clabki_advertising_data_array[index++] = LSB_16(minor_value);

    //************************************************///

    uint8_array_t clabki_data_array;                                        // Array for Service Data structure.
    clabki_data_array.p_data = (uint8_t *) clabki_advertising_data_array;   // Pointer to the data to advertise.
    clabki_data_array.size = sizeof(clabki_advertising_data_array);         // Size of the data to advertise.

    ble_advdata_service_data_t service_data;                     // Structure to hold Service Data.
    service_data.service_uuid = APP_CLABKI_UUID;                 // Clabki UUID to allow discoverability on iOS devices.
    service_data.data = clabki_data_array;                       // Array for service advertisement data.

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_NO_NAME;
    advdata.include_appearance      = false;
    advdata.flags                   = flags;
    advdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = adv_uuids;
    advdata.p_service_data_array    = &service_data;                // Pointer to Service Data structure.
    advdata.service_data_count      = 1;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));
    if(connection_configuration_mode){
    	m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;         // Connectable Advertising
    }
    else{
    	m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND; //Non-connectable Advertising
    }
    m_adv_params.p_peer_addr = NULL;                             // Undirected advertisement.
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = ADVERTISING_TIME_INTERVAL;
    m_adv_params.timeout     = APP_CFG_NON_CONN_ADV_TIMEOUT;
}

/**@brief Function for starting advertising.
 */
void advertising_start(uint8_t connection_configuration_mode)
{
    uint32_t err_code;

    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);

    if(connection_configuration_mode){
        err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
        APP_ERROR_CHECK(err_code);
    }
    else{
        err_code = bsp_indication_set(BSP_INDICATE_IDLE);
        APP_ERROR_CHECK(err_code);
    }
}

void change_advertising_mode(uint8_t connection_configuration_mode){

	advertising_type_status_flag = connection_configuration_mode;
	sd_ble_gap_adv_stop();
	advertising_init(connection_configuration_mode);
    advertising_start(connection_configuration_mode);
}


void no_connection_time_counter(void){

	//LEDS_INVERT(STATUS_LED);
	uint32_t err_code;
	if(advertising_type_status_flag){
		non_connection_time_counter += 1;
		if(non_connection_time_counter >= (MAX_TIME_WITHOUT_CONNECTION/TIMER_INTERVAL)){
            change_advertising_mode(NON_CONNECTABLE_MODE_FLAG);
            advertising_type_status_flag = false;
            non_connection_time_counter = 0;
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            APP_ERROR_CHECK(err_code);
            err_code = app_button_enable();
            APP_ERROR_CHECK(err_code);
		}
	}
}

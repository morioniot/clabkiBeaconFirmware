

#include <stdbool.h>
#include "pca10028.h"
#include "bsp.h"
#include "ble.h"
#include "ble_conn_params.h"
#include "ble_gap.h"
#include "nrf_sdm.h"
#include "softdevice_handler.h"
#include "advertising_functions.h"
#include "password_service.h"
#include "beacon_parameters_update_service.h"
#include "global_services_definitions.h"
#include "connection.h"
#include "ble_functions.h"

#define STATUS_LED            		    BSP_LED_3_MASK

#define CENTRAL_LINK_COUNT              0                                 /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT           1                                 /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/
#define NON_CONNECTABLE_MODE_FLAG       0

/**@brief Function for the application's SoftDevice event handler.
 *
 * @param[in] p_ble_evt SoftDevice event.
 */
void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t                         err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            m_conn_handle.conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            m_conn_handle.authorized  = false;
            advertising_type_status_flag = false;
            err_code = bsp_indication_set(BSP_INDICATE_USER_STATE_OFF);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            err_code = sd_ble_gap_adv_start(&m_adv_params);
            APP_ERROR_CHECK(err_code);
            m_conn_handle.conn_handle = BLE_CONN_HANDLE_INVALID;
			change_advertising_mode(NON_CONNECTABLE_MODE_FLAG);
            err_code = app_button_enable();
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle.conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle.conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_WRITE:
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
        	m_conn_handle.conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;
        	bool authenticated;
        	authenticated = false;
        	switch (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.uuid.uuid)
            {
        		case 0xAAAA:                                             /// Authentication Characteristic Writting Request
        			authenticated = connection_authentication(p_ble_evt);
        			m_conn_handle.authorized = authenticated;
        			response_for_writing_requests(authenticated, m_conn_handle);
        			break;
        		case 0xBEEF:											/// Beacon Parameters Update Characteristic Writting Request
        			authenticated = checking_for_connection_authentication(m_conn_handle);
        			if(authenticated){
        				beacon_parameters_t new_beacon_parameters = beacon_advertising_parameter_update(p_ble_evt);
        				advertising_data_array_updating(new_beacon_parameters);
        			}
        			response_for_writing_requests(authenticated, m_conn_handle);
        			break;
        		case 0xBBBB:                                           /// Update Password Characteristic Writting Request
        			authenticated = checking_for_connection_authentication(m_conn_handle);
        			if(authenticated){
        				update_password(p_ble_evt);
        			}
        			response_for_writing_requests(authenticated, m_conn_handle);
        			break;
        		default:
        			break;
            }
            break;
        default:
            // No implementation needed.
            break;
    }
}

/**@brief Function for dispatching a SoftDevice event to all modules with a SoftDevice
 *        event handler.
 *
 * @details This function is called from the SoftDevice event interrupt handler after a
 *          SoftDevice event has been received.
 *
 * @param[in] p_ble_evt  SoftDevice event.
 */
void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    on_ble_evt(p_ble_evt);
}

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
void ble_stack_init(void)
{
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = { .source        = NRF_CLOCK_LF_SRC_RC,               \
                                        .rc_ctiv       = 16,                                \
                                        .rc_temp_ctiv  = 0,                                 \
                                        .xtal_accuracy = 0};

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    ble_enable_params.common_enable_params.vs_uuid_count   = 2;

    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Subscribe for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}

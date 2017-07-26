

#include "global_services_definitions.h"
#include "password_service.h"
#include "beacon_parameters_update_service.h"
#include "ble_gatts.h"

/**@brief Function for preparing a write_request response.
 *
 * @details This function will be called when a response for a write request is sent to a client.
 *
 *
 * @param[in] response  Response Status .
 */

void response_for_writing_requests(bool response, m_conn_handle_t  connection_handler)
{
	uint32_t  err_code;
	ble_gatts_rw_authorize_reply_params_t write_authorize_reply_params;
	write_authorize_reply_params.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
    if(response){
    	write_authorize_reply_params.params.write.update  = 1;
        write_authorize_reply_params.params.write.offset  = 0;
        write_authorize_reply_params.params.write.len     = 0;
        write_authorize_reply_params.params.write.p_data  = NULL;
    	write_authorize_reply_params.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;
    }
    else{
    	write_authorize_reply_params.params.write.gatt_status = BLE_GATT_STATUS_ATTERR_WRITE_NOT_PERMITTED;
    }
    err_code = sd_ble_gatts_rw_authorize_reply(connection_handler.conn_handle, &write_authorize_reply_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing services that will be used by the application.
 */
void services_init(void)
{
	password_service_init(&m_password);
	beacon_parameters_update_service_init(&m_beacon_parameters_update_service);
}

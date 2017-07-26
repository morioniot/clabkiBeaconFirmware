/* Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *   4. This software must only be used in a processor manufactured by Nordic
 *   Semiconductor ASA, or in a processor manufactured by a third party that
 *   is used in combination with a processor manufactured by Nordic Semiconductor.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>
#include <string.h>
#include "global_services_definitions.h"
#include "ble_gatts.h"
#include "password_service.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "memory_definitions.h"


///**@brief Function for adding Password Authentication characteristic to Password Service.
// *
// * @param[in]   p_password_service_structure        p_password_service structure.
// *
// */
static uint32_t password_authentication_characteristic_add(ble_os_t * p_password_service_structure)
{

	uint32_t            err_code;
	ble_uuid_t          char_uuid;
	ble_uuid128_t       base_uuid = {BLE_UUID_SERVICE_BASE_UUID};
	char_uuid.uuid      = BLE_UUID_AUTHENTICATION_CHARACTERISTC_UUID;
	err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
	APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 0;
    char_md.char_props.write = 1;

    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;

    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;
    attr_md.wr_auth    = 1;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;

    attr_char_value.max_len     = 2;
    attr_char_value.init_len    = 2;
    uint8_t value[2]            = {0x00,0x00};
    attr_char_value.p_value     = value;

    err_code = sd_ble_gatts_characteristic_add(p_password_service_structure->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_password_service_structure->char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}


///**@brief Function for adding Password Authentication characteristic to Password Service.
// *
// * @param[in]   p_password_service_structure        p_password_service structure.
// *
// */
static uint32_t password_update_characteristic_add(ble_os_t * p_password_service_structure)
{

	uint32_t            err_code;
	ble_uuid_t          char_uuid;
	ble_uuid128_t       base_uuid = {BLE_UUID_SERVICE_BASE_UUID};
	char_uuid.uuid      = BLE_UUID_UPDATE_CHARACTERISTC_UUID;
	err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
	APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 0;
    char_md.char_props.write = 1;

    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;

    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;
    attr_md.wr_auth    = 1;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;

    attr_char_value.max_len     = 2;
    attr_char_value.init_len    = 2;
    uint8_t value[2]            = {0x00,0x00};
    attr_char_value.p_value     = value;

    err_code = sd_ble_gatts_characteristic_add(p_password_service_structure->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_password_service_structure->char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

/**@brief Function for initiating our new service.
 *
 * @param[in]   p_our_service        Our Service structure.
 *
 */
void password_service_init(ble_os_t * p_password_service_structure)
{

	uint32_t   		  err_code;
	ble_uuid_t        service_uuid;
	ble_uuid128_t     base_uuid = {BLE_UUID_SERVICE_BASE_UUID};

	service_uuid.uuid = BLE_UUID_PASSWORD_SERVICE;
	err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
	APP_ERROR_CHECK(err_code);

	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
	                                    &service_uuid,
	                                    &p_password_service_structure->service_handle);
	APP_ERROR_CHECK(err_code);

	password_authentication_characteristic_add(p_password_service_structure);
	password_update_characteristic_add(p_password_service_structure);

}


/**@brief Function for authenticating a connection.
 *
 * @details This function will be called when a client sends a write_request for the Authentication Characteristic.
 *
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module when a BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST has been received .
 */
bool connection_authentication(ble_evt_t * p_ble_evt)
{
	uint16_t  password_to_check;
	bool      response_to_write_request;

	password_to_check = (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.data[0]<<8) | p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.data[1];
    pg_size = NRF_FICR->CODEPAGESIZE;
    pg_num  = NRF_FICR->CODESIZE - 2;  // Use last page in flash
    password_addr = (uint32_t *)(pg_size * pg_num);
    uint16_t  current_password = ((*(uint32_t *)password_addr) & 0x0000FFFF);
    if(current_password == password_to_check){
    	response_to_write_request = true;
    }
    else{
    	response_to_write_request = false;
    }

    return (response_to_write_request);
}

/**@brief Function for checking authorized connections.
 *
 * @details This function is called when a client try to write on a protected characteristic in order to check if the connection is already authenticated .
 *
 *
 */
bool checking_for_connection_authentication(m_conn_handle_t  connection_handler)
{

	bool response_to_write_request;
	if(connection_handler.authorized){
		response_to_write_request = true;
	}
	else{
		response_to_write_request = false;
	}
	return (response_to_write_request);
}

/**@brief Function for updating a password.
 *
 * @details This function will be called when a client sends a write_request for the Update password Characteristic.
 *
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module when a BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST has been received .
 */
void update_password(ble_evt_t * p_ble_evt)
{
	uint32_t  new_password;
	new_password = (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.data[0]<<8) | p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.data[1];
    pg_size = NRF_FICR->CODEPAGESIZE;
    pg_num  = NRF_FICR->CODESIZE - 2;
    password_addr = (uint32_t *)(pg_size * pg_num);
    flash_page_erase(password_addr);
    memory_writing(password_addr, new_password);
}

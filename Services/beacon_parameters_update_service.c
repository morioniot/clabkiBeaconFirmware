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
#include  "ble_gatts.h"
#include "global_services_definitions.h"
#include "beacon_parameters_update_service.h"
#include "memory_definitions.h"
#include "ble_srv_common.h"
#include "app_error.h"



/**@brief Function for adding Beacon Update parameter Characteristic to Beacon_parameters_update_service.
 *
 * @param[in]   p_beacon_parameters_update_service        p_beacon_parameters_update structure.
 *
 */
static uint32_t beacon_parameters_update_characteristic_add(ble_os_t * p_beacon_parameters_update_service_structure)
{

	uint32_t            err_code;
	ble_uuid_t          char_uuid;
	ble_uuid128_t       base_uuid = {BLE_UUID_SERVICE_BASE_UUID};
	char_uuid.uuid      = BLE_UUID_BEACON_PARAMETERS_UPDATE_CHARACTERISTC_UUID;
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
    // OUR_JOB: Step 2.H, Set characteristic length in number of bytes
    attr_char_value.max_len     = 4;
    attr_char_value.init_len    = 4;
    uint8_t value[4]            = {0x01,0x02,0x03,0x04};  // Default Major , Minor
    attr_char_value.p_value     = value;

    err_code = sd_ble_gatts_characteristic_add(p_beacon_parameters_update_service_structure->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_beacon_parameters_update_service_structure->char_handles);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}


/**@brief Function for initiating our new service.
 *
 * @param[in]   p_our_service        Our Service structure.
 *
 */
void beacon_parameters_update_service_init(ble_os_t * p_beacon_parameters_update_service_structure)
{

	uint32_t   		  err_code;
	ble_uuid_t        service_uuid;
	ble_uuid128_t     base_uuid = {BLE_UUID_SERVICE_BASE_UUID};

	service_uuid.uuid = BLE_UUID_BPU_SERVICE;
	err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
	APP_ERROR_CHECK(err_code);

	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
	                                    &service_uuid,
	                                    &p_beacon_parameters_update_service_structure->service_handle);
	APP_ERROR_CHECK(err_code);

	beacon_parameters_update_characteristic_add(p_beacon_parameters_update_service_structure);
}


struct beacon_parameters_t beacon_advertising_parameter_update(ble_evt_t * p_ble_evt)
{
	uint16_t  major_value;
	uint16_t  minor_value;
	uint32_t  complete_value;
	major_value = (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.data[0]<<8) | p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.data[1];
	minor_value = (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.data[2]<<8) | p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.data[3];

	struct beacon_parameters_t new_beacon_parameters = {major_value, minor_value};

    pg_size = NRF_FICR->CODEPAGESIZE;
    pg_num  = NRF_FICR->CODESIZE - 1;  // Use last page in flash
    major_minor_addr = (uint32_t *)(pg_size * pg_num);
	flash_page_erase(major_minor_addr);
    complete_value = (major_value<<16) | minor_value;
	memory_writing(major_minor_addr, complete_value);

	return new_beacon_parameters;
}

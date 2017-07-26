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

#ifndef GLOBAL_SERVICES_DEFINITIONS_H__
#define GLOBAL_SERVICES_DEFINITIONS_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "connection.h"
#include "app_error.h"

// Defining 128 bits Base UUID
#define BLE_UUID_SERVICE_BASE_UUID              {0x01, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00} // 128-bit base UUID


/**
 * @brief This structure contains various status information for our service.
 * It only holds one entry now, but will be populated with more items as we go.
 * The name is based on the naming convention used in Nordic's SDKs.
 * 'ble� indicates that it is a Bluetooth Low Energy relevant structure and
 * �os� is short for Our Service).
 */
typedef struct ble_os_t
{
	uint16_t                    conn_handle;
    uint16_t    				service_handle;     /**< Handle of Our Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    char_handles;
}ble_os_t;

//** Service Definition **////

ble_os_t 						 m_beacon_parameters_update_service;     /**< Control for  Beacon Parameters Update Service. */
ble_os_t 						 m_password;                             /**< Control for Authentication Service. */
//***********************/////



/**@brief Function for preparing a write_request response.
 *
 * @details This function will be called when a response for a write request is sent to a client.
 *
 *
 * @param[in] response  Response Status .
 */

void response_for_writing_requests(bool response, m_conn_handle_t  connection_handler);

/**@brief Function for initializing services that will be used by the application.
 */
void services_init(void);

#endif  /* GLOBAL_SERVICES_DEFINITIONS_H__ */

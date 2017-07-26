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

#ifndef PASSWORD_SERVICE_H__
#define PASSWORD_SERVICE_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "global_services_definitions.h"

//  Defining 16-bit service
#define BLE_UUID_PASSWORD_SERVICE                0xCDEF


// Defining 16-bit characteristic UUID
#define BLE_UUID_AUTHENTICATION_CHARACTERISTC_UUID          0xAAAA
// Defining 16-bit characteristic UUID
#define BLE_UUID_UPDATE_CHARACTERISTC_UUID          		0xBBBB



/**@brief Function for initializing our new service.
 *
 * @param[in]   p_our_service       Pointer to Our Service structure.
 */
void password_service_init(ble_os_t * p_password_service_structure);


/**@brief Function for authenticating a connection.
 *
 * @details This function will be called when a client sends a write_request for the Authentication Characteristic.
 *
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module when a BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST has been received .
 */

bool connection_authentication(ble_evt_t * p_ble_evt);

/**@brief Function for checking authorized connections.
 *
 * @details This function is called when a client try to write on a protected characteristic in order to check if the connection is already authenticated .
 *
 *
 */
bool checking_for_connection_authentication( m_conn_handle_t  connection_handler);

/**@brief Function for updating a password.
 *
 * @details This function will be called when a client sends a write_request for the Update password Characteristic.
 *
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module when a BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST has been received .
 */
void update_password(ble_evt_t * p_ble_evt);

#endif  /* _ PASSWORD_SERVICE_H__ */

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

#ifndef BEACON_PARAMETERS_UPDATE_SERVICE_H__
#define BEACON_PARAMETERS_UPDATE_SERVICE_H__

#include <stdint.h>
#include "ble.h"
#include "global_services_definitions.h"
#include "ble_srv_common.h"

//  Defining 16-bit service
#define BLE_UUID_BPU_SERVICE                0xABCD // Just a random, but recognizable value


// Defining 16-bit characteristic UUID
#define BLE_UUID_BEACON_PARAMETERS_UPDATE_CHARACTERISTC_UUID          0xBEEF // Just a random, but recognizable value


typedef struct beacon_parameters_t{
	uint16_t major;
	uint16_t minor;
}beacon_parameters_t;

/**@brief Function for initializing our new service.
 *
 * @param[in]   p_our_service       Pointer to Our Service structure.
 */
void beacon_parameters_update_service_init(ble_os_t * p_beacon_parameters_update_service_structure);


/**@brief Function for updating the MAJOR and MINOR beacon's parameter.
 *
 * @details This function will is called when a client updates the updateMajorMinorParameters characteristic .
 *
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
 struct beacon_parameters_t  beacon_advertising_parameter_update(ble_evt_t * p_ble_evt);

#endif  /* BEACON_PARAMETERS_UPDATE_SERVICE_H__ */

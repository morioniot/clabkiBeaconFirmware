#ifndef CONNECTION_H__
#define CONNECTION_H__

#include "ble_conn_params.h"

/**
 * @brief This structure contains important status for a current connection.
 */
typedef struct
{
	uint16_t                conn_handle;
    bool    				authorized;     /**< Authentication status */
}m_conn_handle_t;

m_conn_handle_t  m_conn_handle;

/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
void on_conn_params_evt(ble_conn_params_evt_t * p_evt);


/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
void conn_params_error_handler(uint32_t nrf_error);


/**@brief Function for initializing the Connection Parameters module.
 */
void conn_params_init(void);

/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
void gap_params_init(void);

#endif  /* CONNECTION_H__ */

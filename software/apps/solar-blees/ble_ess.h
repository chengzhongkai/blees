#ifndef BLE_ESS_H__

#define BLE_ESS_H__


#include <stdint.h>
#include <stdbool.h>

#include "ble.h"
#include "ble_srv_common.h"
#include "stdio.h"


//ESS UUIDs
#define ESS_UUID_BASE {0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define ESS_UUID_SERVICE 0x181A

#define ESS_UUID_ES_MEAS_DESC 0x290C
#define ESS_UUID_ES_TRIGGER_SETTING 0x290D

//Character UUIDs
#define ESS_UUID_PRES_CHAR 0x2A6D
#define ESS_UUID_HUM_CHAR 0x2A6F
#define ESS_UUID_TEMP_CHAR 0x2A6E
#define ESS_UUID_LUX_CHAR 0xC55A62C082624A628F9AA5C5124ECADC
#define ESS_UUID_LUX_CHAR_SHORT 0xC512
#define ESS_UUID_ACC_CHAR 0xCD3F4A3482F34F7E909D28F8014F36DA
#define ESS_UUID_ACC_CHAR_SHORT 0xF801


//Character lengths
#define MAX_PRES_LEN 4
#define MAX_HUM_LEN 2
#define MAX_TEMP_LEN 2
#define MAX_LUX_LEN 2
#define MAX_ACC_LEN 1

//Trigger condition types
#define TRIG_INACTIVE 0x00
#define TRIG_FIXED_INTERVAL 0x01
#define TRIG_NO_LESS 0x02
#define TRIG_VALUE_CHANGE 0x03
#define TRIG_WHILE_LT 0x04
#define TRIG_WHILE_LTE 0x05
#define TRIG_WHILE_GT 0x06
#define TRIG_WHILE_GTE 0x07
#define TRIG_WHILE_E 0x08
#define TRIG_WHILE_NE 0x09

#define MAX_TRIG_LEN 32

/**@brief ESS event type. */
typedef enum
{
    BLE_ESS_EVT_NOTIFICATION_ENABLED,                                       /**< ESS value notification enabled event. */
    BLE_ESS_EVT_NOTIFICATION_DISABLED,                                      /**< ESS value notification disabled event. */
    BLE_ESS_EVT_NOTIFICATION_CONFIRMED                                      /**< Confirmation of an ESS notification has been received. */
} ble_ess_evt_type_t;

/**@brief ESS event. */
typedef struct
{
    ble_ess_evt_type_t evt_type;                                           	/**< Type of event. */
} ble_ess_evt_t;

// Forward declaration of the ble_ess_t type.
typedef struct ble_ess_s ble_ess_t;

// Forward declaration of the ess_trig_set_desc_t type.
typedef struct ess_trig_set_desc_s ess_trig_set_desc_t;

//typedef struct ess_meas_des_s ess_meas_des_t;

/**@brief ESS event handler type. */
typedef void (*ble_ess_evt_handler_t) (ble_ess_t * p_ess, ble_ess_evt_t * p_evt);

typedef struct 
{
	uint32_t 	time_interval;
	uint8_t 	condition;

} ess_char_trigger_init_data_t;


/**@brief ESS init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    ble_ess_evt_handler_t         	evt_handler; 				/**< Event handler to be called for handling events in the ESS. */
    bool							is_notify_supported;		/**< Determines if notifications are supported */

	uint32_t 						init_pres_data;	
	uint32_t						pres_trigger_val_var;
	ess_char_trigger_init_data_t	pres_trigger_data;

	uint16_t 						init_hum_data;	
	uint16_t						hum_trigger_val_var;
	ess_char_trigger_init_data_t	hum_trigger_data;

	int16_t 						init_temp_data;	
	int16_t							temp_trigger_val_var;
	ess_char_trigger_init_data_t	temp_trigger_data;

	uint16_t 						init_lux_data;	
	uint16_t						lux_trigger_val_var;
	ess_char_trigger_init_data_t	lux_trigger_data;

	uint16_t 						init_acc_data;	
	uint16_t						acc_trigger_val_var;
	ess_char_trigger_init_data_t	acc_trigger_data;
	
} ble_ess_init_t;


typedef struct 
{
	uint8_t	   					val_last[4];
	uint8_t 					trigger_val_cond;
	uint8_t 					trigger_val_buff[5];
	uint16_t 					trigger_handle;

} ess_char_data_t;

/**@brief ESS structure. This contains various status information for the service. */
typedef struct ble_ess_s
{
    ble_ess_evt_handler_t         	evt_handler;              	/**< Event handler to be called for handling events in the ESS. */
    bool							is_notify_supported;		/**< TRUE if notifications are supported */
    uint16_t                      	ess_service_handle;         /**< Handle of ESS (as provided by the BLE stack). */
    uint8_t                       	uuid_type;
    uint16_t                      	conn_handle;                /**< Handle of the current connection 
    															 *	  (as provided by the BLE stack, 
    															 *	   is BLE_CONN_HANDLE_INVALID if not in a connection). */
   
	ble_gatts_char_handles_t      	pres_char_handles;          //< Handles related to the Pressure characteristic. 
	ble_gatts_char_handles_t     	hum_char_handles;           //< Handles related to the Humidity characteristic. 
	ble_gatts_char_handles_t      	temp_char_handles;          //< Handles related to the Temperature characteristic. 
	ble_gatts_char_handles_t     	lux_char_handles;           //< Handles related to the Humidity characteristic. 
	ble_gatts_char_handles_t     	acc_char_handles;           //< Handles related to the Acceleration characteristic. 

	ess_char_data_t 			  	pressure;
	ess_char_data_t 			  	humidity;
	ess_char_data_t 			  	temperature;
	ess_char_data_t 			  	lux;
	ess_char_data_t 			  	acceleration;
    
} ble_ess_t;

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Battery Service.
 *
 * @param[in]   p_ess      ESS structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_ess_on_ble_evt(ble_ess_t * p_ess, ble_evt_t * p_ble_evt);

/**@brief Function for initializing the ESS.
 *
 * @param[out]  p_ess       ESS structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_ess_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_ess_init(ble_ess_t * p_ess, ble_ess_init_t * p_ess_init);

uint32_t ble_ess_char_value_update(	ble_ess_t * p_ess, 
									ess_char_data_t * char_data, 
									uint8_t  * ess_meas_val, 
									uint16_t char_len, 
									bool is_signed,  
									ble_gatts_char_handles_t * char_handles );

int ble_ess_intcmp( uint8_t * buff_1, uint8_t * buff_2, uint16_t length, bool is_signed);

#endif // BLE_ESS_H__

/** @} */
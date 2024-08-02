/* 
 * File:   ecu_button.h
 * Author: Ali
 *
 * Created on March 3, 2024, 2:38 PM
 */

#ifndef ECU_BUTTON_H
#define	ECU_BUTTON_H

/* Section : includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_button_cfg.h"

/* Section : Macro Declarations*/

/* Section : Macro Functions Declarations*/

/* Section : Data Type Declarations*/
typedef enum{
    BUTTON_PRESSED,
    BUTTON_RELEASED        
}button_state_t;

typedef enum{
    BUTTON_ACTIVE_HIGH,
    BUTTON_ACTIVE_LOW
}button_active_t;

typedef struct{
    pin_config_t button_pin;
    button_state_t button_state;
    button_active_t button_connection;
}button_t;
/* Section : Function Declarations*/
Std_ReturnType button_initialize(const button_t *btn);
Std_ReturnType button_read_state(const button_t *btn,button_state_t *btn_state);
#endif	/* ECU_BUTTON_H */


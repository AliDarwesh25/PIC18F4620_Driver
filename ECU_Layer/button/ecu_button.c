/* 
 * File:   ecu_button.c
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on March 3, 2024, 2:38 PM
 */

#include "ecu_button.h"

/**
 * @brief Initialize the assigned pin to be Input.
 * @param btn pointer to the button configurations
 * @return  Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue while performing this action
 */
Std_ReturnType button_initialize(const button_t *btn){
    Std_ReturnType ret = E_OK;
    if(NULL == btn){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_direction_initalize(&(btn->button_pin));
    }
    return ret;
}

/**
 * @brief Read the state of the button
 * @param btn pointer to the button configurations
 * @param btn_state button state @ref button_state_t
 * @return  Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue while performing this action
 */
Std_ReturnType button_read_state(const button_t *btn,button_state_t *btn_state){
    Std_ReturnType ret = E_OK;
    logic_t Pin_Logic_Status = GPIO_LOW;
    if(NULL == btn || NULL == btn_state){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_read_logic(&(btn->button_pin),&Pin_Logic_Status);
        if(BUTTON_ACTIVE_HIGH == btn->button_connection){
            if(GPIO_HIGH == Pin_Logic_Status){
                *btn_state = BUTTON_PRESSED;
            }
            else{
                *btn_state = BUTTON_RELEASED;
            }
        }
        else if(BUTTON_ACTIVE_LOW == btn->button_connection){
            if(GPIO_HIGH == Pin_Logic_Status){
                *btn_state = BUTTON_RELEASED;
            }
            else{
                *btn_state = BUTTON_PRESSED;
            }
        }
        else{/* NOTHING */}
        ret = E_OK;
    }
    return ret;
}

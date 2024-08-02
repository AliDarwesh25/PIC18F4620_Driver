/* 
 * File:   ecu_keypad.c
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on April 20, 2024, 5:13 PM
 */
#include "ecu_keypad.h"
static const uint8 btn_values[ECU_KEYPAD_ROWS][ECU_KEYPAD_COLUMNS] ={
                                                                    {'7','8','9','/'},
                                                                    {'4','5','6','*'},
                                                                    {'1','2','3','-'},
                                                                    {'#','0','=','+'}
                                                                    };

/**
 * @brief Initialize the keypad assigned pins
 * @param _keypad_obj pointer to the keypad configurations
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType keypad_initialize(const keypad_t * _keypad_obj){
    Std_ReturnType ret = E_OK;
    uint8 rows_counter = ZERO_INIT,columns_counter = ZERO_INIT;
    if(NULL == _keypad_obj){
        ret = E_NOT_OK;
    }
    else{ 
        for(rows_counter = ZERO_INIT;rows_counter < ECU_KEYPAD_ROWS;rows_counter++){
            ret = gpio_pin_initalize(&(_keypad_obj->keypad_row_pins[rows_counter]));
        }
        for(columns_counter = ZERO_INIT;columns_counter < ECU_KEYPAD_COLUMNS;columns_counter++){
            ret = gpio_pin_direction_initalize(&(_keypad_obj->keypad_columns_pins[columns_counter])); 
        }
    }
    return ret;  
}

/**
 * @brief Get the value of the button pressed by the user and performing the scanning algorithm
 * @param _keypad_obj pointer to the keypad configurations
 * @param value Value of the button pressed by the user
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action 
 */
Std_ReturnType keypad_get_value(const keypad_t * _keypad_obj,uint8 *value){
    Std_ReturnType ret = E_OK;
    uint8 column_logic = ZERO_INIT;
    uint8 rows_counter = ZERO_INIT,columns_counter = ZERO_INIT,counter = ZERO_INIT;
    if((NULL == _keypad_obj)||(NULL == value)){
        ret = E_NOT_OK;
    }
    else{ 
        for(rows_counter = ZERO_INIT;rows_counter < ECU_KEYPAD_ROWS;rows_counter++){
            for(counter = ZERO_INIT;counter < ECU_KEYPAD_ROWS;counter++){
                gpio_pin_write_logic(&(_keypad_obj->keypad_row_pins[counter]),GPIO_LOW);
            }
            gpio_pin_write_logic(&(_keypad_obj->keypad_row_pins[rows_counter]),GPIO_HIGH);
            for(columns_counter = ZERO_INIT;columns_counter < ECU_KEYPAD_COLUMNS;columns_counter++){
            ret = gpio_pin_read_logic(&(_keypad_obj->keypad_columns_pins[columns_counter]),&column_logic);
            if(column_logic == GPIO_HIGH){
                *value = btn_values[rows_counter][columns_counter];
            }
        }
        }
    }
    return ret; 
}

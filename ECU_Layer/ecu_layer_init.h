/* 
 * File:   ecu_layer_init.h
 * Author: Ali
 *
 * Created on April 20, 2024, 5:47 PM
 */

#ifndef ECU_LAYER_INIT_H
#define	ECU_LAYER_INIT_H

/* Section : includes */
#include "Led/ecu_led.h"
#include "button/ecu_button.h"
#include "relay/ecu_relay.h"
#include "Dc_Motor/ecu_dc_motor.h"
#include "7_segment/ecu_seven_segment.h"
#include "Keypad/ecu_keypad.h"
#include "chr_LCD/ecu_chr_lcd.h"
#include "../MCAL_Layer/EEPROM/hal_eeprom.h"
#include "../MCAL_Layer/ADC/hal_adc.h"
#include "../MCAL_Layer/Timer0/hal_timer0.h"
#include "../MCAL_Layer/Timer1/hal_timer1.h"
#include "../MCAL_Layer/Timer2/hal_timer2.h"
#include "../MCAL_Layer/Timer3/hal_timer3.h"
#include "../MCAL_Layer/CCP/hal_ccp.h"
#include "../MCAL_Layer/usart/hal_usart.h"
#include "../MCAL_Layer/SPI/hal_spi.h"
#include "../MCAL_Layer/I2C/hal_i2c.h"
/* Section : Macro Declarations*/

/* Section : Macro Functions Declarations*/

/* Section : Data Type Declarations*/

/* Section : Function Declarations*/
void ecu_layer_initialize(void);

#endif	/* ECU_LAYER_INIT_H */


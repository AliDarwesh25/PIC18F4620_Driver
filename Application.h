/* 
 * File:   Application.h
 * Author: Ali
 *
 * Created on January 23, 2024, 7:16 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* Section : includes */
#include "ECU_Layer/ecu_layer_init.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"

/* Section : Macro Declarations*/

/* Section : Macro Functions Declarations*/

/* Section : Data Type Declarations*/
extern keypad_t keypad1;
extern led_t led1;
extern chr_lcd_8bit_t lcd2;
extern chr_lcd_4bit_t lcd1;
/* Section : Function Declarations*/
void application_intialize(void);;

#endif	/* APPLICATION_H */


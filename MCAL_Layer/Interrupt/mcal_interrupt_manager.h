/* 
 * File:   mcal_interrupt_manager.h
 * Author: Ali
 *
 * Created on June 21, 2024, 8:41 PM
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H

/* Section : includes */
#include "mcal_interrupt_config.h"

/* Section : Macro Declarations*/

/* Section : Macro Functions Declarations*/

/* Section : Data Type Declarations*/

/* Section : Function Declarations*/
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);
void RB4_ISR(uint8 RB4_source);
void RB5_ISR(uint8 RB5_source);
void RB6_ISR(uint8 RB6_source);
void RB7_ISR(uint8 RB7_source);
void ADC_ISR(void);
void Timer0_ISR(void);
void Timer1_ISR(void);
void Timer2_ISR(void);
void Timer3_ISR(void);
void CCP1_ISR(void);
void CCP2_ISR(void);
void EUSAER_TX_ISR(void);
void EUSAER_RX_ISR(void);
void MSSP_SPI_ISR(void);
void MSSP_I2C_ISR(void);
void MSSP_I2C_BC_ISR(void);
#endif	/* MCAL_INTERRUPT_MANAGER_H */


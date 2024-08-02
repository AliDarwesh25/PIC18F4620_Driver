/* 
 * File:   hal_timer2.h
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on June 27, 2024, 10:27 PM
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H

/* Section : includes */

#include "../proc/pic18f4620.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations*/

/* Timer2 Input Clock Post-scaler */
#define TIMER2_POSTSCALER_DIV_BY_1       0
#define TIMER2_POSTSCALER_DIV_BY_2       1
#define TIMER2_POSTSCALER_DIV_BY_3       2
#define TIMER2_POSTSCALER_DIV_BY_4       3
#define TIMER2_POSTSCALER_DIV_BY_5       4
#define TIMER2_POSTSCALER_DIV_BY_6       5
#define TIMER2_POSTSCALER_DIV_BY_7       6
#define TIMER2_POSTSCALER_DIV_BY_8       7
#define TIMER2_POSTSCALER_DIV_BY_9       8
#define TIMER2_POSTSCALER_DIV_BY_10      9
#define TIMER2_POSTSCALER_DIV_BY_11      10
#define TIMER2_POSTSCALER_DIV_BY_12      11
#define TIMER2_POSTSCALER_DIV_BY_13      12
#define TIMER2_POSTSCALER_DIV_BY_14      13
#define TIMER2_POSTSCALER_DIV_BY_15      14
#define TIMER2_POSTSCALER_DIV_BY_16      15


/* Timer2 Input Clock Pre-scaler */
#define TIMER2_PRESCALER_DIV_BY_1        0
#define TIMER2_PRESCALER_DIV_BY_4        1
#define TIMER2_PRESCALER_DIV_BY_16       2



/* Section : Macro Functions Declarations*/

/* Enable or Disable Timer2 Module */
#define TIMER2_MODULE_ENABLE()                (T2CONbits.TMR2ON = 1)
#define TIMER2_MODULE_DISABLE()               (T2CONbits.TMR2ON = 0)

/* Timer2 Input Clock Post-scaler */
#define TIMER2_PRESCALER_SELECT(_PRESCALER) (T2CONbits.T2CKPS = _PRESCALER)

/* Timer2 Input Clock Post-scaler */
#define TIMER2_POSTSCALER_SELECT(_POSTSCALER) (T2CONbits.TOUTPS = _POSTSCALER)

/* Section : Data Type Declarations*/

typedef struct{
#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (* TIMER2_InterruptHandler) (void);
    interrupt_priority_cfg priority;
#endif
    uint8 timer2_preload_value;        /* Preload value configuration */
    uint8 timer2_postscaler_value : 4 ; /* postscaler value configuration */
    uint8 timer2_prescaler_value  : 2 ; /* Prescale value configuration */
    uint8 timer2_reserved         : 2 ;
}timer2_t;

/* Section : Function Declarations*/

Std_ReturnType Timer2_Init(const timer2_t *_timer);
Std_ReturnType Timer2_DeInit(const timer2_t *_timer);
Std_ReturnType Timer2_Write_Value(const timer2_t *_timer, uint8 _value);
Std_ReturnType Timer2_Read_Value(const timer2_t *_timer, uint8 *_value);

#endif	/* HAL_TIMER2_H */


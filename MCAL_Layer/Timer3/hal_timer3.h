/* 
 * File:   hal_timer3.h
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on June 27, 2024, 11:36 PM
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H

/* Section : includes */

#include "../proc/pic18f4620.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations*/

/* Timer3 Clock Source (Timer or Counter) */
#define TIMER3_TIMER_MODE              0
#define TIMER3_COUNTER_MODE            1
/* Timer3 External Clock Input Synchronization */
#define TIMER3_ASYNC_COUNTER_MODE      1
#define TIMER3_SYNC_COUNTER_MODE       0
/* Timer3 Input Clock Pre-scaler */
#define TIMER3_PRESCALER_DIV_BY_1      0
#define TIMER3_PRESCALER_DIV_BY_2      1
#define TIMER3_PRESCALER_DIV_BY_4      2
#define TIMER3_PRESCALER_DIV_BY_8      3
/* 16-Bit Read/Write Mode */
#define TIMER3_RW_REG_8BIT_MODE        0
#define TIMER3_RW_REG_16BIT_MODE       1

/* Section : Macro Functions Declarations*/

/* Enable or Disable Timer3 Module */
#define TIMER3_MODULE_ENABLE()                (T3CONbits.TMR3ON = 1)
#define TIMER3_MODULE_DISABLE()               (T3CONbits.TMR3ON = 0)
/*  Timer3 Clock Source (Timer or Counter) */
#define TIMER3_TIMER_MODE_ENABLE()            (T3CONbits.TMR3CS = 0)
#define TIMER3_COUNTER_MODE_ENABLE()          (T3CONbits.TMR3CS = 1)
/*  Timer3 External Clock Input Synchronization */
#define TIMER3_ASYNC_COUNTER_MODE_ENABLE()    (T3CONbits.T3SYNC = 1)
#define TIMER3_SYNC_COUNTER_MODE_ENABLE()     (T3CONbits.T3SYNC = 0)
/* Timer1 Input Clock Pre-scaler */
#define TIMER3_PRESCALER_SELECT(_PRESCALER)   (T3CONbits.T3CKPS = _PRESCALER)
/* 16-Bit Read/Write Mode */
#define TIMER3_RW_REG_8BIT_MODE_ENABLE()      (T3CONbits.RD16 = 0)
#define TIMER3_RW_REG_16BIT_MODE_ENABLE()     (T3CONbits.RD16 = 1)

/* Section : Data Type Declarations*/

typedef struct{
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (* TIMER3_InterruptHandler) (void);
    interrupt_priority_cfg priority;
#endif
    uint16 timer3_preload_value;        /* Preload value configuration */
    uint8  timer3_prescaler_value : 2 ; /* Prescale value configuration */
    uint8  timer3_mode            : 1 ; /* Timer Mode / Counter Mode */
    uint8  timer3_counter_mode    : 1 ; /* Synchronous or ASynchronous Counter */
    uint8  timer3_reg_rw_mode     : 2 ; /* One 16-bit operation or Two 8-bit operations */
    uint8  timer3_reserved        : 3 ;
}timer3_t;

/* Section : Function Declarations*/

Std_ReturnType Timer3_Init(const timer3_t *_timer);
Std_ReturnType Timer3_DeInit(const timer3_t *_timer);
Std_ReturnType Timer3_Write_Value(const timer3_t *_timer, uint16 _value);
Std_ReturnType Timer3_Read_Value(const timer3_t *_timer, uint16 *_value);

#endif	/* HAL_TIMER3_H */


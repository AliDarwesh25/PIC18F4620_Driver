/* 
 * File:   hal_timer2.c
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on June 27, 2024, 10:27 PM
 */

#include "hal_timer2.h"

#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void(*TIMER2_InterruptHandler) (void) = NULL;
#endif

static volatile uint8 timer2_preload = ZERO_INIT;

Std_ReturnType Timer2_Init(const timer2_t *_timer){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _timer)){
        ret = E_NOT_OK;
    }
    else{
        TIMER2_MODULE_DISABLE();
        TIMER2_PRESCALER_SELECT(_timer->timer2_prescaler_value);
        TIMER2_POSTSCALER_SELECT(_timer->timer2_postscaler_value);
        TMR2 = _timer->timer2_preload_value;
        timer2_preload = _timer->timer2_preload_value;
        #if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptEnable();
        TIMER2_InterruptFlagClear();
        TIMER2_InterruptHandler = _timer->TIMER2_InterruptHandler;
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER2_HighPrioritySet();
        }
        else if (INTERRUPT_LOW_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER2_LowPrioritySet();
        }
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
        #endif
        TIMER2_MODULE_ENABLE();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType Timer2_DeInit(const timer2_t *_timer){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _timer)){
        ret = E_NOT_OK;
    }
    else{
        TIMER2_MODULE_DISABLE();
        #if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptDisable();
        #endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType Timer2_Write_Value(const timer2_t *_timer, uint8 _value){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _timer)){
        ret = E_NOT_OK;
    }
    else{
        TMR2 = _value;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType Timer2_Read_Value(const timer2_t *_timer, uint8 *_value){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _timer)||(NULL == _value)){
        ret = E_NOT_OK;
    }
    else{
        *_value = TMR2;
        ret = E_OK;
    }
    return ret;
}


void Timer2_ISR(void){
    TIMER2_InterruptFlagClear();
    TMR2 = timer2_preload;
    if(TIMER2_InterruptHandler){
        TIMER2_InterruptHandler();
    }
}


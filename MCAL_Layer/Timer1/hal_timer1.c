/* 
 * File:   hal_timer1.c
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on June 27, 2024, 8:11 PM
 */

#include "hal_timer1.h"

#if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void(*TIMER1_InterruptHandler) (void) = NULL;
#endif

static volatile uint16 timer1_preload = ZERO_INIT;
static inline void Timer1_Mode_Select(const timer1_t *_timer);

Std_ReturnType Timer1_Init(const timer1_t *_timer){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _timer)){
        ret = E_NOT_OK;
    }
    else{
        TIMER1_MODULE_DISABLE();
        TIMER1_PRESCALER_SELECT(_timer->timer1_prescaler_value);
        Timer1_Mode_Select(_timer);
        TMR1H = (_timer->timer1_preload_value) >> 8;
        TMR1L = (uint8)(_timer->timer1_preload_value);
        timer1_preload = _timer->timer1_preload_value;
        #if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptEnable();
        TIMER1_InterruptFlagClear();
        TIMER1_InterruptHandler = _timer->TIMER1_InterruptHandler;
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER1_HighPrioritySet();
        }
        else if (INTERRUPT_LOW_PRIORITY == _timer->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER1_LowPrioritySet();
        }
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
        #endif
        TIMER1_MODULE_ENABLE();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType Timer1_DeInit(const timer1_t *_timer){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _timer)){
        ret = E_NOT_OK;
    }
    else{
        TIMER1_MODULE_DISABLE();
        #if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptDisable();
        #endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType Timer1_Write_Value(const timer1_t *_timer, uint16 _value){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _timer)){
        ret = E_NOT_OK;
    }
    else{
        TMR1H = (_value) >> 8;
        TMR1L = (uint8)(_value);
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType Timer1_Read_Value(const timer1_t *_timer, uint16 *_value){
    Std_ReturnType ret = E_NOT_OK;
    uint8 l_tmr1l = ZERO_INIT,l_tmr1h = ZERO_INIT;
    if((NULL == _timer)||(NULL == _value)){
        ret = E_NOT_OK;
    }
    else{
        l_tmr1l = TMR1L;
        l_tmr1h = TMR1H;
        *_value = (uint16)(((l_tmr1h)<<8)+l_tmr1l);
        ret = E_OK;
    }
    return ret;
}


void Timer1_ISR(void){
    TIMER1_InterruptFlagClear();
    TMR1H = (timer1_preload) >> 8;
    TMR1L = (uint8)(timer1_preload);
    if(TIMER1_InterruptHandler){
        TIMER1_InterruptHandler();
    }
}


static inline void Timer1_Mode_Select(const timer1_t *_timer){
    if(TIMER1_TIMER_MODE == _timer->timer1_mode){
        TIMER1_TIMER_MODE_ENABLE();
    }
    else if(TIMER1_COUNTER_MODE == _timer->timer1_mode){
        TIMER1_COUNTER_MODE_ENABLE();
        if(TIMER1_ASYNC_COUNTER_MODE == _timer->timer1_counter_mode){
            TIMER1_ASYNC_COUNTER_MODE_ENABLE();
        }
        else if(TIMER1_SYNC_COUNTER_MODE == _timer->timer1_counter_mode){
            TIMER1_SYNC_COUNTER_MODE_ENABLE();
        }
        else{/*Nothing*/}
    }
    else{/*Nothing*/}
}
/* 
 * File:   hal_ccp.c
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on June 28, 2024, 8:11 PM
 */

#include "hal_ccp.h"

#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void(*CCP1_InterruptHandler) (void) = NULL;
#endif

#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void(*CCP2_InterruptHandler) (void) = NULL;
#endif

static void CCP_Interrupt_Config(const ccp_t *_ccp_obj);
static void CCP_PEM_Mode_Config(const ccp_t *_ccp_obj);
static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj);
static Std_ReturnType CCP_capture_Mode_Config(const ccp_t *_ccp_obj);
static Std_ReturnType CCP_Compare_Mode_Config(const ccp_t *_ccp_obj);

Std_ReturnType CCP_Init(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        /* CCP Module Disable */
        if(_ccp_obj->ccp_inst == CCP1_INST){
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
        }
        else if(_ccp_obj->ccp_inst == CCP2_INST){
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
        }
        else{/*Nothing*/}
        
        /* CCP Module Capture Mode Initialization */
        if(CCP_CFG_CAPTURE_MODE_SELECTED == _ccp_obj->ccp_mode){
            ret = CCP_capture_Mode_Config(_ccp_obj); 
        }
        /* CCP Module Compare Mode Initialization */
        else if(CCP_CFG_COMPARE_MODE_SELECTED == _ccp_obj->ccp_mode){
            CCP_Compare_Mode_Config(_ccp_obj);
        }
#if(CCP1_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED)||(CCP2_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED)
        /* CCP Module PWM Mode Initialization */
        else if(CCP_CFG_PWM_MODE_SELECTED == _ccp_obj->ccp_mode){
            CCP_PEM_Mode_Config(_ccp_obj);
        }
        else{/*Nothing*/}
#endif
        /* PIN Configurations */
        ret = gpio_pin_initalize(&(_ccp_obj->ccp_pin));
        /* Interrupt Configurations for CCP1 and CCP2 Modules */
        CCP_Interrupt_Config(_ccp_obj);
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType CCP_DeInit(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        if(_ccp_obj->ccp_inst == CCP1_INST){
            CCP1_SET_MODE(CCP_MODULE_DISABLE);/* CCP1 Module Disable */
            /* Interrupt Configurations */
#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptDisable();
#endif
        }
        else if(_ccp_obj->ccp_inst == CCP2_INST){
            CCP2_SET_MODE(CCP_MODULE_DISABLE);/* CCP2 Module Disable */
            /* Interrupt Configurations */
#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP2_InterruptDisable();
#endif
        }
        else{/*Nothing*/}

        ret = E_OK;
    }
    return ret;
}


#if(CCP1_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED)
Std_ReturnType CCP_IsCapturedDataReady(uint8 *_capture_status){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _capture_status){
        ret = E_NOT_OK;
    }
    else{
        if(PIR1bits.CCP1IF == CCP_CAPTURE_READY){
            *_capture_status = CCP_CAPTURE_READY;

            CCP1_InterruptFlagClear();

        }
        else{
            *_capture_status = CCP_CAPTURE_NOT_READY;
        }
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType CCP_Capture_Mode_Read_Value(uint16 *capture_value){
    Std_ReturnType ret = E_NOT_OK;
    CCP_REG_T capture_temp_value = {.ccpr_low = ZERO_INIT, .ccpr_high = ZERO_INIT};
    if(NULL == capture_value){
        ret = E_NOT_OK;
    }
    else{
        /* Copy captured value */
        capture_temp_value.ccpr_low = CCPR1L;
        capture_temp_value.ccpr_high = CCPR1H;
        /* Write the 16-bit capture value */
        *capture_value = capture_temp_value.ccpr_16bit;
        ret = E_OK;
    }
    return ret;
}

#endif

#if(CCP1_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED)
Std_ReturnType CCP_IsCompareComplete(uint8 *_compare_status){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _compare_status){
        ret = E_NOT_OK;
    }
    else{
        if(CCP_COMPARE_READY == PIR1bits.CCP1IF){
            *_compare_status = CCP_COMPARE_READY;
            CCP1_InterruptFlagClear();
        }
        else{
            *_compare_status = CCP_COMPARE_NOT_READY;
        }
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType CCP_Compare__Mode_Set_Value(const ccp_t *_ccp_obj,uint16 compare_value){
    Std_ReturnType ret = E_NOT_OK;
    CCP_REG_T capture_temp_value = {.ccpr_low = 0, .ccpr_high = 0};
    
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Copy compared value */
        capture_temp_value.ccpr_16bit = compare_value;

        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCPR1L = capture_temp_value.ccpr_low;
            CCPR1H = capture_temp_value.ccpr_high;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCPR2L = capture_temp_value.ccpr_low;
            CCPR2H = capture_temp_value.ccpr_high;
        }
        else{ /* Nothing */ }
        
        ret = E_OK;
    }
    return ret;
}

#endif

#if(CCP1_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED)
Std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj,const uint8 _duty){
    Std_ReturnType ret = E_NOT_OK;
    uint16 l_duty_temp = 0;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        l_duty_temp = (uint16)(4 * (PR2 +1) * (_duty / 100.0));
        if(_ccp_obj->ccp_inst == CCP1_INST){
            CCP1CONbits.DC1B = (uint8)(l_duty_temp & 0x0003);
            CCPR1L = (uint8)(l_duty_temp >> 2);
        }
        else if(_ccp_obj->ccp_inst == CCP2_INST){
            CCP2CONbits.DC2B = (uint8)(l_duty_temp & 0x0003);
            CCPR2L = (uint8)(l_duty_temp >> 2);
        }
        ret = E_OK;
    }

    
    ret = E_OK;
    return ret;
}

Std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        if(_ccp_obj->ccp_inst == CCP1_INST){
            CCP1CONbits.CCP1M = CCP_PWM_MODE;
        }
        else if(_ccp_obj->ccp_inst == CCP2_INST){
            CCP2CONbits.CCP2M = CCP_PWM_MODE;
        }
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_NOT_OK;
    
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }
    else{
        if(_ccp_obj->ccp_inst == CCP1_INST){
            CCP1CONbits.CCP1M = CCP_MODULE_DISABLE;
        }
        else if(_ccp_obj->ccp_inst == CCP2_INST){
            CCP2CONbits.CCP2M = CCP_MODULE_DISABLE;
        }
        ret = E_OK;
    }
    return ret;
}

#endif


void CCP1_ISR(void){
    CCP1_InterruptFlagClear();
    if(CCP1_InterruptHandler){
        CCP1_InterruptHandler();
    }
}


void CCP2_ISR(void){
    CCP2_InterruptFlagClear();
    if(CCP2_InterruptHandler){
        CCP2_InterruptHandler();
    }
}


#if(CCP1_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED)||(CCP2_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED)
static void CCP_PEM_Mode_Config(const ccp_t *_ccp_obj){
    if(_ccp_obj->ccp_inst == CCP1_INST){
        if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant){
            CCP1_SET_MODE(CCP_PWM_MODE);
        }
        else{/*Nothing*/} 
    }
    else if(_ccp_obj->ccp_inst == CCP2_INST){
        if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant){
            CCP2_SET_MODE(CCP_PWM_MODE);
        }
        else{/*Nothing*/}
    }  
    /* PWM Frequency Initialization */
    PR2 = (uint8)((_XTAL_FREQ / (_ccp_obj->PWM_Frequency * 4.0 * _ccp_obj->timer2_postscaler_value * _ccp_obj->timer2_prescaler_value)) -1);
        
}
#endif

static void CCP_Interrupt_Config(const ccp_t *_ccp_obj){
    /* CCP1 Interrupt Configurations */
#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    CCP1_InterruptDisable();
    CCP1_InterruptFlagClear();
    CCP1_InterruptHandler = _ccp_obj->CCP1_InterruptHandler;
    CCP1_InterruptEnable();
    /* Interrupt Priority Configurations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->CCP1_priority){
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        CCP1_HighPrioritySet();
    }
    else if (INTERRUPT_LOW_PRIORITY == _ccp_obj->CCP1_priority){
        INTERRUPT_GlobalInterruptLowEnable();
        CCP1_LowPrioritySet();
    }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        
#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    CCP2_InterruptEnable();
    CCP2_InterruptFlagClear();
    CCP2_InterruptHandler = _ccp_obj->CCP2_InterruptHandler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->CCP2_priority){
        INTERRUPT_GlobalInterruptHighEnable();
        CCP2_HighPrioritySet();
    }
    else if (INTERRUPT_LOW_PRIORITY == _ccp_obj->CCP2_priority){
        INTERRUPT_GlobalInterruptLowEnable();
        CCP2_LowPrioritySet();
    }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
}

static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj){
    if(CCP1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 1;
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for CCP2 */
        /* Timer1 is the capture/compare clock source for CCP1 */
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_CCP2_TIMER1 == _ccp_obj->ccp_capture_timer){
        /* Timer1 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else{/*Nothing*/}
}

static Std_ReturnType CCP_capture_Mode_Config(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(_ccp_obj->ccp_inst == CCP1_INST){
        /* CCP1 Module Capture variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1_FALLING_EDGE : CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE); break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE  : CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE); break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE  : CCP1_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE); break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE : CCP1_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE); break;
            default : ret = E_NOT_OK;/* Not supported variant */
        }
    }
    else if(_ccp_obj->ccp_inst == CCP2_INST){
        /* CCP2 Module Capture variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1_FALLING_EDGE : CCP2_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE); break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE  : CCP2_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE); break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE  : CCP2_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE); break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE : CCP2_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE); break;
            default : ret = E_NOT_OK;/* Not supported variant */
        }    
    }
    CCP_Mode_Timer_Select(_ccp_obj);
    return ret;
}


static Std_ReturnType CCP_Compare_Mode_Config(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    if(_ccp_obj->ccp_inst == CCP1_INST){
        /* CCP1 Module Compare variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_COMPARE_MODE_SET_PIN_LOW      : CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW); break;
            case CCP_COMPARE_MODE_SET_PIN_HIGH     : CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH); break;
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH  : CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH); break;
            case CCP_COMPARE_MODE_GEN_SW_INTERRUPT : CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
            case CCP_COMPARE_MODE_GEN_EVENT        : CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT); break;
            default : ret = E_NOT_OK;/* Not supported variant */

    }
    }
    else if(_ccp_obj->ccp_inst == CCP2_INST){
        /* CCP1 Module Compare variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_COMPARE_MODE_SET_PIN_LOW      : CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW); break;
            case CCP_COMPARE_MODE_SET_PIN_HIGH     : CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH); break;
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH  : CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH); break;
            case CCP_COMPARE_MODE_GEN_SW_INTERRUPT : CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
            case CCP_COMPARE_MODE_GEN_EVENT        : CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT); break;
            default : ret = E_NOT_OK;/* Not supported variant */
        }
    }    
    CCP_Mode_Timer_Select(_ccp_obj);
    return ret;
}
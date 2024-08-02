
/* 
 * File:   mcal_external_interrupt.c
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on June 21, 2024, 8:40 PM
 */

#include "mcal_external_interrupt.h"

/*Pointer  to function to hold callback function for INTx*/
static InterruptHandler INT0_InterruptHandler  = NULL;
static InterruptHandler INT1_InterruptHandler  = NULL;
static InterruptHandler INT2_InterruptHandler  = NULL;
/*Pointer  to function to hold callback function for RBx*/
static InterruptHandler RB4_InterruptHandler_High = NULL;
static InterruptHandler RB4_InterruptHandler_Low  = NULL;
static InterruptHandler RB5_InterruptHandler_High = NULL;
static InterruptHandler RB5_InterruptHandler_Low  = NULL;
static InterruptHandler RB6_InterruptHandler_High = NULL;
static InterruptHandler RB6_InterruptHandler_Low  = NULL;
static InterruptHandler RB7_InterruptHandler_High = NULL;
static InterruptHandler RB7_InterruptHandler_Low  = NULL;

static Std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Priority_Init(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Edge_Init(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Pin_Init(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t *int_obj);

static Std_ReturnType INT0_SetInterruptHandler(void(*InterruptHandler)(void));
static Std_ReturnType INT1_SetInterruptHandler(void(*InterruptHandler)(void));
static Std_ReturnType INT2_SetInterruptHandler(void(*InterruptHandler)(void));
static Std_ReturnType interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t *int_obj);


static Std_ReturnType Interrupt_RBx_Enable(const interrupt_RBx_t *int_obj);
static Std_ReturnType Interrupt_RBx_Disable(const interrupt_RBx_t *int_obj);
static Std_ReturnType Interrupt_RBx_Priority_Init(const interrupt_RBx_t *int_obj);
static Std_ReturnType Interrupt_RBx_Pin_Init(const interrupt_RBx_t *int_obj);

/**
 * @brief Initialize the external interrupt INT0, INT1 and INT2
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType interrupt_INTx_init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable the External interrupt for INTx*/
        ret = Interrupt_INTx_Disable(int_obj);
        /*Clear Interrupt Flag: External interrupt did not occur*/
        ret |= Interrupt_INTx_Clear_Flag(int_obj);
        /*Configure External interrupt edge*/
        ret |= Interrupt_INTx_Edge_Init(int_obj);
        /*Configure External interrupt I/O pin*/
        ret |= Interrupt_INTx_Pin_Init(int_obj);
        /*Configure Default interrupt callback */
        ret |= interrupt_INTx_SetInterruptHandler(int_obj);
        /*enable the External interrupt for INTx*/
        ret |= Interrupt_INTx_Enable(int_obj);
    }
    return ret;
}

/**
 * External Interrupt 0 MCAL Helper function 
 */
void INT0_ISR(void){
    /*The INT0 external interrupt occurred (must be cleared in software)*/
    EXT_INT0_InterruptFlagClear();
    /*Code*/
    
    /*Callback function gets called every time this ISR executes*/
    if(INT0_InterruptHandler){
        INT0_InterruptHandler();
    }
}

/**
 * External Interrupt 1 MCAL Helper function 
 */
void INT1_ISR(void){
    /*The INT1 external interrupt occurred (must be cleared in software)*/
    EXT_INT1_InterruptFlagClear();
    /*Code*/
    
    /*Callback function gets called every time this ISR executes*/
    if(INT1_InterruptHandler){INT1_InterruptHandler();}
}

/**
 * External Interrupt 2 MCAL Helper function 
 */
void INT2_ISR(void){
    /*The INT2 external interrupt occurred (must be cleared in software)*/
    EXT_INT2_InterruptFlagClear();
    /*Code*/
    
    /*Callback function gets called every time this ISR executes*/
    if(INT2_InterruptHandler){INT2_InterruptHandler();}
}

/**
 * External Interrupt RB4 MCAL Helper function 
 */
void RB4_ISR(uint8 RB4_source){
    /*The RB4 external interrupt occurred (must be cleared in software)*/
    EXT_RBx_InterruptFlagClear();
    /*Code*/
    
    /*Callback function gets called every time this ISR executes*/
    if (0 == RB4_source){
        if(RB4_InterruptHandler_High){RB4_InterruptHandler_High();}
    }
    else if (1 == RB4_source){
        if(RB4_InterruptHandler_Low){RB4_InterruptHandler_Low();}
    }
    else{/*nothing*/}
    
}

/**
 * External Interrupt RB5 MCAL Helper function 
 */
void RB5_ISR(uint8 RB5_source){
    /*The RB4 external interrupt occurred (must be cleared in software)*/
    EXT_RBx_InterruptFlagClear();
    /*Code*/
    
    /*Callback function gets called every time this ISR executes*/
    if (0 == RB5_source){
        if(RB5_InterruptHandler_High){RB5_InterruptHandler_High();}
    }
    else if (1 == RB5_source){
        if(RB5_InterruptHandler_Low){RB5_InterruptHandler_Low();}
    }
    else{/*nothing*/}
}

/**
 * External Interrupt RB6 MCAL Helper function 
 */
void RB6_ISR(uint8 RB6_source){
    /*The RB4 external interrupt occurred (must be cleared in software)*/
    EXT_RBx_InterruptFlagClear();
    /*Code*/
    
    /*Callback function gets called every time this ISR executes*/
    if (0 == RB6_source){
        if(RB6_InterruptHandler_High){RB6_InterruptHandler_High();}
    }
    else if (1 == RB6_source){
        if(RB6_InterruptHandler_Low){RB6_InterruptHandler_Low();}
    }
    else{/*nothing*/}
}

/**
 * External Interrupt RB7 MCAL Helper function 
 */
void RB7_ISR(uint8 RB7_source){
    /*The RB4 external interrupt occurred (must be cleared in software)*/
    EXT_RBx_InterruptFlagClear();
    /*Code*/
    
    /*Callback function gets called every time this ISR executes*/
    if (0 == RB7_source){
        if(RB7_InterruptHandler_High){RB7_InterruptHandler_High();}
    }
    else if (1 == RB7_source){
        if(RB7_InterruptHandler_Low){RB7_InterruptHandler_Low();}
    }
    else{/*nothing*/}
}

/**
 * @brief DeInitialize the interrupt module
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType interrupt_INTx_Deinit(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        Interrupt_INTx_Disable(int_obj);
    }
    return ret;
}

/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType interrupt_RBx_init(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable the External interrupt for RBX*/
        EXT_RBx_InterruptDisable();
        /*This routine clears the interrupt flag for the the external interrupt, RBx */
        EXT_RBx_InterruptFlagClear();
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelsEnable();
            if(INTERRUPT_LOW_PRIORITY == int_obj->priority){
                /*This macro will enable low priority global interrupt */
                INTERRUPT_GlobalInterruptLowEnable();
                /*This routine set the RBx External Interrupt Priority to be Low Priority*/
                EXT_RBx_Priority_Low();
            }
            else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority){
                /*This macro will disable high priority global interrupt */
                INTERRUPT_GlobalInterruptHighEnable();
                /*This routine set the RBx External Interrupt Priority to be High Priority*/
                EXT_RBx_Priority_High();
            }
            else{/*NO THING*/}
        #else
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
        #endif
        /*initialize the RBx pin to be input*/
            ret = gpio_pin_direction_initalize(&(int_obj->mcu_pin));
        /*initialize the callback function*/
        switch(int_obj->mcu_pin.pin){
            case GPIO_PIN4:
                RB4_InterruptHandler_High = int_obj->EXT_InterruptHandler_High;
                RB4_InterruptHandler_Low = int_obj->EXT_InterruptHandler_Low;
                break;
            case GPIO_PIN5:
                RB5_InterruptHandler_High = int_obj->EXT_InterruptHandler_High;
                RB5_InterruptHandler_Low = int_obj->EXT_InterruptHandler_Low;
                break;
            case GPIO_PIN6:
                RB6_InterruptHandler_High = int_obj->EXT_InterruptHandler_High;
                RB6_InterruptHandler_Low = int_obj->EXT_InterruptHandler_Low;
                break;
            case GPIO_PIN7:
                RB7_InterruptHandler_High = int_obj->EXT_InterruptHandler_High;
                RB7_InterruptHandler_Low = int_obj->EXT_InterruptHandler_Low;
                break;
            default:
                ret = E_NOT_OK;
        }    
        /*Enable the External interrupt for RBX*/
        EXT_RBx_InterruptEnable();
    }
    return ret;
}

/**
 * @brief  
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType interrupt_RBx_Deinit(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        /*This routine clears the interrupt enable for the external interrupt, RBx*/
        EXT_RBx_InterruptDisable();     
        /*This routine clears the interrupt flag for the the external interrupt, RBx */
        EXT_RBx_InterruptFlagClear();   
    }
    return ret;
}

/**
 * @brief Enable the INTx interrupt source
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0:
                #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                INTERRUPT_GlobalInterruptHighEnable();
                #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
                #endif
                EXT_INT0_InterruptEnable();
                ret= E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT1:
                #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority){
                    INTERRUPT_GlobalInterruptLowEnable();
                }
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority){
                    INTERRUPT_GlobalInterruptHighEnable();
                }
                else{/*NO THING*/}
                #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
                #endif
                EXT_INT1_InterruptEnable();
                ret= E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT2:
                #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority){
                    INTERRUPT_GlobalInterruptLowEnable();
                }
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority){
                    INTERRUPT_GlobalInterruptHighEnable();
                }
                else{/*NO THING*/}
                #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
                #endif
                EXT_INT2_InterruptEnable();
                ret= E_OK;
                break;
            default: ret= E_NOT_OK;
        }
    }
    return ret;
}

/**
 * @brief Disable the INTx interrupt source
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0:
                EXT_INT0_InterruptDisable();
                ret= E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT1:
                EXT_INT1_InterruptDisable();
                ret= E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT2:
                EXT_INT2_InterruptDisable();
                ret= E_OK;
                break;
            default: ret= E_NOT_OK;
        }
    }
    return ret;
}

/**
 * @brief Define the Priority interrupt levels for INTx
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
static Std_ReturnType Interrupt_INTx_Priority_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT1:
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority){
                    EXT_INT1_LowPrioritySet();
                }
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority){
                    EXT_INT1_HighPrioritySet();
                }
                else{/*NO THING*/}
                ret= E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT2:
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority){
                    EXT_INT2_LowPrioritySet();
                }
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority){
                    EXT_INT2_HighPrioritySet();
                }
                else{/*NOTHING*/}
                ret= E_OK;
                break;
            default: ret= E_NOT_OK;
        }
    }
    return ret;
}
#endif

/**
 * @brief Define the edge for INTx interrupt (Falling edge or Raising edge)
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Edge_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0:
                if(INTERRUPT_FALLING_EDGE == int_obj->edge){
                    EXT_INT0_FallingEdgeSet();
                }
                else if(INTERRUPT_RISING_EDGE == int_obj->edge){
                    EXT_INT0_RisingEdgeSet();
                }
                else{/*NOTHING*/}
                ret= E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT1:
                if(INTERRUPT_FALLING_EDGE == int_obj->edge){
                    EXT_INT1_FallingEdgeSet();
                }
                else if(INTERRUPT_RISING_EDGE == int_obj->edge){
                    EXT_INT1_RisingEdgeSet();
                }
                else{/*NOTHING*/}
                ret= E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT2:
                if(INTERRUPT_FALLING_EDGE == int_obj->edge){
                    EXT_INT2_FallingEdgeSet();
                }
                else if(INTERRUPT_RISING_EDGE == int_obj->edge){
                    EXT_INT2_RisingEdgeSet();
                }
                else{/*NOTHING*/}
                ret= E_OK;
                break;
            default: ret= E_NOT_OK;
        }
    }
    return ret;
}

/**
 * @brief initialize the pin of interrupt as input
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Pin_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_direction_initalize(&(int_obj->mcu_pin));
    }
    return ret;
}

/**
 * @brief Clear the INTx interrupt flag
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0:
                EXT_INT0_InterruptFlagClear();
                ret= E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT1:
                EXT_INT1_InterruptFlagClear();
                ret= E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT2:
                EXT_INT2_InterruptFlagClear();
                ret= E_OK;
                break;
            default: ret= E_NOT_OK;
        }
    }
    return ret;
}

/**
 * @brief Set Interrupt callback for INT0
 * @param InterruptHandler 
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType INT0_SetInterruptHandler(void(*InterruptHandler)(void)){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler){
        ret = E_NOT_OK;
    }
    else{
        INT0_InterruptHandler = InterruptHandler;
        ret = E_OK;
    }
    return ret;
}

/**
 * @brief Set Interrupt callback for INT1
 * @param InterruptHandler 
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType INT1_SetInterruptHandler(void(*InterruptHandler)(void)){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler){
        ret = E_NOT_OK;
    }
    else{
        INT1_InterruptHandler = InterruptHandler;
        ret = E_OK;
    }
    return ret;
}

/**
 * @brief Set Interrupt callback for INT2
 * @param InterruptHandler 
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType INT2_SetInterruptHandler(void(*InterruptHandler)(void)){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler){
        ret = E_NOT_OK;
    }
    else{
        INT2_InterruptHandler = InterruptHandler;
        ret = E_OK;
    }
    return ret;
}

/**
 * @brief Set Interrupt callback for INTx using the last 3 functions
 * @param int_obj  Pointer to the Interrupt configuration object
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
static Std_ReturnType interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0:
                ret = INT0_SetInterruptHandler(int_obj->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT1:
                ret = INT1_SetInterruptHandler(int_obj->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT2:
                ret = INT2_SetInterruptHandler(int_obj->EXT_InterruptHandler);
                break;
            default: ret= E_NOT_OK;
        }
    }
    return ret;
}
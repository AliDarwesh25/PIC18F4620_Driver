/* 
 * File:   hal_usart.c
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on July 2, 2024, 8:16 PM
 */

#include "hal_usart.h"

#if EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void(*EUSART_TxInterruptHandler) (void) = NULL;
#endif

#if EUSART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void(*EUSART_RxInterruptHandler) (void) = NULL;
static void(*EUSART_FramingErrorHandler)(void)= NULL;
static void(*EUSART_OverrunErrorHandler)(void)= NULL;
#endif

static void EUSART_Baud_Rate_Calculation(const usart_t *_eusart);
static void EUSART_ASYNC_TX_Init(const usart_t *_eusart);
static void EUSART_ASYNC_RX_Init(const usart_t *_eusart);

Std_ReturnType EUSART_ASYNC_Init(const usart_t *_eusart){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _eusart){
        ret = E_NOT_OK;
    }
    else{
        RCSTAbits.SPEN = EUSART_MODULE_DISABLE;
        TRISCbits.RC7 = 1;
        TRISCbits.RC6 = 1;
        EUSART_Baud_Rate_Calculation(_eusart);
        EUSART_ASYNC_TX_Init(_eusart);
        EUSART_ASYNC_RX_Init(_eusart);
        RCSTAbits.SPEN = EUSART_MODULE_ENABLE;
    }
    return ret;
}

Std_ReturnType EUSART_ASYNC_DeInit(const usart_t *_eusart){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _eusart){
        ret = E_NOT_OK;
    }
    else{
        RCSTAbits.SPEN = EUSART_MODULE_DISABLE;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType EUSART_ASYNC_ReadByteBlocking(uint8 *_data){
    Std_ReturnType ret = E_OK;
    while(!PIR1bits.RCIF);
    *_data = RCREG;
    return ret;
}

Std_ReturnType EUSART_ASYNC_ReadByteNonBlocking(uint8 *_data){
    Std_ReturnType ret = E_NOT_OK;
    if(1 == PIR1bits.RCIF){
       *_data = RCREG; 
       ret = E_OK;
    }
    else{
        ret = E_NOT_OK;
    }
    return ret;
}

Std_ReturnType EUSART_ASYNC_RX_Restart(void){
    Std_ReturnType ret = E_OK;
    RCSTAbits.CREN = 0;
    RCSTAbits.CREN = 1;
    return ret;
}

Std_ReturnType EUSART_ASYNC_WriteByteBlocking(uint8 _data){
    Std_ReturnType ret = E_OK;
    while(!TXSTAbits.TRMT);
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    EUSART_TX_InterruptEnable();
#endif
    TXREG = _data;
    return ret;
}

Std_ReturnType EUSART_ASYNC_WriteStringBlocking(uint8 *_data,uint16 string_len){
    Std_ReturnType ret = E_OK;
    uint16 char_counter = ZERO_INIT;
    for(char_counter = 0 ; char_counter < string_len ; char_counter++ ){
        EUSART_ASYNC_WriteByteBlocking(_data[char_counter]);
    }
    return ret;
}

Std_ReturnType EUSART_ASYNC_WriteByteNonBlocking(uint8 _data){
    Std_ReturnType ret = E_OK;
    if(1 == PIR1bits.TXIF){
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        EUSART_TX_InterruptEnable();
#endif
        TXREG = _data;
    }
    else{
        ret = E_NOT_OK;
    }
    return ret;
}

Std_ReturnType EUSART_ASYNC_WriteStringNonBlocking(uint8 *_data, uint16 string_len){
    Std_ReturnType ret = E_OK;
    uint16 L_counter = ZERO_INIT;
        for(L_counter = ZERO_INIT; L_counter < string_len; L_counter++){
            ret = EUSART_ASYNC_WriteByteNonBlocking(_data[L_counter]);
        }
    return ret;
}

static void EUSART_Baud_Rate_Calculation(const usart_t *_eusart){
    float Baud_Rate_temp = 0 ;
    switch(_eusart->baudrate_gen_config){
        case BAUDRATE_ASYN_8BIT_LOW_SPEED :        
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUDCONbits.BRG16 = EUSART_8BIT_BAUDRATE_GEN;
            Baud_Rate_temp = ((_XTAL_FREQ/(float)_eusart->baudrate) / 64) - 1;
            break;
        case BAUDRATE_ASYN_8BIT_HIGH_SPEED :        
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUDCONbits.BRG16 = EUSART_8BIT_BAUDRATE_GEN;
            Baud_Rate_temp = ((_XTAL_FREQ/(float)_eusart->baudrate) / 16) - 1;
            break;
        case BAUDRATE_ASYN_16BIT_LOW_SPEED :        
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            Baud_Rate_temp = ((_XTAL_FREQ/(float)_eusart->baudrate) / 16) - 1;
            break;
        case BAUDRATE_ASYN_16BIT_HIGH_SPEED :        
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            Baud_Rate_temp = ((_XTAL_FREQ/(float)_eusart->baudrate) / 4) - 1;
            break;
        case BAUDRATE_SYN_8BIT :        
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_8BIT_BAUDRATE_GEN;
            Baud_Rate_temp = ((_XTAL_FREQ/(float)_eusart->baudrate) / 4) - 1;
            break;
        case BAUDRATE_SYN_16BIT :        
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            Baud_Rate_temp = ((_XTAL_FREQ/(float)_eusart->baudrate) / 4) - 1;
            break;
        default : ;
    }
    SPBRG = (uint8)((uint32)Baud_Rate_temp);
    SPBRGH = (uint8)(((uint32)Baud_Rate_temp) >> 8);
}

static void EUSART_ASYNC_TX_Init(const usart_t *_eusart){
    if(EUSART_ASYNCHRONOUS_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_enable){
        TXSTAbits.TXEN = EUSART_ASYNCHRONOUS_TX_ENABLE;

        EUSART_TxInterruptHandler = _eusart->EUSART_TxDefaultInterruptHandler;

        if(EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_interrupt_enable){
            PIE1bits.TXIE = EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE;
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            EUSART_TX_InterruptEnable();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _eusart->usart_tx_cfg.usart_tx_int_priority){
            INTERRUPT_GlobalInterruptHighEnable();
            EUSART_TX_HighPrioritySet();
        }
        else if (INTERRUPT_LOW_PRIORITY == _eusart->usart_tx_cfg.usart_tx_int_priority){
            INTERRUPT_GlobalInterruptLowEnable();
            EUSART_TX_LowPrioritySet();
        }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        }
        else if(EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE == _eusart->usart_tx_cfg.usart_tx_interrupt_enable){
            PIE1bits.TXIE = EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE;
        }
        else{/*Nothing*/}

        if(EUSART_ASYNCHRONOUS_9BIT_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_9bit_enable){
            TXSTAbits.TX9 = EUSART_ASYNCHRONOUS_9BIT_TX_ENABLE;
        }
        else if(EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE == _eusart->usart_tx_cfg.usart_tx_9bit_enable){
            TXSTAbits.TX9 = EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE;
        }
        else{/*Nothing*/}
    }
    else{/*Nothing*/}
}

static void EUSART_ASYNC_RX_Init(const usart_t *_eusart){
    if(EUSART_ASYNCHRONOUS_RX_ENABLE == _eusart->usart_rx_cfg.usart_rx_enable){
        RCSTAbits.CREN = EUSART_ASYNCHRONOUS_TX_ENABLE;
        EUSART_RxInterruptHandler = _eusart->EUSART_RxDefaultInterruptHandler;
        EUSART_FramingErrorHandler = _eusart->EUSART_FramingErrorHandler;
        EUSART_OverrunErrorHandler = _eusart->EUSART_OverrunErrorHandler;
        if(EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE == _eusart->usart_rx_cfg.usart_rx_interrupt_enable){
            PIE1bits.RCIE = EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE;
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            EUSART_RX_InterruptEnable();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _eusart->usart_rx_cfg.usart_rx_int_priority){
            INTERRUPT_GlobalInterruptHighEnable();
            EUSART_RX_HighPrioritySet();
        }
        else if (INTERRUPT_LOW_PRIORITY == _eusart->usart_rx_cfg.usart_rx_int_priority){
            INTERRUPT_GlobalInterruptLowEnable();
            EUSART_RX_LowPrioritySet();
        }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        }
        else if(EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE == _eusart->usart_rx_cfg.usart_rx_interrupt_enable){
            PIE1bits.RCIE = EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE;
        }
        else{/*Nothing*/}
        
        if(EUSART_ASYNCHRONOUS_9BIT_RX_ENABLE == _eusart->usart_rx_cfg.usart_rx_9bit_enable){
            RCSTAbits.RX9 = EUSART_ASYNCHRONOUS_9BIT_RX_ENABLE;
        }
        else if(EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE == _eusart->usart_rx_cfg.usart_rx_9bit_enable){
            RCSTAbits.RX9 = EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE;
        }
        else{/*Nothing*/}
    }
    else{/*Nothing*/}
}

void EUSAER_TX_ISR(void){
    EUSART_TX_InterruptDisable();
    if(EUSART_TxInterruptHandler){
        EUSART_TxInterruptHandler();
    }
}
void EUSAER_RX_ISR(void){
    if(EUSART_RxInterruptHandler){
        EUSART_RxInterruptHandler();
    }
    if(EUSART_FramingErrorHandler){
        EUSART_FramingErrorHandler();
    }
    if(EUSART_OverrunErrorHandler){
        EUSART_OverrunErrorHandler();
    }
    
}

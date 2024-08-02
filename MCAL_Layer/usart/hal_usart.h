/* 
 * File:   hal_usart.h
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on July 2, 2024, 8:16 PM
 */

#ifndef HAL_USART_H
#define	HAL_USART_H

/* Section : includes */

#include "../proc/pic18f4620.h"
#include "../mcal_std_types.h"
#include "hal_usart_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/* Section : Macro Declarations*/

/* Enable / Disable EUSART Module */
#define EUSART_MODULE_ENABLE                        1
#define EUSART_MODULE_DISABLE                       0

/* Selecting EUSART Working Mode */
#define EUSART_SYNCHRONOUS_MODE                     1
#define EUSART_ASYNCHRONOUS_MODE                    0

/* Baud Rate Generator Asynchronous Speed Mode */
#define EUSART_ASYNCHRONOUS_HIGH_SPEED              1
#define EUSART_ASYNCHRONOUS_LOW_SPEED               0

/* Baud Rate Generator Register Size */
#define EUSART_16BIT_BAUDRATE_GEN                   1
#define EUSART_8BIT_BAUDRATE_GEN                    0

/* EUSART Transmit Enable */
#define EUSART_ASYNCHRONOUS_TX_ENABLE               1
#define EUSART_ASYNCHRONOUS_TX_DISABLE              0

/* EUSART Transmit Interrupt Enable */
#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE     1
#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE    0

/* EUSART 9-Bit Transmit Enable */
#define EUSART_ASYNCHRONOUS_9BIT_TX_ENABLE          1
#define EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE         0

/* EUSART Receiver Enable */
#define EUSART_ASYNCHRONOUS_RX_ENABLE               1
#define EUSART_ASYNCHRONOUS_RX_DISABLE              0

/* EUSART Receiver Interrupt Enable */
#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE     1
#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE    0

/* EUSART 9-Bit Receiver Enable */
#define EUSART_ASYNCHRONOUS_9BIT_RX_ENABLE          1
#define EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE         0

/* EUSART Framing Error */
#define EUSART_FRAMING_ERROR_DETECTED               1
#define EUSART_FRAMING_ERROR_CLEARED                0

/* EUSART Overrun Error */
#define EUSART_OVERRUN_ERROR_DETECTED               1
#define EUSART_OVERRUN_ERROR_CLEARED                0

/* Section : Macro Functions Declarations*/

/* Section : Data Type Declarations*/

typedef enum{
    BAUDRATE_ASYN_8BIT_LOW_SPEED,
    BAUDRATE_ASYN_8BIT_HIGH_SPEED,
    BAUDRATE_ASYN_16BIT_LOW_SPEED,
    BAUDRATE_ASYN_16BIT_HIGH_SPEED,
    BAUDRATE_SYN_8BIT,
    BAUDRATE_SYN_16BIT
}baudrate_gen_t;

typedef struct{
    uint8 usart_tx_reserved             : 5 ;
    uint8 usart_tx_enable               : 1 ;
    uint8 usart_tx_interrupt_enable     : 1 ;
    uint8 usart_tx_9bit_enable          : 1 ;
    interrupt_priority_cfg usart_tx_int_priority;
}usart_tx_cfg_t;

typedef struct{
    uint8 usart_rx_reserved             : 5 ;
    uint8 usart_rx_enable               : 1 ;
    uint8 usart_rx_interrupt_enable     : 1 ;
    uint8 usart_rx_9bit_enable          : 1 ;
    interrupt_priority_cfg usart_rx_int_priority;
}usart_rx_cfg_t;

typedef union{
    struct{
        uint8 usart_tx_reserved         : 6 ;
        uint8 usart_ferr                : 1 ;
        uint8 usart_oerr                : 1 ;
    };
    uint8 status;
}usart_error_status_t;

typedef struct{
    uint32 baudrate;
    baudrate_gen_t baudrate_gen_config;
    usart_tx_cfg_t usart_tx_cfg;
    usart_rx_cfg_t usart_rx_cfg;
    usart_error_status_t error_status;
    void(*EUSART_TxDefaultInterruptHandler)(void);
    void(*EUSART_RxDefaultInterruptHandler)(void);
    void(*EUSART_FramingErrorHandler)(void);
    void(*EUSART_OverrunErrorHandler)(void);
}usart_t;

/* Section : Function Declarations*/
Std_ReturnType EUSART_ASYNC_Init(const usart_t *_eusart);
Std_ReturnType EUSART_ASYNC_DeInit(const usart_t *_eusart);
Std_ReturnType EUSART_ASYNC_RX_Restart(void);

Std_ReturnType EUSART_ASYNC_ReadByteBlocking(uint8 *_data);
Std_ReturnType EUSART_ASYNC_ReadByteNonBlocking(uint8 *_data);

Std_ReturnType EUSART_ASYNC_WriteByteBlocking(uint8 _data);
Std_ReturnType EUSART_ASYNC_WriteStringBlocking(uint8 *_data,uint16 string_len);
Std_ReturnType EUSART_ASYNC_WriteByteNonBlocking(uint8 _data);
Std_ReturnType EUSART_ASYNC_WriteStringNonBlocking(uint8 *_data, uint16 string_len);

#endif	/* HAL_USART_H */


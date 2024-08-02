/* 
 * File:   hal_spi.c
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on July 5, 2024, 5:38 PM
 */

#include "hal_spi.h"

#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (* SPI_InterruptHandler)(void) = NULL;
#endif

static void MSSP_SPI_Interrupt_Init(const SPI_Config *Config);
static void MSSP_SPI_Master_Mode_GPIO_PIN_Configurations(const SPI_Config *Config);
static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_Config *Config);

Std_ReturnType SPI_Init(const SPI_Config *Config){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == Config)){
        ret = E_NOT_OK;
    }
    else{
        /* Disable SPI Module */
        SPI_DISABLE_MODULE();
        /* MSSP SPI Mode Select */
        SSPCON1bits.SSPM = Config->spi_mode;
        /* GPIO PIN Configurations */
        if((SPI_MASTER_FOSC_DIV4 == Config->spi_mode) || (SPI_MASTER_FOSC_DIV16 == Config->spi_mode) || 
             (SPI_MASTER_FOSC_DIV64 == Config->spi_mode) || (SPI_MASTER_FOSC_TMR2 == Config->spi_mode)){
            MSSP_SPI_Master_Mode_GPIO_PIN_Configurations(Config);
        }
        else if((SPI_SLAVE_SS_ENABLE == Config->spi_mode) || (SPI_SLAVE_SS_DISABLE == Config->spi_mode)){
            MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(Config);
        }
        /* Clock Polarity Select */
        SSPCON1bits.CKP = Config->spi_config.ClockPolarity;
        /* SPI Clock Select */
        SSPSTATbits.CKE = Config->spi_config.ClockSelect;
        /* SPI Sample Select */
        SSPSTATbits.SMP = Config->spi_config.SampleSelect;
        /* MSSP SPI Interrupt Configurations*/
        MSSP_SPI_Interrupt_Init(Config);
        /* Enable SPI Module */
        SPI_ENABLE_MODULE();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType SPI_DeInit(const SPI_Config *Config){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == Config)){
        ret = E_NOT_OK;
    }
    else{
        /* Disable SPI Module */
        SPI_DISABLE_MODULE();
        /* Interrupt Configurations */
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        MSSP_SPI_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType SPI_Send_Byte(const SPI_Config *Config, const uint8 _data){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == Config)){
        ret = E_NOT_OK;
    }
    else{
        SSPBUF = _data;
        while(!(PIR1bits.SSPIF));
        PIR1bits.SSPIF = 0;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType SPI_Read_Byte(const SPI_Config *Config, uint8 *_data){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == Config)||(NULL == _data)){
        ret = E_NOT_OK;
    }
    else{
        while(!SSPSTATbits.BF);
        *_data = SSPBUF;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType SPI_Send_Byte_NonBlocking(const SPI_Config *Config, const uint8 _data){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == Config)){
        ret = E_NOT_OK;
    }
    else{
        if(PIR1bits.SSPIF == 0){
            SSPBUF = _data;
        }
        else if(PIR1bits.SSPIF == 1){
            PIR1bits.SSPIF = 0;
        }
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType SPI_Read_Byte_NonBlocking(const SPI_Config *Config, uint8 *_data){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == Config)||(NULL == _data)){
        ret = E_NOT_OK;
    }
    else{
        if(SSPSTATbits.BF == 1){
            *_data = SSPBUF;
        }
        ret = E_OK;
    }
    return ret;
}
void MSSP_SPI_ISR(void){
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    MSSP_SPI_InterruptFlagClear();
    if(SPI_InterruptHandler){
        SPI_InterruptHandler();
    }
#endif
}


static void MSSP_SPI_Interrupt_Init(const SPI_Config *Config){
/* Interrupt Configurations */
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        MSSP_SPI_InterruptEnable();
        MSSP_SPI_InterruptFlagClear();
        SPI_InterruptHandler = Config->MSSP_SPI_InterruptHandler;
/* Interrupt Priority Configurations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == Config->priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalInterruptHighEnable();
            MSSP_SPI_HighPrioritySet();
        }
        else if (INTERRUPT_LOW_PRIORITY == Config->priority){
            /* Enables all unmasked peripheral interrupts */
            INTERRUPT_GlobalInterruptLowEnable();
            MSSP_SPI_LowPrioritySet();
        }
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
        #endif
}

static void MSSP_SPI_Master_Mode_GPIO_PIN_Configurations(const SPI_Config *Config){
    pin_config_t SPI_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_OUTPUT};
    pin_config_t SPI_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_DIRECTION_OUTPUT};
    
    gpio_pin_direction_initalize(&SPI_SDI);
    gpio_pin_direction_initalize(&SPI_SDO);
    gpio_pin_direction_initalize(&SPI_CLK);
}

static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_Config *Config){
    pin_config_t SPI_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_OUTPUT};
    pin_config_t SPI_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_DIRECTION_OUTPUT};
    pin_config_t SPI_SS = {.port = PORTA_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_OUTPUT};
    
    gpio_pin_direction_initalize(&SPI_SDI);
    gpio_pin_direction_initalize(&SPI_SDO);
    gpio_pin_direction_initalize(&SPI_CLK);
    
    if((SPI_SLAVE_SS_ENABLE == Config->spi_mode)){
        gpio_pin_direction_initalize(&SPI_SS);
    }
}

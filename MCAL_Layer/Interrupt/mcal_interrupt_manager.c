/* 
 * File:   mcal_interrupt_manager.c
 * Author: Ali
 * https://www.linkedin.com/in/ali-darwesh-752678216
 * Created on June 21, 2024, 8:41 PM
 */

#include "mcal_interrupt_manager.h"

static volatile uint8 RB4_flag = 1,RB5_flag = 1,RB6_flag = 1,RB7_flag = 1;

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

void __interrupt(high_priority) InterruptManagerHigh(void){
    if((INTCONbits.INT0IE == INTERRUPT_ENABLE)&&(INTCONbits.INT0IF == INTERRUPT_OCCUR)){
        INT0_ISR();
    }
    else{/*Nothing*/}
    if((INTCON3bits.INT2IE == INTERRUPT_ENABLE)&&(INTCON3bits.INT2IF == INTERRUPT_OCCUR)){
        INT2_ISR();
    }
    else{/*Nothing*/}
    
}

void __interrupt(low_priority) InterruptManagerLow(void){
    if((INTCON3bits.INT1IE == INTERRUPT_ENABLE)&&(INTCON3bits.INT1IF == INTERRUPT_OCCUR)){
        INT1_ISR();
    }
    else{/*Nothing*/}
}

#else

void __interrupt() InterruptManagerHigh(void){
                /* ============ INTx External On Change Interrupt Start ============ */
    if((INTCONbits.INT0IE == INTERRUPT_ENABLE)&&(INTCONbits.INT0IF == INTERRUPT_OCCUR)){
        INT0_ISR();
    }
    else{/*Nothing*/}
    if((INTCON3bits.INT1IE == INTERRUPT_ENABLE)&&(INTCON3bits.INT1IF == INTERRUPT_OCCUR)){
        INT1_ISR();
    }
    else{/*Nothing*/}
    if((INTCON3bits.INT2IE == INTERRUPT_ENABLE)&&(INTCON3bits.INT2IF == INTERRUPT_OCCUR)){
        INT2_ISR();
    }
    else{/*Nothing*/}
                /* ============ INTx External Interrupt End ============ */
                /* ============ PortB External On Change Interrupt Start ============ */
    if((INTCONbits.RBIE == INTERRUPT_ENABLE)&&(INTCONbits.RBIF == INTERRUPT_OCCUR)&&(PORTBbits.RB4 == GPIO_HIGH) && (RB4_flag == 1)){
        RB4_ISR(0);
        RB4_flag = 0;
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE)&&(INTCONbits.RBIF == INTERRUPT_OCCUR)&&(PORTBbits.RB4 == GPIO_LOW) && (RB4_flag == 0)){
        RB4_ISR(1);
        RB4_flag = 1;
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE)&&(INTCONbits.RBIF == INTERRUPT_OCCUR)&&(PORTBbits.RB5 == GPIO_HIGH) && (RB5_flag == 1)){
        RB5_ISR(0);
        RB5_flag = 0;
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE)&&(INTCONbits.RBIF == INTERRUPT_OCCUR)&&(PORTBbits.RB5 == GPIO_LOW) && (RB5_flag == 0)){
        RB5_ISR(1);
        RB5_flag = 1;
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE)&&(INTCONbits.RBIF == INTERRUPT_OCCUR)&&(PORTBbits.RB6 == GPIO_HIGH) && (RB6_flag == 1)){
        RB6_ISR(0);
        RB6_flag = 0;
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE)&&(INTCONbits.RBIF == INTERRUPT_OCCUR)&&(PORTBbits.RB6 == GPIO_LOW) && (RB6_flag == 0)){
        RB6_ISR(1);
        RB6_flag = 1;
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE)&&(INTCONbits.RBIF == INTERRUPT_OCCUR)&&(PORTBbits.RB7 == GPIO_HIGH) && (RB7_flag == 1)){
        RB7_ISR(0);
        RB7_flag = 0;
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE)&&(INTCONbits.RBIF == INTERRUPT_OCCUR)&&(PORTBbits.RB7 == GPIO_LOW) && (RB7_flag == 0)){
        RB7_ISR(1);
        RB7_flag = 1;
    }
    else{/*Nothing*/}
                /* ============ PortB External On Change Interrupt End ============ */
                /* ============ ADC Module Interrupt Start ============ */
    if((INTERRUPT_ENABLE == PIE1bits.ADIE) && (INTERRUPT_OCCUR == PIR1bits.ADIF)){
        ADC_ISR();
    }
    else{ /* Nothing */ }
                /* ============ ADC Module Interrupt END ============ */
                /* ============ Timer Module Interrupt Start ============ */
    if((INTERRUPT_ENABLE == INTCONbits.TMR0IE) && (INTERRUPT_OCCUR == INTCONbits.TMR0IF)){
        Timer0_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == PIE1bits.TMR1IE) && (INTERRUPT_OCCUR == PIR1bits.TMR1IF)){
        Timer1_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == PIE1bits.TMR2IE) && (INTERRUPT_OCCUR == PIR1bits.TMR2IF)){
        Timer2_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == PIE2bits.TMR3IE) && (INTERRUPT_OCCUR == PIR2bits.TMR3IF)){
        Timer3_ISR();
    }
    else{ /* Nothing */ }
                /* ============ Timer Module Interrupt End ============ */
                /* ============ CCP1 and CCP2 Module Interrupt Start ============ */
    if((INTERRUPT_ENABLE == PIE1bits.CCP1IE) && (INTERRUPT_OCCUR == PIR1bits.CCP1IF)){
        CCP1_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == PIE2bits.CCP2IE) && (INTERRUPT_OCCUR == PIR2bits.CCP2IF)){
        CCP2_ISR();
    }
    else{ /* Nothing */ }
                /* ============ CCP1 and CCP2 Module Interrupt End ============ */
                /* ============ EUSART Module Interrupt Start ============ */
    if((INTERRUPT_ENABLE == PIE1bits.TXIE) && (INTERRUPT_OCCUR == PIR1bits.TXIF)){
        EUSAER_TX_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == PIE1bits.RCIE) && (INTERRUPT_OCCUR == PIR1bits.RCIF)){
        EUSAER_RX_ISR();
    }
    else{ /* Nothing */ }
                /* ============ EUSART Module Interrupt End ============ */
                /* ============ MSSP_SPI Module Interrupt Start ============ */
    if((INTERRUPT_ENABLE == PIE1bits.SSPIE) && (INTERRUPT_OCCUR == PIR1bits.SSPIF)){
        MSSP_SPI_ISR();
    }
    else{ /* Nothing */ }
                /* ============ MSSP_SPI Module Interrupt End ============ */
                /* ============ MSSP_I2C Module Interrupt Start ============ */
    if((INTERRUPT_ENABLE == PIE1bits.SSPIE) && (INTERRUPT_OCCUR == PIR1bits.SSPIF)){
        MSSP_I2C_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == PIE2bits.BCLIE) && (INTERRUPT_OCCUR == PIR2bits.BCLIF)){
        MSSP_I2C_BC_ISR();
    }
    else{ /* Nothing */ }
                /* ============ MSSP_I2C Module Interrupt End ============ */
}

#endif
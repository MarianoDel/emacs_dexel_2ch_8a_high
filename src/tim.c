//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### TIM.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "tim.h"
#include "stm32f10x.h"
#include "hard.h"
#include "soft_pwm.h"


// Module Private Types Constants and Macros -----------------------------------
#define RCC_TIM1_CLK    (RCC->APB2ENR & 0x00000800)
#define RCC_TIM1_CLKEN    (RCC->APB2ENR |= 0x00000800)
#define RCC_TIM1_CLKDIS    (RCC->APB2ENR &= ~0x00000800)

#define RCC_TIM2_CLK    (RCC->APB1ENR & 0x00000001)
#define RCC_TIM2_CLKEN    (RCC->APB1ENR |= 0x00000001)
#define RCC_TIM2_CLKDIS    (RCC->APB1ENR &= ~0x00000001)

#define RCC_TIM3_CLK    (RCC->APB1ENR & 0x00000002)
#define RCC_TIM3_CLKEN    (RCC->APB1ENR |= 0x00000002)
#define RCC_TIM3_CLKDIS    (RCC->APB1ENR &= ~0x00000002)

#define RCC_TIM4_CLK    (RCC->APB1ENR & 0x00000004)
#define RCC_TIM4_CLKEN    (RCC->APB1ENR |= 0x00000004)
#define RCC_TIM4_CLKDIS    (RCC->APB1ENR &= ~0x00000004)

#ifdef STM32F10X_HD
#define RCC_TIM5_CLK    (RCC->APB1ENR & 0x00000008)
#define RCC_TIM5_CLKEN    (RCC->APB1ENR |= 0x00000008)
#define RCC_TIM5_CLKDIS    (RCC->APB1ENR &= ~0x00000008)

#define RCC_TIM6_CLK    (RCC->APB1ENR & 0x00000010)
#define RCC_TIM6_CLKEN    (RCC->APB1ENR |= 0x00000010)
#define RCC_TIM6_CLKDIS    (RCC->APB1ENR &= ~0x00000010)

#define RCC_TIM7_CLK    (RCC->APB1ENR & 0x00000020)
#define RCC_TIM7_CLKEN    (RCC->APB1ENR |= 0x00000020)
#define RCC_TIM7_CLKDIS    (RCC->APB1ENR &= ~0x00000020)

#define RCC_TIM8_CLK    (RCC->APB2ENR & 0x00002000)
#define RCC_TIM8_CLKEN    (RCC->APB2ENR |= 0x00002000)
#define RCC_TIM8_CLKDIS    (RCC->APB2ENR &= ~0x00002000)
#endif


// Externals -------------------------------------------------------------------
extern volatile unsigned short wait_ms_var;


// Globals ---------------------------------------------------------------------


// Module Functions ------------------------------------------------------------
void Wait_ms (unsigned short a)
{
    wait_ms_var = a;
    while (wait_ms_var);
}


//////////////////
// TIM1 Configs //
//////////////////
#define VALUE_FOR_LEAST_FREQ    2133    //30KHz on 64MHz
#define VALUE_FOR_CONSTANT_OFF    230    //3.6us tick 15.6ns
// #define VALUE_FOR_CONSTANT_OFF    128    //2us tick 15.6ns
void TIM1_Init_pwm_CH1_CH1N_trig_CH2 (void)
{
    if (!RCC_TIM1_CLK)
        RCC_TIM1_CLKEN;

    // timer configuration
    TIM1->CR1 = 0x00 | TIM_CR1_ARPE;        //clk int / 1, auto preload enable
    TIM1->CR2 = 0x00;
    
    //Reset mode, trigger with TI2
    TIM1->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_1;

    //CH1 output PWM mode 2 (channel active TIM1->CNT > TIM1->CCR1)
    //CH2 input map IC2->TI2; filtered / 4 N = 6
    TIM1->CCMR1 = 0x0070 |
        TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1;
    
    TIM1->CCMR2 = 0x0000;
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1NE;    // CH1 CH1N enable
    
    TIM1->ARR = VALUE_FOR_LEAST_FREQ;    // each tick 15.6ns

    TIM1->CNT = 0;
    TIM1->PSC = 0;

    // general enable for OC, and dead-time    
    // TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->BDTR |= TIM_BDTR_MOE | (0 + 20);    //300ns @ 64MHz
    // TIM1->BDTR |= TIM_BDTR_MOE | (0 + 64);    //1us @ 64MHz    
    // TIM1->BDTR |= TIM_BDTR_MOE | (0 + 5);    //100ns @ 48MHz    
    // TIM1->BDTR |= TIM_BDTR_MOE | (128 + 28);    //3.8us @ 48MHz    
        
    // Enable timer ver UDIS
    TIM1->CCR1 = VALUE_FOR_CONSTANT_OFF;
    TIM1->CR1 |= TIM_CR1_CEN;
}


void TIM1_Enable (void)
{
    TIM1->ARR = VALUE_FOR_LEAST_FREQ;
    TIM1->EGR |= TIM_EGR_UG;
    LED_ON;
}


void TIM1_Disable (void)
{
    TIM1->ARR = 0;
    LED_OFF;
}


void TIM1_Init (void)
{
    if (!RCC_TIM1_CLK)
        RCC_TIM1_CLKEN;

    // Base timer config
    TIM1->CR1 = 0x00;        //clk int / 1;
    TIM1->CR2 = 0x00;
    TIM1->SMCR = 0x0000;

    // TIM1->CCMR1 = 0x0060;    //CH1 output PWM mode 2 (channel active TIM1->CNT < TIM1->CCR1)
    TIM1->CCMR1 = 0x0000; 
    TIM1->CCMR2 = 0x2100;    // CH4 input on TI4 filter fclk / 4
    // TIM1->CCER = 0x0000;
    TIM1->BDTR = 0x0000;
    TIM1->CCER |= TIM_CCER_CC4P | TIM_CCER_CC4E;    // CH4 input inverted
    // TIM1->BDTR |= TIM_BDTR_MOE;

    TIM1->ARR = DUTY_100_PERCENT - 1;    // 1000 pts -> 7.2KHz
    TIM1->PSC = 9;    // 13.88us tick
    
    TIM1->CNT = 0;

    // Set ints and Enable timer
    TIM1->DIER |= TIM_DIER_UIE | TIM_DIER_CC4IE;
    TIM1->CR1 |= TIM_CR1_CEN;

    // NVIC 
    // Timer1 Update event Int
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    NVIC_SetPriority(TIM1_UP_IRQn, 4);

    // Timer1 CH4 input capture
    NVIC_EnableIRQ(TIM1_CC_IRQn);
    NVIC_SetPriority(TIM1_CC_IRQn, 8);
    
}


void TIM1_UP_IRQHandler (void)
{
    // reset int flag
    if (TIM1->SR & TIM_SR_UIF)
        TIM1->SR &= ~TIM_SR_UIF;

    //Code Handler
    // timer1_seq_ready = 1;
    // if (timer1_seq_cnt < 65000)
    //     timer1_seq_cnt++;
    
}


void TIM1_CC_IRQHandler (void)
{
    // reset int flag
    if (TIM1->SR & TIM_SR_CC4IF)
        TIM1->SR &= ~TIM_SR_CC4IF;

    //Code Handler
    // sync_last_capt = sync_capt;
    // sync_capt = TIM1->CCR4;
    // sync_cnt = timer1_seq_cnt;
    // timer1_seq_cnt = 0;
    // sync_int = 1;
    // sync_verify = 1;

    // if (LED1)
    //     LED1_OFF;
    // else
    //     LED1_ON;
    
}


void TIM3_Init (void)
{
    if (!RCC_TIM3_CLK)
        RCC_TIM3_CLKEN;

    //Configuracion del timer.
    TIM3->CR1 |= TIM_CR1_OPM;        //clk int / 1; upcounting; one pulse
    //TIM3->CR1 = 0x0000;        //clk int / 1; upcounting;
    //TIM3->CR2 |= TIM_CR2_MMS_1;        //UEV -> TRG0
    TIM3->CR2 = 0x0000;
    //TIM3->SMCR |= TIM_SMCR_SMS_2 |TIM_SMCR_SMS_1 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0;    //reset mode
    //TIM3->SMCR |= TIM_SMCR_SMS_2;    //reset mode link timer1    OJO no anda
    TIM3->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;    //trigger mode link timer1
    //TIM3->SMCR = 0x0000;    //
    //TIM3->CCMR1 = 0x6000;            //CH2 output PWM mode 1
    //  TIM3->CCMR1 = 0x0060;            //CH1 output PWM mode 1
    TIM3->CCMR1 = 0x0070;            //CH1 output PWM mode 2 (channel inactive TIM3->CNT < TIM3->CCR1)
    TIM3->CCMR2 = 0x0000;            //
    //  TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;    //CH1 enable on pin active high
    TIM3->CCER |= TIM_CCER_CC1E;    //CH1 enable on pin active high
    //TIM3->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2P;    //CH2 enable on pin active high
    TIM3->ARR = DUTY_50_PERCENT;
    TIM3->CNT = 0;
    TIM3->PSC = 0;
    //TIM3->EGR = TIM_EGR_UG;    //generate event

    // Enable timer ver UDIS
    //TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;

    //TIM3->CCR2 = 512;        //delay = TIM3->CCRx = 512 - TIM1->CCR2
    TIM3->CCR1 = (DUTY_50_PERCENT + 1);        //delay = TIM3->CCRx = 512 - TIM1->CCR2
}

void TIM3_IRQHandler (void)	//1 ms
{
    //Code Handler
    
    //bajar flag
    if (TIM3->SR & 0x01)	//bajo el flag
        TIM3->SR = 0x00;
}


void TIM4_Init(void)
{
    //---- Clk ----//
    if (!RCC_TIM4_CLK)
        RCC_TIM4_CLKEN;

    //--- Config ----//
    TIM4->CR1 = 0x00;
    TIM4->CR2 = 0x00;
    
    TIM4->CCMR1 = 0x6000;    //CH2 output PWM mode 2 (channel active CNT < CCR1)
    TIM4->CCMR2 = 0x0060;    //CH3 output PWM mode 2 (channel active CNT < CCR1)
    
    TIM4->CCER |= TIM_CCER_CC3E | TIM_CCER_CC2E;
    
    TIM4->ARR = DUTY_100_PERCENT - 1;    //1000 pwm points freq-> 72MHz / 10 = 7.2KHz
    TIM4->PSC = 9;

    // Enable timer ver UDIS
    TIM4->CR1 |= TIM_CR1_CEN;
}


void TIM4_Update_CH2 (unsigned short a)
{
    TIM4->CCR2 = a;
}


void TIM4_Update_CH3 (unsigned short a)
{
    TIM4->CCR3 = a;
}


#ifdef STM32F10X_HD
//-- Timer 5 Functions --
void TIM5_Init (void)
{
//    Counter Register (TIMx_CNT)
//    Prescaler Register (TIMx_PSC)
//    Auto-Reload Register (TIMx_ARR)
//    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)

    //---- Clk ----//
    if (!RCC_TIM5_CLK)
        RCC_TIM5_CLKEN;

    //--- Config ----//
    TIM5->CR1 = 0x0000;        //clk int / 1; upcounting;
    TIM5->CR2 = 0x0000;

    TIM5->CCMR1 = 0x6060;    //CH1 CH2 output PWM mode 2 (channel active TIM5->CNT < TIM5->CCR1)
    TIM5->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;    // ch1 ch2 enabled

    // try to be near 7KHz
    TIM5->ARR = DUTY_100_PERCENT - 1;    //1000 pwm points freq-> 72MHz / 10 = 7.2KHz
    TIM5->PSC = 9;
    
    // Enable the timer
    TIM5->CR1 |= TIM_CR1_CEN;
}


void TIM5_Update_CH1 (unsigned short a)
{
    TIM5->CCR1 = a;
}


void TIM5_Update_CH2 (unsigned short a)
{
    TIM5->CCR2 = a;
}


///////////////////////
// Timer 6 Functions //
///////////////////////
void TIM6_Init(void)
{
//    Counter Register (TIMx_CNT)
//    Prescaler Register (TIMx_PSC)
//    Auto-Reload Register (TIMx_ARR)
//    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
//    Activate with ints

    //---- Clk ----//
    if (!RCC_TIM6_CLK)
        RCC_TIM6_CLKEN;

    //--- Config ----//
    TIM6->ARR = 1000;
    TIM6->CNT = 0;
    TIM6->PSC = 63;
    TIM6->EGR = TIM_EGR_UG; //update registers

    // Enable timer with ints
    TIM6->DIER |= TIM_DIER_UIE;
    // TIM6->CR1 |= TIM_CR1_CEN;

    // NVIC enable for timer 6
    NVIC_EnableIRQ(TIM6_IRQn);
    NVIC_SetPriority(TIM6_IRQn, 10);
}


void TIM6_Change (unsigned short new_arr, unsigned short new_psc)
{
    TIM6->CR1 &= TIM_CR1_CEN;
    TIM6->ARR = new_arr;
    TIM6->PSC = new_psc;
    TIM6->CR1 |= TIM_CR1_CEN;
}


// void TIM6_IRQHandler (void)	//100mS
// {
// 	UART_Tim6 ();

// 	//bajar flag
// 	if (TIM6->SR & 0x01)	//bajo el flag
// 		TIM6->SR = 0x00;
// }


//////////////////
// TIM7 Configs //
//////////////////
void TIM7_Init(void)
{
    //---- Clk ----//
    if (!RCC_TIM7_CLK)
        RCC_TIM7_CLKEN;

    //--- Config ----//
    TIM7->ARR = 25 - 1;    // ~40KHz
    TIM7->CNT = 0;
    TIM7->PSC = 63;

    // Enable timer ver UDIS
    TIM7->DIER |= TIM_DIER_UIE;
    TIM7->CR1 |= TIM_CR1_CEN;

    // NVIC Enable
    // Interrupt timer7. priority 10
    NVIC_EnableIRQ(TIM7_IRQn);
    NVIC_SetPriority(TIM7_IRQn, 10);
}


void TIM7_IRQHandler (void)
{
    if (TIM7->SR & 0x01)
    {
        TIM7->SR = 0x00;    //low flag

        // if (LED2)
        //     LED2_OFF;
        // else
        //     LED2_ON;

        Soft_PWM_Int_Handler ();
    }    
}




//////////////////////////////
// TIM8 Configs - Functions //
//////////////////////////////
void TIM8_Init_pwm_CH1_CH1N_trig_CH2 (void)
{
    if (!RCC_TIM8_CLK)
        RCC_TIM8_CLKEN;

    // timer configuration
    TIM8->CR1 = 0x00 | TIM_CR1_ARPE;        //clk int / 1, auto preload enable
    TIM8->CR2 = 0x00;
    
    //Reset mode, trigger with TI2
    TIM8->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_1;

    //CH1 output PWM mode 2 (channel active TIM8->CNT > TIM8->CCR1)
    //CH2 input map IC2->TI2; filtered / 4 N = 6
    TIM8->CCMR1 = 0x0070 |
        TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1;
    
    TIM8->CCMR2 = 0x0000;
    TIM8->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1NE;    // CH1 CH1N enable
    
    TIM8->ARR = VALUE_FOR_LEAST_FREQ;    // each tick 15.6ns

    TIM8->CNT = 0;
    TIM8->PSC = 0;

    // general enable for OC, and dead-time    
    // TIM8->BDTR |= TIM_BDTR_MOE;
    TIM8->BDTR |= TIM_BDTR_MOE | (0 + 20);    //300ns @ 64MHz
    // TIM8->BDTR |= TIM_BDTR_MOE | (0 + 64);    //1us @ 64MHz    
    // TIM8->BDTR |= TIM_BDTR_MOE | (0 + 5);    //100ns @ 48MHz    
    // TIM8->BDTR |= TIM_BDTR_MOE | (128 + 28);    //3.8us @ 48MHz    
        
    // Enable timer ver UDIS
    TIM8->CCR1 = VALUE_FOR_CONSTANT_OFF;
    TIM8->CR1 |= TIM_CR1_CEN;
}


void TIM8_Enable (void)
{
    TIM8->ARR = VALUE_FOR_LEAST_FREQ;
    TIM8->EGR |= TIM_EGR_UG;        
}


void TIM8_Disable (void)
{
    TIM8->ARR = 0;
}


void TIM8_Init (void)
{
//    Counter Register (TIMx_CNT)
//    Prescaler Register (TIMx_PSC)
//    Auto-Reload Register (TIMx_ARR)
//    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
//
    //---- Clk ----//
    if (!RCC_TIM8_CLK)
        RCC_TIM8_CLKEN;

    //--- Config ----//
    TIM8->CR1 = 0x0000;        //clk int / 1; upcounting;
    TIM8->CR2 = 0x0000;

    TIM8->CCMR1 = 0x6060;    //CH2 CH1 output PWM mode 2 (channel active CNT < CCR1)
    TIM8->CCMR2 = 0x6060;    //CH4 CH3 output PWM mode 2 (channel active CNT < CCR1)

    // ch1 ch2 ch3 ch4 enabled    
    TIM8->CCER |= TIM_CCER_CC4E | TIM_CCER_CC3E | TIM_CCER_CC2E | TIM_CCER_CC1E;
    TIM8->BDTR |= TIM_BDTR_MOE;
    
    // try to be near 7KHz
    TIM8->ARR = DUTY_100_PERCENT - 1;    //1000 pwm points freq-> 72MHz / 10 = 7.2KHz
    TIM8->PSC = 9;
    
    // Enable the timer
    TIM8->CR1 |= TIM_CR1_CEN;
}


void TIM8_Update_CH1 (unsigned short a)
{
    TIM8->CCR1 = a;
}


void TIM8_Update_CH2 (unsigned short a)
{
    TIM8->CCR2 = a;
}


void TIM8_Update_CH3 (unsigned short a)
{
    TIM8->CCR3 = a;
}


void TIM8_Update_CH4 (unsigned short a)
{
    TIM8->CCR4 = a;
}
#endif    //STM32F10X_HD

//--- end of file ---//

//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.H #################################
//---------------------------------------------

#ifndef HARD_H_
#define HARD_H_


//----------- Defines For Configuration -------------

//----- Board Configuration -------------------//
//--- Hardware ------------------//
#define HARDWARE_VERSION_1_0    // first prototype


//--- Software ------------------//
#define SOFTWARE_VERSION_1_0    // init version


//-------- Type of Program (depending on software version) ----------------


//--- Serial Number / Device Id Bytes length ----------
#define USE_DEVICE_ID_4BYTES
// #define USE_DEVICE_ID_12BYTES


//-------- Oscillator and Crystal selection (Freq in startup_clocks.h) ---
#define HSI_INTERNAL_RC
// #define HSE_CRYSTAL_OSC

#ifdef HSE_CRYSTAL_OSC
// #define CRYSTAL_8MHZ
#define CRYSTAL_12MHZ
#endif

#ifdef HSE_CRYSTAL_OSC
// #define SYSCLK_FREQ_72MHz
#define SYSCLK_FREQ_8MHz
#endif

#ifdef HSI_INTERNAL_RC
#define SYSCLK_FREQ_64MHz
// #define SYSCLK_FREQ_8MHz
#endif

//-------- End Of Defines For Configuration ------




// Exported Types --------------------------------------------------------------
#ifdef HARDWARE_VERSION_1_0

// PA defines ----
// PA0 
// PA1 
// PA2 
// PA3 NC

// PA4 Analog DAC Output (Channel 1)
// PA5 Analog DAC Output (Channel 2)

// PA6 NC

// PA7 Alternative TIM8_CH1N

// PA8 Alternative TIM1_CH1
// PA9 Alternative TIM1_CH2

// PA10 PA11 NC

// PA12
#define LED    ((GPIOA->ODR & 0x1000) != 0)
#define LED_ON    (GPIOA->BSRR = 0x00001000)
#define LED_OFF    (GPIOA->BSRR = 0x10000000)

// PA13 PA14 PA15 NC

// PB defines ----
// PB0 PB1 PB2 PB3 NC
// PB4 PB5 PB6 PB7 NC
// PB8 PB9 NC

// PB10 PB11 Alternative Usart3 Tx Rx

// PB12 NC

// PB13 Alternative TIM1_CH1N

// PB14 PB15 NC

// PC defines ----
// PC0 PC1 PC2 PC3 NC
// PC4 PC5 NC

// PC6 Alternative TIM8_CH1
// PC7 Alternative TIM8_CH2

// PC8 PC9 PC10 PC11 NC
// PC12 PC13 PC14 PC15 NC

// PD defines ----
// PD0 PD1 PD2 NC

#endif //HARDWARE_VERSION_1_0


// Exported Module Functions ---------------------------------------------------
void Led_On (void);
void Led_Off (void);
unsigned char Led_Is_On (void);
char * HARD_GetHardwareVersion (void);
char * HARD_GetSoftwareVersion (void);

#endif    /* _HARD_H_ */

//--- end of file ---//


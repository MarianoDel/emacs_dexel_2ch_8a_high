//---------------------------------------------------------------
// #### PROJECT MICRO-CURRENTS POWER-OUT F103 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### MAIN.C ##################################################
//---------------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "hard.h"

#include "gpio.h"
#include "dac.h"
#include "tim.h"
#include "usart.h"

#include "test_functions.h"
#include "soft_pwm.h"
#include "comms.h"
#include "filters_and_offsets.h"

#include <stdio.h>
#include <string.h>


// Private Types Constants and Macros ------------------------------------------
typedef enum {
    MAIN_HARD_INIT,
    MAIN_RUNNING,
    MAIN_IN_OVERTEMP
    
} main_state_e;


// Externals -------------------------------------------------------------------
//   for timers
volatile unsigned short wait_ms_var = 0;
//   for dmx info 
volatile unsigned char last_ch_values [2] = { 0 };


// Globals ---------------------------------------------------------------------
//-- Timers globals ----------------------------------
volatile unsigned short timer_standby = 0;


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);


// Module Functions ------------------------------------------------------------
int main (void)
{
    // Gpio Configuration.
    GpioInit();

    // Systick Timer Activation
    if (SysTick_Config(64000))
        SysTickError();

    // Hardware Tests
    // TF_Hardware_Tests ();

    // Hardware Inits. ---------------------------
    DAC_Config ();
    
    // Start of Complete Channel 1
    DAC_Output1(0);
    TIM1_Init_pwm_CH1_CH1N_trig_CH2 ();
    
    // Start of Complete Channel 2
    DAC_Output2(0);
    TIM8_Init_pwm_CH1_CH1N_trig_CH2 ();    

    // Start Usart3 for Comms
    Usart3Config();

    // Init TIM 7 for Soft_Pwm
    TIM7_Init ();

    Soft_PWM_Set_Channels (1, 0);
    Soft_PWM_Set_Channels (2, 0);
            
    main_state_e main_state = MAIN_HARD_INIT;

    while (1)
    {
        switch (main_state)
        {
        case MAIN_HARD_INIT:
            FiltersAndOffsets_Filters_Reset ();
            

            main_state++;
            break;

        case MAIN_RUNNING:
            if (!timer_standby)
            {
                timer_standby = 1;
                FiltersAndOffsets_Post_Mapping_SM (last_ch_values);
            }            
            break;

        case MAIN_IN_OVERTEMP:

            // if (!timer_check_temp)
            // {
            //     if (Temp_Channel < TEMP_RECONNECT)
            //         main_state = MAIN_HARD_INIT;
                
            //     timer_check_temp = 2000;    //check again in two seconds            
            // }
            break;

        default:
            main_state = MAIN_HARD_INIT;
            break;
        }

        Comms_Update();

    }    //end of while 1

    return 0;
}

//--- End of Main ---//



void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    // HARD_Timeouts();
    
    if (timer_standby)
        timer_standby--;

}


void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}

//--- end of file ---//


//----------------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SOFT_PWM.C #########################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "soft_pwm.h"
#include "hard.h"
#include "tim.h"
#include "stm32f0xx.h"


// Module Private Types & Macros -----------------------------------------------
#define SOFT_PWM_STEPS    256


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
unsigned short soft_pwm_cnt = 0;
unsigned short soft_saved_pwm_ch1 = 0;
unsigned short soft_saved_pwm_ch2 = 0;


// Module Private Functions ----------------------------------------------------
void Soft_PWM_Reset_Output_Ch1 (void);
void Soft_PWM_Reset_Output_Ch2 (void);
void Soft_PWM_Set_Output_Ch1 (void);
void Soft_PWM_Set_Output_Ch2 (void);


// Module Functions ------------------------------------------------------------
void Soft_PWM_Int_Handler (void)
{
    if (soft_pwm_cnt)
    {
        if (soft_saved_pwm_ch1 <= soft_pwm_cnt)        
            Soft_PWM_Reset_Output_Ch1 ();

        if (soft_saved_pwm_ch2 <= soft_pwm_cnt)
            Soft_PWM_Reset_Output_Ch2 ();
        
        if (soft_pwm_cnt < (SOFT_PWM_STEPS - 1))
            soft_pwm_cnt++;
        else
            soft_pwm_cnt = 0;
        
    }
    else
    {
        if (soft_saved_pwm_ch1)
            Soft_PWM_Set_Output_Ch1 ();

        if (soft_saved_pwm_ch2)
            Soft_PWM_Set_Output_Ch2 ();

        soft_pwm_cnt++;
    }
}


void Soft_PWM_Set_Channels (unsigned char ch, unsigned short value)
{
    if (ch == 1)
        soft_saved_pwm_ch1 = value;

    if (ch == 2)
        soft_saved_pwm_ch2 = value;
}


void Soft_PWM_Set_Output_Ch2 (void)
{
    // TIM1->ARR = VALUE_FOR_LEAST_FREQ;
    // TIM1->EGR |= TIM_EGR_UG;    
}


void Soft_PWM_Reset_Output_Ch2 (void)
{
    // TIM1->ARR = 0;
}


void Soft_PWM_Set_Output_Ch1 (void)
{
    TIM1->ARR = VALUE_FOR_LEAST_FREQ;
    TIM1->EGR |= TIM_EGR_UG;
}


void Soft_PWM_Reset_Output_Ch1 (void)
{
    TIM1->ARR = 0;
}


//--- end of file ---//

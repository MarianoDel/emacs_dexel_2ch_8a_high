//----------------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PWM.C ##############################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "pwm.h"
#include "soft_pwm.h"


// Module Private Types & Macros -----------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------

unsigned char top_min_curr = 8;    // 8 default to 4amps
unsigned char top_mult = 106;    // default to 4amps
// unsigned char top_mult = 102;    // 8.03 amps
// unsigned char top_mult = 97;    // 7.64 amps
// unsigned char top_mult = 116;    // default for 4.2, 8.3amps
void PWM_Map_Post_Filter_Top_Multiplier (unsigned char top_multiplier,
                                         unsigned char min_curr)
{
    top_mult = top_multiplier;
    top_min_curr = min_curr;
}


// #define MIN_FOR_SOFT_PWM    24
// #define MIN_FOR_SOFT_PWM    33
#define MIN_FOR_SOFT_PWM    100
// #define MIN_FOR_SOFT_PWM    40
// get dmx_filtered from 0 to 4095
// answer pwm_ena 0 to 255
// answer pwm_ch 0 to 4095
// pwm with minimun
void PWM_Map_Post_Filter (unsigned short dmx_filtered, unsigned short * pwm_ena, unsigned short * pwm_ch)
{
    unsigned short dmx_ena = 0;
    unsigned int dmx_ch = 0;

    // adjust for max pwm 4095
    unsigned int top_value = dmx_filtered * top_mult;
    dmx_filtered = top_value / 100;
    
    if (dmx_filtered > (SOFT_PWM_STEPS - 1))
    {
        dmx_ena = (SOFT_PWM_STEPS - 1);
        dmx_ch = dmx_filtered - (SOFT_PWM_STEPS - 1) + MIN_FOR_SOFT_PWM;

        if (dmx_ch > 4095)
            dmx_ch = 4095;
    }
    else    // values from 0 to 255
    {
        if (dmx_filtered < top_min_curr + 2)
        {
            // divided by three but not for one amp
            if (top_min_curr > 2)
                dmx_filtered = dmx_filtered / 3;
            else
                dmx_filtered >>= 1;
        }
        // else if (dmx_filtered < 40)
        // {
        //     // divided by two
        //     dmx_filtered >>= 1;
        // }
        // else if (dmx_filtered < 80)
        // {
        //     unsigned short a = dmx_filtered * 2;
        //     // divided by 1.5
        //     dmx_filtered = a / 3;
        // }
        else if (dmx_filtered < 60)
        {
            if (top_min_curr != 2)
            {
                unsigned short a = dmx_filtered * 8;
                // divided by 1.25
                dmx_filtered = a / 10;
            }
        }
        
        dmx_ena = dmx_filtered;
        dmx_ch = MIN_FOR_SOFT_PWM;
    }

    *pwm_ena = dmx_ena;
    *pwm_ch = dmx_ch;
}


void PWM_Soft_Handler_Low_Freq_Roof_Set (unsigned short new_roof)
{
    // soft_pwm_steps = new_roof;    
}

//--- end of file ---//

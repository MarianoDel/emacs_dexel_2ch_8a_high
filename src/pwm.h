//----------------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PWM.H ##############################################
//----------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __PWM_H_
#define __PWM_H_


// Exported Types --------------------------------------------------------------


// Exported Constants or Macros ------------------------------------------------


// Exported Functions ----------------------------------------------------------
void PWM_Map_Post_Filter (unsigned short dmx_filtered, unsigned short * pwm_ena, unsigned short * pwm_ch);
void PWM_Map_Post_Filter_Top_Multiplier (unsigned char top_multiplier,
                                         unsigned char min_curr);

void PWM_Soft_Handler_Low_Freq_Roof_Set (unsigned short new_roof);

#endif    /* __PWM_H_ */

//--- end of file ---//


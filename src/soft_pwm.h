//----------------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SOFT_PWM.H #########################################
//----------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __SOFT_PWM_H_
#define __SOFT_PWM_H_



// Exported Types Constants and Macros -----------------------------------------
#define SOFT_PWM_STEPS    255



// Exported Functions ----------------------------------------------------------
void Soft_PWM_Int_Handler (void);
void Soft_PWM_Set_Channels (unsigned char ch, unsigned short value);


#endif    /* __SOFT_PWM_H_ */

//--- end of file ---//


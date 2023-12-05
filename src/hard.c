//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.C ################################
//---------------------------------------------
#include "hard.h"
#include "stm32f10x.h"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Led_On (void)
{
    LED_ON;
}


void Led_Off (void)
{
    LED_OFF;
}


unsigned char Led_Is_On (void)
{
    return LED;
}


#if defined HARDWARE_VERSION_2_1
char hardware_version [] = {"Hrd 2.1"};
#elif defined HARDWARE_VERSION_1_0
char hardware_version [] = {"Hrd 1.0"};
#else
#error "No hard version selected on hard.c"
#endif
char * HARD_GetHardwareVersion (void)
{
    return hardware_version;
}

#if defined SOFTWARE_VERSION_1_1
char software_version [] = {"Soft 1.1"};
#elif defined SOFTWARE_VERSION_1_0
char software_version [] = {"Soft 1.0"};
#else
#error "No soft version selected on hard.c"
#endif

char * HARD_GetSoftwareVersion (void)
{
    return software_version;
}

//--- end of file ---//

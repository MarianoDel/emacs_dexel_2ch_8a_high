//---------------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS.C ##################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms.h"
#include "usart.h"
#include "hard.h"
#include "utils.h"
#include "pwm.h"

#include "filters_and_offsets.h"

#include <string.h>
#include <stdio.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned char last_ch_values [];


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------



// Module Functions ------------------------------------------------------------
void Comms_Update (void)
{
    unsigned char readed = 0;
    char buff_local [127] = { 0 };
    
    if (Usart3HaveData())
    {
        Led_On();
        Usart3HaveDataReset();
        readed = Usart3ReadBuffer(buff_local, 127);
        if (readed == sizeof("ch1 255 ch2 255 sum 511"))    //readed incluye \0
        {
            unsigned short ch1 = 0;
            unsigned short ch2 = 0;
            unsigned short sum = 0;
            int qtty = 0;

            qtty = StringIsANumber(buff_local + sizeof("ch1 ") - 1, &ch1);
            if (qtty == 3)
            {
                qtty = StringIsANumber(buff_local + sizeof("ch1 255 ch2 ") - 1, &ch2);
                if (qtty == 3)
                {
                    qtty = StringIsANumber(buff_local + sizeof("ch1 255 ch2 255 sum ") - 1, &sum);
                    if (qtty == 3)
                    {
                        if ((ch1 + ch2) == sum)
                        {
                            last_ch_values[0] = ch1;
                            last_ch_values[1] = ch2;

                        }
                        else
                            Usart3Send("err verif\n");
                    }
                    else
                        Usart3Send("err sum\n");
                }
                else
                    Usart3Send("err ch2\n");
            }
            else
                Usart3Send("err ch1\n");
        }

        if (!strncmp(buff_local, "current config ", sizeof("current config ") - 1))
        {
            char cint = 0;
            char cdec = 0;
            
            // check for new or old config
            if (*(buff_local + (sizeof("current config 8") - 1)) == '.')
            {
                // new config mode
                cint = *(buff_local + (sizeof("current config ") - 1));
                cdec = *(buff_local + (sizeof("current config 8.") - 1));            
                if ((cint >= '1') && (cint <= '8') &&
                    (cdec >= '0') && (cdec <= '9'))
                {
                    FiltersAndOffsets_Set_Current(cint - '0', cdec - '0');                
                }
                else
                {
                    return;
                }
            }
            else
            {
                // old config mode
                cint = *(buff_local + (sizeof("current config ") - 1));
                if ((cint >= '1') && (cint <= '8'))
                {
                    FiltersAndOffsets_Set_Current(cint - '0', 0);                
                }
                else
                {
                    return;
                }
            }
            
            if (cint == '8')
            {
                PWM_Map_Post_Filter_Top_Multiplier(97, 16);    // for 100pts min
                // PWM_Map_Post_Filter_Top_Multiplier(101, 16, 4);    // for 33pts min                    
                PWM_Soft_Handler_Low_Freq_Roof_Set (256);
            }
            else if (cint == '7')
            {
                PWM_Map_Post_Filter_Top_Multiplier(99, 14);    // for 100pts min
                // PWM_Map_Post_Filter_Top_Multiplier(103, 14, 3);    // for 33pts min                    
                PWM_Soft_Handler_Low_Freq_Roof_Set (256);
            }
            else if (cint == '6')
            {
                PWM_Map_Post_Filter_Top_Multiplier(101, 12);    // for 100pts min
                // PWM_Map_Post_Filter_Top_Multiplier(105, 12, 3);    // for 33pts min                    
                PWM_Soft_Handler_Low_Freq_Roof_Set (256);
            }
            else if (cint == '5')
            {
                PWM_Map_Post_Filter_Top_Multiplier(107, 10);    // 5amps adjust
                // PWM_Map_Post_Filter_Top_Multiplier(103, 10);    // for 100pts min
                PWM_Soft_Handler_Low_Freq_Roof_Set (256);
            }
            else if (cint == '4')
            {
                PWM_Map_Post_Filter_Top_Multiplier(112, 8);    // 4amps adjust
                // PWM_Map_Post_Filter_Top_Multiplier(106, 8);    // for 100pts min                
                PWM_Soft_Handler_Low_Freq_Roof_Set (256);
            }
            else if (cint == '3')
            {
                PWM_Map_Post_Filter_Top_Multiplier(122, 6);    // 3.1A adjust
                // PWM_Map_Post_Filter_Top_Multiplier(111, 6);    // for 100pts min                
                PWM_Soft_Handler_Low_Freq_Roof_Set (256);
            }
            else if (cint == '2')
            {
                PWM_Map_Post_Filter_Top_Multiplier(138, 4);    // 2.06A
                // PWM_Map_Post_Filter_Top_Multiplier(126, 4);    // 1.8A                
                // PWM_Map_Post_Filter_Top_Multiplier(158, 4);    // 2.5A                
                // PWM_Map_Post_Filter_Top_Multiplier(108, 4);    // 1.37A for 100pts min                
                PWM_Soft_Handler_Low_Freq_Roof_Set (128);
            }
            else if (cint == '1')
            {
                PWM_Map_Post_Filter_Top_Multiplier(172, 2);    // 1.03 adjust
                // PWM_Map_Post_Filter_Top_Multiplier(167, 2);    // 0.98 adjust                
                // PWM_Map_Post_Filter_Top_Multiplier(200, 2);    // 1.2A adjust                
                // PWM_Map_Post_Filter_Top_Multiplier(110, 2);    // 0.55A for 100pts min                
                PWM_Soft_Handler_Low_Freq_Roof_Set (64);
            }

            Usart3Send("ok\n");
        }

        if (!strncmp(buff_local, "version", sizeof("version") - 1))
        {
            char buff [30] = { 0 };
            sprintf(buff,"%s %s\n", HARD_GetHardwareVersion(), HARD_GetSoftwareVersion());
            Usart3Send(buff);
        }
            
        Led_Off();;
    }
}


//---- end of file ----//

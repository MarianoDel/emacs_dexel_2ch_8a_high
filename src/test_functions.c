//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C #########################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "hard.h"
#include "stm32f10x.h"

#include "tim.h"
#include "dac.h"
#include "soft_pwm.h"

// #include "pwm.h"
// #include "adc.h"
// #include "dma.h"
// #include "temperatures.h"
#include "usart.h"
#include "utils.h"

// #include "dsp.h"

#include <stdio.h>


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------

// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led (void);

void TF_Led_Dac (void);
void TF_Two_Complete_Channels_Hardware (void);

void TF_Soft_Pwm (void);

void TF_Usart3_Tx (void);
void TF_Usart3_Tx_Rx_Int (void);
void TF_Two_Complete_Channels_Usart (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // Begin Hardware Tests - check test_functions module
    // TF_Led();

    // TF_Led_Dac ();

    // TF_Two_Complete_Channels_Hardware ();

    TF_Soft_Pwm ();

    // TF_Usart3_Tx ();
    // TF_Usart3_Tx_Rx_Int ();
    // TF_Two_Complete_Channels_Usart ();
    
    // End Hardware Tests -------------------------------
}


void TF_Led (void)
{
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        Wait_ms(1000);
    }
}


void TF_Led_Dac (void)
{
    DAC_Config ();
    
    while (1)
    {
        if (LED)
        {
            LED_OFF;
            DAC_Output1 (2047);
            DAC_Output2 (4095);
        }
        else
        {
            LED_ON;
            DAC_Output1 (4095);
            DAC_Output2 (2047);
        }
        
        Wait_ms(200);
    }
}


void TF_Two_Complete_Channels_Hardware (void)
{
    DAC_Config ();
    
    // Start of Complete Channel 1
    DAC_Output1(512);
    TIM1_Init_pwm_CH1_CH1N_trig_CH2 ();
    
    // Start of Complete Channel 2
    DAC_Output2(512);    
    TIM8_Init_pwm_CH1_CH1N_trig_CH2 ();    
    
    while (1);
    
}


void TF_Soft_Pwm (void)
{
    // test only on ch1
    TIM1_Init_pwm_CH1_CH1N_trig_CH2 ();
    
    // Init TIM 7 for Soft or Int init
    TIM7_Init ();

    while (1)
    {
        Wait_ms (5000);

        Soft_PWM_Set_Channels (1, 0);    

        for (int i = 0; i < (255 + 1); i++)
        {
            Soft_PWM_Set_Channels (1, i);
            Wait_ms(200);
        }

        Wait_ms (5000);
    }
}

void TF_Usart3_Tx (void)
{
    Usart3Config();

    Usart3Send("Test single string send on 2 secs.\n");
    while (1)
    {
        Usart3Send("Mariano\n");
        Wait_ms(2000);
    }
}


void TF_Usart3_Tx_Rx_Int (void)
{
    // start usart3 and loop rx -> tx after 3 secs
    Usart3Config();
    char buff_local [128] = { 0 };
    unsigned char readed = 0;

    Usart3Send("Test string loop. Answers every 3 secs.\n");    
    while(1)
    {
        Wait_ms(2800);
        if (Usart3HaveData())
        {
            LED_ON;
            Usart3HaveDataReset();
            readed = Usart3ReadBuffer(buff_local, 127);
            *(buff_local + readed) = '\n';    //cambio el '\0' por '\n' antes de enviar
            *(buff_local + readed + 1) = '\0';    //ajusto el '\0'
            Usart3Send(buff_local);
            Wait_ms(200);
            LED_OFF;
        }
    }    
}


void TF_Two_Complete_Channels_Usart (void)
{
    // Start of Complete Channel 1
    TIM1_Init_pwm_CH1_CH1N_trig_CH2 ();
    
    // Start of Complete Channel 2
    TIM8_Init_pwm_CH1_CH1N_trig_CH2 ();    

    // Start Usart
    Usart3Config();
    char buff_local [128] = { 0 };
    unsigned char readed = 0;

    Usart3Send("Test usart string to pwm channels\n");

    // usart channels values
    unsigned char channel1 = 0;
    unsigned char channel2 = 0;

    while (1)
    {
        if (Usart3HaveData())
        {
            LED_ON;
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
                                channel1 = ch1 << 4;    // to 4092
                                channel2 = ch2 << 4;

                                // always have a minimun for ints
                                if (channel1 < 40)
                                    channel1 = 40;

                                if (channel2 < 40)
                                    channel2 = 40;
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
            else
                Usart3Send("err number of bytes\n");
            
            LED_OFF;
        }
    }
}

//--- end of file ---//

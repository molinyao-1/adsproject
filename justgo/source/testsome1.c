/*
 * testsome1.c
 *
 *  Created on: 2021��3��6��
 *      Author: jimi
 */

/*
 * testsome1.c
 *
 *  Created on: 2021��2��28��
 *      Author: jimi
 */
#include "testsome.h"
#include "stdint.h"
/*�������ɼ���ֵ����*/
extern int16 leftFTM;
extern int16 rightFTM;



void Encoder_get()
{
    leftFTM = -SmartCar_Encoder_Get(GPT12_T4);
    rightFTM = SmartCar_Encoder_Get(GPT12_T6);
    SmartCar_Encoder_Clear(GPT12_T4);
    SmartCar_Encoder_Clear(GPT12_T6);
}

/**/

/*�����ֵ����*/




/**/

/*����������*/
void Motortest()
{
    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_0_TOUT53_P21_2_OUT,0);
    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT54_P21_3_OUT,2000);
    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_2_TOUT55_P21_4_OUT,2000);
    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_3_TOUT56_P21_5_OUT,0);
}

void GuangDian_Init()
{
    GPIO_Init(P15,4,PULLUP,0);
    GPIO_Init(P11,10,PULLUP,0);
    GPIO_Init(P33,7,PULLUP,0);
    GPIO_Init(P20,9,PULLUP,0);
}

/**/









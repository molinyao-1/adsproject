/*
 * testsome1.c
 *
 *  Created on: 2021年3月6日
 *      Author: jimi
 */

/*
 * testsome1.c
 *
 *  Created on: 2021年2月28日
 *      Author: jimi
 */
#include "testsome.h"
#include "stdint.h"
/*编码器采集数值测试*/
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

/*舵机中值测试*/




/**/

/*电机方向测试*/
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









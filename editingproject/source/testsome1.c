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
#include"menu.h"

/*�������ɼ���ֵ����*/
extern int16 leftFTM;
extern int16 rightFTM;

extern float motor_lefdevn ;
extern float motor_rigdevn ;
extern float N_motor_lefdevn,N_motor_rigdevn;
extern uint32_t motor_lefdevnU;
extern uint32_t motor_rigdevnU;
extern uint32_t N_motor_lefdevnU,N_motor_rigdevnU;

extern int a;

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
    GPIO_Init(P15,8,PULLUP,0);
    GPIO_Init(P15,4,PULLUP,0);
}

/**/


/*
 ============================================================================
 Name        : KaerMan.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

static double p_last = 0;
static double x_last = 0;

//��������   ������¶ȼ��ж��ơ���
#define P_Q 0.1
//��������
#define M_R 0.3
/*
        Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
        R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��
        ����p�ĳ�ֵ�������ȡ�����ǲ���Ϊ0��Ϊ0�Ļ��������˲�������Ϊ�Ѿ��������˲����ˣ�
q,r��ֵ��Ҫ�����Գ����������˾���(������¶ȼ��ж��ƣ��Լ���ĳ������ж�ǿ)
r���������˲����������ʵ�����ߵ�����̶ȣ�rԽСԽ�ӽ���
q�������˲��������ƽ���̶ȣ�qԽСԽƽ����
*/
double KalmanFilter(double ResrcData)
{

    double R = M_R;
    double Q = P_Q;

    double x_mid = x_last;
    double x_now;

    double p_mid;
    double p_now;

    double kg;

    //����p_last ���� kalmanFilter_A ��pֱ��ȡ0
    x_mid = x_last;                       //x_last=x(k-1|k-1),x_mid=x(k|k-1)
    p_mid = p_last + Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����

    /*
      *  �������˲��������Ҫ��ʽ
      */
    kg = p_mid / (p_mid + R);                 //kgΪkalman filter��R Ϊ����
    x_now = x_mid + kg * (ResrcData - x_mid);   //���Ƴ�������ֵ
    p_now = (1 - kg) * p_mid;                 //����ֵ��Ӧ��covariance
    p_last = p_now;                     //����covariance ֵ
    x_last = x_now;                     //����ϵͳ״ֵ̬

    return x_now;
}

float kalmanFilter_A(float inData)
{
    static float prevData = 0;
    //����p�ĳ�ֵ�������ȡ�����ǲ���Ϊ0��Ϊ0�Ļ��������˲�������Ϊ�Ѿ��������˲����ˣ�
    static float p = 0.01, q = P_Q, r = M_R, kGain = 0;
    p = p + q;
    kGain = p / (p + r);

    inData = prevData + (kGain * (inData - prevData));
    p = (1 - kGain) * p;

    prevData = inData;

    return inData;
}




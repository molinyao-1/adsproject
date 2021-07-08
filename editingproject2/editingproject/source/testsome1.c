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
#include"menu.h"

/*编码器采集数值测试*/
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

//过程噪音   买的破温度计有多破。。
#define P_Q 0.1
//测量噪声
#define M_R 0.3
/*
        Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
        R:测量噪声，R增大，动态响应变慢，收敛稳定性变好
        其中p的初值可以随便取，但是不能为0（为0的话卡尔曼滤波器就认为已经是最优滤波器了）
q,r的值需要我们试出来，讲白了就是(买的破温度计有多破，以及你的超人力有多强)
r参数调整滤波后的曲线与实测曲线的相近程度，r越小越接近。
q参数调滤波后的曲线平滑程度，q越小越平滑。
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

    //这里p_last 等于 kalmanFilter_A 的p直接取0
    x_mid = x_last;                       //x_last=x(k-1|k-1),x_mid=x(k|k-1)
    p_mid = p_last + Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声

    /*
      *  卡尔曼滤波的五个重要公式
      */
    kg = p_mid / (p_mid + R);                 //kg为kalman filter，R 为噪声
    x_now = x_mid + kg * (ResrcData - x_mid);   //估计出的最优值
    p_now = (1 - kg) * p_mid;                 //最优值对应的covariance
    p_last = p_now;                     //更新covariance 值
    x_last = x_now;                     //更新系统状态值

    return x_now;
}

float kalmanFilter_A(float inData)
{
    static float prevData = 0;
    //其中p的初值可以随便取，但是不能为0（为0的话卡尔曼滤波器就认为已经是最优滤波器了）
    static float p = 0.01, q = P_Q, r = M_R, kGain = 0;
    p = p + q;
    kGain = p / (p + r);

    inData = prevData + (kGain * (inData - prevData));
    p = (1 - kGain) * p;

    prevData = inData;

    return inData;
}




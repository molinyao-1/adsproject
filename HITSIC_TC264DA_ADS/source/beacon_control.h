/*
 * beacon_control.h
 *
 *  Created on: 2020年12月20日
 *      Author: jimi
 */

#ifndef BEACON_CONTROL_H_
#define BEACON_CONTROL_H_

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include <ifxCpu_Irq.h>
#include "IfxScuWdt.h"
#include "SmartCar_Uart.h"
#include "SmartCar_Upload.h"
#include "SmartCar_Oled.h"
#include "SmartCar_Pwm.h"
#include "SmartCar_MPU.h"
#include "SmartCar_MT9V034.h"
#include "SmartCar_Systick.h"
#include "SmartCar_GPIO.h"
#include "SmartCar_PIT.h"
#include "SmartCar_Encoder.h"
#include "SmartCar_Upload.h"
#include "common.h"
#include "stdlib.h"
#include "stdint.h"

typedef struct
{
    float Kp;
    float Kd;
    float Ki;
    float ekSum;//误差总和
    float ekL; //上次的的误差
    float ekL2;//上上次的偏差
    float pulse;//输出
    float dek;//当前误差与上次误差的差
    float ek; //当前误差（目标值与当前值的差）
}pid;


typedef struct
{

    IfxGtm_Atom_ToutMap* chnlNumber;
    uint32_t dutyFreq;
    float dutyCycle;
}PWMCFG_T;

typedef struct
{
   float kp;
   float kd;
   float errPrev;
   float errCurr;
   float errDiff;
}
pidCtrl_t;

void FINAL_CONTROL();
void CAMERA_JUDGE();
void MODE_JUDGE();
void OBSTACLE_JUDGE();
void ERROR();
void SIDE_JUDGE();
void FIND_LIGHT();

void Servo_motorinit();
void Encoder_init();

void Servo_control();
void Motor_control();
//float Motorbpi(pid *pidm, float _err);
void STOP_JUDGE();
void TEST_DISTANCE();
float CarmSqrt(float x);

void Imudata_get();
void Imudataprocess();



////陀螺仪
//
//#define CTRL_SPDA_CTRL_MS   (10U)
//#define CTRL_ANG_CTRL_MS    (5U)
//#define CTRL_SPD_CTRL_MS    (100U)
//#define CTRL_DIR_CTRL_MS    (20U)
//#define CTRL_1G             (9.80f)
////#define CTRL_ASIN(x)        (arm_arcsin_f32(x))
//#define CTRL_ASIN(x)        (asin(x))
//#define CTRL_ACOS(x)        (acos(x))
//#define CTRL_SQRT(x,y)      (sqrt(x*x+y*y))
//#define CTRL_PI             (3.1415926f)
//#define CTRL_DEG2RAD(x)     (x * (CTRL_PI / 180.0f))
//#define CTRL_RAD2DEG(x)     (x * (180.0f / CTRL_PI))
////#define CTRL_ENCO_SPD_COEFF (270.689 / ((float)CTRL_SPD_CTRL_MS))
//#define CTRL_ENCO_SPD_COEFF (0.17122 / ((float)CTRL_SPD_CTRL_MS))
////void CTRL_FilterUpdate(uint32_t updateTime_ms);
//void CTRL_AngCtrl();
//
//void CTRL_FilterInit();


#endif /* BEACON_CONTROL_H_ */


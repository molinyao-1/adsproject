/*
 * testsome1.h
 *
 *  Created on: 2021Äê2ÔÂ28ÈÕ
 *      Author: jimi
 */

#ifndef SOURCE_TESTSOME1_H_
#define SOURCE_TESTSOME1_H_

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
#include "SmartCar_ADC.h"
#include "common.h"
#include <adrc.h>
#include "beacon_control.h"

void Encoder_get();
void Motortest();
void GuangDian_Init();
double KalmanFilter(double ResrcData);
float kalmanFilter_A(float inData);

#endif /* SOURCE_TESTSOME1_H_ */


/*
 * VA_get.h
 *
 *  Created on: 2021Äê3ÔÂ31ÈÕ
 *      Author: jimi
 */

#ifndef SOURCE_VA_GET_H_
#define SOURCE_VA_GET_H_

#include "SmartCar_Pwm.h"
#include "SmartCar_ADC.h"
#include "stdlib.h"
#include "SmartCar_Systick.h"

//void ChargepidInit();
//void PIDCTRL_ErrUpdate(pid1* chargepwm, float err);
//float PIDCTRL_CalcPIDGain(pid1* chargepwm);
///float Chargectrl(float err);
void ChargeVA();

void Battery_Get();
void Icharge_Get();
void BatterySCM_Get();
void IchargeSCM_Get();


void merge1(int a[],int left,int right,int mid);
void mergesort(int a[],int left,int right);




#endif /* SOURCE_VA_GET_H_ */

/**
 * Copyright 2018 - 2021 HITSIC
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef UTILITIES_LIB_ADRCCTRL_H
#define UTILITIES_LIB_ADRCCTRL_H


#include <stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct
{
    float v1;// v���˲���������ʱ��Ķ��׵�����������-r,r֮��
    float v2;// v1�ĵ���
    float h;//����ʱ��or��������
    float r;//�Σ���v1�Ķ�ʱ��Ķ��׵���������-r,r֮��
}adrc_td_t;

typedef struct
{
    //�����������ٶȻ�����
    float h;//����ʱ��or�������ڣ����ƻ����ڣ�
    float v;//�����Ŀ�꣨�趨�ٶȣ�
    float v1;
    float v2;
    float r;//�Σ���v1�Ķ��׵���������-r,r֮��
    float e;
    float e1;
    float e2;
    float z1;
    float z2;
    float z3;
    float u0;
    float u;//����Ŀ�����(pwm)
    float u_max;//������޷�
    float u_min;//������޷�
    float y;//����Ĳ�����(���������ص��ٶ�)
    float b;//�Σ�����ϵ��
    float b01;
    float b02;
    float b03;
    float w0;//�Σ��۲�������
    float kp;//��
    float kd;//��
}adrc_t;

//��ʼ����������������п���
void ADRCCTRL_TDInit(adrc_td_t* td_t, float h);

//����һ��TD
void ADRCCTRL_TD(adrc_td_t* td, float v);

//��ʼ����������������п���
void ADRCCTRL_Init(adrc_t* p,float h);

//����һ��adrc����
void ADRCCTRL_Update(adrc_t* p);

//����һ��adrc���ƣ�ֻ�����Ƿ�����d(�������΢�ִ�С��̬����kd,d_line�ǵ����ķֽ��ߣ������������)
void ADRCCTRL_UpdateFal(adrc_t* p, float d_line);







#ifdef __cplusplus
}
#endif



#endif // ! UTILITIES_LIB_ADRCCTRL_H



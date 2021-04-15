/**
 * Copyright 2018 - 2020 HITSIC
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
/**
 * @file    :   lib_pidctrl.h
 * @author  :   Chekhov Mark/�����(qq:905497173)
 * @version :   v0.1-beta.0
 *
 * @date    :   v0.1-beta.0 2019.10.18
 *
 * @brief   :   PID�����㷨
 */

#ifndef _LIB_PIDCTRL_H_
#define _LIB_PIDCTRL_H_

#include "stddef.h"
/*!
 * @addtogroup pidctrl
 * @{
 */

//#define assert(...)

typedef struct
{
    float kp, ki, kd;
    float errCurr, errIntg, errDiff, errPrev;
}pidCtrl_t;

typedef struct
{
    pidCtrl_t outer, inner;
}pid2Ctrl_t;

/**
 * @brief ����PID�ṹ�����
 *
 * @param _pid  Ҫ���õ�PID�����ṹ��ָ��
 * @param _kp  ����ϵ��
 * @param _ki  ����ϵ��
 * @param _kd  ΢��ϵ��
 */
void PIDCTRL_Setup(pidCtrl_t *_pid, float _kp, float _ki, float _kd);

/**
 * @brief ���첢����һ��PID�ṹ��ָ��
 *
 * @param _kp  ����ϵ��
 * @param _ki  ����ϵ��
 * @param _kd  ΢��ϵ��
 * @return pidCtrl_t*  ������ɵ�PID�����ṹ��ָ��
 */
pidCtrl_t* PIDCTRL_Construct(float _kp, float _ki, float _kd);

/**
 * @brief ����PIDƫ���֪ƫ����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _err  ƫ��
 */
void PIDCTRL_ErrUpdate(pidCtrl_t *_pid, float _err);

/**
 * @brief ����PIDƫ���֪΢����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _diff  ƫ���΢����
 */
void PIDCTRL_ErrUpdateByDiff(pidCtrl_t *_pid, float _diff);

/**
 * @brief ����PIDƫ���֪ƫ�����΢����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _err  ƫ��
 * @param _diff  ƫ���΢����
 */
void PIDCTRL_ErrUpdateByErrAndDiff(pidCtrl_t *_pid, float _err, float _diff);

/**
 * @brief ����������
 *
 * @param _pid  PID�����ṹ��ָ��
 * @return float ����������
 */
float PIDCTRL_CalcPGain(pidCtrl_t *_pid);

/**
 * @brief ����������
 *
 * @param _pid  PID�����ṹ��ָ��
 * @return float ���ֽ�����
 */
float PIDCTRL_CalcIGain(pidCtrl_t *_pid);

/**
 * @brief ����΢�����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @return float ΢�ֽ�����
 */
float PIDCTRL_CalcDGain(pidCtrl_t *_pid);

/**
 * @brief ����PID�����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @return float PID������
 */
float PIDCTRL_CalcPIDGain(pidCtrl_t *_pid);

/**
 * @brief ����ƫ�������PID�������֪ƫ����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _err  ƫ��
 * @return float PID������
 */
float PIDCTRL_UpdateAndCalcPID(pidCtrl_t *_pid, float _err);

/**
 * @brief ����ƫ�������PID�������֪΢����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _diff  ƫ���΢����
 * @return float PID������
 */
float PIDCTRL_UpdateByDiffAndCalcPID(pidCtrl_t *_pid, float _diff);

/**
 * @brief ���ô���PID�ṹ�������
 *
 * @param _pid  Ҫ���õĴ���PID�����ṹ��ָ��
 * @param _okp  �⻷����ϵ��
 * @param _oki  �⻷����ϵ��
 * @param _okd  �⻷΢��ϵ��
 * @param _ikp  �ڻ�����ϵ��
 * @param _iki  �ڻ�����ϵ��
 * @param _ikd  �ڻ�΢��ϵ��
 */
void PID2CTRL_Setup(pid2Ctrl_t *_pid, float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd);

/**
 * @brief ���첢����һ������PID�ṹ��ָ��
 *
 * @param _okp  �⻷����ϵ��
 * @param _oki  �⻷����ϵ��
 * @param _okd  �⻷΢��ϵ��
 * @param _ikp  �ڻ�����ϵ��
 * @param _iki  �ڻ�����ϵ��
 * @param _ikd  �ڻ�΢��ϵ��
 * @return pid2Ctrl_t*  ����PID�����ṹ��ָ��
 */
pid2Ctrl_t* PID2CTRL_Construct(float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd);

/**
 * @brief ����ƫ������㴮��PID�������֪ƫ����
 *
 * @param _pid  ����PID�����ṹ��ָ��
 * @param _err  ƫ������
 * @return float ����PID������
 */
float PID2CTRL_UpdateAndCalcPID(pid2Ctrl_t *_pid, float _err);

/**
 * @brief ����ƫ������㴮��PID�������֪΢����
 *
 * @param _pid  ����PID�����ṹ��ָ��
 * @param _diff  ƫ��΢������
 * @return float ����PID������
 */
float PID2CTRL_UpdateByDiffeAndCalcPID(pid2Ctrl_t *_pid, float _diff);

/* @} */

#endif // ! _LIB_PIDCTRL_H_

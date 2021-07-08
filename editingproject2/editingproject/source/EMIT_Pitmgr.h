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

/***************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018, HITSIC/���������ܳ����¾��ֲ�
 * All rights reserved.
 *
 * @brief           PIT�жϹ����� C���԰汾
 * @file            sys_pitmgr.h
 * @company         HITSIC/���������ܳ����¾��ֲ�
 * @author          C.M. (QQ:905497173)
 * @version         v3.2
 * @Software
 * @Target_core     NXP MK66F18 or other K Series MCU
 * @date            ver1.0 2018.11.25 by C.M.
 * @date            ver1.5 2019.02.07 by C.M.
 * @date            ver2.1 2019.04.12 by C.M.
 * @date            ver3.1 2019.09.28 by C.M.
 * @date            ver3.2 2019.09.29 by C.M.
 *
 * @note����ģ��ʹ����PIT Chnl 0,1,2��ʹ��ʱ��ע�������ж����ȼ�����
        ������ͬһ���ж����������ж�������³�ʱ����ʱ����ɿ���ʱ��
        ���ң��������ſ��Ƶ��������С�
        ��ģ�����ʵ����us����ȷ��ʱ����������ʱ��ms����ʱ�жϹ�����
        ȷ��ʱ�����ڲ�����������ʱ�䡣��ʱ������Ƕ��ʹ�ã����ụ���ͻ��
        ����Ӧ�ñ������жϷ�������ʹ����ʱ��
****************************************************************/

#pragma once

#ifndef _EMT1_PITMGR_H_
#define _EMT1_PITMGR_H_

#include "common.h"
#include "limits.h"
#include "SmartCar_PIT.h"
#include "EMIT_List.h"

typedef int32_t status_t;

enum
{
    kStatus_Success = 0,  /*!< Generic status for Success. */
    kStatus_Fail = 1,      /*!< Generic status for Fail. */
};

//#define RTE_PIT_CLKFREQ CLOCK_GetFreq(kCLOCK_BusClk)

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief : PIT�������жϷ�������
 */
typedef void (*pitmgr_handler_t)(void * userData);

/**
 * @brief : PIT�������������ã�����־λö�١�
 */
typedef enum pitmgr_pptFlag// : uint32_t
{
    pitmgr_pptEnable = 1 << 0,
    pitmgr_pptRunOnce = 1 << 1,
}pitmgr_pptFlag_t;

/**
 * @brief : PIT��������������ڴ洢ÿ��PIT�жϵ����ݡ�
 */
typedef struct pitmgr_handle
{
    uint32_t ms, mso;           /* ���ж���ÿms����ĵ�_mso�������С� */
    pitmgr_handler_t handler;   /* ָ���жϷ�������ָ�롣 */
    uint32_t pptFlag;           /* ���Ա�־λ����pitmgr_pptFlag_t�İ�λ��ϡ� */
    uint64_t prevTime_us;       /* ���ж��ϴ����к�ʱ�� */
    void* userData;             /* �û�����ָ�� */
}pitmgr_handle_t;

/**
 * @brief : �жϷ������ļ�ʱ��������λΪms
 */
extern uint32_t pitmgr_timer_ms;

/**
 * PIT�жϱ��Ե�������洢��
 */
extern list_struct_t pitmgr_isrSet;

/**
 * @brief : PIT��������ʼ����
 *  ��ʼ��������PIT���衢PIT�жϡ�
 *
 * @return {status_t} : �ɹ�����kStatus_Success���쳣����kStatus_Fail��
 */
status_t PITMGR_Init(void);

/**
 * @brief : ��ʼ��һ��PIT��ʱ�жϾ����
    �ú����������ݼ�鲢��ֵ����Ҫ���Ᵽ����

 * @param {pitmgr_handle_t*} _h       : Ҫ���õ�PIT�жϾ��
 * @param {uint32_t} _ms              : ��PIT�жϵ��жϼ����
 * @param {uint32_t} _mso             : ��PIT�жϵ��ж���λ�������ֶ����ؾ��⡣
 * @param {pitmgr_handler_t} _handler : ��PIT�жϵ��жϷ�������
 * @param {uint32_t} _ppt             : ��PIT�жϵ�����Flag��
 */
void PITMGR_HandleSetup(pitmgr_handle_t* _h, uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt);

/**
 * @brief : ��PIT�жϱ�ĩβ����һ���µ��жϾ����
 *  �ú����������ݼ�鲢��ֵ����Ҫ���Ᵽ����
 *
 * @param {uint32_t} _ms              : ��PIT�жϵ��жϼ����
 * @param {uint32_t} _mso             : ��PIT�жϵ��ж���λ�������ֶ����ؾ��⡣
 * @param {pitmgr_handler_t} _handler : ��PIT�жϵ��жϷ�������
 * @param {uint32_t} _ppt             : ��PIT�жϵ�����Flag��
 * @return {pitmgr_handle_t*}         : ����ָ���²����PIT�жϾ����ָ�롣�������ʧ���򷵻ؿ�ָ�롣�û������м�顣
 */
pitmgr_handle_t* PITMGR_HandleInsert(uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt);

/**
 * @brief : ����ĳ��PIT�жϾ�������ԡ�
 *
 * @param {pitmgr_handle_t*} _h : Ҫ���õ�PIT�жϾ��
 * @param {bool} b              : �Ƿ����ø��жϾ��
 */
void PITMGR_HandleSetEnable(pitmgr_handle_t* _h, bool b);
status_t PITMGR_HandleRemove(pitmgr_handle_t* _h);


/**
 * @brief : PIT�жϵĴ���������IRQHandler���á�
 */
void PITMGR_Isr(void);

#ifdef __cplusplus
}
#endif


#endif // HITSIC_USE_SYS_PITMGR

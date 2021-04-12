/**
 * Copyright 2018 - 2019 HITSIC
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

/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,���������ܳ����¾��ֲ�
 * All rights reserved.
 *
 * @brief           ��������
 * @file            list.h
 * @company         ���������ܳ����¾��ֲ�
 * @author          Ф���� (QQ:1761690868); C.M. (QQ:905497173)
 * @version         v1.1
 * @Software
 * @Target_core     any platform
 * @date            ver1.0 2019.09.26 by Ф����
 * @date            ver1.1 2019.09.28 by C.M.
 *
 * @note����������ҵ��ѧ���ܳ����¾��ֲ�ר�ã�����й¶
***************************************************************************************************************************/

#pragma once
#ifndef _EMT1_LIST_H_
#define _EMT1_LIST_H_
#include "common.h"
#include <stdlib.h>
#include "stdbool.h"
#include "stdint.h"
#include "SmartCar_Assert.h"

#ifdef __cplusplus
extern "C" {
#endif


// Library version
#define LIST_VERSION "0.1.5"

// Memory management macros
#ifndef LIST_MALLOC
#define LIST_MALLOC malloc
#endif
#ifndef LIST_FREE
#define LIST_FREE free
#endif

/*
* list_t node struct.
*/
typedef struct list_node {
    void *data;
    struct list_node *prev;
    struct list_node *next;
} list_node_t;

/*
 * list_t struct.
 */
typedef struct list_struct {
    list_node_t *head;
    list_node_t *tail;
    unsigned int SizeOfData;
    unsigned int size;
} list_struct_t;

/**
 * @brief   ����Ĺ��캯����
 *
 * @param  {list_struct_t*} p    : list��������
 * @param  {unsigned} int : ÿ��node�洢�����ݶ��
 */
void List_Constructor(list_struct_t *p, unsigned int SizeOfData_);

/**
 * @brief   ���������������
 *
 * @param  {list_struct_t*} p    : list��������
 */
void List_Destructor(list_struct_t *p);

/**
 * @brief   �������������ɵ����ڵ�������
 */
uint32_t List_MaxSize(void);

/**
 * @brief   ��������Ƿ�Ϊ�ա�
 *
 * @param  {list_struct_t*} p    : list��������
 * @return {bool}         : ��������true���ǿ�������false��
 */
bool List_IsEmpty(list_struct_t *p);

/**
 * @brief  ��� ��������Ч�ڵ�ĸ�����
 *
 * @param  {list_struct_t*} p    : list��������
 * @return {uint32_t}     : �����ڽڵ�ĸ�������������0��
 */
uint32_t List_GetSize(list_struct_t *p);

/**
 * @brief                     : ���������½��ڵ㡣
 *
 * @param  {list_struct_t*} p        : list��������
 * @param  {list_node_t*} pos : Ҫ����ڵ��λ�á��½ڵ����ǲ��뵽��λ��֮ǰ��
 * @return {list_node_t*}     : ����ָ�򴴽��Ľڵ��ָ�롣
 */
list_node_t* List_Create(list_struct_t* p, list_node_t* pos);

/**
 * @brief                     : �������ڲ���ڵ㡣
 *
 * @param  {list_struct_t*} p        : list��������
 * @param  {list_node_t*} pos : Ҫ����ڵ��λ�á��½ڵ����ǲ��뵽��λ��֮ǰ��
 * @param  {void*} data       : Ҫ����ڵ��ڵ����ݡ����ݴ�СӦ�빹������ʱһ�¡�
 * @return {list_node_t*}     : ����ָ�����Ľڵ��ָ�롣
 */
list_node_t* List_Insert(list_struct_t *p, list_node_t *pos, void *data);

/**
 * @brief   ɾ�������һ���ڵ�
 *
 * @param  {list_struct_t*} p        : list������
 * @param  {list_node_t*} pos : Ҫɾ���Ľڵ�
 * @return {int}              : �������
 */
void List_Erase(list_struct_t *p, list_node_t *pos);

/**
 * @brief   ɾ�������ڵ�һ��Ԫ��
 *
 * @param  {list_struct_t*} p    : list������
 */
void List_PopFront(list_struct_t *p);

/**
 * @brief   ɾ�����������һ��Ԫ��
 *
 * @param  {list_struct_t*} p    : list������
 */
void List_PopBack(list_struct_t *p);

/**
 * @brief   ������ͷ������ڵ㡣
 *
 * @param  {list_struct_t*} p    : list������
 * @param  {void*} data   : Ҫ����ڵ��ڵ����ݡ����ݴ�СӦ�빹������ʱһ�¡�
 * @return {list_node_t*} : ����ָ�����Ľڵ��ָ�롣
 */
list_node_t* List_PushFront(list_struct_t *p, void *data);

/**
 * @brief   ������β������ڵ㡣
 *
 * @param  {list_struct_t*} p    : list������
 * @param  {void*} data   : Ҫ����ڵ��ڵ����ݡ����ݴ�СӦ�빹������ʱһ�¡�
 * @return {list_node_t*} : ����ָ�����Ľڵ��ָ�롣
 */
list_node_t* List_PushBack(list_struct_t *p, void *data);

/**
 * @brief   �������
 *
 * @param  {list_struct_t*} p    : list������
 */
void List_Clear(list_struct_t *p);

/**
 * @brief   ����ָ�������ڵ�һ���ڵ����洢Ԫ�ص�ָ�롣
 *
 * �û���Ҫ���м�������Ƿ�Ϊ�ա�
 *
 * @param  {list_struct_t*} p    : list������
 */
void *List_GetFront(list_struct_t *p);

/**
 * @brief   ����ָ�����������һ����Ч�ڵ����洢Ԫ�ص�ָ�롣
 *
 * @param  {list_struct_t*} p    : list��������
 */
void *List_GetBack(list_struct_t *p);

/**
 * @brief   ����ָ�������һ���ڵ��ָ�롣
 *
 * @param  {list_struct_t*} p    : list��������
 */
list_node_t* List_ItBegin(list_struct_t *p);

/**
 * @brief   ����ָ���������һ����Ч�ڵ�֮��Ľڵ��ָ�롣
 *
 * @param  {list_struct_t*} p    : list��������
 */
list_node_t* List_ItEnd(list_struct_t *p);



#ifdef __cplusplus
}
#endif
#endif //_EMT1_LIST_H_

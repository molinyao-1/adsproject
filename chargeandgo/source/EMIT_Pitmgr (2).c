

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

#include "EMIT_Pitmgr.h"



#ifdef __cplusplus
extern "C" {
#endif

uint32_t pitmgr_timer_ms;
list_struct_t pitmgr_isrSet;

status_t PITMGR_Init(void)
{
    List_Constructor(&pitmgr_isrSet, sizeof(pitmgr_handle_t));
    pitmgr_timer_ms = 0;
    Pit_Init_ms(CCU6_0, PIT_CH0, 1);
    return kStatus_Success;
}

void PITMGR_HandleSetup(pitmgr_handle_t* _h, uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt)
{
    assert(_h);
    assert(_ms);
    assert(_handler);
    _h->ms = _ms;
    _h->mso = _mso;
    _h->handler = _handler;
    _h->pptFlag = _ppt;
    _h->userData = NULL;
}

pitmgr_handle_t* PITMGR_HandleInsert(uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt)
{
    pitmgr_handle_t* ret = (pitmgr_handle_t*)(List_Create(&pitmgr_isrSet, List_ItEnd(&pitmgr_isrSet)) -> data);
    PITMGR_HandleSetup(ret, _ms, _mso, _handler, _ppt);
    return ret;
}

void PITMGR_HandleSetEnable(pitmgr_handle_t* _h, bool b)
{
    if (b)
    {
        _h->pptFlag |= pitmgr_pptEnable;
    }
    else
    {
        _h->pptFlag &= ~pitmgr_pptEnable;
    }
}

status_t PITMGR_HandleRemove(pitmgr_handle_t* _h)
{
    for (list_node_t* it = List_ItBegin(&pitmgr_isrSet); it != List_ItEnd(&pitmgr_isrSet); it = it->next)
    {
        if (it->data == _h)
        {
            List_Erase(&pitmgr_isrSet, it);
            return kStatus_Success;
        }
    }
    return kStatus_Fail;
}

void PITMGR_Isr(void)
{
    for (list_node_t* it = List_ItBegin(&pitmgr_isrSet); it != List_ItEnd(&pitmgr_isrSet); it = it->next)
    {
        pitmgr_handle_t* handle = (pitmgr_handle_t*)(it->data);
        if (handle->pptFlag & pitmgr_pptEnable && pitmgr_timer_ms % handle->ms == handle->mso)
        {
            (*(handle->handler))(handle -> userData);
            if (handle->pptFlag & pitmgr_pptRunOnce)
            {
                handle->pptFlag ^= pitmgr_pptEnable;
            }
        }
    }
    ++pitmgr_timer_ms;
}



#ifdef __cplusplus
}
#endif






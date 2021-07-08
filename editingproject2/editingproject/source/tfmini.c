/*
 * tfmini.c
 *
 *  Created on: 2021年5月30日
 *      Author: jimi
 */
#include "tfmini.h"
IfxAsclin_Asc TFmini_uart;
TFmini_Data_t tfminiData = {0};
uint8 TFmini_receive_num = 0;
uint8 TFmini_receive_flag = 0;
uint8 TFminiBuffer[9] = {0};
extern int Dist;
extern int Strength;

/*双机通信*/
extern int lightx2 ;
extern int lighty2 ;
extern int area2 ;
extern int dis2 ;
extern uint8 connectnumB;//连通域数


/**
 *  初始化激光测距
 **/
void TFmini_init(void)
{
    SmartCar_Uart_Init(IfxAsclin0_TX_P14_0_OUT,IfxAsclin0_RXA_P14_1_IN,115200,0);
    TFmini_uart = uart[0];
}
/*
 * 获得UART缓存
 */
boolean TFmini_uartRead(uint8* txData, Ifx_SizeT count)
{
    return IfxAsclin_Asc_read(&TFmini_uart,txData,&count,TIME_INFINITE);
}
/*
 * 获得测距结构体
 */
void TFmini_getData(TFmini_Data_t* TFminiHandle)
{
    if(TFmini_receive_flag==1)
    {
        TFminiHandle->isReadSuccess = 1;
        TFminiHandle->distance = (TFminiBuffer[3]<<8)+TFminiBuffer[2];
        TFminiHandle->strength = (TFminiBuffer[5]<<8)+TFminiBuffer[4];
        if(TFminiHandle->strength<100||TFminiHandle->strength==OVEREXPOSURE)
        {
            TFminiHandle->isConfident = 0;
        }
        else
        {
            TFminiHandle->isConfident = 1;
        }
        if(TFminiHandle->isConfident == 1)
        {
            Dist = tfminiData.distance;
            Strength = tfminiData.strength;
        }
        TFmini_receive_flag = 0;
    }
}

uint8 TFmini_statusDetect(TFmini_Data_t* TFminiHandle)
{
    if(TFminiHandle->isReadSuccess==1&&TFminiHandle->isConfident==1)
    {
        if(TFminiHandle->distance>=100)
        {
            return 0;
        }
        else if(TFminiHandle->distance>50&&TFminiHandle->distance<100)
        {
            return 1;
        }
        else if(TFminiHandle->distance<=50)
        {
            return 2;
        }
    }
    else
    {
        return 3;
    }
    return 4;
}

uint8 TFmini_Uart_Queue(uint8* rxData, uint8 uart_num)
{
    if(IfxAsclin_Asc_getReadCount(&uart[uart_num]) > 0)
    {
        *rxData = IfxAsclin_Asc_blockingRead(&uart[uart_num]);
        return 1;
    }
    return 0;
}


void TFmini_UARTCallBack(void)
{
    while(TFmini_Uart_Queue(&TFminiBuffer[TFmini_receive_num], UARTNUM))
    {
        TFmini_receive_num++;
        if(1 == TFmini_receive_num && 0x59 != TFminiBuffer[0])
        {
            TFmini_receive_num = 0;
        }
        if(2 == TFmini_receive_num && 0x59 != TFminiBuffer[1])
        {
            TFmini_receive_num = 0;
        }
        if(9 == TFmini_receive_num)
        {
            unsigned int temp = TFminiBuffer[0]+TFminiBuffer[1]+TFminiBuffer[2]+TFminiBuffer[3]+TFminiBuffer[4]+TFminiBuffer[5]+TFminiBuffer[6]+TFminiBuffer[7];
            if((uint8)temp == TFminiBuffer[8])
            {
                TFmini_receive_flag = 1;
            }
            else
            {
                TFmini_receive_flag = 0;
            }
            TFmini_receive_num = 0;
            IfxAsclin_Asc_clearRx(&uart[UARTNUM]);
        }
    }
}

void TC212_UARTCallBack(void)
{
    while(TFmini_Uart_Queue(&TFminiBuffer[TFmini_receive_num], UARTNUM))
    {
        TFmini_receive_num++;
        if(1 == TFmini_receive_num && '@'!= TFminiBuffer[0])
        {
            TFmini_receive_num = 0;
        }
        if(7 == TFmini_receive_num)
        {
            if('$' == TFminiBuffer[6])
            {
                TFmini_receive_flag = 1;
            }
            else
            {
                TFmini_receive_flag = 0;
            }
            if(TFmini_receive_flag == 1)
            {
                lightx2 = TFminiBuffer[1];
                lighty2 = TFminiBuffer[2];
                area2 = TFminiBuffer[3];
                dis2 = TFminiBuffer[4];
                connectnumB = TFminiBuffer[5];
            }
            TFmini_receive_num = 0;
            IfxAsclin_Asc_clearRx(&uart[UARTNUM]);
        }
    }
}




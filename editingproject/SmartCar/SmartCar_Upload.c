/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @file            SmartCar_Encoder
 * @company         哈工大智能车创新俱乐部
 * @author          李洋 qq:2367381108
 * @version         v1.0
 * @Software        ADS
 * @Target core     TC264
 * @date            2021-2-05
 *
 * @note：上位机数据及图像发送

    哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/



#include "SmartCar_Upload.h"
#include "stdarg.h"
#include "stdlib.h"

void SmartCar_Uart_Upload(uint8* txData, Ifx_SizeT count,uint8 uartnum)
{
    SmartCar_Uart_Transfer(txData, count, uartnum);
}


void SmartCar_VarUpload(float *my_var, uint8 var_num)
{
    uint8 cmdf[7] =
    { 0x55, 0xaa, 0x11, 0x55, 0xaa, 0xff, 0x01 };
    uint8 cmdr = 0x01;
    uint8 begin_cmd[3] =
    { 0x55, 0xaa, 0x11 };
    /*! 发送帧头 */
    SmartCar_Uart_Upload(begin_cmd, sizeof(begin_cmd),0);
    /*! 发送数据个数 */
    SmartCar_Uart_Upload(cmdf, sizeof(cmdf),0);
    SmartCar_Uart_Upload(&var_num, 1,0);
    /*! 发送数据 */
    SmartCar_Uart_Upload((uint8* )(my_var), var_num * 4U,0);
    /*! 发送帧尾 */
    SmartCar_Uart_Upload(&cmdr, 1,0);
}

void SmartCar_ImgUpload(uint8 *upload_img, uint8 row, uint8 col)
{
    uint8 new_upload_image[96][150] = {0};
    uint8 cmd = 3, icmd = ~3;
    memcpy(&new_upload_image[0][0],upload_img, row * col);
//    uint8 cmd = 0, icmd = 0xff,jcmd = 0x01,kcmd = 0x01;
    uint8 cmdf[2] =
    { cmd, icmd };
    uint8 cmdr[2] =
    { icmd, cmd };
    /*! 发送帧头 */
    SmartCar_Uart_Upload(cmdf, sizeof(cmdf),0);
    /*! 发送数据 */
    SmartCar_Uart_Upload(new_upload_image, row * col,0);
    /*! 发送帧尾 */
    SmartCar_Uart_Upload(cmdr, sizeof(cmdr),0);
}

void PRINFT(const char* ftm, ...)
{
    va_list args;
    va_start(args, ftm);
    //char p_ch[100] = {0};
    char* p_ch = (char* )malloc(sizeof(char) * 100);
    if(p_ch != NULL)
    {
        vsnprintf(p_ch, 100, ftm, args);
        uint8 i = 0;
        while(p_ch[i] != '\0')
        {
            SmartCar_Uart_Upload((uint8 *)p_ch+i, 1,2);
            i++;
        }
    }
    free(p_ch);
    p_ch = NULL;
}

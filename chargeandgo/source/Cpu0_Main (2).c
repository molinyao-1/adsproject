/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/
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
#include "SmartCar_ADC.h"
#include "common.h"
#include "image.h"
#include "menu.h"
#include "beacon_control.h"
#include "testsome.h"
#include "VA_get.h"
#include "EMIT_Pitmgr.h"
#include "EMIT_List.h"
#include "SmartCar_Assert.h"



#pragma section all "cpu0_dsram"
//IfxCpu_syncEvent g_cpuSyncEvent;
mpu_t this_mpu;
float time;
float myvar[20] = {0};
extern int16 leftFTM;
extern int16 rightFTM;
/*电压电流采集值*/
extern float battery;
extern float Icharge;
extern float battery_last;
extern float battery_prelast;
extern float powerduty;
/**/


/*extern专区*/
extern int mode_flag;
extern int error_simu;
extern float servo_pulse;
extern int light_flag;
extern int aver_x;
extern int simu_y;
extern int motordutyset;
extern float motor_rigdevn;
extern float motor_lefdevn;
extern int side_flag;
extern int servoinit;
extern int flagservo;
extern float Pcharge;
extern float powererror;

extern float power;
extern float setpower;
extern float setpower1;
extern uint32 powerduty1;
extern int countflag;
extern float chargeKp;
extern float chargeKi;
extern float chargeKd;
extern float chargeek;
extern float chargeekL;
extern float chargedek;
extern int lightsensor[2];
/**/


extern uint32_t motor_lefdevnU;
extern uint32_t motor_rigdevnU;
extern uint32_t N_motor_lefdevnU,N_motor_rigdevnU;
extern uint8 connectnum;
extern int areaA_time;
extern uint32_t servofinal;
extern int servodirection;
extern float lighty;
extern int aver_y;
extern float lightx,lights,lightc;
extern int servoduty;
extern float Rdesiredspeed;
extern float Ldesiredspeed;
extern int light_square;
extern int squarelimit;

int status = 0;
int gogoflag = 0;
/*oled灯和图像的切换*/
int mode = 0;
int timeget = 0;
float acc[3] = {0};
float gyro[3] = {0};
float gyrolast[3] = {0};
float gyronow[3] = {0};
extern float angle_x,angle_y,angle_z;

extern int freshposi;
extern int freshname;

pitmgr_handle_t* ctrl_motorCtrlHandle = NULL;
pitmgr_handle_t* ctrl_servoCtrlHandle = NULL;
pitmgr_handle_t* ctrl_chargeCtrlHandle = NULL;

#define CTRL_MOTOR_CTRL_MS 5
#define CTRL_SERVO_CTRL_MS 20
#define CTRL_CHARGE_CTRL_MS 5


 int core0_main(void)
{
    IfxCpu_disableInterrupts();

    get_clk();
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    /****初始化外设****/
    /**/

    /*总钻风初始化*/
    SmartCar_MT9V034_Init();

    /*初始化mpu*/
    SmartCar_MPU_Set_DefaultConfig(&this_mpu);
    SmartCar_MPU_Init2(&this_mpu);
 //   SmartCar_GyroOffset(this_mpu);
    /**/
    /**/
    /*定时中断初始化*/
//    /*电机*/
//    Pit_Init(CCU6_0,PIT_CH0,5*1000);
////    Pit_Enable_Interrupt(CCU6_0,PIT_CH0);
////    Pit_Start(CCU6_0,PIT_CH0);
    Pit_Init(CCU6_1,PIT_CH0,0.5*1000);
//    /*舵机*/
//    Pit_Init(CCU6_1,PIT_CH1,5*1000);

    PITMGR_Init();
    ctrl_motorCtrlHandle = PITMGR_HandleInsert(CTRL_MOTOR_CTRL_MS,2U,Motor_control,pitmgr_pptEnable);
    assert(ctrl_motorCtrlHandle);
    ctrl_servoCtrlHandle = PITMGR_HandleInsert(CTRL_SERVO_CTRL_MS,0U,Servo_control,pitmgr_pptEnable);
    assert(ctrl_servoCtrlHandle);
    ctrl_chargeCtrlHandle = PITMGR_HandleInsert(CTRL_CHARGE_CTRL_MS,3U,ChargeVA,pitmgr_pptEnable);
    assert(ctrl_chargeCtrlHandle);
    /*PWM初始化*/
    Servo_motorinit();
    SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_6_TOUT6_P02_6_OUT,20000,0);
    /**/

    /*编码器初始化*/
    Encoder_init();

    /*串口初始化*/
    SmartCar_Uart_Init(IfxAsclin0_TX_P14_0_OUT,IfxAsclin0_RXA_P14_1_IN,921600,0);
    //SmartCar_Uart_Init(IfxAsclin2_TX_P10_5_OUT,IfxAsclin2_RXD_P10_6_IN,921600,2);
    /**/

    /*GPIO初始化*/
    /*菜单用GPIO初始化*/
    Menu_gpioinit();

//
    /*ADC初始化*/
//    ADC_Init(ADC_0,ADC0_CH8_A8);
//    ADC_Init(ADC_0,ADC0_CH11_A11);
//    ADC_Init(ADC_0,ADC0_CH0_A0);
    ADC_Init(ADC_1,ADC1_CH5_A21);//����
    ADC_Init(ADC_1,ADC1_CH4_A20);//��ѹ
    /**/

////    /*电机舵机测试*/
//    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,860);
////    /**/
//
    /*外部中断初始化*/
    Eru_Init(CH5_P15_8,FALLING);
    Eru_Init(CH0_P15_4,FALLING);
    Eru_Init(CH4_P33_7,FALLING);
//    Eru_Init(CH7_P20_9,FALLING);
    /*光电管*/
    GuangDian_Init();
//   ChargepidInit();
    /**/
    /*OLED初始化*/
    SmartCar_Oled_Init();
    /*初始化菜单*/
    SmartCar_OLED_Fill(0);
    MENU_Tree();
//    /*显示首页面*/
    MENU_Start();
    MENU_Read();

    IfxCpu_enableInterrupts();
    Eru_Disable(CH0_P15_4);
    Eru_Disable(CH4_P33_7);
//    Eru_Disable(CH7_P20_9);
//
    while(TRUE)
    {
//        while(!mt9v034_finish_flag){}
//            mt9v034_finish_flag = 0;
//        Systick_Start(STM0);
//        THRE();
//        image_main();
//        time = GetTime_ms(STM0);
//        myvar[0] = time;
//        /*测输入电压电流*/
//        GetVA();
//        myvar[1] = CV_IN;
//        myvar[2] = CA_IN; //电流
//        lightsensor[0] = GPIO_Read(P15,4);
//        lightsensor[1] = GPIO_Read(P11,10);
//        lightsensor[2] = GPIO_Read(P33,7);
//        lightsensor[3] = GPIO_Read(P20,9);
//        lightsensor[0]= GPIO_Read(P11,10);
//        lightsensor[1]= GPIO_Read(P20,9);
//        Imudata_get();
//        Imudata_get();

//        myvar[3] = side_flag;
//        myvar[4] = light_flag;
//        myvar[5] = gyro[2];
////        myvar[6] = ;
////        myvar[7] = battery;
////
////        myvar[8] = Icharge;

        /*显示图像*/
//
            if(mode == 0){
                MENU_Make();
                //SmartCar_VarUpload(myvar,9);
                //MENU_fresh();
            }
            else if(1==mode)
            {
                MENU_fresh();
                //MENU_Make();
            }
            else if(2==mode)
            {
                SmartCar_Show_IMG(&IMG[0][0],120,188);
            }
//        /**/
//        /*按键操作*/
//        else if(mode == 1){
//            SmartCar_Show_IMG(&IMG[0][0],120,188);
//        }
//        else if(mode == 2)
//        {
//            SmartCar_VarUpload(myvar,9);
//        }
//        else if(mode == 3){}
//
//        if(battery>=9&&battery_last>=9&&battery_prelast>=9&&timeget == 500)
//        {
//            gogoflag = 1;
//        }

        /*采图*/
//      SmartCar_ImgUpload((uint8*)IMG,120,188);  //采图用函数
 //      SmartCar_ImgUpload((uint8*)mt9v034_image,120,188);  //采图用函数
       /**/
    }
}
//oled刷新
 MENU_freshadd()
 {
//     MENU_freshfloat("gyro[0]",gyro[0]);
//     MENU_freshfloat("gyro[1]",gyro[1]);
     //MENU_freshfloat("Ldesiredsp",Ldesiredspeed);
          //MENU_freshfloat("Rdesiredsp",Rdesiredspeed);
  //   MENU_freshfloat("gyro[2]",gyro[2]);
//     MENU_freshfloat("angle_x",angle_x);
//     MENU_freshfloat("angle_y",angle_y);
//     MENU_freshfloat("angle_z",angle_z);
//     MENU_freshfloat("time",time);
//     MENU_freshint("servoinit",servoinit);
     MENU_freshint("gogoflag",gogoflag);
     MENU_freshfloat("battery",battery);

     MENU_freshfloat("Pcharge",Pcharge);

     MENU_freshfloat("powerduty",powerduty);
//     MENU_freshint("side_flag",side_flag);
//     MENU_freshint("lightsensor[0]",lightsensor[0]);
//     MENU_freshint("lightsensor[1]",lightsensor[1]);
//     MENU_freshint("light_square",light_square);
//     MENU_freshint("mode_flag",mode_flag);

 }


/*定时器中断*/
/*20ms*/
IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    enableInterrupts();//开启中断嵌套
    PIT_CLEAR_FLAG(CCU6_0, PIT_CH0);
    PITMGR_Isr();
}
///*5ms*/
IFX_INTERRUPT(cc61_pit_ch1_isr, 0, CCU6_1_CH1_ISR_PRIORITY)
{
    enableInterrupts();//开启中断嵌套
//    if(gogoflag == 1){
//        Servo_control();
//    }

//    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,servoduty);
    PIT_CLEAR_FLAG(CCU6_1, PIT_CH1);
    ChargeVA();
}
///*5ms*/
IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    enableInterrupts();//开启中断嵌套
    PIT_CLEAR_FLAG(CCU6_1, PIT_CH0);
    FINAL_CONTROL();
}
/**/

/*外部中断*/
IFX_INTERRUPT(eru_ch1_ch5_isr, 0, ERU_CH1_CH5_INT_PRIO)
{
    enableInterrupts();//开启中断嵌套
    if(GET_GPIO_FLAG(CH1_P10_8))//通道1中断
    {
        CLEAR_GPIO_FLAG(CH1_P10_8);
    }
    if(GET_GPIO_FLAG(CH5_P15_8))//通道5中断
    {
        CLEAR_GPIO_FLAG(CH5_P15_8);
        mode++;
        if(mode == 3)
        {
            mode = 0;
        }
    }
}
/**/

MENU_fresh()
{
    Delay_ms(STM0,100);
    freshposi=0;
    MENU_freshadd();
    freshname=1;

}



#pragma section all restore

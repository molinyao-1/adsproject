//*
// * beacon_control.c
// *
// *  Created on: 2020年12月20日
// *      Author: jimi
// */
#include "beacon_control.h"
#include "image.h"
#include "stdlib.h"
#include "stdbool.h"
#include "VA_get.h"
#include <adrc.h>
#include "testsome.h"

#define deadgeerleft 650  //5900
#define deadgeerright 680  //4570
extern int gogoflag;

float angle_z = 0;
extern float acc[3];
extern float gyro[3];
extern float gyronow[3];
#define gyrooffset2 0.35

float pulseleft;
float pulseright;

int speed0A = 60; //(菜单调节)
int speed1A = 60; //减速的速度
int speed2A = 20; //减速的速度
int speed3A = 60; //转弯的速度

int speed0B = 60; //(菜单调节)
int speed1B = 60; //减速的速度
int speed2B = 20; //减速的速度
int speed3B = 60; //转弯的速度

/**/
int side_flag = 0;
int side_last = 1;

int light_square = 0; //灯的面积
int Long_square;
/*MPU接收参数*/
extern mpu_t this_mpu;
/**/
/*自定义一些变量*/
int servodirection = 0;              //控制舵机转向打死方向 （-1：左打死 ；1：右打死）
pidCtrl_t  servo_pidA; //舵机PID初始化
pidCtrl_t  servo_pidB; //舵机PID初始化
float servokpA = 5; //(菜单调）
float servokdA = 14;
float servokpB = 5; //(菜单调）
float servokdB = 14;
float servo_pulse = 0;

float Ldesiredspeed=0;
float Rdesiredspeed=0;

pid rmotorpiA,lmotorpiA;
pid rmotorpiB,lmotorpiB;

float RAmotorkp = 30,RAmotorki = 20,LAmotorkp = 30,LAmotorki = 20; //(菜单调)
float RBmotorkp = 30,RBmotorki = 20,LBmotorkp = 30,LBmotorki = 20; //(菜单调)

int16 leftFTM = 0;
int16 rightFTM = 0;
int16 lastleftFTM = 0;
int16 lastrightFTM = 0;
float speed_r = 0;
float speed_l = 0;
float motor_lefdevn =0;
float motor_rigdevn =0;
float N_motor_lefdevn=0,N_motor_rigdevn=0;

uint32 Flefdevn =0;
uint32 Frigdevn =0;
uint32 NFlefdevn=0,NFrigdevn=0;
//以下为各类标志位
int mode_flag = 0;  //寻灯状态标志位，0直线追灯，1近灯，2灭灯旋转，3看到灯顺正->回归0
//以下为图像处理相关变量
int distance = 0;   //信标灯与参考点距离（表示信标灯和车距离）
int error_simu = 0; //模拟信标灯偏差
int connect_max = 3; //没有连通域的次数大于该值时认为处于灭灯旋转状态
int roll_time;
int mode_last = 2;    //上一次状态标志位
int go_flag = 1;    //进入跑车程序标志位
/*前摄像头*/
int aver_x = 0, aver_y = 0;
int averbA_x = 100,averbA_y = 120;  //摄像头中点坐标（这个摄像头具体中点坐标后续要改）
int averA_x = 100,averA_y = 88;     //摄像头中点坐标(两个摄像头中点坐标，可能有别的用处)
int last_averA_x = 100, last_averA_y = 120;   //上一次红外(前期不用红外，后期如果用要进行更改，数据肯定不准)中点坐标
/*后摄像头*/
int averbB_x = 88,averbB_y = 70;  //摄像头中点坐标（这个摄像头具体中点坐标后续要改）
int averB_x = 88,averB_y = 70;     //摄像头中点坐标(两个摄像头中点坐标，可能有别的用处)
int last_averB_x = 100, last_averB_y = 70;   //上一次红外(前期不用红外，后期如果用要进行更改，数据肯定不准)中点坐标
/*前摄中心点*/
int originA_x = 145;  //模拟原点行数
int originA_y = 87;   //模拟圆心纵坐标
/*后摄中心点*/
int originB_x = 145;  //模拟原点行数
int originB_y = 87;   //模拟圆心纵坐标
int error_dx = 0,error_dy = 0;           //实际灯和参考点横纵差值
int areaA_time = 0;  //连通域为0次数标志位
int areaB_time = 0;  //连通域为0次数标志位
//未分类（可放在beacon_control.c文件里）
int mode_time = 0;    //记录各状态持续次数

extern uint8 connectnum;//连通域数
extern uint8 connectnumB;//连通域数
extern float lightx, lighty, lights, lightc;
extern float lightdis;
extern int lightx2;
extern int lighty2;
extern int area2;
extern int dis2;

extern int servoinit;
int flagservo = 1;

float Ior = 0;
float Iol = 0;
float I_r = 0;
float I_l = 0;
float I_rf = 0;
float I_lf = 0;
float I_rf2 = 0;
float I_lf2 = 0;

float fk1 = 0.75; //一阶滞后迭代滤波器系数-第一次迭代(放菜单调)
float fk2 = 0.8;//第二次迭代(调)

float Lspeed_error_last = 0;
float Rspeed_error_last = 0;
float Lspeed_error = 0;
float Rspeed_error = 0;
float kp_acc_r = 0;
float ki_acc_r = 0;
float kp_acc_l = 0;
float ki_acc_l = 0;
float kp_I_r = 0;
float ki_I_r = 0;
float kp_I_l = 0;
float ki_I_l = 0;

float I_want_l = 0;
float I_want_r = 0;
float I_error_last_l = 0;
float I_error_last_r = 0;
float I_error_l = 0;
float I_error_r = 0;

extern float battery;
float bauplimit = 0;
float balowlimit = 0;
int speedever = 0;

float Ldirs = 0;
float Rdirs = 0;
adrc_t Lctrl;
adrc_t Rctrl;

float Lrset = 20;
float Rrset = 20;
adrc_td_t Lctrl_t;
adrc_td_t Rctrl_t;
int b = 0;

#define period 5
float baspeed = 6;
int d = 0;
extern float time;

float ax,ay,az;
float gx,gy,gz;

extern int Dist;
//卡尔曼参数
float Q_angle = 0.001;
float Q_gyro  = 0.005;
float R_angle = 0.5;
float dt      = 0.002;//dt为kalman滤波器采样时间
char  C_0     = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

float Q_bias1, Angle_err1;
float PCt_01, PCt_11, E1;
float K_01, K_11, t_01, t_11;
float Pdot1[4] ={0,0,0,0};
float PP1[2][2] = { { 1, 0 },{ 0, 1 } };

int Camera = 0;    //摄像头工作模式选择(菜单调节) 0为双摄,1 2分别为单摄,3 4分别为双摄单头(双摄是指用两个摄像头跑,正反跑都可以,单摄好理解,一个摄像头跑,双摄单头是指用前或后摄像头跑,另一个摄像头辅助转向)
int camera_last,camera_flag;
int shifttime = 3; //(菜单调)
int shift_time;
int Blimit = 35; //(菜单调) //正反跑时有用
int Alimit = 35; //(菜单调)
int errormaxA = 36; //(菜)
int errormaxB = 40;//(菜)
int back_flag = 0;  //视野外信标灯位置

int lowlimit0A = 40;
int lowlimit1A = 50;
int lowlimit2A = 53;
int lowlimit3A = 60;
int lowlimit4A = 60;

int lowlimit0B = 26; //(调)
int lowlimit1B = 32;
int lowlimit2B = 39;
int lowlimit3B = 45;
int lowlimit4B = 60;
int speedmarkA = 0; //0:高速 1:中速 2:低速
int speedmarkB = 0; //0:高速 1:中速 2:低速

float correctspeed =0.8;
float leftcorrect = 1;
float rightcorrect = 1;
float correctspeed1 =0.8;
float leftcorrect1 = 1;
float rightcorrect1 = 1;

int dislimitA = 85;
int dislimitB = 85;

int servoduty;
int formiddle;
int backmiddle;

void Encoder_init()
{
    SmartCar_Encoder_Init(GPT12_T4,IfxGpt120_T4INA_P02_8_IN,IfxGpt120_T4EUDA_P00_9_IN);  //Left
    SmartCar_Encoder_Init(GPT12_T6,IfxGpt120_T6INA_P20_3_IN,IfxGpt120_T6EUDA_P20_0_IN);  //right
}

void FINAL_CONTROL()
{
//    Systick_Start(STM0);
    MODE_JUDGE();
    STOP_JUDGE();
//    SIDE_JUDGE();
    ERROR();
    BatterySCM_Get();
    /*防翻车用*/
//    MPU6050get();
}


//对于连通域，始终在更新两个area_flag，处理的时候利用这两个变量
void CAMERA_JUDGE()
{
    averbA_x = lightx;
    averbA_y = lighty;
    averbB_x = lightx2;
    averbB_y = lighty2;
    if(connectnum ==0)                                        //如果没有找到连通域，记录连通域为零次数
    {
        areaA_time++;                        //连通域为零次数加一
    }
    else
    {
        areaA_time = 0;
        last_averA_x = averbA_x;
        last_averA_y = averbA_y;
    }
    if(areaA_time == 0)
    {
        averA_x = last_averA_x;
        averA_y = last_averA_y;
    }

    if(connectnumB == 0)
    {
        areaB_time++;
    }
    else
    {
        areaB_time = 0;
        last_averB_x = averbB_x;
        last_averB_y = averbB_y;
    }
    if(areaB_time == 0)
    {
        averB_x = last_averB_x;
        averB_y = last_averB_y;
    }

    if(camera_flag == 0)
     {
         aver_x = averA_x;
         aver_y = averA_y;
         error_dx = averA_x - originA_x;
         error_dy = averA_y - originA_y;
         distance =  lightdis;//CarmSqrt((float)(error_dx*error_dx + error_dy*error_dy));   //计算灯距
         speedever = (leftFTM+rightFTM)/20;
         /*速度分段*/
         if(averA_x<lowlimit0A&&(averA_x+speedever)<lowlimit1A)
         {
 //            GPIO_Set(P20,9,0);
             speedmarkA = 0;
         }

         else
         {
             if(averA_x<=lowlimit2A)
             {
                 speedmarkA = 1;
 //                GPIO_Set(P20,9,0);
             }
         }

         if((averA_x>lowlimit2A&&(averA_x+speedever)>lowlimit3A)||averA_x>lowlimit4A)
         {
 //            GPIO_Set(P20,9,1);
             speedmarkA = 2;
         }
     }
}



/***************************** 状态判断处理程序 *******************************/
void MODE_JUDGE()
{
    /*状态机*/
    if(0 == mode_flag&&((camera_flag ==0&&distance<dislimitA)||speedmarkA == 2))
    {
        mode_last = mode_flag;
        mode_flag = 1;
        mode_time = 0;
        back_flag = 0;
    }
    if(1 == mode_flag&&((camera_flag ==0&&distance>(dislimitA+18))||speedmarkA ==0||speedmarkA == 1)) //灯被对手灭,远处还有灯,判定的两次距离差,并且大于此阈值就转换状态
    {
        mode_last = mode_flag;
        mode_flag = 0;
        mode_time = 0;
        back_flag = 0;
    }
//    if((1 == mode_flag)&&battery<balowlimit) //处于近灯状态,并且电压不够
//    {
//        mode_last = mode_flag;
//        mode_flag = 3;
//        mode_time = 0;
//    }
//    if(3 == mode_flag&&(battery>bauplimit)&&((camera_flag ==0&&areaA_time>connect_max))) //无灯且充电满
//    {
//        mode_last = mode_flag;
//        mode_flag = 2;
//        mode_time = 0;
//    }
//    if(3 == mode_flag&&(battery>bauplimit)&&((camera_flag ==0&&areaA_time==0)))
//    {
//        mode_last = mode_flag;
//        mode_flag = 0;
//        mode_time = 0;
//        back_flag = 0;
//    }
    if(mode_flag !=2 &&((camera_flag ==0&&areaA_time>connect_max)))
    {
        mode_last = mode_flag;
        mode_flag = 2;
        mode_time = 0;
    }
    if(2 == mode_flag&&((camera_flag ==0&&areaA_time==0)))
    {
        mode_last = mode_flag;
        mode_flag = 0;
        mode_time = 0;
        back_flag = 0;
    }
    mode_time++; //mode_time最少是1！！！！(记录着一个状态持续时间)

    //无灯时可选择切换摄像头,有死区(初始camera_flag不需要更改,会自己修正)
    if(mode_flag == 2)
    {
        if(back_flag == 0)
        {
            if(areaB_time == 0)
            {
                back_flag = -1;//右转
            }
            else if(areaB_time>0)
            {
                back_flag = 1;//左转
            }
        }
    }
    else if(mode_flag == 0||mode_flag == 1)
    {
        back_flag = 0;
    }
}

/***************************** 坐标偏差控制程序 *******************************/
void ERROR()
{
    if(camera_flag == 0)
    {
        if(0 == mode_flag||1 == mode_flag)
            error_simu = aver_y - formiddle;
    }
    else if(camera_flag == 1)
    {
        if(0 == mode_flag||1 == mode_flag)
            error_simu =backmiddle - aver_y;
    }
}

void Servo_motorinit()
{
        /*舵机的占空比中值需要重新测*/
//        SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,333,5250);
//左
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,20000,0);
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,20000,0); //给占空比正转
//右
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,20000,0); //给正占空比转
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,20000,0);
}

void Servo_control()
{
    if(servoinit == 1&&flagservo == 1)
    {
        SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,333,5250);
        flagservo = 0;
    }
    if(servoinit == 1&&flagservo ==0)
    {
        if(mode_flag == 0||mode_flag == 1)
        {
            servodirection = 0;
            side_flag = 0;
            back_flag = 0;
        }
        if(back_flag!=0)
        {
            side_flag = back_flag;
            side_last = side_flag;
        }
        if(mode_flag == 2)
        {
            if(side_flag == 1)
                servodirection = 1;
            else if(side_flag == -1)
                servodirection = -1;
            else
                servodirection = side_last;
        }
        if(servodirection == -1)
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(5250-deadgeerright));
        }
        else if(servodirection == 1)
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(5250+deadgeerleft));
        }

        else if(servodirection == 0)
        {
            if(camera_flag == 0)
            {
            //四轮车顺正状态和追灯状态共用一个舵机控制方法 //也需要优化更改,舵机打角与速度挂钩
                servo_pidA.kp = servokpA;
                servo_pidA.kd = servokdA;
                servo_pidA.errPrev = servo_pidA.errCurr;
                servo_pidA.errCurr = (float)error_simu;
                servo_pidA.errDiff = servo_pidA.errCurr - servo_pidA.errPrev;

                servo_pulse=servo_pidA.kp*servo_pidA.errCurr+servo_pidA.kd*servo_pidA.errDiff;

                if(servo_pulse>680)
                {
                    servo_pulse =680;
                }
                else if(servo_pulse<-650)
                {
                    servo_pulse = -650;
                }
            }
            else if(camera_flag == 1)
            {
                servo_pidB.kp = servokpB;
                servo_pidB.kd = servokdB;
                servo_pidB.errPrev = servo_pidB.errCurr;
                servo_pidB.errCurr = (float)error_simu;
                servo_pidB.errDiff = servo_pidB.errCurr - servo_pidB.errPrev;

                servo_pulse=servo_pidB.kp*servo_pidB.errCurr+servo_pidB.kd*servo_pidB.errDiff;

                if(servo_pulse>680)
                {
                    servo_pulse =680;
                }
                else if(servo_pulse<-650)
                {
                    servo_pulse = -650;
                }
            }
            if(gogoflag == 1)
            {
                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(5250-servo_pulse));
            }

        }
    }
//    Lforward = ADC_Get(ADC_0,ADC0_CH2_A2,ADC_12BIT);
//    Lbackward = ADC_Get(ADC_0,ADC0_CH3_A3,ADC_12BIT);
}

void Motor_control()
{

//    Battery_Get();
    if(gogoflag == 1)
    {
        if(camera_flag == 0)
        {
            leftFTM = SmartCar_Encoder_Get(GPT12_T4);
            rightFTM = -SmartCar_Encoder_Get(GPT12_T6);
        }
        else if(camera_flag == 1)
        {
            leftFTM = -SmartCar_Encoder_Get(GPT12_T4);
            rightFTM = SmartCar_Encoder_Get(GPT12_T6);
        }
        SmartCar_Encoder_Clear(GPT12_T4);
        SmartCar_Encoder_Clear(GPT12_T6);
        if(camera_flag == 0)
        {
            if(mode_flag == 0||mode_flag == 1) // 除灭灯和充电状态，其余状态计算电机的减速线
            {
                if(0 == speedmarkA)
                {
                    Ldesiredspeed = (float)speed0A;
                    Rdesiredspeed = (float)speed0A;
                }
                if(1 == speedmarkA)
                {
                    Ldesiredspeed = (float)speed1A;
                    Rdesiredspeed = (float)speed1A;
                }

                if(2 == speedmarkA)
                {
                    if(battery>baspeed)
                    {
                        Ldesiredspeed = (float)speed2A;
                        Rdesiredspeed = (float)speed2A;
                    }
                    else
                    {
                        Ldesiredspeed = (float)speed0A;
                        Rdesiredspeed = (float)speed0A;
                    }
                }

                /*NEW ITEM*/
                if(servo_pulse > 125) //右
                {
                    if(servo_pulse>250&&abs(angle_z)<=140)
                    {
                        Rdesiredspeed = (Rdesiredspeed)*((-0.001427*(servo_pulse*0.01)*(servo_pulse*0.01)-0.06414*(servo_pulse*0.01)+0.9826)*correctspeed1);
                    }
                    else
                    {
                        Rdesiredspeed = (Rdesiredspeed)*(-0.001427*(servo_pulse*0.01)*(servo_pulse*0.01)-0.06414*(servo_pulse*0.01)+0.9826)*rightcorrect1;
                    }
                }
                else if(servo_pulse<-125) //左
                {
                    if(servo_pulse<-250&&abs(angle_z)<=140)
                    {

                        Ldesiredspeed = (Ldesiredspeed)*((0.001868*(servo_pulse*0.01)*(servo_pulse*0.01)-0.09044*(-servo_pulse*0.01)+1.006)*correctspeed1);
                    }
                    else
                    {
                        Ldesiredspeed = (Ldesiredspeed)*(0.001868*(servo_pulse*0.01)*(servo_pulse*0.01)-0.09044*(-servo_pulse*0.01)+1.006)*leftcorrect1;
                    }
                }
            /**/
            }
            else if(mode_flag == 2)//看不到灯
            {
                Ldesiredspeed = speed3A;
                Rdesiredspeed = speed3A;
                if(servodirection == 1) //左转
                {
                    if(abs((int)angle_z)<=150)
                    {
                        Ldesiredspeed = Ldesiredspeed*0.5*((0.001868*(deadgeerleft*0.01)*(deadgeerleft*0.01)-0.09044*(-deadgeerleft*0.01)+1.006)*correctspeed);
                    }
                    else
                    {
                        Ldesiredspeed = Ldesiredspeed*0.5*((0.001868*(deadgeerleft*0.01)*(deadgeerleft*0.01)-0.09044*(-deadgeerleft*0.01)+1.006)*leftcorrect);
                    }
                    Rdesiredspeed = 2*speed3A-Ldesiredspeed;
                }
                else if(servodirection == -1)
                {
                    if(abs((int)angle_z)<=150)
                    {
                        Rdesiredspeed = Rdesiredspeed*0.5*((-0.001427*(deadgeerright*0.01)*(deadgeerright*0.01)-0.06414*(deadgeerright*0.01)+0.9826)*correctspeed);
                    }
                    else
                    {
                        Rdesiredspeed = Rdesiredspeed*0.5*((-0.001427*(deadgeerright*0.01)*(deadgeerright*0.01)-0.06414*(deadgeerright*0.01)+0.9826)*rightcorrect);
                    }
                    Ldesiredspeed = 2*speed3A-Rdesiredspeed;
                }
            }

            else if(mode_flag == 3)
            {
            }
            if(mode_flag == 0||mode_flag == 1||mode_flag == 2)
            {
                rmotorpiA.Kp = RAmotorkp;
                rmotorpiA.Ki = RAmotorki;
                lmotorpiA.Kp = LAmotorkp;
                lmotorpiA.Ki = LAmotorki;
                rmotorpiA.ekL = rmotorpiA.ek;
                rmotorpiA.ek = (Rdesiredspeed-(float)rightFTM);
                lmotorpiA.ekL = lmotorpiA.ek;
                lmotorpiA.ek = (Ldesiredspeed-(float)leftFTM);
                pulseleft = lmotorpiA.Kp*(lmotorpiA.ek-lmotorpiA.ekL) + lmotorpiA.Ki*lmotorpiA.ek;
                pulseright = rmotorpiA.Kp*(rmotorpiA.ek-rmotorpiA.ekL) + rmotorpiA.Ki*rmotorpiA.ek;
                motor_lefdevn += pulseleft;
                motor_rigdevn += pulseright;
                if(motor_lefdevn<0)
                {
                    N_motor_lefdevn=-motor_lefdevn;
                    motor_lefdevn=0;
                }
                else if(motor_lefdevn>=0)
                {
                    N_motor_lefdevn=0;
                }
                if(motor_rigdevn<0)
                {
                    N_motor_rigdevn=-motor_rigdevn;
                    motor_rigdevn=0;
                }
                else if(motor_rigdevn>=0)
                {
                    N_motor_rigdevn=0;
                }
                if(motor_rigdevn>9900){motor_rigdevn=9900;}
                if(motor_lefdevn>9900){motor_lefdevn=9900;}
                if(N_motor_rigdevn>9900){N_motor_rigdevn=9900;}
                if(N_motor_lefdevn>9900){N_motor_lefdevn=9900;}
            }
        }
        if(camera_flag == 1)
        {
            if(mode_flag == 0||mode_flag == 1) // 除灭灯和充电状态，其余状态计算电机的减速线
            {
                if(0 == speedmarkB)
                {
                    Ldesiredspeed = (float)speed0B;
                    Rdesiredspeed = (float)speed0B;
                }
                if(1 == speedmarkB)
                {
                    Ldesiredspeed = (float)speed1B;
                    Rdesiredspeed = (float)speed1B;
                }

                if(2 == speedmarkB)
                {
                    if(battery>baspeed)
                    {
                        Ldesiredspeed = (float)speed2B;
                        Rdesiredspeed = (float)speed2B;
                    }
                    else
                    {
                        Ldesiredspeed = (float)speed0B;
                        Rdesiredspeed = (float)speed0B;
                    }
                }

                /*NEW ITEM*/
                if(servo_pulse > 125) //左
                {
                    if(servo_pulse>250&&abs(angle_z)<=140)
                    {
                        Rdesiredspeed = (Rdesiredspeed)*((-0.001427*(servo_pulse*0.01)*(servo_pulse*0.01)-0.06414*(servo_pulse*0.01)+0.9826)*correctspeed);
                    }
                    else
                    {
                        Rdesiredspeed = (Rdesiredspeed)*(-0.001427*(servo_pulse*0.01)*(servo_pulse*0.01)-0.06414*(servo_pulse*0.01)+0.9826)*rightcorrect;
                    }
                }
                else if(servo_pulse<-125) //右
                {
                    if(servo_pulse<-250&&abs(angle_z)<=140)
                    {

                        Ldesiredspeed = (Ldesiredspeed)*((-0.00693*(servo_pulse*0.01)*(servo_pulse*0.01)-0.05136*(-servo_pulse*0.01)+0.9545)*correctspeed);
                    }
                    else
                    {
                        Ldesiredspeed = (Ldesiredspeed)*(-0.00693*(servo_pulse*0.01)*(servo_pulse*0.01)-0.05136*(-servo_pulse*0.01)+0.9545)*leftcorrect;
                    }
                }
            /**/
            }
            else if(mode_flag == 2)//看不到灯
            {
                Ldesiredspeed = speed3B;
                Rdesiredspeed = speed3B;
                if(servodirection == 1) //左转
                {
                    if(abs((int)angle_z)<=150)
                    {
                        Ldesiredspeed = Ldesiredspeed*0.5*(1+(-0.00693*(deadgeerleft*0.01)*(deadgeerleft*0.01)-0.05136*(-deadgeerleft*0.01)+0.9545)*correctspeed);
                    }
                    else
                    {
                        Ldesiredspeed = Ldesiredspeed*0.5*(1+(-0.00693*(deadgeerleft*0.01)*(deadgeerleft*0.01)-0.05136*(-deadgeerleft*0.01)+0.9545)*leftcorrect);
                    }
                    Rdesiredspeed = 2*speed3B-Ldesiredspeed;
                }
                else if(servodirection == -1)
                {
                    if(abs((int)angle_z)<=150)
                    {
                        Rdesiredspeed = Rdesiredspeed*0.5*(1+(-0.003824*(servo_pulse*0.01)*(servo_pulse*0.01)-0.06035*(servo_pulse*0.01)+0.991)*correctspeed);
                    }
                    else
                    {
                        Rdesiredspeed = Rdesiredspeed*0.5*(1+(-0.003824*(servo_pulse*0.01)*(servo_pulse*0.01)-0.06035*(servo_pulse*0.01)+0.991)*rightcorrect);
                    }
                    Ldesiredspeed = 2*speed3B-Rdesiredspeed;
                }
            }

            else if(mode_flag == 3)
            {
            }
            if(mode_flag == 0||mode_flag == 1||mode_flag == 2)
            {
                rmotorpiB.Kp = RBmotorkp;
                rmotorpiB.Ki = RBmotorki;
                lmotorpiB.Kp = LBmotorkp;
                lmotorpiB.Ki = LBmotorki;
                rmotorpiB.ekL = rmotorpiB.ek;
                rmotorpiB.ek = (Rdesiredspeed-(float)rightFTM);
                lmotorpiB.ekL = lmotorpiB.ek;
                lmotorpiB.ek = (Ldesiredspeed-(float)leftFTM);
                pulseleft = lmotorpiB.Kp*(lmotorpiB.ek-lmotorpiB.ekL) + lmotorpiB.Ki*lmotorpiB.ek;
                pulseright = rmotorpiB.Kp*(rmotorpiB.ek-rmotorpiB.ekL) + rmotorpiB.Ki*rmotorpiB.ek;
                motor_lefdevn += pulseleft;
                motor_rigdevn += pulseright;
                if(motor_lefdevn<0)
                {
                    N_motor_lefdevn=-motor_lefdevn;
                    motor_lefdevn=0;
                }
                else if(motor_lefdevn>=0)
                {
                    N_motor_lefdevn=0;
                }
                if(motor_rigdevn<0)
                {
                    N_motor_rigdevn=-motor_rigdevn;
                    motor_rigdevn=0;
                }
                else if(motor_rigdevn>=0)
                {
                    N_motor_rigdevn=0;
                }
                if(motor_rigdevn>9900){motor_rigdevn=9900;}
                if(motor_lefdevn>9900){motor_lefdevn=9900;}
                if(N_motor_rigdevn>9900){N_motor_rigdevn=9900;}
                if(N_motor_lefdevn>9900){N_motor_lefdevn=9900;}
            }
        }
        if(camera_flag == 0)
        {
            Flefdevn = motor_lefdevn;
            Frigdevn = motor_rigdevn;
            NFlefdevn= N_motor_lefdevn;
            NFrigdevn= N_motor_rigdevn;
        }
        else if(camera_flag == 1)
        {
            NFlefdevn = motor_lefdevn;
            NFrigdevn = motor_rigdevn;
            Flefdevn= N_motor_lefdevn;
            Frigdevn= N_motor_rigdevn;
        }
        if(go_flag) //进入跑车状态
        {
//左
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,20000,(uint32)NFlefdevn);
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,20000,(uint32)Flefdevn); //给占空比正转
//右
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,20000,(uint32)Frigdevn); //给正占空比转
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,20000,(uint32)NFrigdevn);
        }
        if(go_flag == 0)//要停车了
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,0);
            Delay_us(STM0,10);
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,0);
            Delay_us(STM0,10);
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,0);
            Delay_us(STM0,10);
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,0);
        }
    }
}


/***************************** 停车判断处理程序 *******************************/
void STOP_JUDGE()
{
    if((camera_flag == 0 &&areaA_time == 0)||(camera_flag == 1 && areaB_time == 0)||mode_flag == 3)
        roll_time = 0;
    else{
        roll_time ++;
    }
    if(roll_time > 1000)  //设为0则会长时间无灯停车
    {
        go_flag = 0;
        roll_time = 1000;
    }
}


//求平方根
float CarmSqrt(float x){
 union{
  int intPart;
  float floatPart;
 } convertor;
 union{
  int intPart;
  float floatPart;
 } convertor2;
 convertor.floatPart = x;
 convertor2.floatPart = x;
 convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
 convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
 return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
}


/*（1）切灯还可以将距离换成x拟合一组数据，（2）编码器值-平移距离*/


/**/

//IMU
void Imudata_get()
{
    SmartCar_MPU_Getacc2(&this_mpu);
    SmartCar_MPU_Getgyro2(&this_mpu);
    acc[0] = this_mpu.mpu_rawdata.acc_x;
    acc[1] = this_mpu.mpu_rawdata.acc_y;
    acc[2] = this_mpu.mpu_rawdata.acc_z;

    gyro[0] = this_mpu.mpu_rawdata.gyro_x;
    gyro[1] = this_mpu.mpu_rawdata.gyro_y;
    gyro[2] = this_mpu.mpu_rawdata.gyro_z;

    if(gyro[0]<-4.2&&gyro[0]>-4.8)
    {
        gyro[0] = -4.5;
    }

    if(gyro[1]<0.5&&gyro[1]>-0.5)
    {
        gyro[1] = 0;
    }

    if(gyro[2]<-8.2&&gyro[2]>-8.8)
    {
        gyro[2] = -8.5;
    }
}

float Angle_x_temp;
float Angle_y_temp;
float Angle_X_Final = 0;
float Angle_Y_Final = 0;

void MPU6050get(void)
{
    SmartCar_MPU_Getacc2(&this_mpu);
    SmartCar_MPU_Getgyro2(&this_mpu);
    acc[0] = this_mpu.mpu_rawdata.acc_x;
    acc[1] = this_mpu.mpu_rawdata.acc_y;
    acc[2] = this_mpu.mpu_rawdata.acc_z;

    gyro[0] = this_mpu.mpu_rawdata.gyro_x;
    gyro[1] = this_mpu.mpu_rawdata.gyro_y;
    gyro[2] = this_mpu.mpu_rawdata.gyro_z;
    ax = acc[0];
    ay = acc[1];
    az = acc[2];
    //用加速度计算三个轴和水平面坐标系之间的夹角
//    Angle_x_temp=(atan(ay/az))*180/3.14;
//    Angle_y_temp=(atan(ax/az))*180/3.14;
//    if(ax*ay*az == 0)
//          return;
//    else
//    {
//            Kalman_Filter_Y(Angle_y_temp,gyro[1]);  //卡尔曼滤波计算Y倾角
//        Kalman_Filter_X(Angle_x_temp,gyro[0]);  //卡尔曼滤波计算X倾角
//    }

    if(gyro[2]<0.6&&gyro[2]>0.2)
    {
        gyro[2] = 0.35;
    }

    if((abs(servo_pulse)>250)||(mode_flag == 2))
        angle_z += (gyro[2]-gyrooffset2)*dt;
    else
        angle_z = 0;
}


void Kalman_Filter_X(float Accel,float Gyro) //卡尔曼函数
{
    Angle_X_Final += (Gyro - Q_bias) * dt; //先验估计

    Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

    Pdot[1]= -PP[1][1];
    Pdot[2]= -PP[1][1];
    Pdot[3]= Q_gyro;

    PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
    PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    Angle_err = Accel - Angle_X_Final;  //zk-先验估计
    Angle_X_Final += K_0 * Angle_err;    //后验估计
    Q_bias        += K_1 * Angle_err;    //后验估计
    gyronow[0]         = Gyro - Q_bias;  //输出值（后验估计）的微分 = 角速度

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0;       //后验估计误差协方差
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;


}

void Kalman_Filter_Y(float Accel,float Gyro) //卡尔曼函数
{
    Angle_Y_Final += (Gyro - Q_bias) * dt; //先验估计

    Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

    Pdot[1]=- PP[1][1];
    Pdot[2]=- PP[1][1];
    Pdot[3]=Q_gyro;

    PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
    PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;


    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    Angle_err = Accel - Angle_Y_Final;  //zk-先验估计

    Angle_Y_Final   += K_0 * Angle_err;  //后验估计
    Q_bias  += K_1 * Angle_err;  //后验估计
    gyronow[1]   = Gyro - Q_bias;    //输出值（后验估计）的微分 = 角速度

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0;       //后验估计误差协方差
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;


}

void Imudataprocess()
{
    SmartCar_MPU_Getacc2(&this_mpu);
    SmartCar_MPU_Getgyro2(&this_mpu);
    gyronow[2] = this_mpu.mpu_rawdata.gyro_z;


    gyronow[2] = kalmanFilter_A(gyronow[2]);

    if(gyronow[2]<-0.2&&gyronow[2]>-0.7)
    {
        gyronow[2] = -0.45;
    }

    if((abs(servo_pulse)>250)||(mode_flag == 2))
        angle_z += (gyronow[2]-gyrooffset2)*dt;
    else
        angle_z = 0;
}


void motorI_correct()
{
    for(int i = 0;i<50;i++)
    {
        Ior += ADC_Get(ADC_1,ADC1_CH4_A20,ADC_8BIT);//具体引脚要根据电流环引脚修改
        Iol += ADC_Get(ADC_1,ADC1_CH4_A20,ADC_8BIT);//
    }
    Ior = Ior/50;
    Iol = Iol/50;
}



void motorI_get()
{
    I_r = I_l = 0;
    for(int i = 0;i<10;i++)
    {
        I_r += ADC_Get(ADC_1,ADC1_CH4_A20,ADC_8BIT);//具体要修改
        I_l += ADC_Get(ADC_1,ADC1_CH4_A20,ADC_8BIT);
    }
    I_r = I_r/10 - Ior;
    I_l = I_l/10 - Iol;
    //电流滤波器
    I_rf = (1-fk1)*I_r+fk1*I_rf;
    I_lf = (1-fk1)*I_l+fk1*I_lf;

    I_rf2 = (1-fk2)*I_rf+fk2*I_rf2;
    I_lf2 = (1-fk2)*I_lf+fk2*I_lf2;

    I_r = I_rf2;
    I_l = I_lf2;
}

void motorpid()
{
    lastleftFTM = leftFTM;
    lastrightFTM = rightFTM;
    leftFTM = -SmartCar_Encoder_Get(GPT12_T4);
    rightFTM = SmartCar_Encoder_Get(GPT12_T6);
    SmartCar_Encoder_Clear(GPT12_T4);
    SmartCar_Encoder_Clear(GPT12_T6);

    speed_l = (float)(lastleftFTM+leftFTM)/2;
    speed_r = (float)(lastrightFTM+rightFTM)/2;

    Lspeed_error_last = Lspeed_error;
    Rspeed_error_last = Rspeed_error;
    Lspeed_error = Ldesiredspeed - speed_l;
    Rspeed_error = Rdesiredspeed - speed_r;

    I_want_l += ki_acc_l*Lspeed_error+kp_acc_l*(Lspeed_error-Lspeed_error_last);
    I_want_r += ki_acc_r*Rspeed_error+kp_acc_r*(Rspeed_error-Rspeed_error_last);

    float I_limit= 0 ;
    if(I_want_l>I_limit)
        I_want_l = I_limit;
    if(I_want_r>I_limit)
        I_want_r = I_limit;
    if(I_want_l<-I_limit)
        I_want_l = -I_limit;
    if(I_want_r<-I_limit)
        I_want_r = -I_limit;
    I_error_last_l = I_error_l;
    I_error_last_r = I_error_r;
    I_error_l = I_want_l - I_l;
    I_error_r = I_want_r - I_r;

    pulseleft = kp_I_l*(I_error_l-I_error_last_l) + ki_I_r*I_error_l;
    pulseright = kp_I_r*(I_error_r-I_error_last_r) + ki_I_r*I_error_r;
    motor_lefdevn += pulseleft;
    motor_rigdevn += pulseright;
    if(motor_lefdevn<0)
    {
        N_motor_lefdevn=-motor_lefdevn;
        motor_lefdevn=0;
    }
    else if(motor_lefdevn>=0)
    {
        N_motor_lefdevn=0;
    }
    if(motor_rigdevn<0)
    {
        N_motor_rigdevn=-motor_rigdevn;
        motor_rigdevn=0;
    }
    else if(motor_rigdevn>=0)
    {
        N_motor_rigdevn=0;
    }
    if(motor_rigdevn>8000){motor_rigdevn=8000;}
    if(motor_lefdevn>8000){motor_lefdevn=8000;}
    if(N_motor_rigdevn>8000){N_motor_rigdevn=8000;}
    if(N_motor_lefdevn>8000){N_motor_lefdevn=8000;}

}

void Motorctrl()
{
    leftFTM = SmartCar_Encoder_Get(GPT12_T4);
    rightFTM = -SmartCar_Encoder_Get(GPT12_T6);
    SmartCar_Encoder_Clear(GPT12_T4);
    SmartCar_Encoder_Clear(GPT12_T6);
    rmotorpiB.Kp = RBmotorkp;
    rmotorpiB.Ki = RBmotorki;
    lmotorpiB.Kp = LBmotorkp;
    lmotorpiB.Ki = LBmotorki;
    rmotorpiB.ekL = rmotorpiB.ek;
    rmotorpiB.ek = (speed0A-(float)rightFTM);
    lmotorpiB.ekL = lmotorpiB.ek;
    lmotorpiB.ek = (speed0A-(float)leftFTM);
    pulseleft = lmotorpiB.Kp*(lmotorpiB.ek-lmotorpiB.ekL) + lmotorpiB.Ki*lmotorpiB.ek;
    pulseright = rmotorpiB.Kp*(rmotorpiB.ek-rmotorpiB.ekL) + rmotorpiB.Ki*rmotorpiB.ek;
    motor_lefdevn += pulseleft;
    motor_rigdevn += pulseright;
    if(motor_lefdevn<0)
    {
        N_motor_lefdevn=-motor_lefdevn;
        motor_lefdevn=0;
    }
    else if(motor_lefdevn>=0)
    {
        N_motor_lefdevn=0;
    }
    if(motor_rigdevn<0)
    {
        N_motor_rigdevn=-motor_rigdevn;
        motor_rigdevn=0;
    }
    else if(motor_rigdevn>=0)
    {
        N_motor_rigdevn=0;
    }
    if(motor_rigdevn>9900){motor_rigdevn=9900;}
    if(motor_lefdevn>9900){motor_lefdevn=9900;}
    if(N_motor_rigdevn>9900){N_motor_rigdevn=9900;}
    if(N_motor_lefdevn>9900){N_motor_lefdevn=9900;}
    //左
                SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,20000,(uint32)N_motor_lefdevn);
                SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,20000,(uint32)motor_lefdevn); //给占空比正转
    //右
                SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,20000,(uint32)motor_rigdevn); //给正占空比转
                SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,20000,(uint32)N_motor_rigdevn);
}


void Motor_ADRC()
{
    leftFTM = -SmartCar_Encoder_Get(GPT12_T4);
    rightFTM = SmartCar_Encoder_Get(GPT12_T6);
    SmartCar_Encoder_Clear(GPT12_T4);
    SmartCar_Encoder_Clear(GPT12_T6);

//    Lctrl.y = leftFTM;
//    Rctrl.y = rightFTM;
//
//    Lctrl.v = Ldirs;
//    Lctrl.b = Lb;
//    Lctrl.kp = Lkp;
//    Lctrl.kd = Lkd;
//    Lctrl.w0 = Lw0;
//
//    Rctrl.v = Rdirs;
//    Rctrl.b = 250; //变大后响应慢,需要增加其他量
//    Rctrl.kp = 50;   //太小响应不够,太大有超调
//    Rctrl.kd = 0;
//    Rctrl.w0 = 90;    //过度增大w0无法响应,减小超调减小但响应速度慢
////
    if(b<1000)
    {
        ADRCCTRL_Update(&Lctrl);
        ADRCCTRL_Update(&Rctrl);
        Lctrl_t.r = Lrset;
        Rctrl_t.r = Rrset;
        ADRCCTRL_TD(&Lctrl_t,Ldirs);
        ADRCCTRL_TD(&Rctrl_t,Rdirs);
        Ldesiredspeed = Lctrl_t.v1;
        Rdesiredspeed = Rctrl_t.v1;
        b++;
    }
    else
    {
            Rdesiredspeed = Rdirs;
            Ldesiredspeed = Ldirs;
    }
}
void Ctrlinit()
{
    ADRCCTRL_Init(&Lctrl,0.001*period);
    ADRCCTRL_Init(&Rctrl,0.001*period);
}

void Ctrltinit()
{
    ADRCCTRL_TDInit(&Lctrl_t,0.001*period);
    ADRCCTRL_TDInit(&Rctrl_t,0.001*period);
}



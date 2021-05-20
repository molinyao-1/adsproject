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

#define deadgeerleft 500
#define deadgeerright 550


int angle_time = 0;
extern int gogoflag;

float init_ax = 0,init_ay = 0,init_az = 0;
float acc_angle_x = 0,gyro_angle_x = 0;    //一阶互补的参数传递
float angle_x = 0,angle_y = 0,angle_z = 0;

float acc_x = 0,acc_y = 0,acc_z = 0;
float gyro_x = 0,gyro_y = 0,gyro_z = 0;

extern float acc[3];
extern float gyro[3];

extern float gyrolast[3];
extern float gyronow[3];

#define gyrooffset0 -4.5
#define gyrooffset1 0
#define gyrooffset2 0.45
static float dt = 0.005;

float one_filter_angle = 0;

float pulseleft;
float pulseright;

int L_time = 0; //左侧光电管计数
int R_time = 0;

int slowline ;//减速线设置
float slowladjust = 0.7; //(菜单调节)
uint32_t servofinal;

int speed0 = 80; //(菜单调节)
int speed1 = 80; //减速的速度
int speed2 = 80; //减速的速度
int speed3 = 80; //转弯的速度

//(菜单调)
int mode0limit = 10;
int correctlimit = 15;
int LOWplus = 15;
int reshunlimit = 40;
/**/

int side_flag = 0;
int i = 0;
int red_flag = 0; //光电管状态切换标志位
int light_last = 0;
int red_out_flag = 0;//是否触发光电管标志位
int light_square = 0; //灯的面积
int squarebA = 0; //灯的面积
int last_squarebA = 0; //灯的面积
int squareA = 0; //灯的面积
int squarelimit = 0;

int Long_square;


/*MPU接收参数*/
float send_buf[3] = {0};
extern mpu_t this_mpu;
/**/

/*以下为切灯系数*/
int KLA = 1.055;
int KRA = -1.144;
/**/

/*自定义一些变量*/
int servodirection = 0;              //控制舵机转向打死方向 （-1：左打死 ；1：右打死）
pidCtrl_t  servo_pid; //舵机PID初始化
float servokp = 1.3; //(菜单调）
float servokd = 2.5;
float servo_pulse = 0;

int motorstate = 0;  //(后期如果状态多的话考虑二维数组)0:加速 ；1：减速
float Ldesiredspeed=0;
float Rdesiredspeed=0;
pid motorpir,motorpil;
float motorbkp_R = 0,motorbki_R = 0,motorbkp_L = 0,motorbki_L = 0; //(菜单调)
int16 leftFTM = 0;
int16 rightFTM = 0;
int16 lastleftFTM = 0;
int16 lastrightFTM = 0;
float speed_r = 0;
float speed_l = 0;
float motor_lefdevn =0;
float motor_rigdevn =0;
float N_motor_lefdevn=0,N_motor_rigdevn=0;

uint32_t motor_lefdevnU =0;
uint32_t motor_rigdevnU =0;
uint32_t N_motor_lefdevnU=0,N_motor_rigdevnU=0;
/**/

/*鄙人已知作用的变量*/

//以下为各类标志位
int mode_flag = 2;  //寻灯状态标志位，0直线追灯，1近灯，2灭灯旋转，3看到灯顺正->回归0
//以下为图像处理相关变量
int distance = 0;   //信标灯与参考点距离（表示信标灯和车距离）
int dislimit = 90;
int lastdistance = 0;
int errordis = 50;

int error_simu = 0; //模拟信标灯偏差
float simu_distance = 0;                  //模拟坐标时用的距离中间变量
int  simu_y = 161;          //模拟信标灯坐标

int connect_max = 4; //没有连通域的次数大于该值时认为处于灭灯旋转状态
//以下为速度控制相关变量
int roll_time;                      //记录旋转次数变量

//未分类（可放在beacon_control.c文件里）
int correct_time = 0; //顺正时间
int mode_last = 2;    //上一次状态标志位
int LOW = 50;    //具体使用的摄像头行数 (应该后期可以修改，距离小于这一值就进入近灯状态)
/**/

/*鄙人未知作用的变量*/

//以下为各类标志位
int hit_flag = 0;
int light_flag = 0; //公用前灯标志位
int back_flag = 0;  //视野外信标灯位置
int go_flag = 1;    //进入跑车程序标志位
int light_change_flag = 0;    //可切换切灯方向标志位

//以下为图像处理相关变量
int aver_x = 0, aver_y = 0;        //红外中点坐标
int averbA_x = 100,averbA_y = 120;  //摄像头中点坐标（这个摄像头具体中点坐标后续要改）
int averA_x = 100,averA_y = 120;     //摄像头中点坐标(两个摄像头中点坐标，可能有别的用处)
int last_averA_x = 100, last_averA_y = 120;   //上一次红外(前期不用红外，后期如果用要进行更改，数据肯定不准)中点坐标
int origin_x = 145;  //模拟原点行数
int origin_y = 87;   //模拟圆心纵坐标
int error_dx = 0,error_dy = 0;           //实际灯和参考点横纵差值

int long_flag = 0;    //追远灯标志位

int contflag = 0;


int areaA_time = 0;  //连通域为0次数标志位
//未分类（可放在beacon_control.c文件里）
int mode_time = 0;    //记录各状态持续次数
int Avoid_Low = 0;    //避障最小距离

float servo_pulsem;
float smcoup = 0.25;

//mmc
extern uint8 connectnum;//连通域数
extern float lightx, lighty, lights, lightc;
extern float lightdis;


int servoduty;
extern int servoinit;
int flagservo = 1;
int lightsensor[2] = {1};
int lightsenlast[2] = {1};

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
float Lforward;
float Lbackward;

float Ldirs = 0;
float Rdirs = 0;
adrc_t Lctrl;
adrc_t Rctrl;

float Lrset = 20;
float Rrset = 20;
adrc_td_t Lctrl_t;
adrc_td_t Rctrl_t;

float Lb;
float Lkp;
float Lkd;
float Lw0;

int b = 0;

extern float Rb ;
extern float Rkp ;
extern float Rkd ;
extern float Rw0 ;

#define period 5

/*新加菜单调的量*/
int lowlimit0 = 20;
int lowlimit1 = 26;
int lowlimit2 = 34;
int lowlimit3 = 40;
int lowlimit4 = 60;
int speedmark = 0; //0:高速 1:中速 2:低速
float correctspeed;
float leftcorrect = 1;
float rightcorrect = 1;

extern int aa;


void Encoder_init()
{
    SmartCar_Encoder_Init(GPT12_T4,IfxGpt120_T4INA_P02_8_IN,IfxGpt120_T4EUDA_P00_9_IN);  //Left
    SmartCar_Encoder_Init(GPT12_T6,IfxGpt120_T6INA_P20_3_IN,IfxGpt120_T6EUDA_P20_0_IN);  //right
}

void FINAL_CONTROL()
{
    MODE_JUDGE();
    STOP_JUDGE();
//    SIDE_JUDGE();
    ERROR();
    FIND_LIGHT();
    BatterySCM_Get();
}


//对于连通域，始终在更新两个area_flag，处理的时候利用这两个变量
void CAMERA_JUDGE()
{
    averbA_x = lightx;
    averbA_y = lighty;
    squarebA = (int)lights;
    if(connectnum ==0)                                        //如果没有找到连通域，记录连通域为零次数
    {
        areaA_time++;                        //连通域为零次数加一
    }
    else
    {
        areaA_time = 0;
        last_averA_x = averbA_x;
        last_averA_y = averbA_y;
        last_squarebA = squarebA;
    }
    if(areaA_time == 0)
    {
        averA_x = last_averA_x;
        averA_y = last_averA_y;
        squareA = last_squarebA;
    }
    aver_x = averA_x;
    aver_y = averA_y;
    light_square = squareA;
}


/***************************** 状态判断处理程序 *******************************/
void MODE_JUDGE()
{
    lastdistance = distance;
    error_dx = aver_x - origin_x;
    error_dy = aver_y - origin_y;
    distance = CarmSqrt(error_dx*error_dx + error_dy*error_dy);   //计算灯距
    speedever = (leftFTM+rightFTM)/20;

    /*速度分段*/
    if(aver_x<lowlimit0&&(aver_x+speedever)<lowlimit1)
    {
        speedmark = 0;
    }

    else
    {
        if(aver_x<=lowlimit2)
        speedmark = 1;
    }

    if((aver_x>lowlimit2&&(aver_x+speedever)>lowlimit3)||aver_x>lowlimit4)
    {
        speedmark = 2;
    }

    /*状态机*/
    if(mode_flag == 0&&distance<dislimit)  //近灯状态
    {
        mode_last = mode_flag;
        mode_flag = 1; //设置为近灯状态
        mode_time = 0;
    }

    if(1 == mode_flag&&abs(distance-lastdistance)>errordis) //灯被对手灭,远处还有灯,判定的两次距离差,并且大于此阈值就转换状态
    {
        mode_last = mode_flag;
        mode_flag = 0;
        mode_time = 0;
    }
    if(1 == mode_flag&&battery<balowlimit) //处于近灯状态,并且电压不够
    {
        mode_last = mode_flag;
        mode_flag = 3;
        mode_time = 0;
    }
    if(3 == mode_flag&&(battery>bauplimit)&&(areaA_time>connect_max)) //无灯且充电满
    {
        mode_last = mode_flag;
        mode_flag = 2;
        mode_time = 0;
    }
    if(3 == mode_flag&&(battery>bauplimit)&&(areaA_time == 0))
    {
        mode_last = mode_flag;
        mode_flag = 0;
        mode_time = 0;
    }
    if(mode_flag !=2 &&(areaA_time>connect_max))
    {
        mode_last = mode_flag;
        mode_flag = 2;
        mode_time = 0;
    }
    if(2 == mode_flag&&areaA_time == 0)
    {
        mode_last = mode_flag;
        mode_flag = 0;
        mode_time = 0;
    }

    mode_time++; //mode_time最少是1！！！！(记录着一个状态持续时间)
}


/***************************** 坐标偏差控制程序 *******************************/
void ERROR()
{
    if(0 == mode_flag||1 == mode_flag)
    {
        error_simu = aver_y - 88;
    }
}
/***************************** 侧灯判断处理程序 *******************************/
void SIDE_JUDGE()    //因为没有光电管，前期先不加
{//side_flag 在车身左为-1，右为1

    if(mode_flag == 2||mode_flag == 3)
    {
        Eru_Enable(CH0_P15_4);
        Eru_Enable(CH5_P15_8);
        if(L_time>=2)
        {
            side_flag = 1;
            Eru_Disable(CH0_P15_4);
            Eru_Disable(CH5_P15_8);
        }
        if(R_time>=2)
        {
            side_flag = -1;
            Eru_Disable(CH0_P15_4);
            Eru_Disable(CH5_P15_8);
        }
    }
    else
    {
        side_flag = 0;
        R_time = L_time = 0;
    }
}
/******************************* 灭找亮程序（灭完灯后找下一个灯） **********************************/
void FIND_LIGHT()
{
    if(mode_flag !=2)
    {
        servodirection = 0;
    }
    else if(mode_flag == 2)  //结束近灯处于灭灯旋转状态
    {
//        if(side_flag != 0)
//        {
//            switch(side_flag){
//                case 1:servodirection = 1;break;
//                case -1:servodirection = -1;break;
//                default:break;
//            }
//        }
//        if(servodirection == 0)
//        {
//            contflag++;
//        }
//        else if(servodirection!=0)
//        {
//            contflag = 0;
//        }
//        if(contflag == 4)
//        {
//            servodirection = 1; //(一律左转)
//        }
        servodirection = 1; //(一律左转)
    }
}

//舵机控制

void Servo_motorinit()
{
        /*舵机的占空比中值需要重新测*/
//        SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,333,6225);
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,20000,0);
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,20000,0); //给正占空比转

        /*左*/
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,20000,0); //给占空比正转
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,20000,0);


}

void Servo_control()
{
    if(servoinit == 1&&flagservo == 1)
    {
        SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,333,6225);
        flagservo = 0;
    }
    if(servoinit == 1&&flagservo ==0)
    {
        if(servodirection == -1)
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(6225-deadgeerright));
        }
        else if(servodirection == 1)
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(6225+deadgeerleft));
        }
        else
        {
            //四轮车顺正状态和追灯状态共用一个舵机控制方法 //也需要优化更改,舵机打角与速度挂钩
            servo_pid.kp = servokp;
            servo_pid.kd = servokd;
            servo_pid.errPrev = servo_pid.errCurr;
            servo_pid.errCurr = (float)error_simu;
            servo_pid.errDiff = servo_pid.errCurr - servo_pid.errPrev;

            servo_pulse=servo_pid.kp*servo_pid.errCurr+servo_pid.kd*servo_pid.errDiff;

            if(servo_pulse>550)
            {
                servo_pulse =550;
            }
            else if(servo_pulse<-500)
            {
                servo_pulse = -500;
            }
            if(gogoflag == 1)
            {
                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(6225-servo_pulse));
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
        lastleftFTM = leftFTM;
        lastrightFTM = rightFTM;
        leftFTM = -SmartCar_Encoder_Get(GPT12_T4);
        rightFTM = SmartCar_Encoder_Get(GPT12_T6);
        SmartCar_Encoder_Clear(GPT12_T4);
        SmartCar_Encoder_Clear(GPT12_T6);

        speed_l = (float)(lastleftFTM+leftFTM)/2;
        speed_r = (float)(lastrightFTM+rightFTM)/2;

        Imudataprocess();//获取车身当前与水平面夹角
        //motorI_get();
        if(mode_flag == 0||mode_flag == 1) // 除灭灯和充电状态，其余状态计算电机的减速线
        {
            if(0 == speedmark)
            {
                Ldesiredspeed = (float)speed0;
                Rdesiredspeed = (float)speed0;
            }
            if(1 == speedmark)
            {
                Ldesiredspeed = (float)speed1;
                Rdesiredspeed = (float)speed1;
            }

            if(2 == speedmark)
            {
                Ldesiredspeed = (float)speed2;
                Rdesiredspeed = (float)speed2;
            }


            /*NEW ITEM*/
            if(servo_pulse > 125) //右
            {
                if(servo_pulse>250&&abs(angle_z)<=140)
                {
                    Rdesiredspeed = (Rdesiredspeed)*((-0.003824*(servo_pulse*0.01)*(servo_pulse*0.01)-0.06035*(servo_pulse*0.01)+0.991)*correctspeed);
                }
                else
                {
                    Rdesiredspeed = (Rdesiredspeed)*(-0.003824*(servo_pulse*0.01)*(servo_pulse*0.01)-0.06035*(servo_pulse*0.01)+0.991)*rightcorrect;
                }
            }
            else if(servo_pulse<-125) //左
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
            Ldesiredspeed = speed3;
            Rdesiredspeed = speed3;
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
                Rdesiredspeed = 2*speed3-Ldesiredspeed;
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
                Ldesiredspeed = 2*speed3-Rdesiredspeed;
            }
        }

        else if(mode_flag == 3)
        {
            if(Lforward>=300&&Lbackward<300)
            {
               SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,0);
               if(1){
               SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,3000);
               }
               if(1){
                   SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,3000);
               }
               if(1){
                   SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,0);
               }
            }
            else if(Lforward<300&&Lbackward>=300)
            {

               SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,3000);
               if(1){
                   SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,0);
               }
               if(1){

                   SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,0);
               }
               if(1){
                   SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,3000);
               }
            }
            else if(Lforward<300&&Lbackward<300)
            {
                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,0);
                if(1){
                    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,0);
                }
                if(1){
                    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,0);
                }
                if(1){
                    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,0);
                }
            }
            else if(Lforward>300&&Lbackward>300)
            {
                if(leftFTM>10||rightFTM>10)
                {
                    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,0);
                    if(1){
                        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,3500);
                    }
                    if(1){
                        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,3500);
                    }
                    if(1){
                        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,0);
                    }
                }
                else
                {
                    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,0);
                    if(1){
                        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,0);
                    }
                    if(1){
                        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,0);
                    }
                    if(1){
                        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,0);
                    }
                }
            }
        }
        if(mode_flag == 0||mode_flag == 1||mode_flag == 2)
        {
            motorpir.Kp = motorbkp_R;
            motorpir.Ki = motorbki_R;
            motorpil.Kp = motorbkp_L;
            motorpil.Ki = motorbki_L;
            motorpir.ekL = motorpir.ek;
            motorpir.ek = (Rdesiredspeed-(float)rightFTM);
            motorpil.ekL = motorpil.ek;
            motorpil.ek = (Ldesiredspeed-(float)leftFTM);
            pulseleft = motorpil.Kp*(motorpil.ek-motorpil.ekL) + motorpil.Ki*motorpil.ek;
            pulseright = motorpir.Kp*(motorpir.ek-motorpir.ekL) + motorpir.Ki*motorpir.ek;
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


            if(go_flag) //进入跑车状态
            {
                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,(uint32_t)N_motor_rigdevn);
                if(1){
                    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,(uint32_t)motor_rigdevn);
                }
                if(1){
                    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,(uint32_t)motor_lefdevn);
                }
                if(1){
                    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,(uint32_t)N_motor_lefdevn);
                }
            }
        }
        if(go_flag == 0)//要停车了
        {
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,0);
            if(1){
                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,0);
            }
            if(1){
                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,0);
            }
            if(1){
                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,0);
            }
        }
    }
}

void Motorctrlq()
{
        leftFTM = -SmartCar_Encoder_Get(GPT12_T4);
        rightFTM = SmartCar_Encoder_Get(GPT12_T6);
        SmartCar_Encoder_Clear(GPT12_T4);
        SmartCar_Encoder_Clear(GPT12_T6);
//        Ldesiredspeed = (float)aa;
//        Rdesiredspeed = (float)aa;
//        motorpir.Kp = motorbkp_R;
//        motorpir.Ki = motorbki_R;
//        motorpil.Kp = motorbkp_L;
//        motorpil.Ki = motorbki_L;
//        motorpir.ekL = motorpir.ek;
//        motorpir.ek = (Rdesiredspeed-(float)rightFTM);
//        motorpil.ekL = motorpil.ek;
//        motorpil.ek = (Ldesiredspeed-(float)leftFTM);
//        pulseleft = motorpil.Kp*(motorpil.ek-motorpil.ekL) + motorpil.Ki*motorpil.ek;
//        pulseright = motorpir.Kp*(motorpir.ek-motorpir.ekL) + motorpir.Ki*motorpir.ek;
//        motor_lefdevn += pulseleft;
//        motor_rigdevn += pulseright;
//        if(motor_lefdevn<0)
//        {
//            N_motor_lefdevn=-motor_lefdevn;
//            motor_lefdevn=0;
//        }
//        else if(motor_lefdevn>=0)
//        {
//            N_motor_lefdevn=0;
//        }
//        if(motor_rigdevn<0)
//        {
//            N_motor_rigdevn=-motor_rigdevn;
//            motor_rigdevn=0;
//        }
//        else if(motor_rigdevn>=0)
//        {
//            N_motor_rigdevn=0;
//        }
//        if(motor_rigdevn>8000){motor_rigdevn=8000;}
//        if(motor_lefdevn>8000){motor_lefdevn=8000;}
//        if(N_motor_rigdevn>8000){N_motor_rigdevn=8000;}
//        if(N_motor_lefdevn>8000){N_motor_lefdevn=8000;}
//
//        motor_rigdevnU = (uint32)motor_rigdevn;
//        motor_lefdevnU = (uint32)motor_lefdevn;
//        N_motor_rigdevnU = (uint32)N_motor_rigdevn;
//        N_motor_lefdevnU = (uint32)N_motor_lefdevn;
//
//            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,(uint32_t)N_motor_rigdevn);
//            if(1){
//                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,(uint32_t)motor_rigdevn);
//            }
//            if(1){
//                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,(uint32_t)motor_lefdevn);
//            }
//            if(1){
//                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,(uint32_t)N_motor_lefdevn);
//            }
}

/***************************** 停车判断处理程序 *******************************/
void STOP_JUDGE()
{
    if(areaA_time == 0||mode_flag == 3)
        roll_time = 0;
    else{
        roll_time ++;
    }
    if(roll_time > 5000)  //设为0则会长时间无灯停车
    {
        go_flag = 0;
        roll_time = 5000;
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

void Imudataprocess()
{
    SmartCar_MPU_Getacc2(&this_mpu);
    SmartCar_MPU_Getgyro2(&this_mpu);
//    gyro[0] = this_mpu.mpu_rawdata.gyro_x;
//    gyro[1] = this_mpu.mpu_rawdata.gyro_y;
    gyronow[2] = this_mpu.mpu_rawdata.gyro_z;
//    if(gyro[0]<-4.2&&gyro[0]>-4.8)
//    {
//        gyro[0] = -4.5;
//    }
//
//    if(gyro[1]<0.5&&gyro[1]>-0.5)
//    {
//        gyro[1] = 0;
//    }
//
    gyronow[2] = kalmanFilter_A(gyronow[2]);
    if(gyronow[2]>0.3&&gyronow[2]<0.6)
    {
        gyronow[2] = 0.45;
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

    //转弯时电机控制强度减弱
    motorpir.Kp = motorbkp_R;
    motorpir.Ki = motorbki_R;
    motorpil.Kp = motorbkp_L;
    motorpil.Ki = motorbki_L;

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

    motor_rigdevnU = (uint32)motor_rigdevn;
    motor_lefdevnU = (uint32)motor_lefdevn;
    N_motor_rigdevnU = (uint32)N_motor_rigdevn;
    N_motor_lefdevnU = (uint32)N_motor_lefdevn;

}


void Motor_control1()
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
//    Rdesiredspeed = Rdirs;
//    Ldesiredspeed = Ldirs;
     motorpir.Kp = motorbkp_R;
     motorpir.Ki = motorbki_R;
     motorpil.Kp = motorbkp_L;
     motorpil.Ki = motorbki_L;
     motorpir.ekL = motorpir.ek;
     motorpir.ek = (Rdesiredspeed-(float)rightFTM);
     motorpil.ekL = motorpil.ek;
     motorpil.ek = (Ldesiredspeed-(float)leftFTM);
     pulseleft = motorpil.Kp*(motorpil.ek-motorpil.ekL) + motorpil.Ki*motorpil.ek;
     pulseright = motorpir.Kp*(motorpir.ek-motorpir.ekL) + motorpir.Ki*motorpir.ek;
     motor_lefdevn += pulseleft;
     motor_rigdevn += pulseright;
     if(motor_rigdevn>8000){motor_rigdevn=8000;}
     if(motor_lefdevn>8000){motor_lefdevn=8000;}
     if(N_motor_rigdevn>8000){N_motor_rigdevn=8000;}
     if(N_motor_lefdevn>8000){N_motor_lefdevn=8000;}
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

    SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,(uint32_t)N_motor_rigdevn);
    if(1){
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,(uint32_t)motor_rigdevn);
    }
    if(1){
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,(uint32_t)N_motor_lefdevn);
    }
    if(1){
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,(uint32_t)motor_lefdevn);
    }
}

void changeFTM()
{
    Ldirs += 40;
    Rdirs += 40;
    if(Ldirs>=160)
    {
        Ldirs = 0;
    }
    if(Rdirs>=160)
    {
        Rdirs = 0;
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


void motorctrl()
{
    motorI_get();
    motorpid();
}


///*
// * beacon_control.c
// *
// *  Created on: 2020年12月20日
// *      Author: jimi
// */
#include "beacon_control.h"
#include "image.h"
#include "stdlib.h"

#define threeorfour 0

float pulseleft;
float pulseright;

int slowline ;//减速线设置
float slowladjust = 0.7; //(菜单调节)
uint32_t servofinal;
int speedmostfast = 80; //(菜单调节)
int correctxishu; //修正系数,控制拟合直线上下平移
float correctmenu = 0.2; //菜单调  (减速提前,减小)

//(菜单调)
int mode0limit = 10;
int correctlimit = 15;
int LOWplus = 15;
int reshunlimit = 40;
/**/

int side_flag = 0;
int lightsensor[4] = {0};
int red_flag = 0; //光电管状态切换标志位
int light_last = 0;
int red_out_flag = 0;//是否触发光电管标志位
int light_square; //灯的面积
int Long_square;


/*MPU接收参数*/
float send_buf[3] = {0};
extern mpu_t* this_mpu;
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
int slowspeedwant = 80; //减速的速度
float deadgeer = 80;


int motorstate = 0;  //(后期如果状态多的话考虑二维数组)0:加速 ；1：减速
float Ldesiredspeed;
float Rdesiredspeed;
pid motorpir,motorpil;
float motorbkp_R = 50,motorbki_R = 30,motorbkp_L = 50,motorbki_L = 30; //(菜单调)
int16 leftFTM;
int16 rightFTM;
float motor_lefdevn =0;
float motor_rigdevn =0;
float N_motor_lefdevn=0,N_motor_rigdevn=0;

uint32_t motor_lefdevnU =0;
uint32_t motor_rigdevnU =0;
uint32_t N_motor_lefdevnU=0,N_motor_rigdevnU=0;
/**/

/*鄙人已知作用的变量*/

//以下为各类标志位
int mode_flag = 0;  //寻灯状态标志位，0直线追灯，1近灯，2灭灯旋转，3看到灯顺正->回归0
//以下为图像处理相关变量
int distance = 0;   //信标灯与参考点距离（表示信标灯和车距离）
int error_simu = 0; //模拟信标灯偏差
int obst_x = 121,obst_y = 161; //障碍物坐标
int obstacleA_x = 121,obstacleA_y = 161;  //障碍物坐标
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
int origin_x = 100;  //原点行数
int origin_y = 94;   //圆心纵坐标
int error_dx = 0,error_dy = 0;           //实际灯和参考点横纵差值

int long_flag = 0;    //追远灯标志位


/**/

//int *speedA_M = &date[2][2];        //以下为四个三段速行数和速度
//int *speedA_H = &date[2][3];
//int *klA_adjust = &date[1][3];     //偏差修正用系数(计算模拟中线时的误差修正系数)
//int *krA_adjust = &date[1][4];
float klA_adjust = 64.46;
float krA_adjust = 99.69;

int areaA_time = 10;  //连通域为0次数标志位
//未分类（可放在beacon_control.c文件里）
int mode_time = 0;    //记录各状态持续次数
int Avoid_Low = 0;    //避障最小距离

float servo_pulsem;

//mmc
extern uint8 connectnum;//连通域数
extern float lightx, lighty, lights, lightc;
extern float lightdis;

int servoduty;

void Encoder_init()
{
    SmartCar_Encoder_Init(GPT12_T4,IfxGpt120_T4INA_P02_8_IN,IfxGpt120_T4EUDA_P00_9_IN);  //Left
    SmartCar_Encoder_Init(GPT12_T6,IfxGpt120_T6INA_P20_3_IN,IfxGpt120_T6EUDA_P20_0_IN);  //right
}

void FINAL_CONTROL()
{
    CAMERA_JUDGE();
    STOP_JUDGE();
    MODE_JUDGE();
    SIDE_JUDGE();
    ERROR();
    FIND_LIGHT();
}


//对于连通域，始终在更新两个area_flag，处理的时候利用这两个变量
void CAMERA_JUDGE()
{
    averbA_x = lightx;
    averbA_y = lighty;
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
    aver_x = averA_x;
    aver_y = averA_y;
}


/***************************** 状态判断处理程序 *******************************/
void MODE_JUDGE()
{
    LOW = (int)(120-(leftFTM + rightFTM)*correctmenu);
    if(LOW<25)
        LOW = 25;
    else if(LOW>60)
        LOW = 60;
    if(mode_flag == 3 && fabs(error_simu) < mode0limit ) //看到灯顺正，进入追灯状态，判断条件：信号灯偏差多次小于某一定值时进入追灯，定值10暂定，可以更改
    {
        correct_time++;
        if(correct_time>correctlimit) //顺正所需时间暂定为21次
        {
            mode_last = mode_flag; //存储上一次的状态
            mode_flag = 0; //设置为追灯状态
            correct_time = 0;
            mode_time = 0; //记录各状态持续时间时先进行清零操作（具体作用？？？？）
        }
    }
  //  if(mode_flag == 0 && distance <= LOW)  //近灯状态，有可能不触发 || mode_flag == 1  && fabs(error_adjust) < 20
    if(mode_flag == 0 && aver_x >= LOW)  //近灯状态，有可能不触发 || mode_flag == 1  && fabs(error_adjust) < 20
    {
        mode_last = mode_flag;
        mode_flag = 1; //设置为近灯状态
        mode_time = 0;
    }
    //灭灯旋转状态判断(无论车处于什么状态，只要看不到灯了，就进入灭灯旋转状态)
    if(mode_flag != 2 && (areaA_time > connect_max))
    {
        mode_last = mode_flag;
        mode_flag = 2; //设置为灭灯旋转状态状态
        mode_time = 0;

        if(light_flag != 0)
          light_last = light_flag;   //给光电管用

    }
    if(mode_flag == 2 &&  areaA_time == 0)
    {
        mode_last = mode_flag;
        mode_flag = 3;
        mode_time = 0;
        back_flag = 0; //具体作用还不太清楚（可能是顺正状态下不需要视野外的灯干扰）
    }
//    else if(mode_flag == 1 && distance > LOW + 15)    //灭完灯视野内还有灯，+3是为了防止灭灯之前认为顺正 此时也进入顺正状态
    else if(mode_flag == 1 && aver_x < LOWplus)    //灭完灯视野内还有灯，+3是为了防止灭灯之前认为顺正 此时也进入顺正状态

    {
      mode_last = mode_flag;
      mode_flag = 3;
      mode_time = 0;
      back_flag = 0; //
    }
    else if(mode_flag == 0 && abs((int)error_simu) > reshunlimit) //追灯状态由于避障偏差较大时，进入顺正状态
    {
      mode_last = mode_flag;
      mode_flag = 3;
      mode_time = 0;
      back_flag = 0;
    }
    mode_time++; //mode_time最少是1！！！！(记录着一个状态持续时间)
    if(mode_flag != 3 && correct_time != 0)     //有灯到丢灯清掉顺正计数位（顺正过程中突然灯灭了，就要清除掉记时标志位）
        correct_time = 0;

    /*学长代码，顺正过程中需要对GPIO进行操作
    if(mode_flag==3)
        GPIO_PortSet(GPIOE, 1U << 12U);
    else
        GPIO_PortClear(GPIOE, 1U << 12U);
    */
}


/***************************** 障碍判断处理程序 *******************************/
void OBSTACLE_JUDGE()
{
    static int error_obst_dx = 0,error_obst_dy = 0; //灯距中间变量

    }


/***************************** 坐标偏差控制程序 *******************************/
void ERROR()
{
    static int error_aver = 0;
    error_aver = aver_y - 94;  //y方向与镜头某一位置的偏差（好好思考其意义）
    if((mode_flag == 3 && mode_time == 1)|| (mode_flag == 0 && go_flag == 1)/*进入追灯状态且跑车标志位为1*/ || light_change_flag == 1/*可切换切灯方向为1，认为可以进行切灯方向的变换*/) //锁死切灯位置
    {  //确定切左还是切右！！！！！！！！
        if(light_change_flag == 1)   //临时的状态转换！！！！！！！！！！！！！！！！(具体作用还需要思考)
        {  //在避障过程中发挥作用！！！！！！（不避障没作用）
            if(abs(error_aver) > 10)    //临时转换状态，偏差较大，则可切换切灯方向()
            {
                if(error_aver > 0)
                    light_flag = 1; //light_flag应该是切灯方向的一个标志位
                else
                    light_flag = -1;
            }
        }
        else
        {
            if(error_aver>0)
                light_flag = 1;
            else
                light_flag = -1;
        }
        light_change_flag = 0;
    }
//    error_dx = aver_x - origin_x;
//    error_dy = aver_y - origin_y;
//    distance = CarmSqrt(error_dx*error_dx + error_dy*error_dy);   //计算灯距

    if(mode_flag == 0 && mode_time == 1)                                          //进入追灯状态判断距离
    {
        if(light_square <= Long_square ) //如果信号灯的面积小于某一定值时，认为还是处于追灯状态，加速前进
           long_flag = 1;  //追远灯
        else
           long_flag = 0;
    }
    if(light_flag == 1) //切左
    {
        simu_y = KLA*(aver_x)+klA_adjust; //拟合simu_y和aver_x的关系
        if(simu_y<94)
        {
            simu_y = 94;
        }
        else if(simu_y>150)
        {
            simu_y = 150;
        }
    }
    else if(light_flag == -1)  //切右
    {
        simu_y = KRA*(aver_x) + krA_adjust;
        if(simu_y>94)
        {
            simu_y = 94;
        }
        else if(simu_y<30)
        {
            simu_y = 30;
        }
    }
//    correctxishu = (int)((leftFTM + rightFTM)*correctmenu);
//    if(mode_flag == 0 && mode_time == 1)                                          //进入追灯状态判断距离
//    {
//        if(light_square <= Long_square ) //如果信号灯的面积小于某一定值时，认为还是处于追灯状态，加速前进
//           long_flag = 1;  //追远灯
//        else
//           long_flag = 0;
//    }
//    if(light_flag == 1) //切左
//    {
//        simu_y = KLA*(aver_x-correctxishu)+klA_adjust; //拟合simu_y和aver_x的关系
//        if(simu_y<94)
//        {
//            simu_y = 94;
//        }
//        else if(simu_y>150)
//        {
//            simu_y = 150;
//        }
//    }
//    else if(light_flag == -1)  //切右
//    {
//        simu_y = KRA*(aver_x-correctxishu) + krA_adjust;
//        if(simu_y>94)
//        {
//            simu_y = 94;
//        }
//        else if(simu_y<30)
//        {
//            simu_y = 30;
//        }
//    }

    //以下为计算障碍灯偏差
//    if(mode_flag == 3)
//    {
//        error_simu = aver_y - 94;  //计算模拟信标灯偏差 （作用未知）
//    }
//    else
//    {
        error_simu = aver_y - simu_y;
//    }
}
/***************************** 侧灯判断处理程序 *******************************/
void SIDE_JUDGE()    //因为没有光电管，前期先不加
{//side_flag 在车身左为-1，右为1
    if(/*red_flag == 0&&*/mode_flag == 2&&side_flag == 0)
    {
        lightsensor[0] = GPIO_Read(P15,4);
        lightsensor[1] = GPIO_Read(P11,10);
        lightsensor[2] = GPIO_Read(P33,7);
        lightsensor[3] = GPIO_Read(P20,9);
    }

    if(lightsensor[0] == 0||lightsensor[1] == 0||lightsensor[2] ==0 ||lightsensor[3] == 0){
        if((lightsensor[0]+lightsensor[2])<=(lightsensor[1]+lightsensor[3]))
        {
            side_flag = 1; //在左
            lightsensor[0] = 1;
            lightsensor[1] = 1;
            lightsensor[2] = 1;
            lightsensor[3] = 1;
        }
        else if((lightsensor[0]+lightsensor[2])>(lightsensor[1]+lightsensor[3]))
        {
            side_flag = -1; //在右
            lightsensor[0] = 1;
            lightsensor[1] = 1;
            lightsensor[2] = 1;
            lightsensor[3] = 1;
        }
    }
    else if(mode_flag != 2&&areaA_time < connect_max)  //30可调整
    {
//            red_flag = 2;
        side_flag = 0;
    }
//    else if(areaA_time>100&&mode_flag == 2){
//
//    }
}
//    if(red_flag != 0&&areaA_time == 0)
//    {
//        red_flag = 0;
//    }
//    if(side_flag != 0&&mode_flag!=2)
//    {
//        side_flag = 0;
//    }
//    if(mode_flag == 2&&side_flag == 0)
//    {
//        side_flag = light_last;
//    }

/******************************* 灭找亮程序（灭完灯后找下一个灯） **********************************/
void FIND_LIGHT()
{
    servodirection = 0;
    if(roll_time < 600) //旋转次数小于某一值
    {
        if(mode_flag == 2)  //结束近灯处于灭灯旋转状态
        {
            if((light_flag == -1&&side_flag == -1)||(light_flag == 1&&side_flag == -1))   //切右灯在右边,切左灯在右边
            {
                servodirection = -1;  //向右打死舵机转弯
            }
            else if((light_flag == 1&&side_flag == 1)||(light_flag == -1&&side_flag == 1)) //切左灯在左边,切右灯在左边
            {
                servodirection = 1;  //向左打死舵机转弯
            }
            else
            {
//                if(light_flag == 0){
//                switch(light_flag){
//                  case 1:servodirection = -1;break;
//                  case -1:servodirection = 1;break;
//                  default:break;
//                    }
                //}
                if(side_flag != 0)
                {
                    switch(side_flag){
                        case 1:servodirection = 1;break;
                        case -1:servodirection = -1;break;
                        default:break;
                    }
                }
                else
                {
                    switch(light_flag){
                    case 1:servodirection = -1;break;
                    case -1:servodirection = 1;break;
                    default:break;
                }
            }
//        else if(mode_flag == 2)  //初始看不到灯的情况下一律左旋
//        {
//            servodirection = -1;
//        }
//    else  //后期是给光电管准备(不管侧灯程序是否开启,均开启观点管，现在好像没什么用)
//    {
//
//    }
    }
}
}
}

//舵机控制

void Servo_motorinit()
{
        /*舵机的占空比中值需要重新测*/
        SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,50,570);
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,20000,0);
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,20000,0);
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,20000,0);
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,20000,0);
}

void Servo_control()
{
    if(servodirection == -1)
    {
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(570-deadgeer));
    }
    else if(servodirection == 1)
    {
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(570+deadgeer));
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

        if(servo_pulse>80)
        {
            servo_pulse =80;
        }
        else if(servo_pulse<-80)
        {
            servo_pulse = -80;
        }
        servofinal = (uint32_t)(570-servo_pulse);
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(570-servo_pulse));
    }
}

void Motor_control()
{
    if(mode_flag == 0||mode_flag == 1||mode_flag == 3) // 除灭灯状态，其余状态计算电机的减速线
    {
        slowline = (int)(120-(leftFTM + rightFTM)*correctmenu);
        if(slowline<25)
            LOW = 25;
        else if(slowline>60)
            LOW = 60;
        if((aver_x>slowline)&&(mode_flag == 1||mode_flag == 3))
        {
            Ldesiredspeed = (float)slowspeedwant;
            Rdesiredspeed = (float)slowspeedwant;
        }
        else //灯在减速线以上 , 速度和舵机打角用高次函数挂钩
        {
            Ldesiredspeed = (float)(slowspeedwant+speedmostfast*pow((abs(servo_pulse)-80),2)/pow(80,2));
            Rdesiredspeed = (float)(slowspeedwant+speedmostfast*pow((abs(servo_pulse)-80),2)/pow(80,2));
//            Ldesiredspeed = (float)slowspeedwant;
//            Rdesiredspeed = (float)slowspeedwant;
        }
    }
    else //看不到灯
    {
        if(servodirection == 1)
        {
            Ldesiredspeed = (float)slowspeedwant*0.6;
            Rdesiredspeed = (float)slowspeedwant;
        }
        else if(servodirection == -1)
        {
            Ldesiredspeed = (float)slowspeedwant;
            Rdesiredspeed = (float)slowspeedwant*0.6;
        }
    }
    //Motorpid_control();
//        Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
        //舵机打角大的时候减小电机速度的控制强度
        motorpir.Kp = motorbkp_R;
        servo_pulsem = servo_pulse>=0?servo_pulse:-servo_pulse;
        motorpir.Ki = motorbki_R - servo_pulsem*0.25;
        motorpil.Kp = motorbkp_L;
        motorpil.Ki = motorbki_L - servo_pulsem*0.25;
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

    //    motor_rigdevnU = (uint32)motor_rigdevn;
    //    motor_lefdevnU = (uint32)motor_lefdevn;
    //    N_motor_rigdevnU = (uint32)N_motor_rigdevn;
    //    N_motor_lefdevnU = (uint32)N_motor_lefdevn;

    if(go_flag) //进入跑车状态
    {
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,(uint32_t)N_motor_rigdevn);
        if(1){
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,(uint32_t)motor_rigdevn);
        }
        if(1){
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,(uint32_t)motor_lefdevn);
        }
        if(1){
            SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,(uint32_t)N_motor_lefdevn);
        }
    }
    else if(go_flag == 0)//要停车了
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


/***************************** 停车判断处理程序 *******************************/
void STOP_JUDGE()
{
    if(areaA_time == 0)
        roll_time = 0;
    else
        roll_time ++;
    if(roll_time > 666)  //设为0则会长时间无灯停车
    {
        go_flag = 0;
        roll_time = 700;
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











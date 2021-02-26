///*
// * beacon_control.c
// *
// *  Created on: 2020年12月20日
// *      Author: jimi
// */
#include "beacon_control.h"
#include "image.h"

/*MPU接收参数*/
float send_buf[3] = {0};
extern mpu_t* this_mpu;
/**/

/*以下为切灯系数*/
int KLA;
int KRA;
/**/

/*自定义一些变量*/
int servodirection = 0;              //控制舵机转向打死方向 （-1：左打死 ；1：右打死）
pidCtrl_t* servo_pid; //=PIDCTRL_Construct(0, 0, 0); //舵机PID初始化
float servokp = 0.013;
float servokd = 0.025;
float servo_pulse = 0;
int straspeedplus = 20; //加速的速度
int straspeedcut = 20; //减速的速度
float difference = 0.6 ;
float deadgeer = 0.8;
float Shun_Dengcut = 0.5;//顺灯状态下kp，kd要相应的减小

int motorstate = 0;  //(后期如果状态多的话考虑二维数组)0:加速 ；1：减速
float Ldesiredspeed;
float Rdesiredspeed;
pid motorpir,motorpil;
float motorbkp_R = 3,motorbki_R = 0.28 ,motorbkp_L = 3,motorbki_L = 0.21; //kp，kd
int16 leftFTM;
int16 rightFTM;
float motor_lefdevn =0;
float motor_rigdevn =0;
float N_motor_lefdevn=0,N_motor_rigdevn=0;
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
int LOW = 85;    //具体使用的摄像头行数 (应该后期可以修改，距离小于这一值就进入近灯状态)
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

int Areajin = 430; //近灯面积标志位
int sundengarea = 0;
int servogradual = 0; //近灯状态舵机进行打角
float differences = 1.2; //顺正过程进行速度补偿
float jindengzhan = 0.5;
int xundengtime = 20;

int xundengtimemie = 0;
int xundengtimemie1 = 200;
/**/

//int *speedA_M = &date[2][2];        //以下为四个三段速行数和速度
//int *speedA_H = &date[2][3];
//int *klA_adjust = &date[1][3];     //偏差修正用系数(计算模拟中线时的误差修正系数)
//int *krA_adjust = &date[1][4];
float klA_adjust = 360.3;
float krA_adjust = 166.6;

int areaA_time = 40;  //连通域为0次数标志位
//未分类（可放在beacon_control.c文件里）
int mode_time = 0;    //记录各状态持续次数
int Avoid_Low = 0;    //避障最小距离



//mmc
extern uint8 connectnum;//连通域数
extern float lightx, lighty, lights, lightc;
extern float lightdis;

void Encoder_init()
{
    //SmartCar_Encoder_Init();
    //SmartCar_Encoder_Init();
}

void FINAL_CONTROL()
{
    CAMERA_JUDGE();
    STOP_JUDGE();
    ERROR();
    MODE_JUDGE();
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
    if(mode_flag == 3 && fabs(error_simu) < 10 ) //看到灯顺正，进入追灯状态，判断条件：信号灯偏差多次小于某一定值时进入追灯，定值10暂定，可以更改
    {
        correct_time++;
        if(correct_time>21) //顺正所需时间暂定为21次
        {
            mode_last = mode_flag; //存储上一次的状态
            mode_flag = 0; //设置为追灯状态
            correct_time = 0;
            mode_time = 0; //记录各状态持续时间时先进行清零操作（具体作用？？？？）
        }
    }
    if(mode_flag == 0 && distance <= LOW && lights > Areajin)  //近灯状态，有可能不触发 || mode_flag == 1  && fabs(error_adjust) < 20
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
        /*
         * if(light_flag != 0)                                                         //有隐患奇奇怪挂，有时间再排查
              light_last = light_flag;
              学长代码，作用未知
         */
    }
    if(mode_flag == 2 &&  areaA_time == 0)
    {
        mode_last = mode_flag;
        mode_flag = 3;
        mode_time = 0;
        back_flag = 0; //具体作用还不太清楚（可能是顺正状态下不需要视野外的灯干扰）
    }
    else if(mode_flag == 1 && distance > LOW + 15)    //灭完灯视野内还有灯，+3是为了防止灭灯之前认为顺正 此时也进入顺正状态
    {
      mode_last = mode_flag;
      mode_flag = 3;
      mode_time = 0;
      back_flag = 0; //
    }
    else if(mode_flag == 0 && abs((int)error_simu) > 50) //追灯状态由于避障偏差较大时，进入顺正状态
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
    error_dx = aver_x - origin_x;
    error_dy = aver_y - origin_y;
    distance = CarmSqrt(error_dx*error_dx + error_dy*error_dy);   //计算灯距
//    if(mode_flag == 0 && mode_time == 1)                                          //进入追灯状态判断距离
//    {
//        if(light_square <= LongA_square ) //如果信号灯的面积小于某一定值时，认为还是处于追灯状态，加速前进
//           long_flag = 1;  //追远灯
//        else
//           long_flag = 0;
//    }
//    if(light_flag == 1) //切左
//    {
//        simu_distance = KLA*distance + klA_adjust;
//
//        simu_y = aver_y - simu_distance ;
//    }
//    else if(light_flag == -1)  //切右
//    {
//        simu_distance = KRA*distance + krA_adjust;
//
//        simu_y = aver_y + simu_distance ;
//    }
//
//    //以下为计算障碍灯偏差
//    //error_simu = simu_y - 94;  //计算模拟信标灯偏差 （作用未知）
//    error_simu = aver_y - 94;

    if(light_flag == 1) //切左
    {
        simu_y = -2.819*distance+klA_adjust;
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
        simu_y = 2.606*distance-krA_adjust;
        if(simu_y>94)
        {
            simu_y = 94;
        }
        else if(simu_y<30)
        {
            simu_y = 30;
        }
    }

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
}

/******************************* 灭找亮程序（灭完灯后找下一个灯） **********************************/
void FIND_LIGHT()
{
    servodirection = 0;
    servogradual = 0;
    if(roll_time < 333) //旋转次数小于某一值
    {
        if(mode_last == 0 && mode_flag == 1)
        {
            servogradual = 1;
        }
        if(mode_flag == 2)  //结束近灯处于灭灯旋转状态
        {
            if(light_flag == -1)   //切右绕灯
            {
                servodirection = -1;  //切右后向左打死舵机转弯
            }
            else if(light_flag == 1) //切左绕灯
            {
                servodirection = 1;  //切左后向右打死舵机转弯
            }
        }
//        else if(mode_flag == 2)  //初始看不到灯的情况下一律左旋
//        {
//            servodirection = -1;
//        }
    }
//    else  //后期是给光电管准备
//    {
//
//    }
}

//舵机控制

void Servo_assign()
{
    servo_pid=PIDCTRL_Construct(0, 0, 0);
}

void Servo_motorinit()
{
    //SmartCar_Gtm_Pwm_Init();
    //SmartCar_Gtm_Pwm_Init();
    //SmartCar_Gtm_Pwm_Init();
    //SmartCar_Gtm_Pwm_Init();
    //SmartCar_Gtm_Pwm_Init();
}

void Servo_control()
{
    if(servogradual == 1)
    {
        if(light_flag == -1)
        {
            //SmartCar_Gtm_Pwm_Setduty(FTM3,kFTM_Chnl_7,50,7.94+deadgeer*jindengzhan);
        }
        else if(light_flag == 1)
        {
            //SmartCar_Gtm_Pwm_Setduty(FTM3,kFTM_Chnl_7,50,7.94-deadgeer*jindengzhan);
        }
    }
    else if(servodirection == -1)
    {
        //SmartCar_Gtm_Pwm_Setduty(FTM3,kFTM_Chnl_7,50,7.94+deadgeer);
    }
    else if(servodirection == 1)
    {
        //SmartCar_Gtm_Pwm_Setduty(FTM3,kFTM_Chnl_7,50,7.94-deadgeer);
    }
    else
    {
        if(mode_flag == 3)
        {
            servo_pid->kp = servokp*Shun_Dengcut;
            servo_pid->kd = servokd*Shun_Dengcut;
            servo_pulse=PIDCTRL_UpdateAndCalcPID(servo_pid,error_simu);
        }
        else
        {
            servo_pid->kp = servokp;
            servo_pid->kd = servokd;
            servo_pulse=PIDCTRL_UpdateAndCalcPID(servo_pid,error_simu);
        }
        if(servo_pulse>0.8)
        {
            servo_pulse =0.8;
        }
        else if(servo_pulse<-0.8)
        {
            servo_pulse = -0.8;
        }
        //SmartCar_Gtm_Pwm_Setduty(FTM3,kFTM_Chnl_7,50,7.94-servo_pulse);
    }
}

void Motor_control()
{
    if((mode_last == 3 && mode_flag == 0)
            ||(mode_last == 0 && mode_flag == 3)||(mode_last == 1 && mode_flag == 3)||(mode_last == 2 && mode_flag == 3))
    {
        motorstate = 0;
    }
    else if(mode_last == 2 && mode_flag == 3)
    {
        motorstate = -1;
    }
    else if((mode_last == 0 && mode_flag == 1)||mode_flag == 2)
    {
        motorstate = 1;
    }
    Motorpid_control();
}

void Motorpid_control()
{
    if(go_flag) //进入跑车状态
    {
        if(motorstate == 0)
        {
            Ldesiredspeed = straspeedplus;
            Rdesiredspeed = straspeedplus;
            Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
            sundengarea = 0;
            xundengtimemie = 0;
        }
        else if(motorstate == 1)
        {
            xundengtimemie++;
            if(xundengtimemie<xundengtimemie1)
            {
                if(light_flag == -1)
                {
                    Ldesiredspeed = straspeedcut*difference;
                    Rdesiredspeed = straspeedcut;
                    Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
                }
                else if(light_flag == 1)
                {
                    Ldesiredspeed = straspeedcut;
                    Rdesiredspeed = straspeedcut*difference;
                    Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
                }
            }
            else
            {
                Ldesiredspeed = straspeedplus;
                Rdesiredspeed = straspeedplus;
                Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
                sundengarea = 0;
            }
            if(xundengtimemie>1000)
            {
                xundengtimemie = 1000;
            }
            sundengarea = 0;
        }
        else if(motorstate == -1)
        {
            sundengarea++;
            if(light_flag == -1&&sundengarea<xundengtime)
            {
                Ldesiredspeed = straspeedplus*differences;
                Rdesiredspeed = straspeedplus;
                Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
            }
            else if(light_flag == 1&&sundengarea<xundengtime)
            {
                Ldesiredspeed = straspeedplus;
                Rdesiredspeed = straspeedplus*differences;
                Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
            }
            else
            {
                Ldesiredspeed = straspeedplus;
                Rdesiredspeed = straspeedplus;
                Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
            }
            xundengtimemie = 0;
        }
    }
    else if(go_flag == 0)//要停车了
    {
//        SmartCar_Gtm_Pwm_Setduty(FTM0,kFTM_Chnl_0,20000,0);
//        SmartCar_Gtm_Pwm_Setduty(FTM0,kFTM_Chnl_1,20000,0);
//        SmartCar_Gtm_Pwm_Setduty(FTM0,kFTM_Chnl_2,20000,0);
//        SmartCar_Gtm_Pwm_Setduty(FTM0,kFTM_Chnl_3,20000,0);
    }
}

float Motorbpi(pid *pidm, float _err)
{
    pidm->ekL = pidm->ek;
    pidm->ek = _err;
    pidm->pulse = pidm->Kp*(pidm->ek-pidm->ekL) + pidm->Ki*pidm->ek;
    return pidm->pulse;
}

void Motorbpi_Get(float Ldesired,float Rdesired)
{
    //leftFTM = SmartCar_Encoder_Get();
    //rightFTM = -SmartCar_Encoder_Get();
    //SmartCar_Encoder_Clear();
    //SmartCar_Encoder_Clear();
    motorpir.Kp = motorbkp_R;
    motorpir.Ki = motorbki_R;
    motorpil.Kp = motorbkp_L;
    motorpil.Ki = motorbki_L;
    motor_lefdevn += Motorbpi(&motorpil,Ldesired-leftFTM);
    motor_rigdevn += Motorbpi(&motorpir,Rdesired-rightFTM);
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
    if(motor_rigdevn>90){motor_rigdevn=90;}
    if(motor_lefdevn>90){motor_lefdevn=90;}
    if(N_motor_rigdevn>80){N_motor_rigdevn=80;}
    if(N_motor_lefdevn>80){N_motor_lefdevn=80;}
//    SmartCar_Gtm_Pwm_Setduty(FTM0,kFTM_Chnl_0,20000,N_motor_rigdevn);
//    SmartCar_Gtm_Pwm_Setduty(FTM0,kFTM_Chnl_1,20000,motor_lefdevn);
//    SmartCar_Gtm_Pwm_Setduty(FTM0,kFTM_Chnl_2,20000,motor_rigdevn);
//    SmartCar_Gtm_Pwm_Setduty(FTM0,kFTM_Chnl_3,20000,N_motor_lefdevn);
}

/***************************** 停车判断处理程序 *******************************/
void STOP_JUDGE()
{
    if(areaA_time == 0)
        roll_time = 0;
    else
        roll_time ++;
    if(roll_time > 666)  //设为0则会长时间无灯停车
        go_flag = 0;
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
//
//
//
//
//
//陀螺仪控制

/* ******************** 滤波器 ******************** */

/**
 * @brief   IMU传感器原始数据
 * @ {
 */
//float ctrl_accl[3] = {0.0f, 0.0f, 0.0f};
//float ctrl_gyro[3] = {0.0f, 0.0f, 0.0f};
//float ctrl_filterAngAccl = 0.0f, ctrl_filterAngGyro = 0.0f; ///< 由加速度计、角速度计测出的角度
//float ctrl_acclRefAxis; //= ctrl_accl[2]; //FIXME
//float ctrl_gyroRefAxis; //= ctrl_gyro[1]; //FIXME
//
//float ctrl_filterCompTgReciprocal = 1.0f / 0.8f;    ///< Tg的倒数。除以Tg -> 乘Tg的倒数，优化运算速度。

/**
 * @brief   角度滤波相关变量。
 * @note    借用PIDCTRL的结构体保存当前值、积分值、微分值、历史值。
 * @ {
 */
//pidCtrl_t ctrl_angFilter =
//{
//    .kp = 0.0f, .ki = 0.0f, .kd = 0.0f,
//    .errCurr = 0.0f, .errIntg = 0.0f, .errDiff = 0.0f, .errPrev = 0.0f,
//};
//float &ctrl_filterAngle = ctrl_angFilter.errCurr; ///< 滤波后的当前角度。引用ctrl_angFilter.errCurr即可
//pid ctrl_angFilter;
//float ctrl_filterAngRef = 0.0f; ///< 角度参考点，初始化时读取。初始化时必须保持静止！
//
//void CTRL_FilterInit()
//{
//    const uint32 sampleTime = 1024;
//    float intergration = 0.0f;
//    for(uint32 i = 0; i < sampleTime; ++i){
//        imu_6050.ReadSensorBlocking();
//        imu_6050.Convert(&ctrl_accl[0], &ctrl_accl[1], &ctrl_accl[2], &ctrl_gyro[0], &ctrl_gyro[1], &ctrl_gyro[2]);
//        CTRL_FilterUpdate(0U);
//        intergration += ctrl_filterAngAccl;
//        SDK_DelayAtLeastUs(1000,CLOCK_GetFreq(kCLOCK_CoreSysClk)); // MPU6050更新速度没那么快，连续读取只会得到相同的值。
//    }
//    ctrl_filterAngRef = intergration / ((float)sampleTime);
//    ctrl_filterAngle = ctrl_filterAngRef;
//    ctrl_filterAngGyro = ctrl_filterAngRef;
//}
//void CTRL_FilterUpdate(uint32_t updateTime_ms)
//{
//    float dT = ((float)updateTime_ms) * 0.001f;
//    /** 对加速度值进行限幅，防止进行asin计算时出错 */
//    float acclAxis = ctrl_acclRefAxis;
//    acclAxis = (acclAxis > CTRL_1G - 0.001) ? (CTRL_1G - 0.001) : acclAxis;
//    acclAxis = (acclAxis < - (CTRL_1G - 0.001)) ? (- (CTRL_1G - 0.001)) : acclAxis;
//
//    ctrl_filterAngAccl = CTRL_RAD2DEG(- CTRL_ASIN(acclAxis / CTRL_1G)); // - is pitch down , + is pitch up
//    ctrl_filterAngGyro -= ctrl_gyroRefAxis * dT;
//    float angleDiff = (ctrl_gyroRefAxis + ((ctrl_filterAngAccl - ctrl_filterAngle) * ctrl_filterCompTgReciprocal)) * dT;
//    PIDCTRL_ErrUpdateByDiff(&ctrl_angFilter, angleDiff);
//    PRINTF("\r\nang:%d,%d,%d\r\n",ctrl_filterAngAccl,ctrl_filterAngGyro,ctrl_filterAngle);
//}

void Mpu_Get()
{
    SmartCar_MPU_Getgyro2(this_mpu);
    send_buf[0] = this_mpu->mpu_rawdata.gyro_x;
    send_buf[1] = this_mpu->mpu_rawdata.gyro_y;
    send_buf[2] = this_mpu->mpu_rawdata.gyro_z;
    //上位机
    SmartCar_VarUpload(send_buf,3);
}

//*
// * beacon_control.c
// *
// *  Created on: 2020��12��20��
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

int speed0A = 60; //(�˵�����)
int speed1A = 60; //���ٵ��ٶ�
int speed2A = 20; //���ٵ��ٶ�
int speed3A = 60; //ת����ٶ�

int speed0B = 60; //(�˵�����)
int speed1B = 60; //���ٵ��ٶ�
int speed2B = 20; //���ٵ��ٶ�
int speed3B = 60; //ת����ٶ�

/**/
int side_flag = 0;
int side_last = 1;

int light_square = 0; //�Ƶ����
int Long_square;
/*MPU���ղ���*/
extern mpu_t this_mpu;
/**/
/*�Զ���һЩ����*/
int servodirection = 0;              //���ƶ��ת��������� ��-1������� ��1���Ҵ�����
pidCtrl_t  servo_pidA; //���PID��ʼ��
pidCtrl_t  servo_pidB; //���PID��ʼ��
float servokpA = 5; //(�˵�����
float servokdA = 14;
float servokpB = 5; //(�˵�����
float servokdB = 14;
float servo_pulse = 0;

float Ldesiredspeed=0;
float Rdesiredspeed=0;

pid rmotorpiA,lmotorpiA;
pid rmotorpiB,lmotorpiB;

float RAmotorkp = 30,RAmotorki = 20,LAmotorkp = 30,LAmotorki = 20; //(�˵���)
float RBmotorkp = 30,RBmotorki = 20,LBmotorkp = 30,LBmotorki = 20; //(�˵���)

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
//����Ϊ�����־λ
int mode_flag = 0;  //Ѱ��״̬��־λ��0ֱ��׷�ƣ�1���ƣ�2�����ת��3������˳��->�ع�0
//����Ϊͼ������ر���
int distance = 0;   //�ű����ο�����루��ʾ�ű�ƺͳ����룩
int error_simu = 0; //ģ���ű��ƫ��
int connect_max = 3; //û����ͨ��Ĵ������ڸ�ֵʱ��Ϊ���������ת״̬
int roll_time;
int mode_last = 2;    //��һ��״̬��־λ
int go_flag = 1;    //�����ܳ������־λ
/*ǰ����ͷ*/
int aver_x = 0, aver_y = 0;
int averbA_x = 100,averbA_y = 120;  //����ͷ�е����꣨�������ͷ�����е��������Ҫ�ģ�
int averA_x = 100,averA_y = 88;     //����ͷ�е�����(��������ͷ�е����꣬�����б���ô�)
int last_averA_x = 100, last_averA_y = 120;   //��һ�κ���(ǰ�ڲ��ú��⣬���������Ҫ���и��ģ����ݿ϶���׼)�е�����
/*������ͷ*/
int averbB_x = 88,averbB_y = 70;  //����ͷ�е����꣨�������ͷ�����е��������Ҫ�ģ�
int averB_x = 88,averB_y = 70;     //����ͷ�е�����(��������ͷ�е����꣬�����б���ô�)
int last_averB_x = 100, last_averB_y = 70;   //��һ�κ���(ǰ�ڲ��ú��⣬���������Ҫ���и��ģ����ݿ϶���׼)�е�����
/*ǰ�����ĵ�*/
int originA_x = 145;  //ģ��ԭ������
int originA_y = 87;   //ģ��Բ��������
/*�������ĵ�*/
int originB_x = 145;  //ģ��ԭ������
int originB_y = 87;   //ģ��Բ��������
int error_dx = 0,error_dy = 0;           //ʵ�ʵƺͲο�����ݲ�ֵ
int areaA_time = 0;  //��ͨ��Ϊ0������־λ
int areaB_time = 0;  //��ͨ��Ϊ0������־λ
//δ���ࣨ�ɷ���beacon_control.c�ļ��
int mode_time = 0;    //��¼��״̬��������

extern uint8 connectnum;//��ͨ����
extern uint8 connectnumB;//��ͨ����
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

float fk1 = 0.75; //һ���ͺ�����˲���ϵ��-��һ�ε���(�Ų˵���)
float fk2 = 0.8;//�ڶ��ε���(��)

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
//����������
float Q_angle = 0.001;
float Q_gyro  = 0.005;
float R_angle = 0.5;
float dt      = 0.002;//dtΪkalman�˲�������ʱ��
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

int Camera = 0;    //����ͷ����ģʽѡ��(�˵�����) 0Ϊ˫��,1 2�ֱ�Ϊ����,3 4�ֱ�Ϊ˫�㵥ͷ(˫����ָ����������ͷ��,�����ܶ�����,��������,һ������ͷ��,˫�㵥ͷ��ָ��ǰ�������ͷ��,��һ������ͷ����ת��)
int camera_last,camera_flag;
int shifttime = 3; //(�˵���)
int shift_time;
int Blimit = 35; //(�˵���) //������ʱ����
int Alimit = 35; //(�˵���)
int errormaxA = 36; //(��)
int errormaxB = 40;//(��)
int back_flag = 0;  //��Ұ���ű��λ��

int lowlimit0A = 40;
int lowlimit1A = 50;
int lowlimit2A = 53;
int lowlimit3A = 60;
int lowlimit4A = 60;

int lowlimit0B = 26; //(��)
int lowlimit1B = 32;
int lowlimit2B = 39;
int lowlimit3B = 45;
int lowlimit4B = 60;
int speedmarkA = 0; //0:���� 1:���� 2:����
int speedmarkB = 0; //0:���� 1:���� 2:����

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
    /*��������*/
//    MPU6050get();
}


//������ͨ��ʼ���ڸ�������area_flag�������ʱ����������������
void CAMERA_JUDGE()
{
    averbA_x = lightx;
    averbA_y = lighty;
    averbB_x = lightx2;
    averbB_y = lighty2;
    if(connectnum ==0)                                        //���û���ҵ���ͨ�򣬼�¼��ͨ��Ϊ�����
    {
        areaA_time++;                        //��ͨ��Ϊ�������һ
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
         distance =  lightdis;//CarmSqrt((float)(error_dx*error_dx + error_dy*error_dy));   //����ƾ�
         speedever = (leftFTM+rightFTM)/20;
         /*�ٶȷֶ�*/
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



/***************************** ״̬�жϴ������ *******************************/
void MODE_JUDGE()
{
    /*״̬��*/
    if(0 == mode_flag&&((camera_flag ==0&&distance<dislimitA)||speedmarkA == 2))
    {
        mode_last = mode_flag;
        mode_flag = 1;
        mode_time = 0;
        back_flag = 0;
    }
    if(1 == mode_flag&&((camera_flag ==0&&distance>(dislimitA+18))||speedmarkA ==0||speedmarkA == 1)) //�Ʊ�������,Զ�����е�,�ж������ξ����,���Ҵ��ڴ���ֵ��ת��״̬
    {
        mode_last = mode_flag;
        mode_flag = 0;
        mode_time = 0;
        back_flag = 0;
    }
//    if((1 == mode_flag)&&battery<balowlimit) //���ڽ���״̬,���ҵ�ѹ����
//    {
//        mode_last = mode_flag;
//        mode_flag = 3;
//        mode_time = 0;
//    }
//    if(3 == mode_flag&&(battery>bauplimit)&&((camera_flag ==0&&areaA_time>connect_max))) //�޵��ҳ����
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
    mode_time++; //mode_time������1��������(��¼��һ��״̬����ʱ��)

    //�޵�ʱ��ѡ���л�����ͷ,������(��ʼcamera_flag����Ҫ����,���Լ�����)
    if(mode_flag == 2)
    {
        if(back_flag == 0)
        {
            if(areaB_time == 0)
            {
                back_flag = -1;//��ת
            }
            else if(areaB_time>0)
            {
                back_flag = 1;//��ת
            }
        }
    }
    else if(mode_flag == 0||mode_flag == 1)
    {
        back_flag = 0;
    }
}

/***************************** ����ƫ����Ƴ��� *******************************/
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
        /*�����ռ�ձ���ֵ��Ҫ���²�*/
//        SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,333,5250);
//��
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,20000,0);
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,20000,0); //��ռ�ձ���ת
//��
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,20000,0); //����ռ�ձ�ת
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
            //���ֳ�˳��״̬��׷��״̬����һ��������Ʒ��� //Ҳ��Ҫ�Ż�����,���������ٶȹҹ�
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
            if(mode_flag == 0||mode_flag == 1) // ����ƺͳ��״̬������״̬�������ļ�����
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
                if(servo_pulse > 125) //��
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
                else if(servo_pulse<-125) //��
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
            else if(mode_flag == 2)//��������
            {
                Ldesiredspeed = speed3A;
                Rdesiredspeed = speed3A;
                if(servodirection == 1) //��ת
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
            if(mode_flag == 0||mode_flag == 1) // ����ƺͳ��״̬������״̬�������ļ�����
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
                if(servo_pulse > 125) //��
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
                else if(servo_pulse<-125) //��
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
            else if(mode_flag == 2)//��������
            {
                Ldesiredspeed = speed3B;
                Rdesiredspeed = speed3B;
                if(servodirection == 1) //��ת
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
        if(go_flag) //�����ܳ�״̬
        {
//��
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,20000,(uint32)NFlefdevn);
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,20000,(uint32)Flefdevn); //��ռ�ձ���ת
//��
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,20000,(uint32)Frigdevn); //����ռ�ձ�ת
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_2_TOUT55_P21_4_OUT,20000,(uint32)NFrigdevn);
        }
        if(go_flag == 0)//Ҫͣ����
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


/***************************** ͣ���жϴ������ *******************************/
void STOP_JUDGE()
{
    if((camera_flag == 0 &&areaA_time == 0)||(camera_flag == 1 && areaB_time == 0)||mode_flag == 3)
        roll_time = 0;
    else{
        roll_time ++;
    }
    if(roll_time > 1000)  //��Ϊ0��᳤ʱ���޵�ͣ��
    {
        go_flag = 0;
        roll_time = 1000;
    }
}


//��ƽ����
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


/*��1���еƻ����Խ����뻻��x���һ�����ݣ���2��������ֵ-ƽ�ƾ���*/


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
    //�ü��ٶȼ����������ˮƽ������ϵ֮��ļн�
//    Angle_x_temp=(atan(ay/az))*180/3.14;
//    Angle_y_temp=(atan(ax/az))*180/3.14;
//    if(ax*ay*az == 0)
//          return;
//    else
//    {
//            Kalman_Filter_Y(Angle_y_temp,gyro[1]);  //�������˲�����Y���
//        Kalman_Filter_X(Angle_x_temp,gyro[0]);  //�������˲�����X���
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


void Kalman_Filter_X(float Accel,float Gyro) //����������
{
    Angle_X_Final += (Gyro - Q_bias) * dt; //�������

    Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

    Pdot[1]= -PP[1][1];
    Pdot[2]= -PP[1][1];
    Pdot[3]= Q_gyro;

    PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
    PP[0][1] += Pdot[1] * dt;   // =����������Э����
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    Angle_err = Accel - Angle_X_Final;  //zk-�������
    Angle_X_Final += K_0 * Angle_err;    //�������
    Q_bias        += K_1 * Angle_err;    //�������
    gyronow[0]         = Gyro - Q_bias;  //���ֵ��������ƣ���΢�� = ���ٶ�

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0;       //����������Э����
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;


}

void Kalman_Filter_Y(float Accel,float Gyro) //����������
{
    Angle_Y_Final += (Gyro - Q_bias) * dt; //�������

    Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

    Pdot[1]=- PP[1][1];
    Pdot[2]=- PP[1][1];
    Pdot[3]=Q_gyro;

    PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
    PP[0][1] += Pdot[1] * dt;   // =����������Э����
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;


    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    Angle_err = Accel - Angle_Y_Final;  //zk-�������

    Angle_Y_Final   += K_0 * Angle_err;  //�������
    Q_bias  += K_1 * Angle_err;  //�������
    gyronow[1]   = Gyro - Q_bias;    //���ֵ��������ƣ���΢�� = ���ٶ�

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0;       //����������Э����
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
        Ior += ADC_Get(ADC_1,ADC1_CH4_A20,ADC_8BIT);//��������Ҫ���ݵ����������޸�
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
        I_r += ADC_Get(ADC_1,ADC1_CH4_A20,ADC_8BIT);//����Ҫ�޸�
        I_l += ADC_Get(ADC_1,ADC1_CH4_A20,ADC_8BIT);
    }
    I_r = I_r/10 - Ior;
    I_l = I_l/10 - Iol;
    //�����˲���
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
    //��
                SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,20000,(uint32)N_motor_lefdevn);
                SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,20000,(uint32)motor_lefdevn); //��ռ�ձ���ת
    //��
                SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,20000,(uint32)motor_rigdevn); //����ռ�ձ�ת
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
//    Rctrl.b = 250; //������Ӧ��,��Ҫ����������
//    Rctrl.kp = 50;   //̫С��Ӧ����,̫���г���
//    Rctrl.kd = 0;
//    Rctrl.w0 = 90;    //��������w0�޷���Ӧ,��С������С����Ӧ�ٶ���
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



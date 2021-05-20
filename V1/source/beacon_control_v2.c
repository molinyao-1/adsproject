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

#define deadgeerleft 500
#define deadgeerright 550


int angle_time = 0;
extern int gogoflag;

float init_ax = 0,init_ay = 0,init_az = 0;
float acc_angle_x = 0,gyro_angle_x = 0;    //һ�׻����Ĳ�������
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

int L_time = 0; //�����ܼ���
int R_time = 0;

int slowline ;//����������
float slowladjust = 0.7; //(�˵�����)
uint32_t servofinal;

int speed0 = 80; //(�˵�����)
int speed1 = 80; //���ٵ��ٶ�
int speed2 = 80; //���ٵ��ٶ�
int speed3 = 80; //ת����ٶ�

//(�˵���)
int mode0limit = 10;
int correctlimit = 15;
int LOWplus = 15;
int reshunlimit = 40;
/**/

int side_flag = 0;
int i = 0;
int red_flag = 0; //����״̬�л���־λ
int light_last = 0;
int red_out_flag = 0;//�Ƿ񴥷����ܱ�־λ
int light_square = 0; //�Ƶ����
int squarebA = 0; //�Ƶ����
int last_squarebA = 0; //�Ƶ����
int squareA = 0; //�Ƶ����
int squarelimit = 0;

int Long_square;


/*MPU���ղ���*/
float send_buf[3] = {0};
extern mpu_t this_mpu;
/**/

/*����Ϊ�е�ϵ��*/
int KLA = 1.055;
int KRA = -1.144;
/**/

/*�Զ���һЩ����*/
int servodirection = 0;              //���ƶ��ת��������� ��-1������� ��1���Ҵ�����
pidCtrl_t  servo_pid; //���PID��ʼ��
float servokp = 1.3; //(�˵�����
float servokd = 2.5;
float servo_pulse = 0;

int motorstate = 0;  //(�������״̬��Ļ����Ƕ�ά����)0:���� ��1������
float Ldesiredspeed=0;
float Rdesiredspeed=0;
pid motorpir,motorpil;
float motorbkp_R = 0,motorbki_R = 0,motorbkp_L = 0,motorbki_L = 0; //(�˵���)
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

/*������֪���õı���*/

//����Ϊ�����־λ
int mode_flag = 2;  //Ѱ��״̬��־λ��0ֱ��׷�ƣ�1���ƣ�2�����ת��3������˳��->�ع�0
//����Ϊͼ������ر���
int distance = 0;   //�ű����ο�����루��ʾ�ű�ƺͳ����룩
int dislimit = 90;
int lastdistance = 0;
int errordis = 50;

int error_simu = 0; //ģ���ű��ƫ��
float simu_distance = 0;                  //ģ������ʱ�õľ����м����
int  simu_y = 161;          //ģ���ű������

int connect_max = 4; //û����ͨ��Ĵ������ڸ�ֵʱ��Ϊ���������ת״̬
//����Ϊ�ٶȿ�����ر���
int roll_time;                      //��¼��ת��������

//δ���ࣨ�ɷ���beacon_control.c�ļ��
int correct_time = 0; //˳��ʱ��
int mode_last = 2;    //��һ��״̬��־λ
int LOW = 50;    //����ʹ�õ�����ͷ���� (Ӧ�ú��ڿ����޸ģ�����С����һֵ�ͽ������״̬)
/**/

/*����δ֪���õı���*/

//����Ϊ�����־λ
int hit_flag = 0;
int light_flag = 0; //����ǰ�Ʊ�־λ
int back_flag = 0;  //��Ұ���ű��λ��
int go_flag = 1;    //�����ܳ������־λ
int light_change_flag = 0;    //���л��еƷ����־λ

//����Ϊͼ������ر���
int aver_x = 0, aver_y = 0;        //�����е�����
int averbA_x = 100,averbA_y = 120;  //����ͷ�е����꣨�������ͷ�����е��������Ҫ�ģ�
int averA_x = 100,averA_y = 120;     //����ͷ�е�����(��������ͷ�е����꣬�����б���ô�)
int last_averA_x = 100, last_averA_y = 120;   //��һ�κ���(ǰ�ڲ��ú��⣬���������Ҫ���и��ģ����ݿ϶���׼)�е�����
int origin_x = 145;  //ģ��ԭ������
int origin_y = 87;   //ģ��Բ��������
int error_dx = 0,error_dy = 0;           //ʵ�ʵƺͲο�����ݲ�ֵ

int long_flag = 0;    //׷Զ�Ʊ�־λ

int contflag = 0;


int areaA_time = 0;  //��ͨ��Ϊ0������־λ
//δ���ࣨ�ɷ���beacon_control.c�ļ��
int mode_time = 0;    //��¼��״̬��������
int Avoid_Low = 0;    //������С����

float servo_pulsem;
float smcoup = 0.25;

//mmc
extern uint8 connectnum;//��ͨ����
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

/*�¼Ӳ˵�������*/
int lowlimit0 = 20;
int lowlimit1 = 26;
int lowlimit2 = 34;
int lowlimit3 = 40;
int lowlimit4 = 60;
int speedmark = 0; //0:���� 1:���� 2:����
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


//������ͨ��ʼ���ڸ�������area_flag�������ʱ����������������
void CAMERA_JUDGE()
{
    averbA_x = lightx;
    averbA_y = lighty;
    squarebA = (int)lights;
    if(connectnum ==0)                                        //���û���ҵ���ͨ�򣬼�¼��ͨ��Ϊ�����
    {
        areaA_time++;                        //��ͨ��Ϊ�������һ
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


/***************************** ״̬�жϴ������ *******************************/
void MODE_JUDGE()
{
    lastdistance = distance;
    error_dx = aver_x - origin_x;
    error_dy = aver_y - origin_y;
    distance = CarmSqrt(error_dx*error_dx + error_dy*error_dy);   //����ƾ�
    speedever = (leftFTM+rightFTM)/20;

    /*�ٶȷֶ�*/
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

    /*״̬��*/
    if(mode_flag == 0&&distance<dislimit)  //����״̬
    {
        mode_last = mode_flag;
        mode_flag = 1; //����Ϊ����״̬
        mode_time = 0;
    }

    if(1 == mode_flag&&abs(distance-lastdistance)>errordis) //�Ʊ�������,Զ�����е�,�ж������ξ����,���Ҵ��ڴ���ֵ��ת��״̬
    {
        mode_last = mode_flag;
        mode_flag = 0;
        mode_time = 0;
    }
    if(1 == mode_flag&&battery<balowlimit) //���ڽ���״̬,���ҵ�ѹ����
    {
        mode_last = mode_flag;
        mode_flag = 3;
        mode_time = 0;
    }
    if(3 == mode_flag&&(battery>bauplimit)&&(areaA_time>connect_max)) //�޵��ҳ����
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

    mode_time++; //mode_time������1��������(��¼��һ��״̬����ʱ��)
}


/***************************** ����ƫ����Ƴ��� *******************************/
void ERROR()
{
    if(0 == mode_flag||1 == mode_flag)
    {
        error_simu = aver_y - 88;
    }
}
/***************************** ����жϴ������ *******************************/
void SIDE_JUDGE()    //��Ϊû�й��ܣ�ǰ���Ȳ���
{//side_flag �ڳ�����Ϊ-1����Ϊ1

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
/******************************* ��������������ƺ�����һ���ƣ� **********************************/
void FIND_LIGHT()
{
    if(mode_flag !=2)
    {
        servodirection = 0;
    }
    else if(mode_flag == 2)  //�������ƴ��������ת״̬
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
//            servodirection = 1; //(һ����ת)
//        }
        servodirection = 1; //(һ����ת)
    }
}

//�������

void Servo_motorinit()
{
        /*�����ռ�ձ���ֵ��Ҫ���²�*/
//        SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,333,6225);
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,20000,0);
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_3_TOUT56_P21_5_OUT,20000,0); //����ռ�ձ�ת

        /*��*/
        if(1)
            SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM0_1_TOUT54_P21_3_OUT,20000,0); //��ռ�ձ���ת
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
            //���ֳ�˳��״̬��׷��״̬����һ��������Ʒ��� //Ҳ��Ҫ�Ż�����,���������ٶȹҹ�
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

        Imudataprocess();//��ȡ����ǰ��ˮƽ��н�
        //motorI_get();
        if(mode_flag == 0||mode_flag == 1) // ����ƺͳ��״̬������״̬�������ļ�����
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
            if(servo_pulse > 125) //��
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
            Ldesiredspeed = speed3;
            Rdesiredspeed = speed3;
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


            if(go_flag) //�����ܳ�״̬
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
        if(go_flag == 0)//Ҫͣ����
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

/***************************** ͣ���жϴ������ *******************************/
void STOP_JUDGE()
{
    if(areaA_time == 0||mode_flag == 3)
        roll_time = 0;
    else{
        roll_time ++;
    }
    if(roll_time > 5000)  //��Ϊ0��᳤ʱ���޵�ͣ��
    {
        go_flag = 0;
        roll_time = 5000;
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

    //ת��ʱ�������ǿ�ȼ���
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


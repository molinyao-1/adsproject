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
#define gyrooffset2 -8.5

static float dt = 0.005;

float one_filter_angle = 0;

float pulseleft;
float pulseright;

int L_time = 0; //�����ܼ���
int R_time = 0;

int slowline ;//����������
float slowladjust = 0.7; //(�˵�����)
uint32_t servofinal;
int speedmostfast = 80; //(�˵�����)
int correctxishu; //����ϵ��,�������ֱ������ƽ��
float correctmenu = 0.2; //�˵���  (������ǰ,��С)

float correctspeed = 1;

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
int slowspeedwant = 80; //���ٵ��ٶ�
float deadgeer = 80;


int motorstate = 0;  //(�������״̬��Ļ����Ƕ�ά����)0:���� ��1������
float Ldesiredspeed=0;
float Rdesiredspeed=0;
pid motorpir,motorpil;
float motorbkp_R = 50,motorbki_R = 30,motorbkp_L = 50,motorbki_L = 30; //(�˵���)
int16 leftFTM;
int16 rightFTM;
float motor_lefdevn =0;
float motor_rigdevn =0;
float N_motor_lefdevn=0,N_motor_rigdevn=0;

uint32_t motor_lefdevnU =0;
uint32_t motor_rigdevnU =0;
uint32_t N_motor_lefdevnU=0,N_motor_rigdevnU=0;
/**/

/*������֪���õı���*/

//����Ϊ�����־λ
int mode_flag = 0;  //Ѱ��״̬��־λ��0ֱ��׷�ƣ�1���ƣ�2�����ת��3������˳��->�ع�0
//����Ϊͼ������ر���
int distance = 0;   //�ű����ο�����루��ʾ�ű�ƺͳ����룩
int error_simu = 0; //ģ���ű��ƫ��
int obst_x = 121,obst_y = 161; //�ϰ�������
int obstacleA_x = 121,obstacleA_y = 161;  //�ϰ�������
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
int origin_x = 100;  //ԭ������
int origin_y = 94;   //Բ��������
int error_dx = 0,error_dy = 0;           //ʵ�ʵƺͲο�����ݲ�ֵ

int long_flag = 0;    //׷Զ�Ʊ�־λ

int contflag = 0;

/**/

//int *speedA_M = &date[2][2];        //����Ϊ�ĸ��������������ٶ�
//int *speedA_H = &date[2][3];
//int *klA_adjust = &date[1][3];     //ƫ��������ϵ��(����ģ������ʱ���������ϵ��)
//int *krA_adjust = &date[1][4];
float klA_adjust = 72;
float krA_adjust = 94;

int areaA_time = 0;  //��ͨ��Ϊ0������־λ
//δ���ࣨ�ɷ���beacon_control.c�ļ��
int mode_time = 0;    //��¼��״̬��������
int Avoid_Low = 0;    //������С����

float servo_pulsem;

//mmc
extern uint8 connectnum;//��ͨ����
extern float lightx, lighty, lights, lightc;
extern float lightdis;

int servoduty;

float servocorrect;
extern int servoinit;
int flagservo = 1;
int lightsensor[2] = {1};
int lightsenlast[2] = {1};

void Encoder_init()
{
    SmartCar_Encoder_Init(GPT12_T4,IfxGpt120_T4INA_P02_8_IN,IfxGpt120_T4EUDA_P00_9_IN);  //Left
    SmartCar_Encoder_Init(GPT12_T6,IfxGpt120_T6INA_P20_3_IN,IfxGpt120_T6EUDA_P20_0_IN);  //right
}

void FINAL_CONTROL()
{
    MODE_JUDGE();
    STOP_JUDGE();
    SIDE_JUDGE();
    ERROR();
    FIND_LIGHT();
    BatterySCM_Get();
}


//������ͨ��ʼ���ڸ�������area_flag�������ʱ����������������
void CAMERA_JUDGE()
{
    averbA_x = lightx;
    averbA_y = lighty;
    squarebA = lights;
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
    LOW = (int)(120-(leftFTM + rightFTM)*correctmenu);
    if(LOW<25)
        LOW = 25;
    else if(LOW>60)
        LOW = 60;
    if(mode_flag == 3 && fabs(error_simu) < mode0limit ) //������˳��������׷��״̬���ж��������źŵ�ƫ����С��ĳһ��ֵʱ����׷�ƣ���ֵ10�ݶ������Ը���
    {
        correct_time++;
        if(correct_time>correctlimit) //˳������ʱ���ݶ�Ϊ21��
        {
            mode_last = mode_flag; //�洢��һ�ε�״̬
            mode_flag = 0; //����Ϊ׷��״̬
            correct_time = 0;
            mode_time = 0; //��¼��״̬����ʱ��ʱ�Ƚ�������������������ã���������
        }
    }
  //  if(mode_flag == 0 && distance <= LOW)  //����״̬���п��ܲ����� || mode_flag == 1  && fabs(error_adjust) < 20
    if(mode_flag == 0 && aver_x >= LOW)  //����״̬���п��ܲ����� || mode_flag == 1  && fabs(error_adjust) < 20
    {
        mode_last = mode_flag;
        mode_flag = 1; //����Ϊ����״̬
        mode_time = 0;
    }
    //�����ת״̬�ж�(���۳�����ʲô״̬��ֻҪ���������ˣ��ͽ��������ת״̬)
    if(mode_flag != 2 && (areaA_time > connect_max))
    {
        mode_last = mode_flag;
        mode_flag = 2; //����Ϊ�����ת״̬״̬
        mode_time = 0;

        if(light_flag != 0)
          light_last = light_flag;   //��������

    }
    if(mode_flag == 2 &&  areaA_time == 0)
    {
        mode_last = mode_flag;
        mode_flag = 3;
        mode_time = 0;
        back_flag = 0; //�������û���̫�����������˳��״̬�²���Ҫ��Ұ��ĵƸ��ţ�
    }
//    else if(mode_flag == 1 && distance > LOW + 15)    //�������Ұ�ڻ��еƣ�+3��Ϊ�˷�ֹ���֮ǰ��Ϊ˳�� ��ʱҲ����˳��״̬
    else if(mode_flag == 1 && aver_x < LOWplus)    //�������Ұ�ڻ��еƣ�+3��Ϊ�˷�ֹ���֮ǰ��Ϊ˳�� ��ʱҲ����˳��״̬

    {
      mode_last = mode_flag;
      mode_flag = 3;
      mode_time = 0;
      back_flag = 0; //
    }
    else if(mode_flag == 0 && abs((int)error_simu) > reshunlimit) //׷��״̬���ڱ���ƫ��ϴ�ʱ������˳��״̬
    {
      mode_last = mode_flag;
      mode_flag = 3;
      mode_time = 0;
      back_flag = 0;
    }
    mode_time++; //mode_time������1��������(��¼��һ��״̬����ʱ��)
    if(mode_flag != 3 && correct_time != 0)     //�еƵ��������˳������λ��˳��������ͻȻ�����ˣ���Ҫ�������ʱ��־λ��
        correct_time = 0;

    /*ѧ�����룬˳����������Ҫ��GPIO���в���
    if(mode_flag==3)
        GPIO_PortSet(GPIOE, 1U << 12U);
    else
        GPIO_PortClear(GPIOE, 1U << 12U);
    */
}


/***************************** �ϰ��жϴ������ *******************************/
void OBSTACLE_JUDGE()
{
    static int error_obst_dx = 0,error_obst_dy = 0; //�ƾ��м����

    }


/***************************** ����ƫ����Ƴ��� *******************************/
void ERROR()
{
    static int error_aver = 0;
    error_aver = aver_y - 94;  //y�����뾵ͷĳһλ�õ�ƫ��ú�˼�������壩
    if((mode_flag == 3)|| (mode_flag == 0)/*����׷��״̬���ܳ���־λΪ1*//* || light_change_flag == 1/*���л��еƷ���Ϊ1*/)
    {  //ȷ�����������ң���������������
//        if(light_change_flag == 1)   //��ʱ��״̬ת����������������������������������(�������û���Ҫ˼��)
//        {  //�ڱ��Ϲ����з������ã�������������������û���ã�
//            if(abs(error_aver) > 10)    //��ʱת��״̬��ƫ��ϴ�����л��еƷ���()
//            {
//                if(error_aver > 0)
//                    light_flag = 1; //light_flagӦ�����еƷ����һ����־λ
//                else
//                    light_flag = -1;
//            }
//        }
//        else
//        {
            if(error_aver>0)
                light_flag = 1;
            else
                light_flag = -1;
     }
//        light_change_flag = 0;
//    error_dx = aver_x - origin_x;
//    error_dy = aver_y - origin_y;
//    distance = CarmSqrt(error_dx*error_dx + error_dy*error_dy);   //����ƾ�

    if(mode_flag == 0 && mode_time == 1)                                          //����׷��״̬�жϾ���
    {
        if(light_square <= Long_square ) //����źŵƵ����С��ĳһ��ֵʱ����Ϊ���Ǵ���׷��״̬������ǰ��
           long_flag = 1;  //׷Զ��
        else
           long_flag = 0;
    }
    if(light_flag == 1) //����
    {
        simu_y = KLA*(aver_x)+klA_adjust; //���simu_y��aver_x�Ĺ�ϵ
        if(simu_y<94)
        {
            simu_y = 94;
        }
        else if(simu_y>150)
        {
            simu_y = 150;
        }
    }
    else if(light_flag == -1)  //����
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
//    if(mode_flag == 0 && mode_time == 1)                                          //����׷��״̬�жϾ���
//    {
//        if(light_square <= Long_square ) //����źŵƵ����С��ĳһ��ֵʱ����Ϊ���Ǵ���׷��״̬������ǰ��
//           long_flag = 1;  //׷Զ��
//        else
//           long_flag = 0;
//    }
//    if(light_flag == 1) //����
//    {
//        simu_y = KLA*(aver_x-correctxishu)+klA_adjust; //���simu_y��aver_x�Ĺ�ϵ
//        if(simu_y<94)
//        {
//            simu_y = 94;
//        }
//        else if(simu_y>150)
//        {
//            simu_y = 150;
//        }
//    }
//    else if(light_flag == -1)  //����
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

    //����Ϊ�����ϰ���ƫ��
//    if(mode_flag == 3)
//    {
//        error_simu = aver_y - 94;  //����ģ���ű��ƫ�� ������δ֪��
//    }
//    else
//    {
        error_simu = aver_y - simu_y;
//    }
}
/***************************** ����жϴ������ *******************************/
void SIDE_JUDGE()    //��Ϊû�й��ܣ�ǰ���Ȳ���
{//side_flag �ڳ�����Ϊ-1����Ϊ1
    lightsenlast[0] = lightsensor[0];
    lightsensor[0]= GPIO_Read(P11,10);
    lightsenlast[1] = lightsensor[1];
    lightsensor[1]= GPIO_Read(P20,9);

    if(mode_flag == 2)
    {
        Eru_Enable(CH0_P15_4);
        Eru_Enable(CH4_P33_7);
//        Eru_Enable(CH7_P20_9);
        if((lightsenlast[0] == 1&&lightsensor[0] == 0)||(lightsenlast[1] == 1&&lightsensor[1] == 0))
        {
            L_time++;
        }
        if(R_time>=1)
        {
            side_flag = -1;
            Eru_Disable(CH0_P15_4);
            Eru_Disable(CH4_P33_7);
//            Eru_Disable(CH7_P20_9);
        }

        if(L_time>=1)
        {
            side_flag = 1;
            Eru_Disable(CH0_P15_4);
            Eru_Disable(CH4_P33_7);
//            Eru_Disable(CH7_P20_9);
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
        if(light_flag == 0)
        {
            if(side_flag != 0)
            {
                switch(side_flag){
                    case 1:servodirection = 1;break;
                    case -1:servodirection = -1;break;
                    default:break;
                }
            }
        }
        if((light_flag == -1&&side_flag == -1)||(light_flag == 1&&side_flag == -1))   //���ҵ����ұ�,��������ұ�
        {
            servodirection = -1;  //���Ҵ������ת��
        }
        else if((light_flag == 1&&side_flag == 1)||(light_flag == -1&&side_flag == 1)) //����������,���ҵ������
        {
            servodirection = 1;  //����������ת��
        }
        if(servodirection == 0)
        {
            contflag++;
        }
        else if(servodirection!=0)
        {
            contflag = 0;
        }
        if(contflag == 3)
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
        }
       }
}

void FIND_LIGHT2() //���治��Ҫ����
{
    if(0==mode_flag||3==mode_flag||(1 == mode_flag&&light_square<squarelimit))
    {
    servodirection = 0;
    }
    else if(mode_flag == 2||(1 == mode_flag&&light_square>squarelimit))  //�������ƴ��������ת״̬
    {
        if(light_flag == 0)
        {
            if(side_flag != 0)
            {
                switch(side_flag){
                    case 1:servodirection = 1;break;
                    case -1:servodirection = -1;break;
                    default:break;
                }
            }
        }
        if(light_flag == -1)   //�޹���
        {
            servodirection = 1;  //���Ҵ������ת��
        }
        else if(light_flag == 1) //����������,���ҵ������
        {
            servodirection = -1;  //����������ת��
        }
        if(servodirection == 0)
        {
            contflag++;
        }
        else if(servodirection!=0)
        {
            contflag = 0;
        }
        if(contflag == 3)
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
        }
       }
}

//�������

void Servo_motorinit()
{
        /*�����ռ�ձ���ֵ��Ҫ���²�*/
//        SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,50,570);
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
    if(servoinit == 1&&flagservo == 1)
    {
        SmartCar_Gtm_Pwm_Init(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,50,570);
        flagservo = 0;
    }
    if(servoinit == 1&&flagservo ==0)
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
            //���ֳ�˳��״̬��׷��״̬����һ��������Ʒ��� //Ҳ��Ҫ�Ż�����,���������ٶȹҹ�
            servo_pid.kp = servokp;
            servo_pid.kd = servokd;
            if(mode_flag == 3)
            {
                servo_pid.kp = servokp*servocorrect;
                servo_pid.kd = servokd*servocorrect;
            }
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
            if(gogoflag == 1)
            {
                SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM1_1_TOUT31_P33_9_OUT,(uint32_t)(570-servo_pulse));
            }

        }
    }
}

void Motor_control()
{
    if(gogoflag == 1)
    {
    leftFTM = -SmartCar_Encoder_Get(GPT12_T4);
    rightFTM = SmartCar_Encoder_Get(GPT12_T6);
    SmartCar_Encoder_Clear(GPT12_T4);
    SmartCar_Encoder_Clear(GPT12_T6);

      Imudataprocess();//��ȡ����ǰ��ˮƽ��н�

    if(mode_flag == 0||mode_flag == 1||mode_flag == 3) // �����״̬������״̬�������ļ�����
    {
        slowline = (int)(120-(leftFTM + rightFTM)*correctmenu);
        if(slowline<25)
            LOW = 25;
        else if(slowline>60)
            LOW = 60;
        if((aver_x>slowline))
        {
            Ldesiredspeed = (float)slowspeedwant;
            Rdesiredspeed = (float)slowspeedwant;
        }
        else //���ڼ��������� , �ٶȺͶ������øߴκ����ҹ�
        {
            Ldesiredspeed = (float)(slowspeedwant+speedmostfast*((abs(servo_pulse)-80)*(abs(servo_pulse)-80))/6400);
            Rdesiredspeed = (float)(slowspeedwant+speedmostfast*((abs(servo_pulse)-80)*(abs(servo_pulse)-80))/6400);
//            Ldesiredspeed = (float)slowspeedwant;
//            Rdesiredspeed = (float)slowspeedwant;
        }

        /*NEW ITEM*/
//        if(servo_pulse > 20)
//        {
//            if(servo_pulse>25)
//            {
//                Rdesiredspeed = (Rdesiredspeed)*((0.25*servo_pulse*servo_pulse-4876*servo_pulse+1020000)/1000000*correctspeed);
//
//         //       Ldesiredspeed = (Ldesiredspeed)*((0.000002354*servo_pulse*servo_pulse-0.00485*servo_pulse+0.9837)/correctspeed);
//
//            }
//            else
//            {
//                Rdesiredspeed = (Rdesiredspeed)*((0.25*servo_pulse*servo_pulse-4876*servo_pulse+1020000)/1000000);
//
//
// //               Ldesiredspeed = (Ldesiredspeed)*((0.000002354*servo_pulse*servo_pulse-0.00485*servo_pulse+0.9837));
//            }
//        }
//        else if(servo_pulse<-20) //��
//        {
//            if(servo_pulse<-25)
//            {
//
//                Ldesiredspeed = (Ldesiredspeed)*((0.2354*servo_pulse*servo_pulse+485*servo_pulse+98370)/100000*correctspeed);
//
//
////                Rdesiredspeed = (Rdesiredspeed)*((0.00000025*servo_pulse*servo_pulse+0.004876*servo_pulse+1.02)/correctspeed);
//            }
//            else
//            {
//                Ldesiredspeed = (Ldesiredspeed)*((0.2354*servo_pulse*servo_pulse+485*servo_pulse+98370)/100000);
//
////                Rdesiredspeed = (Rdesiredspeed)*((0.00000025*servo_pulse*servo_pulse+0.004876*servo_pulse+1.02));
//            }
//        }
        /**/
    }
    else if(mode_flag == 2)//��������
    {
        Ldesiredspeed = slowspeedwant;
        Rdesiredspeed = slowspeedwant;
        if(servodirection == 1) //��ת
        {
            if(abs((int)angle_z)<=80)
            {
                Ldesiredspeed =Ldesiredspeed*0.5*(1+(0.2354*deadgeer*deadgeer-485*deadgeer+98370)/100000*correctspeed);
            }
            else
            {
                Ldesiredspeed = Ldesiredspeed/correctspeed;//Ldesiredspeed*0.5*(1+(0.2354*deadgeer*deadgeer-485*deadgeer+98370)/100000);
            }
            Rdesiredspeed = 2*slowspeedwant-Ldesiredspeed;
          //  Ldesiredspeed =Ldesiredspeed*0.5*(1+(0.2354*deadgeer*deadgeer-485*deadgeer+98370)/100000*correctspeed);

        }
        else if(servodirection == -1)
        {
            if(abs((int)angle_z)<=80)
            {
                Rdesiredspeed = (Rdesiredspeed)*0.5*(1+(0.25*deadgeer*deadgeer-4876*deadgeer+1020000)/1000000*correctspeed);
            }
            else
            {
                Rdesiredspeed = Rdesiredspeed/correctspeed;///(Rdesiredspeed)*0.5*(1+(0.25*deadgeer*deadgeer-4876*deadgeer+1020000)/1000000);
            }
            Ldesiredspeed = 2*slowspeedwant-Rdesiredspeed;
        }
        else
        {
            Ldesiredspeed = (float)slowspeedwant;
            Rdesiredspeed = (float)slowspeedwant;
        }
    }
//    if(Ldesiredspeed >120)
//    {
//        Ldesiredspeed = 120;
//    }
//    if(Rdesiredspeed>120)
//    {
//        Rdesiredspeed = 120;
//    }
//    if(Ldesiredspeed<=0)
//    {
//        Ldesiredspeed = 0;
//    }
//    if(Rdesiredspeed<=0)
//    {
//        Rdesiredspeed = 0;
//    }
    //Motorpid_control();
//        Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
        //�����Ǵ��ʱ���С����ٶȵĿ���ǿ��
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

    if(go_flag) //�����ܳ�״̬
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
    else if(go_flag == 0)//Ҫͣ����
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

void Motor_control2()
{
    if(gogoflag == 1)
    {
    leftFTM = -SmartCar_Encoder_Get(GPT12_T4);
    rightFTM = SmartCar_Encoder_Get(GPT12_T6);
    SmartCar_Encoder_Clear(GPT12_T4);
    SmartCar_Encoder_Clear(GPT12_T6);

      Imudataprocess();//��ȡ����ǰ��ˮƽ��н�

    if(mode_flag == 0||(mode_flag == 1&&light_square<squarelimit)||mode_flag == 3) // �����״̬������״̬�������ļ�����
    {
        slowline = (int)(120-(leftFTM + rightFTM)*correctmenu);
        if(slowline<25)
            LOW = 25;
        else if(slowline>60)
            LOW = 60;
        if((aver_x>slowline))
        {
            Ldesiredspeed = (float)slowspeedwant;
            Rdesiredspeed = (float)slowspeedwant;
        }
        else //���ڼ��������� , �ٶȺͶ������øߴκ����ҹ�
        {
            Ldesiredspeed = (float)(slowspeedwant+speedmostfast*((abs(servo_pulse)-80)*(abs(servo_pulse)-80))/6400);
            Rdesiredspeed = (float)(slowspeedwant+speedmostfast*((abs(servo_pulse)-80)*(abs(servo_pulse)-80))/6400);
//            Ldesiredspeed = (float)slowspeedwant;
//            Rdesiredspeed = (float)slowspeedwant;
        }

        /*NEW ITEM*/
//        if(servo_pulse > 20)
//        {
//            if(servo_pulse>25)
//            {
//                Rdesiredspeed = (Rdesiredspeed)*((0.25*servo_pulse*servo_pulse-4876*servo_pulse+1020000)/1000000*correctspeed);
//
//         //       Ldesiredspeed = (Ldesiredspeed)*((0.000002354*servo_pulse*servo_pulse-0.00485*servo_pulse+0.9837)/correctspeed);
//
//            }
//            else
//            {
//                Rdesiredspeed = (Rdesiredspeed)*((0.25*servo_pulse*servo_pulse-4876*servo_pulse+1020000)/1000000);
//
//
// //               Ldesiredspeed = (Ldesiredspeed)*((0.000002354*servo_pulse*servo_pulse-0.00485*servo_pulse+0.9837));
//            }
//        }
//        else if(servo_pulse<-20) //��
//        {
//            if(servo_pulse<-25)
//            {
//
//                Ldesiredspeed = (Ldesiredspeed)*((0.2354*servo_pulse*servo_pulse+485*servo_pulse+98370)/100000*correctspeed);
//
//
////                Rdesiredspeed = (Rdesiredspeed)*((0.00000025*servo_pulse*servo_pulse+0.004876*servo_pulse+1.02)/correctspeed);
//            }
//            else
//            {
//                Ldesiredspeed = (Ldesiredspeed)*((0.2354*servo_pulse*servo_pulse+485*servo_pulse+98370)/100000);
//
////                Rdesiredspeed = (Rdesiredspeed)*((0.00000025*servo_pulse*servo_pulse+0.004876*servo_pulse+1.02));
//            }
//        }
        /**/
    }
    else if(mode_flag == 2||(mode_flag == 1&&light_square>squarelimit))//��������
    {
        Ldesiredspeed = slowspeedwant;
        Rdesiredspeed = slowspeedwant;
        if(servodirection == 1) //��ת
        {
            if(abs((int)angle_z)<=80)
            {
                Ldesiredspeed =Ldesiredspeed*0.5*(1+(0.2354*deadgeer*deadgeer-485*deadgeer+98370)/100000*correctspeed);
            }
            else
            {
                Ldesiredspeed = Ldesiredspeed/correctspeed;//Ldesiredspeed*0.5*(1+(0.2354*deadgeer*deadgeer-485*deadgeer+98370)/100000);
            }
            Rdesiredspeed = 2*slowspeedwant-Ldesiredspeed;
          //  Ldesiredspeed =Ldesiredspeed*0.5*(1+(0.2354*deadgeer*deadgeer-485*deadgeer+98370)/100000*correctspeed);

        }
        else if(servodirection == -1)
        {
            if(abs((int)angle_z)<=80)
            {
                Rdesiredspeed = (Rdesiredspeed)*0.5*(1+(0.25*deadgeer*deadgeer-4876*deadgeer+1020000)/1000000*correctspeed);
            }
            else
            {
                Rdesiredspeed = Rdesiredspeed/correctspeed;///(Rdesiredspeed)*0.5*(1+(0.25*deadgeer*deadgeer-4876*deadgeer+1020000)/1000000);
            }
            Ldesiredspeed = 2*slowspeedwant-Rdesiredspeed;
        }
        else
        {
            Ldesiredspeed = (float)slowspeedwant;
            Rdesiredspeed = (float)slowspeedwant;
        }
    }
//    if(Ldesiredspeed >120)
//    {
//        Ldesiredspeed = 120;
//    }
//    if(Rdesiredspeed>120)
//    {
//        Rdesiredspeed = 120;
//    }
//    if(Ldesiredspeed<=0)
//    {
//        Ldesiredspeed = 0;
//    }
//    if(Rdesiredspeed<=0)
//    {
//        Rdesiredspeed = 0;
//    }
    //Motorpid_control();
//        Motorbpi_Get(Ldesiredspeed,Rdesiredspeed);
        //�����Ǵ��ʱ���С����ٶȵĿ���ǿ��
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

    if(go_flag) //�����ܳ�״̬
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
    else if(go_flag == 0)//Ҫͣ����
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


/***************************** ͣ���жϴ������ *******************************/
void STOP_JUDGE()
{
    if(areaA_time == 0)
        roll_time = 0;
    else
        roll_time ++;
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

    if(gyronow[2]<-8.2&&gyronow[2]>-8.8)
    {
        gyronow[2] = -8.5;
    }
    gyrolast[2] = gyro[2];
    gyro[2] = 0.8*gyronow[2]+0.2*gyrolast[2];
//    angle_x += (gyro[0]-gyrooffset0)*dt;
//    angle_y += (gyro[1]-gyrooffset1)*dt;
    if((abs(servo_pulse)>40)||(mode_flag == 2))
        angle_z += (gyronow[2]-gyrooffset2)*dt;
    else
        angle_z = 0;


}












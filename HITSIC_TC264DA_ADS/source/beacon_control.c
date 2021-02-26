///*
// * beacon_control.c
// *
// *  Created on: 2020��12��20��
// *      Author: jimi
// */
#include "beacon_control.h"
#include "image.h"

/*MPU���ղ���*/
float send_buf[3] = {0};
extern mpu_t* this_mpu;
/**/

/*����Ϊ�е�ϵ��*/
int KLA;
int KRA;
/**/

/*�Զ���һЩ����*/
int servodirection = 0;              //���ƶ��ת��������� ��-1������� ��1���Ҵ�����
pidCtrl_t* servo_pid; //=PIDCTRL_Construct(0, 0, 0); //���PID��ʼ��
float servokp = 0.013;
float servokd = 0.025;
float servo_pulse = 0;
int straspeedplus = 20; //���ٵ��ٶ�
int straspeedcut = 20; //���ٵ��ٶ�
float difference = 0.6 ;
float deadgeer = 0.8;
float Shun_Dengcut = 0.5;//˳��״̬��kp��kdҪ��Ӧ�ļ�С

int motorstate = 0;  //(�������״̬��Ļ����Ƕ�ά����)0:���� ��1������
float Ldesiredspeed;
float Rdesiredspeed;
pid motorpir,motorpil;
float motorbkp_R = 3,motorbki_R = 0.28 ,motorbkp_L = 3,motorbki_L = 0.21; //kp��kd
int16 leftFTM;
int16 rightFTM;
float motor_lefdevn =0;
float motor_rigdevn =0;
float N_motor_lefdevn=0,N_motor_rigdevn=0;
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
int LOW = 85;    //����ʹ�õ�����ͷ���� (Ӧ�ú��ڿ����޸ģ�����С����һֵ�ͽ������״̬)
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

int Areajin = 430; //���������־λ
int sundengarea = 0;
int servogradual = 0; //����״̬������д��
float differences = 1.2; //˳�����̽����ٶȲ���
float jindengzhan = 0.5;
int xundengtime = 20;

int xundengtimemie = 0;
int xundengtimemie1 = 200;
/**/

//int *speedA_M = &date[2][2];        //����Ϊ�ĸ��������������ٶ�
//int *speedA_H = &date[2][3];
//int *klA_adjust = &date[1][3];     //ƫ��������ϵ��(����ģ������ʱ���������ϵ��)
//int *krA_adjust = &date[1][4];
float klA_adjust = 360.3;
float krA_adjust = 166.6;

int areaA_time = 40;  //��ͨ��Ϊ0������־λ
//δ���ࣨ�ɷ���beacon_control.c�ļ��
int mode_time = 0;    //��¼��״̬��������
int Avoid_Low = 0;    //������С����



//mmc
extern uint8 connectnum;//��ͨ����
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


//������ͨ��ʼ���ڸ�������area_flag�������ʱ����������������
void CAMERA_JUDGE()
{
    averbA_x = lightx;
    averbA_y = lighty;
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
    aver_x = averA_x;
    aver_y = averA_y;
}


/***************************** ״̬�жϴ������ *******************************/
void MODE_JUDGE()
{
    if(mode_flag == 3 && fabs(error_simu) < 10 ) //������˳��������׷��״̬���ж��������źŵ�ƫ����С��ĳһ��ֵʱ����׷�ƣ���ֵ10�ݶ������Ը���
    {
        correct_time++;
        if(correct_time>21) //˳������ʱ���ݶ�Ϊ21��
        {
            mode_last = mode_flag; //�洢��һ�ε�״̬
            mode_flag = 0; //����Ϊ׷��״̬
            correct_time = 0;
            mode_time = 0; //��¼��״̬����ʱ��ʱ�Ƚ�������������������ã���������
        }
    }
    if(mode_flag == 0 && distance <= LOW && lights > Areajin)  //����״̬���п��ܲ����� || mode_flag == 1  && fabs(error_adjust) < 20
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
        /*
         * if(light_flag != 0)                                                         //����������ֹң���ʱ�����Ų�
              light_last = light_flag;
              ѧ�����룬����δ֪
         */
    }
    if(mode_flag == 2 &&  areaA_time == 0)
    {
        mode_last = mode_flag;
        mode_flag = 3;
        mode_time = 0;
        back_flag = 0; //�������û���̫�����������˳��״̬�²���Ҫ��Ұ��ĵƸ��ţ�
    }
    else if(mode_flag == 1 && distance > LOW + 15)    //�������Ұ�ڻ��еƣ�+3��Ϊ�˷�ֹ���֮ǰ��Ϊ˳�� ��ʱҲ����˳��״̬
    {
      mode_last = mode_flag;
      mode_flag = 3;
      mode_time = 0;
      back_flag = 0; //
    }
    else if(mode_flag == 0 && abs((int)error_simu) > 50) //׷��״̬���ڱ���ƫ��ϴ�ʱ������˳��״̬
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
    if((mode_flag == 3 && mode_time == 1)|| (mode_flag == 0 && go_flag == 1)/*����׷��״̬���ܳ���־λΪ1*/ || light_change_flag == 1/*���л��еƷ���Ϊ1����Ϊ���Խ����еƷ���ı任*/) //�����е�λ��
    {  //ȷ�����������ң���������������
        if(light_change_flag == 1)   //��ʱ��״̬ת����������������������������������(�������û���Ҫ˼��)
        {  //�ڱ��Ϲ����з������ã�������������������û���ã�
            if(abs(error_aver) > 10)    //��ʱת��״̬��ƫ��ϴ�����л��еƷ���()
            {
                if(error_aver > 0)
                    light_flag = 1; //light_flagӦ�����еƷ����һ����־λ
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
    distance = CarmSqrt(error_dx*error_dx + error_dy*error_dy);   //����ƾ�
//    if(mode_flag == 0 && mode_time == 1)                                          //����׷��״̬�жϾ���
//    {
//        if(light_square <= LongA_square ) //����źŵƵ����С��ĳһ��ֵʱ����Ϊ���Ǵ���׷��״̬������ǰ��
//           long_flag = 1;  //׷Զ��
//        else
//           long_flag = 0;
//    }
//    if(light_flag == 1) //����
//    {
//        simu_distance = KLA*distance + klA_adjust;
//
//        simu_y = aver_y - simu_distance ;
//    }
//    else if(light_flag == -1)  //����
//    {
//        simu_distance = KRA*distance + krA_adjust;
//
//        simu_y = aver_y + simu_distance ;
//    }
//
//    //����Ϊ�����ϰ���ƫ��
//    //error_simu = simu_y - 94;  //����ģ���ű��ƫ�� ������δ֪��
//    error_simu = aver_y - 94;

    if(light_flag == 1) //����
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
    else if(light_flag == -1)  //����
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
}

/******************************* ��������������ƺ�����һ���ƣ� **********************************/
void FIND_LIGHT()
{
    servodirection = 0;
    servogradual = 0;
    if(roll_time < 333) //��ת����С��ĳһֵ
    {
        if(mode_last == 0 && mode_flag == 1)
        {
            servogradual = 1;
        }
        if(mode_flag == 2)  //�������ƴ��������ת״̬
        {
            if(light_flag == -1)   //�����Ƶ�
            {
                servodirection = -1;  //���Һ�����������ת��
            }
            else if(light_flag == 1) //�����Ƶ�
            {
                servodirection = 1;  //��������Ҵ������ת��
            }
        }
//        else if(mode_flag == 2)  //��ʼ�������Ƶ������һ������
//        {
//            servodirection = -1;
//        }
    }
//    else  //�����Ǹ�����׼��
//    {
//
//    }
}

//�������

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
    if(go_flag) //�����ܳ�״̬
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
    else if(go_flag == 0)//Ҫͣ����
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

/***************************** ͣ���жϴ������ *******************************/
void STOP_JUDGE()
{
    if(areaA_time == 0)
        roll_time = 0;
    else
        roll_time ++;
    if(roll_time > 666)  //��Ϊ0��᳤ʱ���޵�ͣ��
        go_flag = 0;
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
//
//
//
//
//
//�����ǿ���

/* ******************** �˲��� ******************** */

/**
 * @brief   IMU������ԭʼ����
 * @ {
 */
//float ctrl_accl[3] = {0.0f, 0.0f, 0.0f};
//float ctrl_gyro[3] = {0.0f, 0.0f, 0.0f};
//float ctrl_filterAngAccl = 0.0f, ctrl_filterAngGyro = 0.0f; ///< �ɼ��ٶȼơ����ٶȼƲ���ĽǶ�
//float ctrl_acclRefAxis; //= ctrl_accl[2]; //FIXME
//float ctrl_gyroRefAxis; //= ctrl_gyro[1]; //FIXME
//
//float ctrl_filterCompTgReciprocal = 1.0f / 0.8f;    ///< Tg�ĵ���������Tg -> ��Tg�ĵ������Ż������ٶȡ�

/**
 * @brief   �Ƕ��˲���ر�����
 * @note    ����PIDCTRL�Ľṹ�屣�浱ǰֵ������ֵ��΢��ֵ����ʷֵ��
 * @ {
 */
//pidCtrl_t ctrl_angFilter =
//{
//    .kp = 0.0f, .ki = 0.0f, .kd = 0.0f,
//    .errCurr = 0.0f, .errIntg = 0.0f, .errDiff = 0.0f, .errPrev = 0.0f,
//};
//float &ctrl_filterAngle = ctrl_angFilter.errCurr; ///< �˲���ĵ�ǰ�Ƕȡ�����ctrl_angFilter.errCurr����
//pid ctrl_angFilter;
//float ctrl_filterAngRef = 0.0f; ///< �ǶȲο��㣬��ʼ��ʱ��ȡ����ʼ��ʱ���뱣�־�ֹ��
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
//        SDK_DelayAtLeastUs(1000,CLOCK_GetFreq(kCLOCK_CoreSysClk)); // MPU6050�����ٶ�û��ô�죬������ȡֻ��õ���ͬ��ֵ��
//    }
//    ctrl_filterAngRef = intergration / ((float)sampleTime);
//    ctrl_filterAngle = ctrl_filterAngRef;
//    ctrl_filterAngGyro = ctrl_filterAngRef;
//}
//void CTRL_FilterUpdate(uint32_t updateTime_ms)
//{
//    float dT = ((float)updateTime_ms) * 0.001f;
//    /** �Լ��ٶ�ֵ�����޷�����ֹ����asin����ʱ���� */
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
    //��λ��
    SmartCar_VarUpload(send_buf,3);
}

/*
 * VA_get.c
 *
 *  Created on: 2021年3月31日
 *      Author: jimi
 */
#include "VA_get.h"

int BatterySCM_Temp[30];//电压采集  //改变采集次数
int Battery_TempH[30];//电压采集  //改变采集次数
int Battery_Temp[30];//电压采集  //改变采集次数
int IchargeSCM_Temp[30];//电流采集
int Icharge_Temp[30];//电流采集
float batterySCM = 0;
float battery = 0;
float batteryH = 0;
float IchargeSCM;
float Icharge;
extern int gogoflag;

float power;
float setpower = 10;
float setpower1;
float powererror;
float powerduty;
uint32 powerduty1;
float Pcharge;
int countflag = 0;

float batterylimit = 9.2;
int servoinit = 0;
float batteryservo = 8;

int statusgo = 1 ;
float chargeKp;
float chargeKi;
float chargeKd;
float chargeek = 0;
float chargeekL = 0;
float chargedek = 0;

extern float time;

//void ChargepidInit()
//{
//    Pwmdutyctrl.Kp = 0;
//    Pwmdutyctrl.Ki = 0;
//    Pwmdutyctrl.Kd = 0;
//    Pwmdutyctrl.ek = 0;
//    Pwmdutyctrl.dek = 0;
//    Pwmdutyctrl.ekL = 0;
//    Pwmdutyctrl.ekL2 = 0;
//    Pwmdutyctrl.ekSum = 0;
//}
//
//float Chargectrl(float err)
//{
//    float dutyset = 0;
//    Pwmdutyctrl.Kp = 0.8;
//    Pwmdutyctrl.Ki = 0.4;
//    Pwmdutyctrl.Kd = chargekd;
//    PIDCTRL_ErrUpdate(&Pwmdutyctrl,err);
//    dutyset = PIDCTRL_CalcPIDGain(&Pwmdutyctrl);
//
//    return dutyset;
//}
//
//void PIDCTRL_ErrUpdate(pid1* chargepwm, float err)
//{
//    chargepwm->ekL = chargepwm->ek;
//    chargepwm->ek = err;
//    chargepwm->ekSum += chargepwm->ek;
//    chargepwm->dek = chargepwm->ek - chargepwm->ekL;
//}
//
//float PIDCTRL_CalcPIDGain(pid1* chargepwm)
//{
//    float out = 0;
//    out = chargepwm->Kp*chargepwm->dek+chargepwm->Ki*chargepwm->ek;
////    out = chargepwm->Kp*chargepwm->ek+chargepwm->Ki*chargepwm->ekSum+chargepwm->Kd*chargepwm->dek;
//    return out;
//}

void BatterySCM_Get()
{
    if(battery>batteryservo)
    {
        servoinit = 1;
    }
    if(battery>batterylimit)
    {
        gogoflag = 1;
    }
}

void IchargeSCM_Get()
{
    for(int i = 0;i<30;i++)
    {
        IchargeSCM_Temp[i] = ADC_Get(ADC_0,ADC0_CH8_A8,ADC_8BIT);//电压
    }
    mergesort(IchargeSCM_Temp,1,30);
    IchargeSCM = 0;                         //求平均值
    for(int k=3;k<=26;k++)
    {
         IchargeSCM += (float)IchargeSCM_Temp[k];
    }
    IchargeSCM = (IchargeSCM/24)/255*3.3/20/0.02;//求平均值并进行补正
}

void ChargeVA()
{
    countflag++;
    if(countflag%2 == 0)
    {
        Battery_Get();
        Icharge_Get();
        //BatteryH_Get();
        Pcharge = battery*Icharge*1.1;   //����
    }
    else if(countflag%2 == 1)
    {
        setpower = 29;
        if(battery <= 4)  setpower1 = setpower*(-0.1*battery+1.3);//��ʼ��繦�ʹ��ߵĲ�������
        else if((battery <= 6)&&(battery > 4)) setpower1 = setpower+(-0.8*battery+8.5);
        else if((battery <= 8)&&(battery > 6)) setpower1 = setpower+3;
        else setpower1 = setpower+2.3; //��ѹԽ�ߣ���繦��ԽС�Ĳ���


        //if(Pcharge<20&&battery > 4) setpower1 = setpower*4;
        powererror = setpower1 - Pcharge;

        chargeKp = 0.8;
        chargeKi = 0.4;
        chargeekL = chargeek;
        chargeek = powererror;
        chargedek = chargeek - chargeekL;
        powerduty += chargeKp*chargedek+chargeKi*chargeek;


        if(battery < 1) powerduty =  (powerduty < 1500)?1500:powerduty;
        //if(battery < 3) powerduty =  (powerduty > 5000)?5000:powerduty;

        if(powerduty>10000)
            powerduty = 10000;
        else if(powerduty<0)
            powerduty = 0;
        powerduty1 = (uint32)powerduty;
        SmartCar_Gtm_Pwm_Setduty(&IfxGtm_ATOM0_6_TOUT6_P02_6_OUT,powerduty1);
    }
}

void Battery_Get()
{
    for(int i = 0;i<30;i++)
    {
        //Battery_Temp[i] = ADC_Get(ADC_0,ADC0_CH0_A0,ADC_8BIT);//��ѹ
        Battery_Temp[i] = ADC_Get(ADC_1,ADC1_CH4_A20,ADC_8BIT);//��ѹ
    }
    mergesort(Battery_Temp,1,30);
    battery = 0;                         //��ƽ��ֵ
    for(int k=3;k<=26;k++)
    {
         battery += (float)Battery_Temp[k];
    }
    //battery = (battery/24)/255*3.3*21;//��ƽ��ֵ�����в���
    battery = (battery/24)/255*3.3*11;//��ƽ��ֵ�����в���
}
void Icharge_Get()
{
    for(int i = 0;i<30;i++)
    {
        //Icharge_Temp[i] = ADC_Get(ADC_0,ADC0_CH1_A1,ADC_8BIT);//��ѹ
        Icharge_Temp[i] = ADC_Get(ADC_1,ADC1_CH5_A21,ADC_8BIT);//��ѹ
    }
    mergesort(Icharge_Temp,1,30);
    Icharge = 0;                         //��ƽ��ֵ
    for(int k=3;k<=26;k++)
    {
         Icharge += (float)Icharge_Temp[k];
    }
    Icharge = (Icharge/24)/255*3.3/20/0.02;//��ƽ��ֵ�����в���
}



void merge1(int a[],int left,int right,int mid)
{
    int n1, n2;
    n1 = mid - left + 1;
    n2 = right - mid;
    int* L;
    int* R;
    L = (int*)malloc(n1*sizeof(int));
    if (!L) {
        exit(1);
    }
    R = (int*)malloc(n2 * sizeof(int));
    if (!R) {
        exit(1);
    }
    for (int i = 0; i < n1; i++)
    {
        L[i] = a[left-1+i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = a[mid + j];
    }
    int i = 0;
    int j = 0;
    for (int k = left-1; k < right; k++)
    {
        if (i >= n1)
        {
            a[k] = R[j];
            j++;
        }
        else if(j >= n2)
        {
            a[k] = L[i];
            i++;
        }
        else if ((i<n1)|(j<n2))
        {
            if (L[i] <= R[j])
            {
                a[k] = L[i];
                i++;
            }
            else
            {
                a[k] = R[j];
                j++;
            }
        }
    }
    free(L);
    free(R);
}
void mergesort(int a[],int left,int right)
{
    int q = 0;
    if (left<right) {
        q = (left + right) / 2;
        mergesort(a, left, q);
        mergesort(a, q+1, right); //这里要是q+1，因为第q项对应已经被应用，左项又是需要减一，所以应是q+1
        merge1(a,left,right,q);
    }
    else if(left == right)
    {
        return;
    }
}



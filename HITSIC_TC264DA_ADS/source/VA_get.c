/*
 * VA_get.c
 *
 *  Created on: 2021年3月31日
 *      Author: jimi
 */
#include "VA_get.h"

int Battery_Temp[30];//电压采集  //改变采集次数
int Battery_TempH[30];//电压采集  //改变采集次数
int Icharge_Temp[30];//电流采集
float battery;
float Icharge;


void Battery_Get()
{
    for(int i = 0;i<30;i++)
    {
        Battery_Temp[i] = ADC_Get(ADC_0,ADC0_CH11_A11,ADC_8BIT);//电压
    }
    mergesort(Battery_Temp,1,30);
    battery = 0;                         //求平均值
    for(int k=3;k<=26;k++)
    {
         battery += (float)Battery_Temp[k];
    }
    battery = (battery/24)/255*3.3*21;//求平均值并进行补正
}

void Icharge_Get()
{
    for(int i = 0;i<30;i++)
    {
        Icharge_Temp[i] = ADC_Get(ADC_0,ADC0_CH8_A8,ADC_8BIT);//电压
    }
    mergesort(Icharge_Temp,1,30);
    Icharge = 0;                         //求平均值
    for(int k=3;k<=26;k++)
    {
         Icharge += (float)Icharge_Temp[k];
    }
    Icharge = (Icharge/24)/255*3.3/20/0.02;//求平均值并进行补正
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


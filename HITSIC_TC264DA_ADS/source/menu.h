#ifndef MENU_H
#define MENU_H

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include <ifxCpu_Irq.h>
#include "IfxScuWdt.h"
#include "SmartCar_Uart.h"
#include "SmartCar_Upload.h"
#include "SmartCar_Oled.h"
#include "SmartCar_Pwm.h"
#include "SmartCar_MPU.h"
#include "SmartCar_MT9V034.h"
#include "SmartCar_Systick.h"
#include "SmartCar_GPIO.h"
#include "common.h"
#include "SmartCar_Flash.h"

struct menu{
    char *data;
    int jump;
    int dataint;
    float datafloat;
    int type;

    int dataj;
};

/*
 * 所有的函数声明
 */
void MENU_storage();
void MENU_Read();
void MENU_Make();
void MENU_Tree();
void MENU_Start();
void MENU_find(int k);
int MENU_keyScan(int mode);
void MENU_new();
void MENU_add(int datint, float datfloat, int page, char* nam);
void MENU_listmmc();
void Menu_gpioinit();
void MENU_give();

void MENU_changeint(int changenum);
void MENU_changefloat(float changenum);


#endif

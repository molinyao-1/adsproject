#ifndef _IMAGE_H
#define _IMAGE_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "common.h"
#include "SmartCar_MT9V034.h"


#define MISS 255
#define CAMERA_H  120                            //ͼƬ�߶�
#define CAMERA_W  188                            //ͼƬ���
#define FAR_LINE 1//ͼ�����ϱ߽�
#define NEAR_LINE 113//ͼ�����±߽�
#define LEFT_SIDE 0//ͼ������߽�
#define RIGHT_SIDE 187//ͼ�����ұ߽�
#define MISS 255
#define white_num_MAX 10//ÿ��������������

/////////////////////////////
#define black 0
#define white 1
#define blue  2
#define green 3
#define red   4
#define gray  5
#define purple 6
///////////////////////////

extern uint8 IMG[CAMERA_H][CAMERA_W];//��ֵ����ͼ������
extern uint8 image_Buffer_0[CAMERA_H][CAMERA_W];
extern uint8* fullBuffer;//ָ��Ҷ�ͼ���׵�ַ

void head_clear(void);
void THRE(void);
int find_f(int a);
void search_white_range();
void find_all_connect();
void find_road();
uint8 find_continue(uint8 i_start, uint8 j_start);
void ordinary_two_line(void);
void image_main();
void get_mid_line(void);

void my_memset(uint8* ptr, uint8 num, uint8 size);


//add
void mmc_connect_num();
void mmc_connect_num1();
int mcmp(const void* a, const void* b);



#endif //

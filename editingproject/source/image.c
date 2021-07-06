#include "image.h"
#include<math.h>
#include<stdlib.h>
int f[10 * CAMERA_H];//������ͨ����ͨ��
//ÿ������������
typedef struct {
    uint8   left;//��߽�
    uint8   right;//�ұ߽�
    int   connect_num;//��ͨ���
}range;

//ÿ�е����а�����
typedef struct {
    uint8   num;//ÿ�а�������
    range   area[white_num_MAX];//���и���������
}all_range;

//����������ÿ������������
typedef struct {
    uint8   left;//��߽�
    uint8   right;//�ұ߽�
    uint8   width;//���
}road_range;

//ÿ������������ÿ��������
typedef struct {
    uint8   white_num;
    road_range   connected[white_num_MAX];
}road;

all_range white_range[CAMERA_H];//���а�����
road my_road[CAMERA_H];//����
uint8 IMG[CAMERA_H][CAMERA_W];//��ֵ����ͼ������
uint8 left_line[CAMERA_H], right_line[CAMERA_H];//���������ұ߽�
uint8 mid_line[CAMERA_H];
int all_connect_num = 0;//���а�������
uint8 top_road;//������ߴ���������
int threshold = 240;//��ֵ

int biaozhi;
//add
int xx, yy;

uint8 connectnum;//��ͨ����
uint8 connectnumB;//��ͨ����
float lightx, lighty, lights, lightc;

float lightdis;
float camx=90, camy=75;

int yzhi;

int flag1 = 0;

//int mcmp(const void* a, const void* b)
//{
//    mmcth* item1 = (mmcth*)a;
//    mmcth* item2 = (mmcth*)b;
//    if (item1->ss - item2->ss > 0)  return 1;
//    else return -1;
//
//}
////////////////////////////////////////////
//���ܣ���ֵ��
//���룺�Ҷ�ͼƬ
//�������ֵ��ͼƬ
//��ע��
///////////////////////////////////////////
void THRE()
{
        //uint8 numup = 0;
        uint8* map;
        uint8* my_map;
        map = &mt9v034_image[0][0];
//        for (int i = 0; i < 120; i++)
//        {
//            for (int j = 0; j < 188; j++)
//            {
//                if ((*map) > 150) numup++;
//                map++;
//            }
//        }
//        map = &mt9v034_image[0][0];
//        if (1)
//        {
            for (int i = 0; i < CAMERA_H; i++)
            {
                my_map = &IMG[i][0];
                for (int j = 0; j < CAMERA_W; j++)
                {
                    if ((*map) > 20+yzhi)
                        (*my_map)  = 255;
                    else (*my_map) = 0;
                    map++;
                    my_map++;
                }
            }
//        }
//        else if (numup <= 10)
//        {
//            for (int i = 0; i <= 10; i++)
//            {
//                my_map = &IMG[i][0];
//                for (int j = 0; j < 188; j++)
//                {
//                    if ((*map) > 50)
//                        (*my_map) = 1;
//                    else (*my_map) = 0;
//                    map++;
//                    my_map++;
//                }
//            }
//            for (int i = 11; i <= 40; i++)
//            {
//                my_map = &IMG[i][0];
//                for (int j = 0; j < 188; j++)
//                {
//                    if ((*map) > 50)
//                        (*my_map) = 1;
//                    else (*my_map) = 0;
//                    map++;
//                    my_map++;
//                }
//            }
//            for (int i = 41; i < 120; i++)
//            {
//                my_map = &IMG[i][0];
//                for (int j = 0; j < 188; j++)
//                {
//                    if ((*map) > 50)
//                        (*my_map) = 1;
//                    else (*my_map) = 0;
//                    map++;
//                    my_map++;
//                }
//            }
//        }

}

////////////////////////////////////////////
//���ܣ�������峵ͷ
//���룺
//�����
//��ע��Ҫ�����Լ���ͷ�Ĵ�С�����޸�
///////////////////////////////////////////
void head_clear(void)
{
    uint8* my_map;
    for (int i = 88; i >= 84; i--)
    {
        my_map = &IMG[i][0];
        for (int j = 40; j <= 135; j++)
        {
            *(my_map + j) = 0;
        }
    }
}

////////////////////////////////////////////
//���ܣ����Ҹ��ڵ�
//���룺�ڵ���
//�������������
//��ע����·��ѹ��
///////////////////////////////////////////
int find_f(int node)
{
    if (f[node] == node)return node;//�ҵ���������ȣ�return
    f[node] = find_f(f[node]);//����Ѱ���Լ��ĸ��ڵ�
    return f[node];
}

////////////////////////////////////////////
//���ܣ���ȡ������ ����ȫ�������ӱ��
//���룺IMG[120][188]
//�����white_range[120]
//��ע��ָ������
///////////////////////////////////////////
void search_white_range()
{
    flag1 = 0;
    uint8 i, j;
    int istart = NEAR_LINE;//������ʼ��
    int iend = FAR_LINE;//������ֹ��
    int tnum = 0;//��ǰ�а�����
    all_connect_num = 0;//������ų�ʼ��
    uint8* map = NULL;
    for (i = istart; i >= iend; i--)
    {
        map = &IMG[i][LEFT_SIDE];//ָ�����߼ӿ�����ٶ�
        tnum = 0;
        for (j = LEFT_SIDE; j <= RIGHT_SIDE; j++, map++)
        {
            if ((*map))//��������߽�
            {
                tnum++;
                if (tnum >= white_num_MAX)break;
                range* now_white = &white_range[i].area[tnum];
                now_white->left = j;

                //��ʼ���һ��һ�����ص�����������ұ߽�
                map++;
                j++;

                while ((*map) && j <= RIGHT_SIDE)
                {
                    map++;
                    j++;
                }
                now_white->right = j - 1;
                now_white->connect_num = ++all_connect_num;//��������һ��������������
            }
        }
        white_range[i].num = tnum;
    }
}

////////////////////////////////////////////
//���ܣ�Ѱ�Ұ�������ͨ�ԣ���ȫ����ͨ�����ӵĽڵ���ˢ����������ȵĽڵ���
//���룺
//�����
//��ע��
///////////////////////////////////////////
void find_all_connect()
{
    //f�����ʼ��
    for (int i = 1; i <= all_connect_num; i++)
        f[i] = i;

    //uΪup dΪdown ��Ϊ��ǰ������������е��������к���������
    //u_num�������а�����
    //u_left�������е�ǰ������߽�
    //u_right�������е�ǰ�����ұ߽�
    //i_u����ǰ�������������ǵ�ǰ���У������У������еĵ�i_u��
    int u_num, i_u, u_left, u_right;
    int d_num, i_d, d_left, d_right;
    all_range* u_white = NULL;
    all_range* d_white = NULL;
    for (int i = NEAR_LINE; i > FAR_LINE; i--)//��Ϊÿ����ÿ���бȽ� ����ѭ����FAR_LINE+1
    {
        u_num = white_range[i - 1].num;
        d_num = white_range[i].num;
        u_white = &white_range[i - 1];
        d_white = &white_range[i];
        i_u = 1; i_d = 1;

        //ѭ������ǰ�л������а��������ľ�Ϊֹ
        while (i_u <= u_num && i_d <= d_num)
        {
            //�����ȱ��棬�����������д�������ҷ���Ч�ʵ�
            u_left = u_white->area[i_u].left;
            u_right = u_white->area[i_u].right;
            d_left = d_white->area[i_d].left;
            d_right = d_white->area[i_d].right;

            if (u_left <= d_right && u_right >= d_left)//�������������ͨ
                f[find_f(u_white->area[i_u].connect_num)] = find_f(d_white->area[i_d].connect_num);//���ڵ�������

            //��ǰ�㷨��������һ�����֪��Ϊɶ������
            if (d_right > u_right)i_u++;
            if (d_right < u_right)i_d++;
            if (d_right == u_right) { i_u++; i_d++; }
        }
    }
}




////////////////////////////////////////////
//���ܣ�ͼ����������
//���룺
//�����
//��ע��
///////////////////////////////////////////
void image_main()
{
    search_white_range();
    find_all_connect();
    //find_road();
    /*���˴�Ϊֹ�������Ѿ��õ������������Ľṹ������my_road[CAMERA_H]*/
   // ordinary_two_line();
    //mmc_connect_num1();
    //get_mid_line();
    mmc_connect_num();


}


////////////////////////////////////////////
//���ܣ���ͨ��
//���룺
//�����
//��ע��
///////////////////////////////////////////
void mmc_connect_num()
{
    flag1 = 1;
    biaozhi=0;
    float newx=0, newy = CAMERA_W, news = 0, newdis = 0;
    int k = 0;
    lightdis = 0;
    lights = 0;
    lightx = 0;
    lighty =CAMERA_W;
    uint8 xxx = 0, yyy = 0;//�Ƶ����ȵ������͵�ǰ�а������
    uint8 connect0 = 0;//�Ƶ����ȱ��
    uint8 connectmax = 0;//�Ƶİ�����
    connectnum = 0;//��ͨ����
//    uint8 connectsum[1000];//�������ӵİ�����
    uint8 father[100] = {0};
    uint8 father1[100] = { 0 };
    uint8 judge[100] = { 0 };
//    for (int i = 1; i < 1000; i++)
//        connectsum[i] = 0;
/*  for (int i = 1; i <= all_connect_num; i++)
        if (f[i] == i)
        {
            connectnum++;
            father[connectnum] = i;
        }*/
    for (int i = CAMERA_H-1; i >= 0; i--)
    {
        for (int j = 1; j <= white_range[i].num; j++)
        {
            if (f[white_range[i].area[j].connect_num] == white_range[i].area[j].connect_num)
            {
                connectnum++;
                father[connectnum] = i;
                father1[connectnum] = white_range[i].area[j].connect_num;
            }
        }
    }

//
//    for (int i = 1; i <= all_connect_num; i++)
//        connectsum[f[i]]++;

//    if(connectnum>30)
//    {
//
//    }
    for (int k = connectnum; k >= 1&&k>=connectnum-10; k--)
    {
        uint8 up = 0, down = 0;
        uint8 maxleft = CAMERA_W, maxright = 0;
        int width = 0;
        int height = 0;
        uint8 jud = 0;
        int fnum = 0;
        float dis = 0;
        float sss = 0;
        float centx = 0;
        float centy = 0;
        down = father[k];
        fnum = father1[k];
        //printf("%d\n", down);
        for (int i = down; i >=1 ; i--)
        {
            for (int j = 1; j <= white_range[i].num; j++)
            {
                if (f[white_range[i].area[j].connect_num] == fnum)
                {
                    //jud = 1;
                    up = i;
                    if (maxleft > white_range[i].area[j].left) maxleft = white_range[i].area[j].left;
                    if (maxright < white_range[i].area[j].right) maxright = white_range[i].area[j].right;
                    sss += white_range[i].area[j].right + 1 - white_range[i].area[j].left;
                }
            }
        }
        width = maxright - maxleft + 1;
        height = down - up + 1;
        centx = (down + up) / 2;
        centy = (maxright + maxleft) / 2;
        float cc = 0, dd = 0;
        cc = centx - camx;
        dd = centy - camy;
        dis = sqrt(cc * cc + dd * dd);
//        if (dis > 95) jud = 0;
//        else if (dis > 70)
//        {
//            if (width * 2 < height) jud = 1;
//            if (width > 15 || height>5) jud = 1;
//            if (sss > 40) jud = 1;
//        }
//        else if (dis > 60)
//        {
//            if (width < height) jud = 1;
//            if (width < 5||width>20) jud = 1;
//            if (height < 3 || height>13) jud = 1;
//            if (sss > 200 || sss < 10) jud = 1;
//        }
//        else if (dis > 50)
//        {
//            if (width < height) jud = 1;
//            if (width < 5 || width>25) jud = 1;
//            if (height < 5 || height>20) jud = 1;
//            if (sss > 400 || sss < 50) jud = 1;
//        }
//        else if (dis > 35)
//        {
//            if (width < height) jud = 1;
//            if (width < 10 || width>40) jud = 1;
//            if (height < 10 || height>30) jud = 1;
//            if (sss > 800 || sss < 100) jud = 1;
//        }
//        else if (dis <= 35)
//        {
//            if (sss < 100) jud = 1;
//        }


        if (jud == 0)
        {
            //lightnum = father[k];
            newx = centx;
            newy = centy;
            news = sss;
            newdis = dis;
            if (lights < news)
            {
                lightx = newx;
                lighty = newy;
                lights = news;
                lightdis = newdis;
            }
            biaozhi=1;
            //break;
        }

        int xxxx=lightx;
        int yyyy=lighty;
        for(int i=0;i<CAMERA_H;i++)
        {
            IMG[i][yyyy+1]=255;
            IMG[i][yyyy]=255;
            //for(int j=0;j<188;j++)
        }
        for(int j=0;j<CAMERA_W;j++)
        {
            IMG[xxxx][j]=255;
            IMG[xxxx+1][j]=255;
        }
        //printf("x:%f   y:%f  ���:%f \n", centx, centy, sss);
        //printf("%d\n", jud);
    }
    if(biaozhi==0)
    {
        connectnum=0;
    }
}




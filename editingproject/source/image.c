#include "image.h"
#include<math.h>
#include<stdlib.h>
int f[10 * CAMERA_H];//考察连通域联通性
//每个白条子属性
typedef struct {
    uint8   left;//左边界
    uint8   right;//右边界
    int   connect_num;//连通标记
}range;

//每行的所有白条子
typedef struct {
    uint8   num;//每行白条数量
    range   area[white_num_MAX];//该行各白条区域
}all_range;

//属于赛道的每个白条子属性
typedef struct {
    uint8   left;//左边界
    uint8   right;//右边界
    uint8   width;//宽度
}road_range;

//每行属于赛道的每个白条子
typedef struct {
    uint8   white_num;
    road_range   connected[white_num_MAX];
}road;

all_range white_range[CAMERA_H];//所有白条子
road my_road[CAMERA_H];//赛道
uint8 IMG[CAMERA_H][CAMERA_W];//二值化后图像数组
uint8 left_line[CAMERA_H], right_line[CAMERA_H];//赛道的左右边界
uint8 mid_line[CAMERA_H];
int all_connect_num = 0;//所有白条子数
uint8 top_road;//赛道最高处所在行数
int threshold = 240;//阈值

int biaozhi;
//add
int xx, yy;

uint8 connectnum;//连通域数
uint8 connectnumB;//连通域数
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
//功能：二值化
//输入：灰度图片
//输出：二值化图片
//备注：
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
//功能：粗犷的清车头
//输入：
//输出：
//备注：要根据自己车头的大小进行修改
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
//功能：查找父节点
//输入：节点编号
//输出：最老祖先
//备注：含路径压缩
///////////////////////////////////////////
int find_f(int node)
{
    if (f[node] == node)return node;//找到最古老祖先，return
    f[node] = find_f(f[node]);//向上寻找自己的父节点
    return f[node];
}

////////////////////////////////////////////
//功能：提取跳变沿 并对全部白条子标号
//输入：IMG[120][188]
//输出：white_range[120]
//备注：指针提速
///////////////////////////////////////////
void search_white_range()
{
    flag1 = 0;
    uint8 i, j;
    int istart = NEAR_LINE;//处理起始行
    int iend = FAR_LINE;//处理终止行
    int tnum = 0;//当前行白条数
    all_connect_num = 0;//白条编号初始化
    uint8* map = NULL;
    for (i = istart; i >= iend; i--)
    {
        map = &IMG[i][LEFT_SIDE];//指针行走加快访问速度
        tnum = 0;
        for (j = LEFT_SIDE; j <= RIGHT_SIDE; j++, map++)
        {
            if ((*map))//遇白条左边界
            {
                tnum++;
                if (tnum >= white_num_MAX)break;
                range* now_white = &white_range[i].area[tnum];
                now_white->left = j;

                //开始向后一个一个像素点找这个白条右边界
                map++;
                j++;

                while ((*map) && j <= RIGHT_SIDE)
                {
                    map++;
                    j++;
                }
                now_white->right = j - 1;
                now_white->connect_num = ++all_connect_num;//白条数加一，给这个白条编号
            }
        }
        white_range[i].num = tnum;
    }
}

////////////////////////////////////////////
//功能：寻找白条子连通性，将全部联通白条子的节点编号刷成最古老祖先的节点编号
//输入：
//输出：
//备注：
///////////////////////////////////////////
void find_all_connect()
{
    //f数组初始化
    for (int i = 1; i <= all_connect_num; i++)
        f[i] = i;

    //u为up d为down 即为当前处理的这两行中的上面那行和下面那行
    //u_num：上面行白条数
    //u_left：上面行当前白条左边界
    //u_right：上面行当前白条右边界
    //i_u：当前处理的这个白条是当前这行（上面行）白条中的第i_u个
    int u_num, i_u, u_left, u_right;
    int d_num, i_d, d_left, d_right;
    all_range* u_white = NULL;
    all_range* d_white = NULL;
    for (int i = NEAR_LINE; i > FAR_LINE; i--)//因为每两行每两行比较 所以循环到FAR_LINE+1
    {
        u_num = white_range[i - 1].num;
        d_num = white_range[i].num;
        u_white = &white_range[i - 1];
        d_white = &white_range[i];
        i_u = 1; i_d = 1;

        //循环到当前行或上面行白条子数耗尽为止
        while (i_u <= u_num && i_d <= d_num)
        {
            //变量先保存，避免下面访问写的冗杂且访问效率低
            u_left = u_white->area[i_u].left;
            u_right = u_white->area[i_u].right;
            d_left = d_white->area[i_d].left;
            d_right = d_white->area[i_d].right;

            if (u_left <= d_right && u_right >= d_left)//如果两个白条联通
                f[find_f(u_white->area[i_u].connect_num)] = find_f(d_white->area[i_d].connect_num);//父节点连起来

            //当前算法规则，手推一下你就知道为啥这样了
            if (d_right > u_right)i_u++;
            if (d_right < u_right)i_d++;
            if (d_right == u_right) { i_u++; i_d++; }
        }
    }
}




////////////////////////////////////////////
//功能：图像处理主程序
//输入：
//输出：
//备注：
///////////////////////////////////////////
void image_main()
{
    search_white_range();
    find_all_connect();
    //find_road();
    /*到此处为止，我们已经得到了属于赛道的结构体数组my_road[CAMERA_H]*/
   // ordinary_two_line();
    //mmc_connect_num1();
    //get_mid_line();
    mmc_connect_num();


}


////////////////////////////////////////////
//功能：连通域
//输入：
//输出：
//备注：
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
    uint8 xxx = 0, yyy = 0;//灯的祖先的行数和当前行白条标记
    uint8 connect0 = 0;//灯的祖先标号
    uint8 connectmax = 0;//灯的白条数
    connectnum = 0;//连通域数
//    uint8 connectsum[1000];//祖先连接的白条数
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
        //printf("x:%f   y:%f  面积:%f \n", centx, centy, sss);
        //printf("%d\n", jud);
    }
    if(biaozhi==0)
    {
        connectnum=0;
    }
}




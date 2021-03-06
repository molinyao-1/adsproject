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
float lightx, lighty, lights, lightc;
float lightdis;
float camx=145, camy=87;

int yzhi;

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
            for (int i = 0; i < 120; i++)
            {
                my_map = &IMG[i][0];
                for (int j = 0; j < 188; j++)
                {
                    if ((*map) > yzhi)
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
    for (int i = 119; i >= 84; i--)
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
//功能：寻找赛道
//输入：
//输出：
//备注：
///////////////////////////////////////////
void find_road()
{
    uint8 istart = NEAR_LINE;
    uint8 iend = FAR_LINE;
    top_road = NEAR_LINE;//赛道最高处所在行数，先初始化话为最低处
    int road_f = -1;//赛道所在连通域父节点编号，先初始化为-1，以判断是否找到赛道
    int while_range_num = 0, roud_while_range_num = 0;
    all_range* twhite_range = NULL;
    road* tmy_road = NULL;
    //寻找赛道所在连通域
    // 寻找最中心的白条子
    for (int i = 1; i <= white_range[istart].num; i++)
        if (white_range[istart].area[i].left <= CAMERA_W / 2
            && white_range[istart].area[i].right >= CAMERA_W / 2 && (white_range[istart].area[i].right - white_range[istart].area[i].left) >= 90)
            road_f = find_f(white_range[istart].area[i].connect_num);

    if (road_f == -1)//若赛道没在中间，在113行选一行最长的认为这就是赛道
    {
        int widthmax = 0, jselect = 1;
        for (int i = 1; i <= white_range[istart].num; i++)
            if (white_range[istart].area[i].right - white_range[istart].area[i].left > widthmax)
            {
                widthmax = white_range[istart].area[i].right - white_range[istart].area[i].left;
                jselect = i;
            }
        road_f = find_f(white_range[istart].area[jselect].connect_num);
    }

    //现在我们已经得到了赛道所在连通域父节点编号，接下来把所有父节点编号是road_f的所有白条子扔进赛道数组就行了
    for (int i = istart; i >= iend; i--)
    {
        //变量保存，避免之后写的冗杂且低效
        twhite_range = &white_range[i];
        tmy_road = &my_road[i];
        while_range_num = twhite_range->num;
        tmy_road->white_num = 0;
        roud_while_range_num = 0;
        for (int j = 1; j <= while_range_num; j++)
        {
            if (find_f(twhite_range->area[j].connect_num) == road_f)
            {
                top_road = i;
                tmy_road->white_num++; roud_while_range_num++;
                tmy_road->connected[roud_while_range_num].left = twhite_range->area[j].left;
                tmy_road->connected[roud_while_range_num].right = twhite_range->area[j].right;
                tmy_road->connected[roud_while_range_num].width = twhite_range->area[j].right - twhite_range->area[j].left;

            }
        }
    }
}

////////////////////////////////////////////
//功能：返回相连下一行白条子编号
//输入：i_start起始行  j_start白条标号
//输出：白条标号
//备注：认为下一行与本行赛道重叠部分对多的白条为选定赛道
///////////////////////////////////////////

////////////////////////////////////////////
//功能：返回相连下一行白条子编号
//输入：i_start起始行  j_start白条标号
//输出：白条标号
//备注：认为下一行与本行赛道重叠部分对多的白条为选定赛道
///////////////////////////////////////////
uint8 find_continue(uint8 i_start, uint8 j_start)
{
    uint8 j_return;
    uint8 j;
    uint8 width_max = 0;
    uint8 width_new = 0;
    uint8 left = 0;
    uint8 right = 0;
    uint8 dright, dleft, uright, uleft;
    j_return = MISS;//如果没找到，输出255
    if (j_start > my_road[i_start].white_num)
        return MISS;
    //选一个重叠最大的
    for (j = 1; j <= my_road[i_start - 1].white_num; j++)
    {
        dleft = my_road[i_start].connected[j_start].left;
        dright = my_road[i_start].connected[j_start].right;
        uleft = my_road[i_start - 1].connected[j].left;
        uright = my_road[i_start - 1].connected[j].right;
        if (//相连
            dleft < uright
            &&
            dright > uleft
            )
        {
            //计算重叠大小
            if (dleft < uleft) left = uleft;
            else left = dleft;

            if (dright > uright) right = uright;
            else right = dright;

            width_new = right - left + 1;

            if (width_new > width_max)
            {
                width_max = width_new;
                j_return = j;
            }
        }

    }
    return j_return;
}

////////////////////////////////////////////
//功能：通用决定双边
//输入：
//输出：
//备注：
///////////////////////////////////////////
void ordinary_two_line(void)
{
    uint8 i;
    uint8 j;
    uint8 j_continue[CAMERA_H];//第一条连通路径
    uint8 i_start;
    uint8 i_end;
    uint8 j_start = MISS;
    int width_max;

    //寻找起始行最宽的白条子
    i_start = NEAR_LINE;
    i_end = FAR_LINE;
    width_max = 0;
    for (j = 1; j <= my_road[i_start].white_num; j++)
    {
        if (my_road[i_start].connected[j].width > width_max)
        {
            width_max = my_road[i_start].connected[j].width;
            j_start = j;
        }
    }
    j_continue[i_start] = j_start;

    //记录连贯区域编号
    for (i = i_start; i > i_end; i--)
    {
        //如果相连编号大于该行白条数，非正常，从此之后都MISS
        if (j_continue[i] > my_road[i].white_num)
        {
            j_continue[i - 1] = MISS;
        }
        else
        {
            j_continue[i - 1] = find_continue(i, j_continue[i]);
        }

    }

    //全部初始化为MISS
    my_memset(left_line, MISS, CAMERA_H);
    my_memset(right_line, MISS, CAMERA_H);


    for (i = i_start; i > i_end; i--)
    {
        if (j_continue[i] <= my_road[i].white_num)
        {
            left_line[i] = my_road[i].connected[j_continue[i]].left;
            right_line[i] = my_road[i].connected[j_continue[i]].right;
            //IMG[i][left_line[i]] = blue;
            //IMG[i][right_line[i]] = red;
        }
        else
        {
            //left_line[i] = MISS;
            //right_line[i] = MISS;
        }
    }
}

////////////////////////////////////////////
//功能：数组初始化
//输入：uint8* ptr 数组首地址, uint8 num初始化的值, uint8 size数组大小
//输出：
//备注：因为k66库中认为memset函数不安全，所以无法使用；因此需要自己写一个my_memset
///////////////////////////////////////////
void my_memset(uint8* ptr, uint8 num, uint8 size)
{
    uint8* p = ptr;
    uint8 my_num = num;
    uint8 Size = size;
    for (int i = 0; i < Size; i++, p++)
    {
        *p = my_num;
    }
}
////////////////////////////////////////////
//功能：中线合成
//输入：左右边界
//输出：中线
//备注：
///////////////////////////////////////////
void get_mid_line(void)
{
    my_memset(mid_line, MISS, CAMERA_H);
    for (int i = NEAR_LINE; i >= FAR_LINE; i--)
        if (left_line[i] != MISS)
        {
            mid_line[i] = (left_line[i] + right_line[i]) / 2;
        }
        else
        {
            mid_line[i] = MISS;
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
    biaozhi=0;
    float newx=0, newy = 187, news = 0, newdis = 0;
    int k = 0;
    lightdis = 0;
    lights = 0;
    lightx = 0;
    lighty =187;
    uint8 xxx = 0, yyy = 0;//灯的祖先的行数和当前行白条标记
    uint8 connect0 = 0;//灯的祖先标号
    uint8 connectmax = 0;//灯的白条数
    connectnum = 0;//连通域数
//    uint8 connectsum[1000];//祖先连接的白条数
    uint8 father[50] = {0};
    uint8 father1[50] = { 0 };
    uint8 judge[50] = { 0 };
//    for (int i = 1; i < 1000; i++)
//        connectsum[i] = 0;
/*  for (int i = 1; i <= all_connect_num; i++)
        if (f[i] == i)
        {
            connectnum++;
            father[connectnum] = i;
        }*/
    for (int i = 119; i >= 0; i--)
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


    for (int k = connectnum; k >= 1; k--)
    {
        uint8 up = 0, down = 0;
        uint8 maxleft = 188, maxright = 0;
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
        float counth = 0;
        float countw = 0;
        float counts = 0;
        counth = 0.0028 * dis * dis - 0.9254 * dis + 73.352;
        countw = -0.4112 * dis + 56.849;
        counts = 0.133 * dis * dis - 33.685 * dis + 2141.9;
        if (up < 10) jud = 1;
        if (down< 10) jud = 1;
        if (dis > 137) jud = 1;
        else if (dis > 120)
        {
            if (abs(counth - height) > 7) jud = 1;
            if (abs(countw - width) > 9) jud = 1;
            if (abs(counts - sss) > 50) jud = 1;
            if (sss > 40) jud = 1;
        }
        else if (dis > 100)
        {
            if (abs(counth - height) > 10) jud = 1;
            if (abs(countw - width) > 12) jud = 1;
            if (abs(counts - sss) > 300) jud = 1;
            if (sss > 600) jud = 1;
        }
        else if (dis > 80)
        {
            if (abs(counth - height) > 12) jud = 1;
            if (abs(countw - width) > 14) jud = 1;
            if (abs(counts - sss) > 400) jud = 1;
            if (sss > 1000) jud = 1;
        }
        else if (dis > 70)
        {
            if (abs(counth - height) > 15) jud = 1;
            if (abs(countw - width) > 17) jud = 1;
            if (abs(counts - sss) > 800) jud = 1;
            if (sss < 400) jud = 1;
        }
        else
        {
            if (sss < 300) jud = 1;
            if (height < 10) jud = 1;
            if (width < 10) jud = 1;
        }


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

//        int xxxx=lightx;
//        int yyyy=lighty;
//        for(int i=0;i<120;i++)
//        {
//            IMG[i][yyyy+1]=255;
//            IMG[i][yyyy]=255;
//            //for(int j=0;j<188;j++)
//        }
//        for(int j=0;j<188;j++)
//        {
//            IMG[xxxx][j]=255;
//            IMG[xxxx+1][j]=255;
//        }
        //printf("x:%f   y:%f  面积:%f \n", centx, centy, sss);
        //printf("%d\n", jud);
    }
    if(biaozhi==0)
    {
        connectnum=0;
    }
}




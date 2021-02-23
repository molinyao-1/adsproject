#include "menu.h"
/*
 * 一些定义
 * menu_node[i]为第i个菜单项（结构体中具体内容在menu。h文件有说明）
 * proj[i][j]为第i个界面的第j项所对应的菜单项结构体数组的位置
 * summ[i]为第i个界面的项目总和
 * p为当前界面光标位置
 * posi为界面位置
 * top与end为判定当前Qled显示的第一个项目和最后一个项目分别为该界面的第几项
 * putint[i]与putfloat[i]为转化flash所用数组
 * buff与Error为读写Flash所用的参量
 */
int proj[100][100],summ[100],p=0,posi=1,top=0,end=7,putint[100];
uint8 buff[4096], Error;
float putfloat[100];
struct menu menu_node[100];
//add
int page2 = 11;
int page3 = 42;
int page2num = 2;
int page3num = 2;

//addddd
int ichangeadd = 1;
float fchangeadd = 1;
int changej = 0;


void MENU_listmmc()
{
    MENU_add(30,0,2,"aaa");
    MENU_add(20, 0, 2, "mmcold");
    MENU_add(0, 1.18, 3, "mmcbirth");
}

/*
 * 建树，将信息存到树中
 * 注:只是采用树的思想，并没有完善的写好每个节点的关系
 */
void MENU_Tree()
{
   MENU_new();
}

/*
 *  初始化首界面并输出
 */
void MENU_Start()
{
    SmartCar_OLED_Fill(0);
    for(int i=0;i<8;i++)
    {
        SmartCar_OLED_P6x8Str(0,i," ");
    }
    SmartCar_OLED_P6x8Str(0,0,">");
    for(int i=0;i<summ[1];i++)
        SmartCar_OLED_P6x8Str(10,i,menu_node[proj[1][i+1]].data);
    SmartCar_OLED_P6x8Str(80,0,"1 /");
    SmartCar_OLED_Printf6x8(101,0,"%d",summ[1]);
}
void MENU_new()
    {
        summ[1] = 4;
        int q = 1;
        menu_node[1].data = "menu";
        for (int i = 1; i <= 4; i++)
        {
            q++;
           proj[1][i] = q;
        }
        menu_node[2].data = "int"; menu_node[2].jump = 2; menu_node[2].type = 1;
        menu_node[3].data = "float"; menu_node[3].jump = 3; menu_node[3].type = 2;
        menu_node[4].data = "shuju";
        menu_node[5].data = "storage";


        for (int i = 1; i <= 2; i++)
            proj[2][i] = i + 9;
        //10-40
        summ[2] = 2;
        menu_node[10].data = "back"; menu_node[10].jump = 1;
        //menu_node[11].data = "change1"; menu_node[11].dataint = 12345;
        //menu_node[12].data = "change2"; menu_node[12].dataint = -4321;
        menu_node[11].data = "x"; menu_node[11].dataint = 1;


        for (int i = 1; i <= 2; i++)
            proj[3][i] = i + 40;
        //41-70
        summ[3] = 2;
        menu_node[41].data = "back"; menu_node[41].jump = 1;
        //menu_node[42].data = "change3"; menu_node[42].datafloat = 12.34;
        //menu_node[43].data = "change4"; menu_node[43].datafloat = 4.321;
        menu_node[42].data = "x"; menu_node[42].datafloat = 1.00;

        MENU_listmmc();
    }
void MENU_add(int datint,float datfloat,int page,char *nam)
{
    if (page == 2)
    {
        summ[page]++;
        menu_node[page2].data = nam;
        menu_node[page2].dataint = 1234;
        menu_node[page2].dataj = 1;
        page2++;
        menu_node[page2].data = "x";
        menu_node[page2].dataint = 1;
        page2num++;
        proj[page][page2num] = page2;
    }
    else if (page == 3)
    {
        summ[page]++;
        menu_node[page3].data = nam;
        menu_node[page3].datafloat = 1.23;
        menu_node[page3].dataj = 2;
        page3++;
        menu_node[page3].data = "x";
        menu_node[page3].datafloat = 1;
        page3num++;
        proj[page][page3num] = page3;
    }

}


//adddd
void MENU_changeint(int changenum)
{
    ichangeadd = 1;
    SmartCar_OLED_Fill(0);
    SmartCar_OLED_Printf6x8(30, 3, "%d", changenum);
    SmartCar_OLED_Printf6x8(30, 5, "%d", ichangeadd );


}
void MENU_changefloat(float changenum)
{
    fchangeadd = 1;
    SmartCar_OLED_Fill(0);
    SmartCar_OLED_Printf6x8(30, 3, "%.4f", changenum);
    SmartCar_OLED_Printf6x8(30, 5, "%.4f", fchangeadd);
}
/*
 *  实现界面的跳转
 *  find(k)表示跳转到第k个界面
 */
void MENU_find(int k)
{
    SmartCar_OLED_Fill(0);
    for(int i=0;i<8;i++)
    {
        SmartCar_OLED_P6x8Str(0,i," ");
    }
    SmartCar_OLED_P6x8Str(0,0,">");
    SmartCar_OLED_P6x8Str(80,0,"1 /");
    SmartCar_OLED_Printf6x8(101,0,"%d",summ[k]);

    if(menu_node[k].type==0)
    {
        for(int i=1;i<=summ[k];i++)
        {
            SmartCar_OLED_P6x8Str(10,i-1,menu_node[proj[k][i]].data);
        }
    }
    else if(menu_node[k].type==1)
    {
        SmartCar_OLED_P6x8Str(10,0,menu_node[proj[k][1]].data);
        for(int i=2;i<summ[k];i++)
        {
            SmartCar_OLED_P6x8Str(10,i-1,menu_node[proj[k][i]].data);
            SmartCar_OLED_Printf6x8(80,i-1,"%d",menu_node[proj[k][i]].dataint);
        }
        SmartCar_OLED_P6x8Str(10,summ[k]-1,menu_node[proj[k][summ[k]]].data);
        SmartCar_OLED_Printf6x8(80,summ[k]-1,"%d",menu_node[proj[k][summ[k]]].dataint);
    }
    else if(menu_node[k].type==2)
    {
        SmartCar_OLED_P6x8Str(10,0,menu_node[proj[k][1]].data);
        for(int i=2;i<summ[k];i++)
        {
            SmartCar_OLED_P6x8Str(10,i-1,menu_node[proj[k][i]].data);
            SmartCar_OLED_Printf6x8(80,i-1,"%.4f",menu_node[proj[k][i]].datafloat);
        }
        SmartCar_OLED_P6x8Str(10,summ[k]-1,menu_node[proj[k][summ[k]]].data);
        SmartCar_OLED_Printf6x8(80,summ[k]-1,"%.4f",menu_node[proj[k][summ[k]]].datafloat);
    }
}

/*
 *  将按键操作封装，并添加延时函数
 *  其中t的值为2，4，5，6，8分别对应下，左，中，右，上
 *  即为数字键盘中各个数字的实际位置
 */
int MENU_keyScan(int mode)
{
    static int key_up=1;
    if(mode)key_up=1;
//    if(key_up&&(!GPIO_Read(GPIOE,7)||!GPIO_Read(GPIOE,8)||!GPIO_Read(GPIOE,9)||!GPIO_Read(GPIOE,10)||!GPIO_Read(GPIOE,11)))
//    {
//        SDK_DelayAtLeastUs(10*1000,180*1000*1000);
//        key_up=0;
//        if(!GPIO_Read(GPIOE,7))return 8;
//        else if(!GPIO_Read(GPIOE,8))return 4;
//        else if(!GPIO_Read(GPIOE,9))return 6;
//        else if(!GPIO_Read(GPIOE,10))return 5;
//        else if(!GPIO_Read(GPIOE,11))return 2;
//    }
//    else if(GPIO_Read(GPIOE,7)&&GPIO_Read(GPIOE,8)&&GPIO_Read(GPIOE,9)&&GPIO_Read(GPIOE,10)&&GPIO_Read(GPIOE,11))key_up=1;
    return 0;
}


/*
 * 按键后进行操作，并实时调用各类函数以实现菜单功能
 */
void MENU_Make()
{
    int t;
    t=MENU_keyScan(0);
        if(t==8)
        {
            if (changej == 0)
            {
                if (summ[posi] <= 8 && p < summ[posi] - 1)
                {
                    SmartCar_OLED_P6x8Str(0, p, " ");
                    p++;
                    if (p == 8) p = 0;
                    SmartCar_OLED_P6x8Str(0, p, ">");
                }
                else if (summ[posi] > 8)
                {
                    if (p >= end && p < summ[posi] - 1)
                    {
                        end++;
                        top++;
                        p++;
                        SmartCar_OLED_Fill(0);
                        SmartCar_OLED_P6x8Str(92, 0, "/");
                        SmartCar_OLED_Printf6x8(101, 0, "%d", summ[posi]);
                        SmartCar_OLED_P6x8Str(0, 7, ">");
                        for (int i = 1; i <= 8; i++)
                        {
                            SmartCar_OLED_P6x8Str(10, i - 1, menu_node[proj[posi][p + i - 7]].data);
                        }
                    }
                    else if (p < summ[posi] - 1 && p < end)
                    {
                        SmartCar_OLED_P6x8Str(0, p - top, " ");
                        p++;
                        SmartCar_OLED_P6x8Str(0, p - top, ">");
                    }
                }

                SmartCar_OLED_P6x8Str(80, 0, "  ");
                SmartCar_OLED_Printf6x8(80, 0, "%d", p + 1);
            }

            //////////

            else if (changej == 1)
            {
                menu_node[proj[posi][p + 1]].dataint -= ichangeadd;
                SmartCar_OLED_P6x8Str(30, 3, "       ");
                SmartCar_OLED_Printf6x8(30, 3, "%d", menu_node[proj[posi][p + 1]].dataint);
            }
            else if (changej == 2)
            {
                menu_node[proj[posi][p + 1]].datafloat -= fchangeadd;
                SmartCar_OLED_P6x8Str(30, 3, "       ");
                SmartCar_OLED_Printf6x8(30, 3, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
            }
        }
        if(t==2)
        {
            if (changej == 0)
            {
                if (summ[posi] <= 8 && p > 0)
                {
                    SmartCar_OLED_P6x8Str(0, p, " ");
                    p--;
                    SmartCar_OLED_P6x8Str(0, p, ">");
                }
                else if (summ[posi] > 8)
                {
                    if (p <= top && p > 0)
                    {
                        top--;
                        end--;
                        p--;
                        SmartCar_OLED_Fill(0);
                        SmartCar_OLED_P6x8Str(92, 0, "/");
                        SmartCar_OLED_Printf6x8(101, 0, "%d", summ[posi]);
                        SmartCar_OLED_P6x8Str(0, 0, ">");
                        for (int i = 1; i <= 8; i++)
                        {
                            SmartCar_OLED_P6x8Str(10, i - 1, menu_node[proj[posi][p + i]].data);
                        }
                    }
                    else if (p > top && p > 0)
                    {
                        SmartCar_OLED_P6x8Str(0, p - top, " ");
                        p--;
                        SmartCar_OLED_P6x8Str(0, p - top, ">");
                    }
                }

                SmartCar_OLED_P6x8Str(80, 0, "  ");
                SmartCar_OLED_Printf6x8(80, 0, "%d", p + 1);
            }


            ////////////////////
            else if (changej == 1)
            {
                menu_node[proj[posi][p + 1]].dataint += ichangeadd;
                SmartCar_OLED_P6x8Str(30, 3, "       ");
                SmartCar_OLED_Printf6x8(30, 3, "%d", menu_node[proj[posi][p + 1]].dataint);
            }
            else if (changej == 2)
            {
                menu_node[proj[posi][p + 1]].datafloat += fchangeadd;
                SmartCar_OLED_P6x8Str(30, 3, "       ");
                SmartCar_OLED_Printf6x8(30, 3, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
            }

        }
        if(t==5)
        {
            if (changej == 0)
            {
                if(proj[posi][p+1]!=0&&menu_node[proj[posi][p+1]].jump!=0)
                {
                    MENU_find(menu_node[proj[posi][p+1]].jump);
                    posi=menu_node[proj[posi][p+1]].jump;
                    p=0;
                    end=7;
                    top=0;
                }
                else if(menu_node[proj[posi][p+1]].data=="storage")
                {
                    MENU_storage();
                }
                //n//////////new
                else if (proj[posi][p + 1] != 0 && menu_node[proj[posi][p + 1]].dataj == 1)
                {
                    changej = 1;
                    MENU_changeint(menu_node[proj[posi][p + 1]].dataint);

                }
                else if (proj[posi][p + 1] != 0 && menu_node[proj[posi][p + 1]].dataj == 2)
                {
                    changej = 2;
                    MENU_changefloat(menu_node[proj[posi][p + 1]].datafloat);
                }
            }
            else if (changej != 0)
            {
                changej = 0;
                MENU_find(posi);
                p = 0;
                end = 7;
                top = 0;
            }

        }

        if(t==6)
        {
            if (changej == 0)
            {
                if (menu_node[posi].type == 1 && p != 0)
                {
                    if (p != summ[posi] - 1)
                    {
                        SmartCar_OLED_P6x8Str(80, p, "       ");
                        menu_node[proj[posi][p + 1]].dataint += menu_node[proj[posi][summ[posi]]].dataint;
                        SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
                    }
                    else
                    {
                        if (menu_node[proj[posi][p + 1]].dataint <= 1000)
                        {
                            SmartCar_OLED_P6x8Str(80, p, "       ");
                            if (menu_node[proj[posi][p + 1]].dataint != 0)  menu_node[proj[posi][p + 1]].dataint *= 10;
                            else menu_node[proj[posi][p + 1]].dataint++;
                            SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
                        }
                    }
                }
                else if (menu_node[posi].type == 2 && p != 0)
                {
                    if (p != summ[posi] - 1)
                    {
                        SmartCar_OLED_P6x8Str(80, p, "       ");
                        menu_node[proj[posi][p + 1]].datafloat += menu_node[proj[posi][summ[posi]]].datafloat;
                        SmartCar_OLED_Printf6x8(80, p, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
                    }
                    else
                    {
                        if (menu_node[proj[posi][p + 1]].datafloat <= 100)
                        {
                            SmartCar_OLED_P6x8Str(80, p, "       ");
                            if (menu_node[proj[posi][p + 1]].datafloat > 0) menu_node[proj[posi][p + 1]].datafloat *= 10;
                            else menu_node[proj[posi][p + 1]].datafloat = 1;
                            SmartCar_OLED_Printf6x8(80, p, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
                        }
                    }
                }
            }


            /////////////////////////////////////
            else if (changej == 1)
            {
                if (ichangeadd < 10000) ichangeadd *= 10;
                SmartCar_OLED_P6x8Str(30, 5, "      ");
                SmartCar_OLED_Printf6x8(30, 5, "%d", ichangeadd);


            }
            else if (changej == 2)
            {
                if (fchangeadd < 1000) fchangeadd *= 10;
                SmartCar_OLED_P6x8Str(30, 5, "      ");
                SmartCar_OLED_Printf6x8(30, 5, "%.4f", fchangeadd);
            }



        }

        if(t==4)
        {
            if (changej == 0)
            {
                if (menu_node[posi].type == 1 && p != 0)
                {
                    if (p != summ[posi] - 1 && menu_node[proj[posi][p + 1]].dataint > menu_node[proj[posi][summ[posi]]].dataint)
                    {
                        menu_node[proj[posi][p + 1]].dataint -= menu_node[proj[posi][summ[posi]]].dataint;
                        SmartCar_OLED_P6x8Str(80, p, "       ");
                        SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
                    }
                    else if (p == summ[posi] - 1)
                    {
                        if (menu_node[proj[posi][p + 1]].dataint > 1)
                        {
                            SmartCar_OLED_P6x8Str(80, p, "       ");
                            menu_node[proj[posi][p + 1]].dataint /= 10;
                            SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
                        }
                        else if (menu_node[proj[posi][p + 1]].dataint == 0)
                        {
                            SmartCar_OLED_P6x8Str(80, p, "       ");
                            menu_node[proj[posi][p + 1]].dataint = 1;
                            SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
                        }
                    }
                }
                else if (menu_node[posi].type == 2 && p != 0)
                {
                    if (p != summ[posi] - 1 && menu_node[proj[posi][p + 1]].datafloat > menu_node[proj[posi][summ[posi]]].datafloat)
                    {
                        SmartCar_OLED_P6x8Str(80, p, "       ");
                        menu_node[proj[posi][p + 1]].datafloat -= menu_node[proj[posi][summ[posi]]].datafloat;
                        SmartCar_OLED_Printf6x8(80, p, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
                    }
                    else if (p == summ[posi] - 1)
                    {
                        if (menu_node[proj[posi][p + 1]].datafloat > 0.0001)
                        {
                            SmartCar_OLED_P6x8Str(80, p, "       ");
                            menu_node[proj[posi][p + 1]].datafloat /= 10;
                            SmartCar_OLED_Printf6x8(80, p, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
                        }
                    }
                }
            }

            /////////////////////////////////////
            else if (changej == 1)
            {
                if (ichangeadd > 1) ichangeadd /= 10;
                SmartCar_OLED_P6x8Str(30, 5, "      ");
                SmartCar_OLED_Printf6x8(30, 5, "%d", ichangeadd);
            }
            else if (changej == 2)
            {
                if (fchangeadd > 0.0001) fchangeadd /= 10;
                SmartCar_OLED_P6x8Str(30, 5, "      ");
                SmartCar_OLED_Printf6x8(30, 5, "%.4f", fchangeadd);
            }
        }
}
/*
 * 从Flash中读取数据
 */
void MENU_Read()
{
    for(int i = 0;i<4;i++)
    {
        for(int j = 0;j<1024;j++)
        {
            buff[1024*i+j] = Page_Read(i,j,uint8);
        }
    }
    memcpy(putint,&buff[0],100*sizeof(int));
    memcpy(putfloat,&buff[200],100*sizeof(float));
    for(int i=0;i<=100;i++)
    {
        menu_node[i].dataint=putint[i];
        menu_node[i].datafloat=putfloat[i];
    }
}

/*
 * 将数据写入Flash
 */
void MENU_storage()
{
    for(int i=0;i<=100;i++)
    {
        putint[i]=menu_node[i].dataint;
        putfloat[i]=menu_node[i].datafloat;
    }
    memcpy(&buff[0],putint,100*sizeof(int));
    memcpy(&buff[200],putfloat,100*sizeof(float));
    for(int j = 0;j<4;j++)
    {
        Sector_Erase(j);
    }
    for(int k = 0;k<4;k++)
    {
        uint32 buff1[1024] = {0};
        for(int w = 0;w<1024;w++)
        {
            buff1[w] = buff[1024*k+w];
        }
        Sector_Program(k,buff1);
    }
}

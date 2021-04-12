#include "menu.h"
/*
 * һЩ����
 * menu_node[i]Ϊ��i���˵���ṹ���о���������menu��h�ļ���˵����
 * proj[i][j]Ϊ��i������ĵ�j������Ӧ�Ĳ˵���ṹ�������λ��
 * summ[i]Ϊ��i���������Ŀ�ܺ�
 * pΪ��ǰ������λ��
 * posiΪ����λ��
 * top��endΪ�ж���ǰQled��ʾ�ĵ�һ����Ŀ�����һ����Ŀ�ֱ�Ϊ�ý���ĵڼ���
 * putint[i]��putfloat[i]Ϊת��flash��������
 * buff��ErrorΪ��дFlash���õĲ���
 */
int proj[10][20],summ[100],p=0,posi=1,top=0,end=7,putint[100];
uint8 buff[1024], Error;
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

int freshposi;
int freshname;

extern float slowladjust; //(�˵�����)
extern int speedmostfast;
extern int slowspeedwant;
extern int mode0limit;
extern int correctlimit;
extern int LOWplus;
extern int reshunlimit;
extern float servokp ; //(�˵�����
extern float servokd ;
extern float motorbkp_R;
extern float motorbki_R;
extern float motorbkp_L;
extern float motorbki_L;
extern float klA_adjust;
extern float krA_adjust;
extern float correctmenu;

extern float correctspeed;
extern int servoduty;
extern float servocorrect;
extern float batterylimit;

void Menu_gpioinit()
{
    GPIO_Init(P20,10,PULLUP,1);
    GPIO_Init(P20,11,PULLUP,1);
    GPIO_Init(P20,12,PULLUP,1);
    GPIO_Init(P20,13,PULLUP,1);
    GPIO_Init(P20,14,PULLUP,1);
}



/*
 * �˵������
 */
void MENU_listmmc()
{
    MENU_add(speedmostfast, 0, 2, "speedmostfast");
    MENU_add(slowspeedwant, 0, 2, "slowspeedwant");
    MENU_add(mode0limit, 0, 2, "mode0limit");
    MENU_add(correctlimit, 0, 2, "correctlimit");
    MENU_add(LOWplus, 0, 2, "LOWplus");
    MENU_add(reshunlimit, 0, 2, "reshunlimit");

    //    MENU_add(20, 0, 2, "mmld");
//    MENU_add(20, 0, 2, "mldfd");
//    MENU_add(20, 0, 2, "mmdfld");

    MENU_add(0, servokp, 3, "servokp");
    MENU_add(0, servokd, 3, "servokd");
    MENU_add(0, motorbkp_R, 3, "motorbkp_R");
    MENU_add(0, motorbki_R, 3, "motorbki_R");
    MENU_add(0, motorbkp_L, 3, "motorbkp_L");
    MENU_add(0, motorbki_L, 3, "motorbki_L");
//    MENU_add(0, slowladjust, 3, "slowladjust");
    MENU_add(0, klA_adjust, 3, "klA_adjust");
    MENU_add(0, krA_adjust, 3, "krA_adjust");
    MENU_add(0, correctmenu, 3, "correctmenu");
    MENU_add(0,correctspeed,3, "correctspeed");
    MENU_add(0,servocorrect,3, "servocorrect");
    MENU_add(0,batterylimit,3, "batterylimit");



}



/*
 * ���ݶ�Ӧ
 */
void MENU_give()
{
    speedmostfast=menu_node[11].dataint;
    slowspeedwant=menu_node[12].dataint;
    mode0limit=menu_node[13].dataint;
    correctlimit=menu_node[14].dataint;
    LOWplus=menu_node[15].dataint;
    reshunlimit=menu_node[16].dataint;

    servokp=menu_node[42].datafloat;
    servokd=menu_node[43].datafloat;
    motorbkp_R=menu_node[44].datafloat;
    motorbki_R=menu_node[45].datafloat;
    motorbkp_L=menu_node[46].datafloat;
    motorbki_L=menu_node[47].datafloat;
//    slowladjust = menu_node[48].datafloat;
    klA_adjust = menu_node[48].datafloat;
    krA_adjust = menu_node[49].datafloat;
    correctmenu = menu_node[50].datafloat;
    correctspeed = menu_node[51].datafloat;
    servocorrect = menu_node[52].datafloat;
    batterylimit = menu_node[53].datafloat;


}

/*
 * ����������Ϣ�浽����
 * ע:ֻ�ǲ�������˼�룬��û�����Ƶ�д��ÿ���ڵ�Ĺ�ϵ
 */
void MENU_Tree()
{
   MENU_new();
}

/*
 *  ��ʼ���׽��沢���
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
    //SmartCar_OLED_P6x8Str(80,0,"1 /");
    //SmartCar_OLED_Printf6x8(101,0,"%d",summ[1]);
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
        //menu_node[11].data = "x"; menu_node[11].dataint = 1;


        for (int i = 1; i <= 2; i++)
            proj[3][i] = i + 40;
        //41-70
        summ[3] = 2;
        menu_node[41].data = "back"; menu_node[41].jump = 1;
        //menu_node[42].data = "change3"; menu_node[42].datafloat = 12.34;
        //menu_node[43].data = "change4"; menu_node[43].datafloat = 4.321;
        //menu_node[42].data = "x"; menu_node[42].datafloat = 1.00;

        MENU_listmmc();
    }
void MENU_add(int datint,float datfloat,int page,char *nam)
{
    if (page == 2)
    {
        summ[page]++;
        menu_node[page2].data = nam;
        menu_node[page2].dataint = datint;
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
        menu_node[page3].datafloat = datfloat;
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
 *  ʵ�ֽ������ת
 *  find(k)��ʾ��ת����k������
 */
void MENU_find(int k)
{
    SmartCar_OLED_Fill(0);
//    for(int i=0;i<8;i++)
//    {
//        SmartCar_OLED_P6x8Str(0,i," ");
//    }
    SmartCar_OLED_P6x8Str(0,0,">");
//    SmartCar_OLED_P6x8Str(80,0,"1 /");
//    SmartCar_OLED_Printf6x8(101,0,"%d",summ[k]);

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
 *  ������������װ���������ʱ����
 *  ����t��ֵΪ2��4��5��6��8�ֱ��Ӧ�£����У��ң���
 *  ��Ϊ���ּ����и������ֵ�ʵ��λ��
 */
int MENU_keyScan(int mode)
{
    static int key_up=1;
    if(mode)key_up=1;
    if(key_up&&(!GPIO_Read(P20,10)||!GPIO_Read(P20,11)||!GPIO_Read(P20,12)||!GPIO_Read(P20,13)||!GPIO_Read(P20,14)))
    {
        Delay_ms(STM0,10);
        key_up=0;
        if(!GPIO_Read(P20,11))return 8;
        else if(!GPIO_Read(P20,12))return 4;
        else if(!GPIO_Read(P20,13))return 6;
        else if(!GPIO_Read(P20,10))return 5;
        else if(!GPIO_Read(P20,14))return 2;
    }
    else if(GPIO_Read(P20,10)&&GPIO_Read(P20,11)&&GPIO_Read(P20,12)&&GPIO_Read(P20,13)&&GPIO_Read(P20,14))key_up=1;
    return 0;
}



/*
 * ��������в�������ʵʱ���ø��ຯ����ʵ�ֲ˵�����
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
                        //SmartCar_OLED_P6x8Str(92, 0, "/");
                        //SmartCar_OLED_Printf6x8(101, 0, "%d", summ[posi]);
                        SmartCar_OLED_P6x8Str(0, 7, ">");
                        for (int i = 1; i <= 8; i++)
                        {
                            SmartCar_OLED_P6x8Str(10, i - 1, menu_node[proj[posi][p + i - 7]].data);
                        }
                        if(menu_node[posi].type==1)
                        {
                            for(int i=1;i<=8;i++)
                            {
                                if(menu_node[proj[posi][p + i - 7]].data!="back")
                                    SmartCar_OLED_Printf6x8(80,i-1,"%d",menu_node[proj[posi][p + i - 7]].dataint);
                            }
                        }
                        else if(menu_node[posi].type==2)
                        {
                            for(int i=1;i<=8;i++)
                            {
                                if(menu_node[proj[posi][p + i - 7]].data!="back")
                                    SmartCar_OLED_Printf6x8(80,i-1,"%.4f",menu_node[proj[posi][p + i - 7]].datafloat);
                            }
                        }
                    }
                    else if (p < summ[posi] - 1 && p < end)
                    {
                        SmartCar_OLED_P6x8Str(0, p - top, " ");
                        p++;
                        SmartCar_OLED_P6x8Str(0, p - top, ">");
                    }
                }

                //SmartCar_OLED_P6x8Str(80, 0, "  ");
                //SmartCar_OLED_Printf6x8(80, 0, "%d", p + 1);
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
//                        SmartCar_OLED_P6x8Str(92, 0, "/");
//                        SmartCar_OLED_Printf6x8(101, 0, "%d", summ[posi]);
                        SmartCar_OLED_P6x8Str(0, 0, ">");
                        for (int i = 1; i <= 8; i++)
                        {
                            SmartCar_OLED_P6x8Str(10, i - 1, menu_node[proj[posi][p + i]].data);
                        }

                        if(menu_node[posi].type==1)
                        {
                            for(int i=1;i<=8;i++)
                            {
                                if(menu_node[proj[posi][p + i]].data!="back")
                                    SmartCar_OLED_Printf6x8(80,i-1,"%d",menu_node[proj[posi][p + i]].dataint);
                            }
                        }
                        else if(menu_node[posi].type==2)
                        {
                            for(int i=1;i<=8;i++)
                            {
                                if(menu_node[proj[posi][p + i]].data!="back")
                                    SmartCar_OLED_Printf6x8(80,i-1,"%.4f",menu_node[proj[posi][p + i]].datafloat);
                            }
                        }


                    }
                    else if (p > top && p > 0)
                    {
                        SmartCar_OLED_P6x8Str(0, p - top, " ");
                        p--;
                        SmartCar_OLED_P6x8Str(0, p - top, ">");
                    }
                }

                //SmartCar_OLED_P6x8Str(80, 0, "  ");
                //SmartCar_OLED_Printf6x8(80, 0, "%d", p + 1);
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
//            if (changej == 0)
//            {
//                if (menu_node[posi].type == 1 && p != 0)
//                {
//                    if (p != summ[posi] - 1)
//                    {
//                        SmartCar_OLED_P6x8Str(80, p, "       ");
//                        menu_node[proj[posi][p + 1]].dataint += menu_node[proj[posi][summ[posi]]].dataint;
//                        SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
//                    }
//                    else
//                    {
//                        if (menu_node[proj[posi][p + 1]].dataint <= 1000)
//                        {
//                            SmartCar_OLED_P6x8Str(80, p, "       ");
//                            if (menu_node[proj[posi][p + 1]].dataint != 0)  menu_node[proj[posi][p + 1]].dataint *= 10;
//                            else menu_node[proj[posi][p + 1]].dataint++;
//                            SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
//                        }
//                    }
//                }
//                else if (menu_node[posi].type == 2 && p != 0)
//                {
//                    if (p != summ[posi] - 1)
//                    {
//                        SmartCar_OLED_P6x8Str(80, p, "       ");
//                        menu_node[proj[posi][p + 1]].datafloat += menu_node[proj[posi][summ[posi]]].datafloat;
//                        SmartCar_OLED_Printf6x8(80, p, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
//                    }
//                    else
//                    {
//                        if (menu_node[proj[posi][p + 1]].datafloat <= 100)
//                        {
//                            SmartCar_OLED_P6x8Str(80, p, "       ");
//                            if (menu_node[proj[posi][p + 1]].datafloat > 0) menu_node[proj[posi][p + 1]].datafloat *= 10;
//                            else menu_node[proj[posi][p + 1]].datafloat = 1;
//                            SmartCar_OLED_Printf6x8(80, p, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
//                        }
//                    }
//                }
//            }
//

            /////////////////////////////////////
            if (changej == 1)
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
//            if (changej == 0)
//            {
//                if (menu_node[posi].type == 1 && p != 0)
//                {
//                    if (p != summ[posi] - 1 && menu_node[proj[posi][p + 1]].dataint > menu_node[proj[posi][summ[posi]]].dataint)
//                    {
//                        menu_node[proj[posi][p + 1]].dataint -= menu_node[proj[posi][summ[posi]]].dataint;
//                        SmartCar_OLED_P6x8Str(80, p, "       ");
//                        SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
//                    }
//                    else if (p == summ[posi] - 1)
//                    {
//                        if (menu_node[proj[posi][p + 1]].dataint > 1)
//                        {
//                            SmartCar_OLED_P6x8Str(80, p, "       ");
//                            menu_node[proj[posi][p + 1]].dataint /= 10;
//                            SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
//                        }
//                        else if (menu_node[proj[posi][p + 1]].dataint == 0)
//                        {
//                            SmartCar_OLED_P6x8Str(80, p, "       ");
//                            menu_node[proj[posi][p + 1]].dataint = 1;
//                            SmartCar_OLED_Printf6x8(80, p, "%d", menu_node[proj[posi][p + 1]].dataint);
//                        }
//                    }
//                }
//                else if (menu_node[posi].type == 2 && p != 0)
//                {
//                    if (p != summ[posi] - 1 && menu_node[proj[posi][p + 1]].datafloat > menu_node[proj[posi][summ[posi]]].datafloat)
//                    {
//                        SmartCar_OLED_P6x8Str(80, p, "       ");
//                        menu_node[proj[posi][p + 1]].datafloat -= menu_node[proj[posi][summ[posi]]].datafloat;
//                        SmartCar_OLED_Printf6x8(80, p, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
//                    }
//                    else if (p == summ[posi] - 1)
//                    {
//                        if (menu_node[proj[posi][p + 1]].datafloat > 0.0001)
//                        {
//                            SmartCar_OLED_P6x8Str(80, p, "       ");
//                            menu_node[proj[posi][p + 1]].datafloat /= 10;
//                            SmartCar_OLED_Printf6x8(80, p, "%.4f", menu_node[proj[posi][p + 1]].datafloat);
//                        }
//                    }
//                }
//            }

            /////////////////////////////////////
            if (changej == 1)
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
 * ��Flash�ж�ȡ����
 */
void MENU_Read()
{
    for(int i = 0;i<1;i++)
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
    MENU_give();
}

/*
 * ������д��Flash
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
    for(int k = 0;k<1;k++)
    {
        uint32 buff1[1024] = {0};
        for(int w = 0;w<1024;w++)
        {
            buff1[w] = buff[1024*k+w];
        }
        Sector_Program(k,buff1);
    }
    MENU_give();
}

void MENU_freshint(char *nam,int numm)
{
    if(freshposi==0&&freshname==0)    SmartCar_OLED_Fill(0);
    if (freshname==0)   SmartCar_OLED_P6x8Str(5,freshposi,nam);
    SmartCar_OLED_P6x8Str(60, freshposi, "            ");
    SmartCar_OLED_Printf6x8(60,freshposi,"%d",numm);
    freshposi++;
}
void MENU_freshfloat(char *nam,float numm)
{
    if(freshposi==0&&freshname==0)    SmartCar_OLED_Fill(0);
    if(freshname==0)   SmartCar_OLED_P6x8Str(5,freshposi,nam);
    SmartCar_OLED_P6x8Str(60, freshposi, "            ");
    SmartCar_OLED_Printf6x8(60,freshposi,"%.4f",numm);
    freshposi++;
}


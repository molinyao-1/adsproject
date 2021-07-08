#include "SmartCar_Isr.h"
#include "SmartCar_Eru.h"
#include "beacon_control.h"
#include "tfmini.h"

extern int mode;
extern int lightx2;
extern int lighty2;
uint8 movecnt = 0;
uint8 cnt;
uint8 tempbuff[10] = {0};

uint8 tempbuff1[15] = {0};
uint8 tempbuff2;


uint8 Dist_L;
uint8 Dist_H;
int Dist;

uint8 Strength_L;
uint8 Strength_H;
int Strength;
uint8 statustf;
extern TFmini_Data_t tfminiData;
extern uint8 TFminiBuffer[9];
extern uint8 TFmini_receive_flag;


int ab = 0;
extern int tfmini[4];
extern int position2[4];
extern int lightx2;
extern int lighty2;
extern int area2 ;
extern int dis2 ;
extern uint8 connectnumB;//连通域数
extern float time;
extern float myvar[20];
            /*pwm有关中断*/



            /*摄像头有关中断*/
IFX_INTERRUPT(eru_ch3_ch7_isr, 0, ERU_CH3_CH7_INT_PRIO)
{
    enableInterrupts();//开启中断嵌套
    if(GET_GPIO_FLAG(CH3_P02_0))//通道3中断,摄像头场中断
    {
        CLEAR_GPIO_FLAG(CH3_P02_0);
        if      (3 == camera_type)  MT9V034_VSYNC();
        //else if (1 == camera_type)  ;

    }
    if(GET_GPIO_FLAG(CH7_P20_9))//通道7中断
    {
        CLEAR_GPIO_FLAG(CH7_P20_9);
//        R_time++;

    }
}

IFX_INTERRUPT(eru_ch0_ch4_isr, 0, ERU_CH0_CH4_INT_PRIO)
{
    enableInterrupts();//开启中断嵌套
    if(GET_GPIO_FLAG(CH0_P15_4))//通道1中断
    {
        CLEAR_GPIO_FLAG(CH0_P15_4);
    }
    if(GET_GPIO_FLAG(CH4_P33_7))//通道5中断
    {
        CLEAR_GPIO_FLAG(CH4_P33_7);
        mode++;
        if(mode == 3)
        {
            mode = 0;
        }
    }
}
IFX_INTERRUPT(eru_ch1_ch5_isr, 0, ERU_CH1_CH5_INT_PRIO)
{
    enableInterrupts();//开启中断嵌套
    if(GET_GPIO_FLAG(CH1_P10_8))//通道1中断
    {
        CLEAR_GPIO_FLAG(CH1_P10_8);
    }
    if(GET_GPIO_FLAG(CH5_P15_8))//通道5中断
    {
        CLEAR_GPIO_FLAG(CH5_P15_8);
    }
}

IFX_INTERRUPT(dma_ch5_isr, 0, ERU_DMA_INT_PRIO)
{
    enableInterrupts();//开启中断嵌套

    if      (3 == camera_type)  MT9V034_DMA();
    //else if (1 == camera_type)  ov7725_dma();
    if(1 == mt9v034_finish_flag)
    {
        mt9v034_finish_flag = 0;
        //Systick_Start(STM0);
        THRE();
        image_main();
        CAMERA_JUDGE();

        //ab = ~ab;
        //GPIO_Set(P21,7,ab);
    }
}
            /*spi有关dma中断*/
IFX_INTERRUPT(qspi0DmaTxISR, 0, IFX_INTPRIO_DMA_CH1 )
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxQspi_SpiMaster_isrDmaTransmit(&oled_spi);
}

IFX_INTERRUPT(qspi0DmaRxISR, 0, IFX_INTPRIO_DMA_CH2)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxQspi_SpiMaster_isrDmaReceive(&oled_spi);
}

IFX_INTERRUPT(qspi0ErISR, 0, IFX_INTPRIO_QSPI0_ER)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxQspi_SpiMaster_isrError(&oled_spi);

    // Process errors. Eg: parity Error is checked below
    //IfxQspi_SpiMaster_Channel *chHandle   = IfxQspi_SpiMaster_activeChannel(&my_spi);
//    if( chHandle->errorFlags.parityError == 1)
//    {
//        // Parity Error
//    }
}


            /*asc有关中断*/
    /*触发中断服务函数，asclin0TxISR asclin0RxISR asclin0ErISR*/
/*uart0，用作上位机以及重定向的printf*/
IFX_INTERRUPT(asclin0TxISR, 0, IFX_INTPRIO_ASCLIN0_TX)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart[0]);
}

IFX_INTERRUPT(asclin0RxISR, 0, IFX_INTPRIO_ASCLIN0_RX)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart[0]);
//    if(uart[0].rxSwFifoOverflow == TRUE)
//    {
//        /*双单片机通信*/
//        SmartCar_Uart_Queue(tempbuff + movecnt,0);
//        if(*(tempbuff+movecnt) == '$'&&*(tempbuff+movecnt-3) == '@')
//        {
//            for(int i = 1;i<3;i++)
//            {
//                position2[i-1] = *(tempbuff+movecnt-i);
//            }
//            movecnt = 0;
//            memset(tempbuff, 0, sizeof(tempbuff));
//            lightx2 = position2[1];
//            lighty2 = position2[0];
//        }
//        if(movecnt>=7)
//        {
//            movecnt = 0;
//            memset(tempbuff, 0, sizeof(tempbuff));
//        }
//        ++movecnt;
//        /**/
//    }
//    TFmini_UARTCallBack();
//    TFmini_getData(&tfminiData);
//    statustf = TFmini_statusDetect(&tfminiData);

}

IFX_INTERRUPT(asclin0ErISR, 0, IFX_INTPRIO_ASCLIN0_ER)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart[0]);
}

IFX_INTERRUPT(asclin1TxISR, 0, IFX_INTPRIO_ASCLIN1_TX)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart[1]);
}

IFX_INTERRUPT(asclin1RxISR, 0, IFX_INTPRIO_ASCLIN1_RX)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart[1]);
    MT9V034_Uart_Callback();
}

IFX_INTERRUPT(asclin1ErISR, 0, IFX_INTPRIO_ASCLIN1_ER)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart[1]);
}

IFX_INTERRUPT(asclin2TxISR, 0, IFX_INTPRIO_ASCLIN2_TX)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart[2]);
}

IFX_INTERRUPT(asclin2RxISR, 0, IFX_INTPRIO_ASCLIN2_RX)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart[2]);
}

IFX_INTERRUPT(asclin2ErISR, 0, IFX_INTPRIO_ASCLIN2_ER)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart[2]);
}

IFX_INTERRUPT(asclin3TxISR, 0, IFX_INTPRIO_ASCLIN3_TX)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrTransmit(&uart[3]);
}

IFX_INTERRUPT(asclin3RxISR, 0, IFX_INTPRIO_ASCLIN3_RX)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrReceive(&uart[3]);
    TC212_UARTCallBack();
}

IFX_INTERRUPT(asclin3ErISR, 0, IFX_INTPRIO_ASCLIN3_ER)
{
    IfxCpu_enableInterrupts();//开启中断嵌套
    IfxAsclin_Asc_isrError(&uart[3]);
}

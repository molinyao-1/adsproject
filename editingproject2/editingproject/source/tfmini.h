#include "SmartCar_Uart.h"
#include "IfxAsclin_Asc.h"
#include "Ifx_Types.h"
#include <stdint.h>

#define OVEREXPOSURE (uint16)(65535)
#define UARTNUM (3)

extern IfxAsclin_Asc uart[4];

struct TFMINIDATA
{
    uint16 distance;
    uint16 strength;
    uint8 isReadSuccess;
    uint8 isConfident;
};
typedef struct TFMINIDATA TFmini_Data_t;

extern TFmini_Data_t tfminiData;
extern uint8 TFminiBuffer[9];
extern uint8 TFmini_receive_flag;

/**
 *  初始化激光测距
 **/
void TFmini_init(void);

/*
 * 获得UART缓存
 */
boolean TFmini_uartRead(uint8* txData, Ifx_SizeT count);

/*
 * 获得测距结构体
 */
void TFmini_getData(TFmini_Data_t* TFminiHandle);

/*
 * 获得距离
 */
#define TFmini_getDistance(TFminiHandle) (TFminiHandle.distance)
#define TFmini_getStrength(TFminiHandle) (TFminiHandle.strength)
#define TFmini_getConfident(TFminiHandle) (TFminiHandle.isConfident)

/*
 * 状态识别
 * 返回值  |         0         |       1      |     2       |      3       |
 * 状态：  |   >=100cm  |50-100cm| <=50cm| 无信号  |
 */
uint8 TFmini_statusDetect(TFmini_Data_t* TFminiHandle);

/*
 *
 */
uint8 TFmini_Uart_Queue(uint8* rxData, uint8 uart_num);
/*
 * UART中断回调
 */
void TFmini_UARTCallBack(void);
void TC212_UARTCallBack(void);


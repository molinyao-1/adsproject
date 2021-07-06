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
 *  ��ʼ��������
 **/
void TFmini_init(void);

/*
 * ���UART����
 */
boolean TFmini_uartRead(uint8* txData, Ifx_SizeT count);

/*
 * ��ò��ṹ��
 */
void TFmini_getData(TFmini_Data_t* TFminiHandle);

/*
 * ��þ���
 */
#define TFmini_getDistance(TFminiHandle) (TFminiHandle.distance)
#define TFmini_getStrength(TFminiHandle) (TFminiHandle.strength)
#define TFmini_getConfident(TFminiHandle) (TFminiHandle.isConfident)

/*
 * ״̬ʶ��
 * ����ֵ  |         0         |       1      |     2       |      3       |
 * ״̬��  |   >=100cm  |50-100cm| <=50cm| ���ź�  |
 */
uint8 TFmini_statusDetect(TFmini_Data_t* TFminiHandle);

/*
 *
 */
uint8 TFmini_Uart_Queue(uint8* rxData, uint8 uart_num);
/*
 * UART�жϻص�
 */
void TFmini_UARTCallBack(void);
void TC212_UARTCallBack(void);


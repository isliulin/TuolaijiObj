//******************************************************************************
// author：        ap
// date:           2016/1/14
//******************************************************************************

#ifndef __ap_S32K1XX_CAN_H_
#define __ap_S32K1XX_CAN_H_

#include "S32K144.h"
#include "arm_cm4.h"

//宏定义
#define CAN0_SEND_IMB			10
#define CAN0_REV_IMB			1
#define CAN1_SEND_IMB			10
#define CAN1_REV_IMB			1
#define CAN2_SEND_IMB			0
#define CAN2_REV_IMB			1

	


#define NUMBER_OF_MB 16
#define swap_4bytes(ptr)	{ uint8_t	byte; \
								byte = (ptr)[0]; (ptr)[0] = (ptr)[3]; (ptr)[3]=byte; \
								byte = (ptr)[1]; (ptr)[1] = (ptr)[2]; (ptr)[2]=byte; \
							}

//CAN报文结构位定义
#define	CAN_MSG_IDE_MASK	(0x80000000L)
#define CAN_MSG_IDE_BIT_NO	(31)
#define CAN_MSG_ID_EXTENDED	(CAN_MSG_ID_MASK)
#define CAN_MSG_ID_STANDARD	(0)
#define CAN_MSG_TYPE_MASK 	(0x40000000L)
#define CAN_MSG_TYPE_BIT_NO	(30)
#define CAN_MSG_TYPE_DATA_FRAME	(CAN_MSG_TYPE_MASK)
#define CAN_MSG_TYPE_REMOTE_FRAME	(0)

//FELEX_MB_CS的位定义和宏
#define FLEXCAN_MB_CS_TIMESTAMP(x)    (((x)&0x0000FFFF)<<0)
#define FLEXCAN_MB_CS_TIMESTAMP_MASK  (0x0000FFFFL)
#define FLEXCAN_MB_CS_LENGTH(x)       (((x)&0x0000000F)<<16)
#define FLEXCAN_MB_CS_RTR             (0x00100000)
#define FLEXCAN_MB_CS_IDE             (0x00200000)
#define FLEXCAN_MB_CS_SRR             (0x00400000)
#define FLEXCAN_MB_CS_CODE(x)         (((x)&0x0000000F)<<24)
#define FLEXCAN_MB_CS_CODE_MASK	      (0x0F000000L)
#define FLEXCAN_MB_CS_DLC_MASK	      (0x000F0000L)
#define FLEXCAN_MB_CODE_RX_INACTIVE	(0)
#define FLEXCAN_MB_CODE_RX_EMPTY	(4)
#define FLEXCAN_MB_CODE_RX_FULL		(2)
#define FLEXCAN_MB_CODE_RX_OVERRUN	(6)
#define FLEXCAN_MB_CODE_RX_BUSY		(1)

#define FLEXCAN_MB_CS_IDE_BIT_NO        (21)
#define FLEXCAN_MB_CS_RTR_BIT_NO        (20)
#define FLEXCAN_MB_CS_DLC_BIT_NO        (16)

#define FLEXCAN_MB_CODE_TX_INACTIVE	        (8)
#define FLEXCAN_MB_CODE_TX_ABORT		    (9)
#define FLEXCAN_MB_CODE_TX_ONCE			    (0x0C)
#define FLEXCAN_MB_CODE_TX_RESPONSE		    (0x0A)
#define FLEXCAN_MB_CODE_TX_RESPONSE_TEMPO	(0x0E)
#define FLEXCAN_get_code(cs)				(((cs) & FLEXCAN_MB_CS_CODE_MASK)>>24)
#define FLEXCAN_get_length(cs)              (((cs) & FLEXCAN_MB_CS_DLC_MASK)>>16)


//FLEXCAN_MB_ID的位定义和宏
#define FLEXCAN_MB_ID_STD_MASK		   (0x1FFC0000L)
#define FLEXCAN_MB_ID_EXT_MASK		   (0x1FFFFFFFL)
#define FLEXCAN_MB_ID_IDEXT(x)        (((x)&0x0003FFFF)<<0)
#define FLEXCAN_MB_ID_IDSTD(x)        (((x)&0x000007FF)<<18)
#define FLEXCAN_MB_ID_PRIO(x)         (((x)&0x00000007)<<29)
#define FLEXCAN_MB_ID_PRIO_BIT_NO	  (29)
#define FLEXCAN_MB_ID_STD_BIT_NO	  (18)
#define FLEXCAN_MB_ID_EXT_BIT_NO	  (0)

//FLEXCAN_MB_WORD0的位定义和宏
#define FLEXCAN_MB_WORD0_DATA3(x)     (((x)&0x000000FF)<<0)
#define FLEXCAN_MB_WORD0_DATA2(x)     (((x)&0x000000FF)<<8)
#define FLEXCAN_MB_WORD0_DATA1(x)     (((x)&0x000000FF)<<16)
#define FLEXCAN_MB_WORD0_DATA0(x)     (((x)&0x000000FF)<<24)

//FLEXCAN_MB_WORD1的位定义和宏
#define FLEXCAN_MB_WORD1_DATA7(x)     (((x)&0x000000FF)<<0)
#define FLEXCAN_MB_WORD1_DATA6(x)     (((x)&0x000000FF)<<8)
#define FLEXCAN_MB_WORD1_DATA5(x)     (((x)&0x000000FF)<<16)
#define FLEXCAN_MB_WORD1_DATA4(x)     (((x)&0x000000FF)<<24)


typedef struct{
	uint32_t CS;                                     /*!< Message Buffer 0 CS Register..Message Buffer 15 CS Register, array offset: 0x80, array step: 0x10 */
    uint32_t ID;                                     /*!< Message Buffer 0 ID Register..Message Buffer 15 ID Register, array offset: 0x84, array step: 0x10 */
    uint32_t Word[2];                                  /*!< Message Buffer 0 WORD0 Register..Message Buffer 15 WORD0 Register, array offset: 0x88, array step: 0x10 */
   // uint32_t WORD1;                                  /*!< Message Buffer 0 WORD1 Register..Message Buffer 15 WORD1 Register, array offset: 0x8C, array step: 0x10 */
} canMBsType0;
typedef struct {
	uint32_t CS;                                     /*!< Message Buffer 0 CS Register..Message Buffer 15 CS Register, array offset: 0x80, array step: 0x10 */
    uint32_t ID;                                     /*!< Message Buffer 0 ID Register..Message Buffer 15 ID Register, array offset: 0x84, array step: 0x10 */
    uint32_t Word[4];                                  /*!< Message Buffer 0 WORD0 Register..Message Buffer 15 WORD0 Register, array offset: 0x88, array step: 0x10 */
}canMBsType1;
typedef struct {
	uint32_t CS;                                     /*!< Message Buffer 0 CS Register..Message Buffer 15 CS Register, array offset: 0x80, array step: 0x10 */
    uint32_t ID;                                     /*!< Message Buffer 0 ID Register..Message Buffer 15 ID Register, array offset: 0x84, array step: 0x10 */
    uint32_t Word[8];                                  /*!< Message Buffer 0 WORD0 Register..Message Buffer 15 WORD0 Register, array offset: 0x88, array step: 0x10 */
  
}canMBsType2;
typedef struct {
	uint32_t CS;									 /*!< Message Buffer 0 CS Register..Message Buffer 15 CS Register, array offset: 0x80, array step: 0x10 */
	uint32_t ID;									 /*!< Message Buffer 0 ID Register..Message Buffer 15 ID Register, array offset: 0x84, array step: 0x10 */
	uint32_t Word[16];								   /*!< Message Buffer 0 WORD0 Register..Message Buffer 15 WORD0 Register, array offset: 0x88, array step: 0x10 */
}canMBsType3;


uint8 canInit(uint8 canChannel,uint32 baudrateKHz,uint8 selfLoop,uint8 idMask);

void CANEnableRXBuff(uint8 CANChannel,uint16 iMB);
void EnableCANInterrupt(uint8 CANChannel,uint16 iMB);
uint8 CANRecData(uint8 CANChannel,uint16 iMB, uint32 *id,uint8 *Datalenght,uint8 Data[]);
uint8 CANSendData(uint8 CANChannel,uint16 iMB, uint32 id,uint8 length,uint8 Data[]);
void CANClearFlag(uint8 CANChannel,uint16 iMB);

#endif


#include "keyProcess.h"


extern unsigned char FirstErrorsNotZero;
extern unsigned char HighMulfunctionFlashLED_On;
extern unsigned char WaterTempFlahLED_On;

//6 modules 6 key table;
//where a key is  means that the key data are generated at that module.
//global key means the key date is broadcasted in the CAN net.
//if you want to use a global key, it's data must be received.(pdo linking)
BOOLEAN Key_IndexToValue(UINT16  verIndex)
{
	UINT8 nodeID;UINT16 keyIndex,byteIndex;
	UINT8 bitIndex;
	UINT8 *keyArray;
	
	nodeID=(UINT8)((verIndex&CONDITION_NODE_INDEX_MARSK)>>CONDITION_KEY_INDEX_BIT_LEN);//nbapo:得到模块
	keyIndex=(UINT16)(verIndex&CONDITION_KEY_INDEX_MARSK);//物理/虚拟开关 + 索引号
	//Global keys(physical keys)
	if ((keyIndex&CONDITION_KEY_VIRTUAL_KEY_MARSK)==0 ) //物理开关
	{
		byteIndex=keyIndex/8;
		bitIndex=keyIndex%8;
		if (nodeID==DASHBOARD_NODE_ID)//仪表
		{
			keyArray = DASHBOARD_MODULE_PHYSICAL_KEYS_ADDRESS;           
		}
		else if(nodeID==FRONT_DRIVER_NODE_ID) //前驱
		{
			keyArray = FRONT_MODULE_PHYSICAL_KEYS_ADDRESS;
		}
		else if (nodeID==BACK_DRIVER_NODE_ID) //后驱
		{
			keyArray=BACK_MODULE_PHYSICAL_KEYS_ADDRESS;
		} 
		else if (nodeID==MIDDLE_DRIVER_NODE_ID) //中驱
		{
			keyArray = MIDDLE_MODULE_PHYSICAL_KEYS_ADDRESS;
		}
		else if (nodeID==TOP_DRIVER_NODE_ID) //顶驱
		{
			keyArray=TOP_MODULE_PHYSICAL_KEYS_ADDRESS;
		} 
		/*else if (nodeID==POWER_MODULE_NODE_ID) 
		{
		keyArray=POWER_MODULE_PHYSICAL_KEYS_ADDRESS;
		}*/ 
		else if(nodeID==POWER_CONTOROL_NODE_ID) //配电
		{
			keyArray=POWER_CONTOROL_MODULE_PHYSICAL_KEYS_ADDRESS;
		}
		else
		{
			return FALSE;
		}
		
		return((keyArray[byteIndex] >> bitIndex)&0x01)!=0;
	}

	//Global keys(virutal keys)
	if ((keyIndex & CONDITION_KEY_VIRTUAL_KEY_MARSK) != 0) 
	{
		keyIndex &= ~CONDITION_KEY_VIRTUAL_KEY_MARSK;
		byteIndex = keyIndex / 8;
		bitIndex = keyIndex % 8;
		if (nodeID==POWER_MODULE_NODE_ID) 
		{
			keyArray = (UINT8 *)POWER_MODULE_SHORT_OPEN_KEYS_ADDRESS;
			return((keyArray[byteIndex] >> bitIndex)&0x01)!=0;
			//return((USR_s_OV.PowerLoadState[byteIndex].Data)&(0x01<<bitIndex))!=0;
		}
		else if(nodeID == DASHBOARD_NODE_ID)
		{
			//if(keyIndex <= DASHBOARD_VIRTUAL_KEYS_NUM)
			//{
			keyArray = DASHBOARD_MODULE_VIRTUAL_KEYS_ADDRESS;
			//} 
			//else
			//{
			//    keyArray = DASHBOARD_MODULE_SHORT_OPEN_KEYS_ADDRESS;
			//}
			return((keyArray[byteIndex]>> bitIndex)&0x01)!=0;
		} 
		else if(nodeID == FRONT_DRIVER_NODE_ID)
		{
			keyArray =(UINT8 *) FRONT_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS;
			return((keyArray[byteIndex]>> bitIndex)&0x01)!=0;
		}
		else if(nodeID == BACK_DRIVER_NODE_ID)
		{
			keyArray = (UINT8 *)BACK_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS;
			return((keyArray[byteIndex]>> bitIndex)&0x01)!=0;
		}
		else if(nodeID == MIDDLE_DRIVER_NODE_ID)
		{
			keyArray = (UINT8 *)MIDDLE_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS;
			return((keyArray[byteIndex]>> bitIndex)&0x01)!=0;
		}
		else if(nodeID == TOP_DRIVER_NODE_ID)
		{
			keyArray = (UINT8 *)TOP_DRIVER_MODULE_SHORT_OPEN_KEYS_ADDRESS;
			return((keyArray[byteIndex]>> bitIndex)&0x01)!=0;
		} 
		else if(nodeID == POWER_CONTOROL_NODE_ID) 
		{
			keyArray = (UINT8 *)POWER_CONTOROL_MODULE_SHORT_OPEN_KEYS_ADDRESS ;
			return((keyArray[byteIndex]>> bitIndex)&0x01)!=0;
		}
	}
	//private keys(physical keys)



	//private keys(virutal keys)
	if (((keyIndex&CONDITION_KEY_PRIVATE_KEY_MARSK)!=0)&&((keyIndex&CONDITION_KEY_VIRTUAL_KEY_MARSK)!=0)) 
	{
#if (MODULE_TYPE==MODULE_TYPE_DRIVE_MODULE)
		if (nodeID==FRONT_DRIVER_NODE_ID||
		nodeID==BACK_DRIVER_NODE_ID||
		nodeID==MIDDLE_DRIVER_NODE_ID||
		nodeID==TOP_DRIVER_NODE_ID
		){

		}
#endif//(MODULE_TYPE==MODULE_TYPE_DRIVE_MODULE)
#if (MODULE_TYPE==MODULE_TYPE_POWER_MODULE)
		if (nodeID==POWER_MODULE_NODE_ID) {
		}
#endif//(MODULE_TYPE==MODULE_TYPE_POWER_MODULE)
#if (MODULE_TYPE==MODULE_TYPE_DASHBOARD_MODULE)
		if (nodeID==DASHBOARD_NODE_ID) 
		{

		}
#endif//(MODULE_TYPE==MODULE_TYPE_DASHBOARD_MODULE)
		return FALSE;
	}
}
/************************************************************************************************
函数名：UINT8* getShortOpenKeyAddr()
************************************************************************************************/
//#pragma CONST_SEG    DEFAULT


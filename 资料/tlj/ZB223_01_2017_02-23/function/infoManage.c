#include	"infoManage.h"


UINT8	  recordingStTime[6];
UINT8 	storageArray[6];//����Ҫ����Ϣ���ȥ
UINT8	  saveCnt[RESOURE_TOTAL] = {0};//�洢��������
UINT16 	storageFlag[RESOURE_TOTAL] = {0};//һ����Դ�������ֻ����16����Ŀ��Ϣ
UINT8	  haveNum[RESOURE_TOTAL];


#if 0

//#define	INFOMANAGE_ADDRESS		THE_SAVECNT_ADDRESS + RESOURE_TOTAL
void infoManageParaterInit(void)
{
	UINT8 i,j=0;

	Eeprom_Read_Fact_Ex(THE_HAVENUM_ADDRESS, read_data, 2*RESOURE_TOTAL);
	for(i = 0; i< 2*RESOURE_TOTAL; i=i+2)
	{
		read_data[i+1] = ~read_data[i+1];
		if(read_data[i] == read_data[i+1]) 
			haveNum[j] = read_data[i];	
		else	 
			haveNum[j] = 0;//

		if(haveNum[j] > StorageManagerCONST[j].num)	
			haveNum[j] = StorageManagerCONST[j].num;
		j++;
	}
	Eeprom_Read_Fact_Ex(THE_SAVECNT_ADDRESS, saveCnt, RESOURE_TOTAL);
	for(i = 0; i < RESOURE_TOTAL; i++)
	{ 
		if(saveCnt[i] >= StorageManagerCONST[i].num)	saveCnt[i] = 0;
		storageFlag[i] = 0;
		///saveCnt[i] = 0;
	}	
}

void singleInfoWrite2EPROM(unsigned char addr,unsigned char* dataAddr,unsigned char dataType)
{
	Eeprom_Write_Fact_Ex(addr, dataAddr, dataType);
}


/*******************************************************
* @brief		��ö�����ʼʱ�䡢����
*******************************************************/
void getStTime(UINT8 *getday,UINT8 *gettime) 
{
	gettime[0] =  RealTimeCnt[3];
	gettime[1] = RealTimeCnt[4];
	gettime[2] = RealTimeCnt[5]; 
	//*asday  = 0x20;
	//*asday  <<=8;
	getday[0] = RealTimeCnt[0];
	getday[1] = RealTimeCnt[1];
	getday[2] = RealTimeCnt[2];
}

/************************************************************
* @brief			д��rom
* @parameter		item	��item��������Ϣ
					dataType ���õ����ݴ�С
					dstAddress�洢��ַ
* @return			����ƫ�Ƶĵ�ַ
*************************************************************/
UINT8 writeAddress(UINT16 srcIndex,UINT8 dataType,UINT16 dstAddress)
{
	unsigned char src;
	unsigned int index;
	unsigned char *pBuf;
	unsigned char i;
	//UINT8 s[] = {0x55,0x63};
	
	src = (UINT8)(srcIndex >> 12);
	index = srcIndex & 0x0fff;

	switch(src)
	{
		case srcFROMJ1939 :
			switch(dataType) 
			{
				case 0  :  pBuf= &StorageBuffer[J1939_OFFSET_BYTE+index]; break;            
				case 1  :  pBuf= &StorageBuffer[J1939_OFFSET_BYTE+index]; break;          
				case 2  :  pBuf= &StorageBuffer[J1939_OFFSET_BYTE+2*index-J1939_STATUS_NUMBER-J1939_CHAR_NUMBER];break;                
				case 3  :  pBuf= &StorageBuffer[J1939_OFFSET_BYTE + 4*index-3*J1939_STATUS_NUMBER - 3*J1939_CHAR_NUMBER-2*J1939_INT_NUMBER + 1];break;//��һ���ֽ��ǿյ� 
				case 4  :  pBuf= &StorageBuffer[J1939_OFFSET_BYTE + 4*index-3*J1939_STATUS_NUMBER - 3*J1939_CHAR_NUMBER-2*J1939_INT_NUMBER];break;                
			};
			//pBuf= s;
			break;
		case srcFROMINSIDE :
			break;
		case 0x03:	//����
			getStTime(&recordingStTime[0],&recordingStTime[3]);
			pBuf =  &recordingStTime[index];
			//singleInfoWrite2EPROM((UINT8)srcAddress,pBuf,3);
			break;
		case 0x02:	//ʱ��
			getStTime(&recordingStTime[0],&recordingStTime[3]);
			pBuf = &recordingStTime[3];
			break;	
		default :
			break;
	}
	singleInfoWrite2EPROM((UINT8)dstAddress,pBuf,dataType);
	//for(i=0;i<dataType;i++)	pBuf[i] = 0;
	return 0;
}

/*********************************************************
* 
**********************************************************/
UINT8 storage2rom(void)
{		
	UINT16	attribute;
	UINT8	dataType = 0;
	unsigned char port;
	unsigned char doItem;
	unsigned char resourceCnt, itemLogicCnt,itemLength,itemlogicLocation;
	unsigned int storageAddress;
	unsigned char portRow,portLine;
	unsigned char i;
	StorageManager1_struct storageManager1Cfg;
	StorageManager2_struct storageManager2Cfg;
	
	for(resourceCnt = 0; resourceCnt < RESOURE_TOTAL; resourceCnt++)
	{
		storageManager1Cfg = StorageManagerCONST[resourceCnt];
		attribute = storageManager1Cfg.attribute;
		if(attribute == 0xff)	break;
		if(!(attribute & 0x80))	break;//continue;//
		
		//��Դ��Ŀ����
		itemLength = storageManager1Cfg.logicLenght;
		itemlogicLocation = storageManager1Cfg.logicLocation;
		for(itemLogicCnt = itemlogicLocation; itemLogicCnt < (itemLength + itemlogicLocation); itemLogicCnt++)
		{	
			storageManager2Cfg = InformationCONST[itemLogicCnt];
			port = storageManager2Cfg.load;		
			if(port == 0xff)	doItem = 1;//���ܸ���ֱ�Ӵ�
			else
			{
				portRow = port >> 3;// /8
				portLine = port & 0x07;//%8
				port = LogicOutPut[portRow].Load & (0X01 << portLine)	;		
				if(!port)
				{
					//��Ӧ������Ч	doItem = 0;
					doItem = 0;
				}
				else doItem = 1;
			}
			
			if(doItem == 0)		continue;//ִ����һ������
			//----------------------------------------------------------------		
			//����doItem != 0 ������
			dataType  = storageManager2Cfg.attribute & 0x0f;
			if(dataType == 0) dataType = 1;
			storageAddress = THE_BASE_ADDRESS + storageManager2Cfg.dstIndex;//romĿ���ַ
			storageAddress += dataType * saveCnt[resourceCnt];
			if((storageAddress + dataType)> 255) 	return 0xff;
			//writeAddress(storageManager2Cfg.srcIndex,dataType,storageAddress);
			//storageFlag[resourceCnt] |= (UINT16)(0x0001 << (itemLogicCnt - itemlogicLocation));
			
			if((storageFlag[resourceCnt] == ((0x0001 << (itemLogicCnt - itemlogicLocation))-1)))//ǰ�漸���Ѿ�����
			{
				if(!(storageFlag[resourceCnt] & (0x0001 << (itemLogicCnt - itemlogicLocation))))//���������ݣ��������������
				{
					writeAddress(storageManager2Cfg.srcIndex,dataType,storageAddress);
					storageFlag[resourceCnt] |= (UINT16)(0x0001 << (itemLogicCnt - itemlogicLocation));
				}	

				if(storageFlag[resourceCnt] == ((0x0001 << itemLength)-1))//����Դ���ô���
				{
					if(haveNum[resourceCnt] < storageManager1Cfg.num)
					{
						haveNum[resourceCnt]++;
						write_data[0]	= haveNum[resourceCnt];
						write_data[1] = ~write_data[0];

						while(1)
						{
							Eeprom_Write_Fact_Ex(THE_HAVENUM_ADDRESS + 2*resourceCnt,write_data,2);
							DelayMs(2);///when write data just,it must wait for a moment,then read data
							 Eeprom_Read_Fact_Ex(THE_HAVENUM_ADDRESS + 2*resourceCnt,read_data,2);
							 read_data[1] = ~read_data[1];
							 if(read_data[0] == read_data[1])	break;
						}
					}
						
					
					saveCnt[resourceCnt]++;
					if(saveCnt[resourceCnt] >= storageManager1Cfg.num)	
						saveCnt[resourceCnt] = 0;
					singleInfoWrite2EPROM(THE_SAVECNT_ADDRESS + resourceCnt,&saveCnt[resourceCnt],1);

					storageFlag[resourceCnt] = 0;//һ�ִ�����־����	
					for(i=0;i<6;i++)
						recordingStTime[i] = 0;
				}
			}									
		}
	}
	
	return 0;
}


#endif
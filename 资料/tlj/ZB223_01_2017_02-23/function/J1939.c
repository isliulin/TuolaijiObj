#define		J1939_C
#include	"J1939.h"
//#pragma CODE_SEG DEFAULT
 

//#pragma CODE_SEG __NEAR_SEG NON_BANKED  
UINT8 StorageBuffer[J1939_STORAGE_SIZE];
J1939OverDealStruct  J1939DetectCnt[200];
UINT8 J1939RequestIndex;
CAN_PACKET_T send_can_packet;
UINT8 J1939IdNum;//ap:���õ�ID����
UINT16 J1939EngSpeedIndex;
J1939_FRAME_STRUCT J1939Msg;
uchar Fual_Require_Time;



void  TXWaitRelay(UINT8 TPMSPosition)
{
	///TX1939_TPMS_Flag=1;
	send_can_packet.identifier = 0x18fef317;
	send_can_packet.byte_count = 8;
	send_can_packet.data[0] = 0xff;
	send_can_packet.data[1] = 0x88;
	send_can_packet.data[2] = 0x88;
	send_can_packet.data[3] = 0x0;
	send_can_packet.data[4] = 0x0;
	send_can_packet.data[5] = 0x0;
	send_can_packet.data[6] = TPMSPosition+1;
	send_can_packet.data[7] = 0x01;
	Trans_CANpkt();
}
void  TX_TPMS_LearnEnd(UINT8 TPMSPosition)
{ 
	///TX1939_TPMS_Flag=1;
	send_can_packet.identifier = 0x18fef317;
	send_can_packet.byte_count = 8;
	send_can_packet.data[0] = 0xff;
	send_can_packet.data[1] = 0x88;
	send_can_packet.data[2] = 0x88;
	send_can_packet.data[3] = 0xff;
	send_can_packet.data[4] = 0xff;
	send_can_packet.data[5] = 0xff;
	send_can_packet.data[6] = TPMSPosition+1;
	send_can_packet.data[7] = 0x01;
	Trans_CANpkt();
}


UINT16 J1939CompareData(UINT8 type,UINT16 index,UINT16 len) 
{
   UINT16 dataIndex,inputDataTemp,inputData;
   UINT8  J1939Type,leng;
   dataIndex =  index;  
   leng =  len;
   if((type&0xf0)!=0x50)  return 0;                           
   type &=0x0f;
   inputData = 0; 
   if(type==2)  inputData = 0xffff;     //��Сֵ 
   J1939Type  = J1939WordDataConfig[dataIndex].attribute&0x0f; 
   if(J1939Type==J1939_Char_Type)    leng = len/2; 
       
    while(leng)
    { 
      switch(J1939Type) 
      {          
        case J1939_Char_Type   :  ///inputDataTemp=StorageBuffer[J1939_OFFSET_BYTE+dataIndex]; break;    
                                   inputDataTemp = StorageBuffer[J1939_OFFSET_BYTE+dataIndex];  dataIndex++;
                                   inputDataTemp <<=  8;
                                   inputDataTemp |= StorageBuffer[J1939_OFFSET_BYTE+dataIndex];  dataIndex++;
                                   break;       
        case J1939_Int_Type    :  inputDataTemp=*(UINT16 *)(&StorageBuffer[J1939_OFFSET_BYTE+2*dataIndex-J1939_STATUS_NUMBER-J1939_CHAR_NUMBER]);
                                  dataIndex++;
                                  break;                
      }; 
      if(type==1)      //���ֵ
      {
        if(inputDataTemp>inputData)    inputData =  inputDataTemp;
      }
      else if(type==2)  //��Сֵ
      {
         if(inputDataTemp<inputData)   inputData =  inputDataTemp;
      } 
      leng--;  
    }    
    return      inputData;
}

/******************************************************************************************
��������void doJ1939Msg(void)
���ܣ�����J1939������Ϣ
******************************************************************************************/
CAN_PACKET_T j1939MsgTemp;
void doJ1939Msg(void)
{ 
	UINT16 pgn;//nbapo:	j1939 Id bit8-23
	UINT8 cnt;
	
	cnt = J1939Msg.amount;//nbapo:֡��

	while(cnt)
	{
		cnt--;
		HIGH_CAN_MODULE_F = 1;
		getJ1939MsgFromRd(&j1939MsgTemp,&J1939Msg);//nbapo:��1939Msg���ݵ�j1939MsgTemp
		pgn = (UINT16)(j1939MsgTemp.identifier>>8);//nbapo:�������
		if(j1939MsgTemp.identifier == START_ELECTRICIZE) ;
        if(j1939MsgTemp.identifier == START_ELECTRICIZE1);
        if(j1939MsgTemp.identifier == END_ELECTRICIZE);

		FeedWatchDog();
		switch(pgn)                                             
		{
			case ENGINE_SPEED_PGN:
				{
					getEngineSpeed(j1939MsgTemp.data);
					break;   //ת��
				}
			case TOTAL_CONSUME_PGN://��������
				{
					getTotalConsume(j1939MsgTemp.data);
					//break;	//nbapo: attention!!!
				}  
			default:
				{
					//nbapo: ������1
					if(EngineDiagnoseCfg.attribute == 0x8000) 
					{
						if(EngineDiagnoseCfg.SignelPackageID==j1939MsgTemp.identifier)   
						{
							doSignelPackageEngineDiagnose(&j1939MsgTemp,0); //break;
						}
						else  if(EngineDiagnoseCfg.MultiPgckageDeclareID==j1939MsgTemp.identifier) //�������
						{
							doCfgPgckageEngineDiagnose(&j1939MsgTemp,0); // break; 
						}
						else  if(EngineDiagnoseCfg.MultiPgckageDatID==j1939MsgTemp.identifier) //�������
						{
							doMultiPgckageEngineDiagnose(&j1939MsgTemp,0);  //break;
						}
					}
					//nbapo: ������2
					if(EngineDiagnoseCfg1.attribute==0x8001) 
					{
						if(EngineDiagnoseCfg1.SignelPackageID ==j1939MsgTemp.identifier) 
						{
							doSignelPackageEngineDiagnose(&j1939MsgTemp,1);  //break;  
						}
						else  if(EngineDiagnoseCfg1.MultiPgckageDeclareID==j1939MsgTemp.identifier) 
						{
							doCfgPgckageEngineDiagnose(&j1939MsgTemp,1);  // break;
						}
						else  if(EngineDiagnoseCfg1.MultiPgckageDatID==j1939MsgTemp.identifier) 
						{
							doMultiPgckageEngineDiagnose(&j1939MsgTemp,1); // break;
						}           
					}
					//nbapo: ������3
					if(EngineDiagnoseCfg2.attribute==0x8002) 
					{
						if(EngineDiagnoseCfg2.SignelPackageID==j1939MsgTemp.identifier) 
						{
							doSignelPackageEngineDiagnose(&j1939MsgTemp,2);  //break;  
						}
						else  if(EngineDiagnoseCfg2.MultiPgckageDeclareID==j1939MsgTemp.identifier) 
						{
							doCfgPgckageEngineDiagnose(&j1939MsgTemp,2);  // break;
						}
						else  if(EngineDiagnoseCfg2.MultiPgckageDatID==j1939MsgTemp.identifier) 
						{
							doMultiPgckageEngineDiagnose(&j1939MsgTemp,2); // break;
						}	           
					}

					doJ1939Data(&j1939MsgTemp,StorageBuffer,&USR_s_OV.DashboardVirtualKey[0].Data,J1939IdNum); 
					J1939IdStartRx(J1939IdNum);
					break;            
				}
		}
	}                                      
}

 /*****************************************************************************
��������void getEngineSpeed(void)
���ܣ���ȡ������ת��
*****************************************************************************/
//void getEngineSpeed(UINT8 * pData)
void getEngineSpeed(UINT8 * pData)
{  
	UINT16 engineSpeed;
	UINT16 index;
	index = J1939EngSpeedIndex;
	engineSpeed =  pData[4];
	engineSpeed <<= 8;
	engineSpeed |= pData[3];
	engineSpeed /= 8;   
	*((UINT16 *)&StorageBuffer[index]) = engineSpeed; 
}
 /***************************************************************************
 ��������void getTotalConsume(void)
 ���ܣ���ȡ���ͺ�
 ***************************************************************************/
 void getTotalConsume(UINT8 * pData)
 {
	 UINT32 temp;
	 UINT8 i;
	 for(i=0;i<4;i++)
	 {
		 temp <<= 0x08;
		 temp += pData[7-i];
	 }
	 TotalConsume = temp;
	 USR_s_OV.totalConsume = temp;			
	 
 }
 /******************************************************************************
 �������� void getJ1939MsgFromRd(CAN_PACKET_T  *pbuf,J1939_FRAME_STRUCT *pJ1939RoundBuf)
 ���ܣ��ӻ��λ�������ȡ��һ֡����
 ����˵����CAN_PACKET_T  *pbuf -> ȡ�������ݴ���ڴ�ָ��ָ��ĵ�ַ��
		   J1939_FRAME_STRUCT *pJ1939RoundBuf -> �Ӹû��λ�������ȥȡ���ݡ�
 ******************************************************************************/
void getJ1939MsgFromRd(CAN_PACKET_T *pbuf,J1939_FRAME_STRUCT *pJ1939RoundBuf)
{
	*pbuf = pJ1939RoundBuf->data[pJ1939RoundBuf->front];
	if(pJ1939RoundBuf->front >= J1939_FRAME_NUM-1)
	{
		pJ1939RoundBuf->front = 0x00;
	}
	else
	{
		pJ1939RoundBuf->front++; 
	}
	
	DisableInterrupts;
	pJ1939RoundBuf->amount--; 
	EnableInterrupts;  
}
 /************************************************************************
 * @brief	 
 * @parameter			 *msg
 *						 *pDataBuf
 *						 *pVirtualKey
 *						 cnt
 *************************************************************************/
 void  doJ1939Data(CAN_PACKET_T *msg,UINT8 *pDataBuf,UINT8 *pVirtualKey,UINT16 cnt) 
 {
	 UINT16 indexOffset;
	 UINT8	indexLenght;
	 UINT16 j1939Index;
	 UINT8	i,j;
	 
	 for(i=0;i<cnt;i++) 
	 {
		 if((msg->identifier==J1939FindIndexConfig[i].J1939ID)) 	 //&&(J1939DetectCnt[i].b.cf==0)
		{
			indexOffset = J1939FindIndexConfig[i].offset;
			indexLenght = J1939FindIndexConfig[i].number;

			DisableInterrupts;//��ֹ���ж�	 
			J1939DetectCnt[i].dat = J1939_DEAL_NUM;//��ʱ����
			EnableInterrupts;//�����ж�
			for(j=0;j<indexLenght;j++) 
			{
				FeedWatchDog();
				j1939Index = J1939IndexConfig[indexOffset+j];
				getJ1939Data(msg,pDataBuf,j1939Index,J1939_NORMAL_MODE);
				getJ1939VirtualKey(pDataBuf,pVirtualKey,j1939Index,J1939_NORMAL_MODE);
			}
		}
	 }
 }
 /***********************************************************************************************************
 
 ************************************************************************************************************/
 void J1939IdStartRx(UINT8 cnt) 
 {
	UINT8 i ;
	for(i=0;i<cnt;i++)
		J1939DetectCnt[i].b.cf  = 0;
 }
 /**************************************************************************
 ��������void doJ1939WordData(UINT16 pgn,UINT8 *pData)
 ���ܣ�����J1939���ֽ�����
 ����˵����UINT16 pgn -> ���ĵ�PGN
		   UINT8  *pData -> J1939������Ϣ
 ����ֵ����
 * @brief			 
 * @prarmeter			 *msg:	 Դ��ַ
 *						 *pDatabuf:Ŀ���ַ
 *						 *cnt:����λ��
 *						 flag:	 J1939_CLEAN_MODE or other mode
 **************************************************************************/  
UINT8 RX_Multi_PGN;
void getJ1939Data(CAN_PACKET_T *msg,UINT8 *pDataBuf,UINT16 cnt,UINT8 flag)
{
	UINT16 i/*����*/,j;
	UINT16 attribute;
	UINT8	J1939Type;
	UINT32 dataTemp;
	UINT8	startByte;
	UINT8	MultiPaMode;
	UINT16 MultiPaTemp;
	UINT32 MultiData;
	UINT8	MultiDataIndex; 
	//UINT16 StorageIndex;
	UINT8	MultiTemp;
	//const UINT16 scale[5] = {1,10,100,1000,10000};
	const UINT8 mask[9] = {0x00,0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff};

	i=cnt;
	MultiPaTemp =	J1939WordDataConfig[i].MultiPgckMode;
	MultiPaMode = (UINT8)(MultiPaTemp>>8); //nbap:�߰�λģʽ

	if(flag != J1939_CLEAN_MODE)		 //J1939_CLEAN_MODE   J1939_NORMAL_MODE
	{
		switch(MultiPaMode) 
		{
			case 1:   
			{
				MultiDataIndex = (UINT8) (J1939WordDataConfig[i].MultiPgckMode & 0x0f);//bit0-3
				MultiData = msg->data[MultiDataIndex];
				if(MultiData!=J1939WordDataConfig[i].MultiPgckIndex) return;//L:���Ƕ�Ӧ������
				break;
			}	
			case 2: 
			{		  
				MultiDataIndex = (UINT8)((MultiPaTemp>>4)&0x0f);//bit4-7
				MultiData = msg->data[MultiDataIndex];
				MultiData <<= 8;  
				MultiDataIndex = (UINT8) (MultiPaTemp&0x000f);
				MultiData |= msg->data[MultiDataIndex]; 
				if(J1939WordDataConfig[i].MultiPgckIndex != MultiData) return;  
				break;
			}
			case 3:						 //������
			{
				MultiData = msg->data[1]; 
				MultiData <<= 8;
				MultiData |= msg->data[2];
				MultiData <<= 8; 
				MultiData |= msg->data[3]; 
				dataTemp  = MultiPaTemp & 0xff;//bit0-7
				dataTemp <<=16;
				dataTemp |= J1939WordDataConfig[i].MultiPgckIndex;
				if(MultiData != dataTemp)	return ; 
				break;
			}
			case 4:	//����λ
			{
				MultiDataIndex = (UINT8) (J1939WordDataConfig[i].MultiPgckMode&0x0f);
				MultiData = msg->data[MultiDataIndex];
				MultiData &= 0xf0;
				if(MultiData !=J1939WordDataConfig[i].MultiPgckIndex) return;
				break;
			} 
			case 5:	//����ʽ	   
			{
				if(J1939RequestIndex!=J1939WordDataConfig[i].MultiPgckIndex) return;
				break;
			}
			case 6:  //����֡	  
			{
				MultiData =  msg->data[6];
				MultiData <<=8;					//pgn
				MultiData |= msg->data[5];
				if(MultiData!=J1939WordDataConfig[i].MultiPgckIndex) return;
				RX_Multi_PGN =  J1939WordDataConfig[i].MultiPgckMode&0xff;
				break;
			}
			case 7:  //������ݽ���   
			{
				MultiData = msg->data[0];
				if(MultiData!=J1939WordDataConfig[i].MultiPgckIndex) return;
				MultiDataIndex = (UINT8) (J1939WordDataConfig[i].MultiPgckMode&0xff);
				if(RX_Multi_PGN!=MultiDataIndex) return;
				break;
			} 
			case 8:	//һ������+mask��
			{
				MultiDataIndex = (UINT8)((MultiPaTemp>>4) & 0x0f);//bit4-7
				//MultiData = msg->data[MultiDataIndex];
				//MultiData <<= 8;  
				// MultiDataIndex = (UINT8) (MultiPaTemp&0x000f);
				//MultiData |= msg->data[MultiDataIndex]; 

				MultiTemp = (UINT8)(J1939WordDataConfig[i].MultiPgckIndex>>8);
				if(MultiTemp != msg->data[MultiDataIndex]) return; 

				MultiDataIndex = (UINT8) (MultiPaTemp&0x000f);
				MultiTemp = (UINT8)(J1939WordDataConfig[i].MultiPgckIndex);
				if((MultiTemp&msg->data[MultiDataIndex])==0) return ;

				//if(J1939WordDataConfig[i].MultiPgckIndex!=MultiData) return;  

				break;
			} 
			case 9:	 //һ������+����λ
			{
				MultiDataIndex = (UINT8) (J1939WordDataConfig[i].MultiPgckMode&0x0f);
				MultiData = msg->data[MultiDataIndex];
				if(MultiData !=J1939WordDataConfig[i].MultiPgckIndex) return;
				break;
			}		   
			default:break;
		}
	}

	//for(i=0;i<cnt;i++)	  
	{
		attribute =  J1939WordDataConfig[i].attribute;
		if((attribute & J1939_VALID))	//&&(msg->identifier == J1939WordDataConfig[i].J1939ID))	// �Ƿ���Ч
		{
			startByte	  = J1939WordDataConfig[i].location;	
			//StorageIndex = J1939_OFFSET_BYTE;					   
			J1939Type	  = attribute & 0x0f;//��������
			dataTemp	  = 0;
			switch(J1939Type) 
			{
				case J1939_Status_Type : //0byte
				{
					if(flag == J1939_CLEAN_MODE)	 dataTemp = 0;
					else {
						dataTemp = msg->data[startByte];
						dataTemp >>= J1939WordDataConfig[i].offset;			   //��ʼλ
						dataTemp &= mask[J1939WordDataConfig[i].bitValue]; //λ����
					}
					pDataBuf[J1939_OFFSET_BYTE + i] = (UINT8)dataTemp;	
					break;  
				}				
				case J1939_Char_Type	:	  // 1 byte
				{
					if(flag == J1939_CLEAN_MODE)	  dataTemp = 0;
					else 
						dataTemp = msg->data[startByte];
					
					if(MultiPaMode == 0x09)	dataTemp >>= 4;
					
					pDataBuf[J1939_OFFSET_BYTE + i] = (UINT8)dataTemp;
					break;		
				}		 
				case J1939_Int_Type	:		// 2 byte
				{
					if(flag==J1939_CLEAN_MODE)   dataTemp = 0;
					else {
						//ap:arm�뵥Ƭ���Ĵ洢��С���෴���������Ե�
						if(attribute& J1939_BIG_ENDION == 0)//ap:j1939�ȴ���λ
						{
							dataTemp = msg->data[startByte];
							dataTemp <<= 0x08; 							  
							dataTemp += msg->data[startByte+0x01]; 
						}
						else //ap : j1939�ȴ���λ
						{
							dataTemp = msg->data[startByte+0x01];
							dataTemp <<= 0x08;
							dataTemp += msg->data[startByte];	  
						}	
					}	 
					*(UINT16 *)(&pDataBuf[J1939_OFFSET_BYTE+2*i-J1939_STATUS_NUMBER-J1939_CHAR_NUMBER]) = (UINT16)dataTemp;
					break;
				}	
				case  J1939_Three_Type:
				{
					if(flag==J1939_CLEAN_MODE)  dataTemp = 0; 
					else {
						if(attribute & J1939_BIG_ENDION == 0)//ap:j1939��λ�ȴ�
						{
							for(j=0;j<3;j++)
							{
								dataTemp <<= 0x08;					  
								dataTemp += msg->data[startByte+j];
							}	
							//dataTemp <<= 8;//add by ap
						}											 
						else//ap: j1939��λ�ȴ�
						{
							for(j=0;j<3;j++)
							{
								dataTemp <<= 0x08;
								dataTemp += msg->data[startByte + 2 - j];
							}
							//dataTemp <<= 8;//add by ap
						} 
					}
					*(UINT32 *)(&pDataBuf[J1939_OFFSET_BYTE + 4*i-3*J1939_STATUS_NUMBER - 3*J1939_CHAR_NUMBER-2*J1939_INT_NUMBER ]) = dataTemp;						  
					break;	
				}
				case J1939_Word_Type	:	  // 4 byte 
				{
					if(flag==J1939_CLEAN_MODE)  dataTemp = 0; 
					else {
						if(attribute& J1939_BIG_ENDION == 0)//���ֽ��ȴ�
						{
							for(j=0;j<4;j++)
							{
								dataTemp <<= 0x08; 					
								dataTemp += msg->data[startByte+j];
							}													
						}											   
						else//���ֽ��ȴ�
						{
							for(j=0;j<4;j++)
							{
								dataTemp <<= 0x08;
								dataTemp += msg->data[startByte+3-j];
							}
						}	 
					}
					*(UINT32 *)(&pDataBuf[J1939_OFFSET_BYTE + 4*i-3*J1939_STATUS_NUMBER - 3*J1939_CHAR_NUMBER-2*J1939_INT_NUMBER ]) = dataTemp; 						
					break;
				}
				default:break;
			}
		}
	}		 
}
 /**************************************************************************
 ��������void doJ1939WordData(UINT16 pgn,UINT8 *pData)
 ���ܣ�����J1939���ֽ�����
 ����˵����UINT16 pgn -> ���ĵ�PGN
		   UINT8  *pData -> J1939������Ϣ
 ����ֵ����
 * @brief
 * @parameter			 *pDataBuf:  ����
 *						 *pVirtualKey:��� ������⿪��ֵ
 *						 cnt:	 λ������
 *						 flag:
 *
 **************************************************************************/  
void getJ1939VirtualKey(UINT8 *pDataBuf,UINT8 *pVirtualKey,UINT16 cnt,UINT8 flag) 
{
	 UINT8 keyIndex;
	 UINT8 row;
	 UINT8 line;
	 UINT16 i,j;
	 UINT32 j1939Data;
	 UINT8 len;
	 //UINT8 bitToByteLenght;
	 UINT16 attribute;
	 UINT8 J1939Type; 
	 UINT32 bitVale;  
	 
	 i = cnt;
	 keyIndex = J1939WordDataConfig[i].keyIndex;
	 row = keyIndex / 0x08;
	 line = keyIndex % 0x08;   
	 //for(i=0;i<cnt;i++)	//20
	 {
		 attribute = J1939WordDataConfig[i].attribute;//b3-0:��������
		 if((attribute& J1939_KEY)&& (attribute & J1939_VALID))
		 {
			 if(flag==J1939_CLEAN_MODE) 
			 {
				 pVirtualKey[row] &= ~(0x01 << line);  
				 return ;
			 } 
			 
			 J1939Type = attribute&0x0f;//��������
			 switch(J1939Type) //nbapo:��ȡ����������������
			 {
				 case  J1939_Status_Type:
					 __asm("nop");
					 j1939Data = pDataBuf[J1939_OFFSET_BYTE+i];
					 __asm("nop");
					 break;
				 case  J1939_Char_Type:
					 __asm("nop");
					 j1939Data = pDataBuf[J1939_OFFSET_BYTE+i];
					 __asm("nop");
					 break;
				 case  J1939_Int_Type:
					 __asm("nop");
					 j1939Data = *(UINT16 *)(&pDataBuf[J1939_OFFSET_BYTE+2*i-J1939_STATUS_NUMBER-J1939_CHAR_NUMBER]);
					 __asm("nop");
					 break; 
				 case  J1939_Three_Type:
					 __asm("nop");
					 j1939Data = *(UINT32 *)(&pDataBuf[J1939_OFFSET_BYTE + 4*i-3*J1939_STATUS_NUMBER - 3*J1939_CHAR_NUMBER-2*J1939_INT_NUMBER ]);
					 __asm("nop");
					 break;
				 case  J1939_Word_Type:
					 __asm("nop");
					 j1939Data = *(UINT32 *)(&pDataBuf[J1939_OFFSET_BYTE + 4*i-3*J1939_STATUS_NUMBER - 3*J1939_CHAR_NUMBER-2*J1939_INT_NUMBER ]);
					 __asm("nop");
					 break;
				 default:break; 
			 }
 
			 if(J1939Type == J1939_Status_Type) //nbapo:״̬����
			 {
				 if(flag == J1939_CLEAN_MODE) 
				 {
					 pVirtualKey[row] &= ~(0x01 << line);
				 }
 
				 len = (UINT8)J1939WordDataConfig[i].thresholdL;//keycode  //nbapo:״̬�����£�ͬһ��״̬�ȽϵĴ���
				 for(j=0;j<len;j++)
				 {
					 if(j1939Data == StatusPrara[i].statusCode[j])
					 {
						 pVirtualKey[row] |= 0x01 << line;	//��λ
						 break;
					 }
					 else
					 {
						 pVirtualKey[row] &= ~(0x01 << line);//����
					 }
				 }
			 } 
			 else //nbapo:��������
			 {
				 bitVale = J1939WordDataConfig[i].bitValue & 0xfffffff;//28bit nbapo:Ȩ�� 
				 j1939Data *= bitVale;//nbapo:j1939����Ȩ�صõ���ʵ��ֵ
 
				 if(j1939Data >J1939WordDataConfig[i].offset)//add 
				 {
					 // nbapo:��ȥƫ��ֵ���õ��ľ���
					 j1939Data -=  J1939WordDataConfig[i].offset ;//* scale[dot];
				 }
				 else  j1939Data=0;//nbapo:��֤��ƫ����������
 
				 //question: pVirtualKey[row]������ֱ�����ʽ
				 //nbapo: cnt(����)��ֵ��ͬ���õ���ͬ��row,line.λ������?
				 //nbapo:�߱���
				 if((J1939WordDataConfig[i].attribute & J1939_WARM_MASK) == J1939_BIGGER_WARM) 
				 {
					 if(j1939Data >= J1939WordDataConfig[i].thresholdH)//nbapo:���������ֵ
					 {
						 pVirtualKey[row] |= 0x01 << line;
					 }
					 else
					 {
						 pVirtualKey[row] &= ~(0x01 << line);
					 }
				 }
				 else if((J1939WordDataConfig[i].attribute & J1939_WARM_MASK)==J1939_LOWER_WARM)
				 {
					 if(j1939Data <= J1939WordDataConfig[i].thresholdL)//nbapo:��������ʱΪ����ֵ��״̬
					 {
						 pVirtualKey[row] |= 0x01 << line;
					 }
					 else
					 {
						 pVirtualKey[row] &= ~(0x01 << line);
					 }						  
				 } 
				 else if((J1939WordDataConfig[i].attribute & J1939_WARM_MASK) == J1939_MIDDLE_WARM)
				 {
					 if(j1939Data >= J1939WordDataConfig[i].thresholdL && j1939Data <= J1939WordDataConfig[i].thresholdH) 
					 {
						 pVirtualKey[row] |= 0x01 << line; 
 
					 }
					 else
					 {
						 pVirtualKey[row] &= ~(0x01 << line);
 
					 }
				 }
				 else if((J1939WordDataConfig[i].attribute & J1939_WARM_MASK) == J1939_BILATERAL_WARM)
				 {
					 if((j1939Data <= J1939WordDataConfig[i].thresholdL) || (j1939Data >= J1939WordDataConfig[i].thresholdH)) 
					 {
						 pVirtualKey[row] |= 0x01 << line; 
					 }
					 else
					 {
						 pVirtualKey[row] &= ~(0x01 << line);
					 }						  
				 }
				 else if((J1939WordDataConfig[i].attribute & J1939_WARM_MASK) == J1939_SCHMITT_TRIGGER_HIGH) 
				 {
					 // 																	   ________
											  // 0.6-0.8  �ر�	   __|___|
					 //  keyv =  (virtualKey[row].Data >>line)&0x01;  // 0.8-0.6  ����	  
					 if(j1939Data<J1939WordDataConfig[i].thresholdL) 
					 { 
						 pVirtualKey[row]  &= ~(0x01 << line);//����
					 } 
					 else if(j1939Data>J1939WordDataConfig[i].thresholdH) 
					 {
						 pVirtualKey[row]  |= (0x01 << line);//��λ
					 }
					 else 
					 {
						 ;
					 }													
				 }
				 else if((J1939WordDataConfig[i].attribute & J1939_WARM_MASK) == J1939_SCHMITT_TRIGGER_LOW) 
				 {
					 // keyv =	(virtualKey[row].Data >>line)&0x01;  // 0.8-0.6  ����
					 if(j1939Data<J1939WordDataConfig[i].thresholdL) 
					 { 
						 pVirtualKey[row]  |= (0x01 << line);
					 }
					 else if(j1939Data >J1939WordDataConfig[i].thresholdH) 
					 {
						 pVirtualKey[row] &= ~(0x01 << line);
					 }
					 else
					 {
						 ;
					 }
				 }		
			 }
		 }
	 }
 
 }
 /****************************************************************************************
 
 *****************************************************************************************/ 
 void fual_value_require(void)
 {
	   send_can_packet.identifier = 0x18EA0021;
	   send_can_packet.byte_count = 0X03;
	   send_can_packet.data[0] = 0XE9;
	   send_can_packet.data[1] = 0XFE;
	   send_can_packet.data[2] = 0X00;
	   Trans_CANpkt();
 }



////////////////////////////////////////////////////////////////////////////////
/////////////////				CAN				////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************
*	brief : can1�����жϵ��á�
*****************************************************************************************/ 
void MSCAN1Rec(UINT8 ch)   
{
	//UINT32 j1939Id = 0;
	UINT16 pgn;
	//UINT8 len = 0;
	UINT8 i,canReFlag = 1;   
	CAN_PACKET_T Isr_msg;


	if(ch==1)canReFlag = CANRecData(1,CAN1_REV_IMB, &Isr_msg.identifier, &Isr_msg.byte_count,Isr_msg.data);//ʧ�ܷ���1
	else if(ch==0)canReFlag = CANRecData(0,CAN0_REV_IMB, &Isr_msg.identifier, &Isr_msg.byte_count,Isr_msg.data);//ʧ�ܷ���1

	if(canReFlag) return;
	pgn = (Isr_msg.identifier >> 0x08) & 0xFFFF;

	if(pgn == ENGINE_SPEED_PGN)   {HIGH_CAN_MODULE_F = 1; getEngineSpeed(Isr_msg.data); return;}//ת�� 

    if(J1939Msg.amount < J1939_FRAME_NUM-1)
    {  
        for(i=0;i<Isr_msg.byte_count;i++)
        {
            J1939Msg.data[J1939Msg.rear].data[i] = Isr_msg.data[i];//*(&CAN1RDSR0 + i);//
        }
        J1939Msg.data[J1939Msg.rear].byte_count = Isr_msg.byte_count;
        J1939Msg.data[J1939Msg.rear].identifier = Isr_msg.identifier;
        if(J1939Msg.rear >= J1939_FRAME_NUM-1)
        {
            J1939Msg.rear = 0x00;
        }
        else
        {
            J1939Msg.rear++;
        }
        J1939Msg.amount++; 
    }
}


/********************************************************************************************
���ܣ�CAN_send
����˵���pcanframe��Ϊ�������ݵ�ָ�롣
ע��CAN0 ��Ϊ����CAN ������Ϊ125k.
********************************************************************************************/
void  CAN0_send( CAN_PACKET_T  *pcanframe) 
{
	CANSendData(2,CAN2_SEND_IMB, pcanframe->identifier, pcanframe->byte_count, pcanframe->data);
}        

void J1939ID_OverTime(void) 
{
	UINT8 i,j;
	UINT16 indexOffset;
	UINT8  indexLenght;
	UINT16 j1939Index;
	CAN_PACKET_T msg;

	if(SleepFlag) return;
	for(i=0;i<J1939IdNum;i++) 
	{
		if(J1939DetectCnt[i].dat==0) 
		{
			DisableInterrupts;
			J1939DetectCnt[i].b.valid =1;
			EnableInterrupts; 
			indexOffset = J1939FindIndexConfig[i].offset;
			indexLenght = J1939FindIndexConfig[i].number;
			if(indexLenght==0xff)    continue;
			if(indexOffset==0xffff)  continue;
			
			for(j=0;j<indexLenght;j++) 
			{
				FeedWatchDog();
				j1939Index = J1939IndexConfig[indexOffset+j];
				getJ1939Data(&msg,StorageBuffer,j1939Index,J1939_CLEAN_MODE);
				getJ1939VirtualKey(StorageBuffer,&USR_s_OV.DashboardVirtualKey[0].Data,j1939Index,J1939_CLEAN_MODE);
			}  
		}
	} 
}

/*****************************************************************************************

*****************************************************************************************/              
extern	USR_t_ApplicationObject 				USR_s_OV;

void J1939_CanMsgOut_Send(CAN_PACKET_T  *pcanframe)    
{
    if(CAN1_RS_READ)   return;  
	CANSendData(1,CAN1_SEND_IMB, pcanframe->identifier, 8, pcanframe->data);
}
void J1939_CanMsgOut2_Send(CAN_PACKET_T  *pcanframe)    
{
    if(CAN0_RS_READ)   return;  
	CANSendData(0,CAN0_SEND_IMB, pcanframe->identifier, 8, pcanframe->data);
}

/*****************************************************************************************
* can1  ���ݷ��ͺ���
*****************************************************************************************/              
void Trans_CANpkt(void)    
{
    if(CAN1_RS_READ)   return;
	CANSendData(1,CAN1_SEND_IMB, send_can_packet.identifier, send_can_packet.byte_count, send_can_packet.data);
	
}

void	J1939_Speed_Send(void)
{	
	send_can_packet.identifier = 0x0CFE6CEE;    
	send_can_packet.byte_count = 8;
	send_can_packet.data[0] = 0xff;
	send_can_packet.data[1] = 0xff;
	send_can_packet.data[2] = 0xff;
	send_can_packet.data[3] = 0xff;
	send_can_packet.data[4] = 0xff;
	send_can_packet.data[5] = 0xff;
	send_can_packet.data[6] = 0;//(unsigned char)speedTemp;
	send_can_packet.data[7] = (unsigned char)(RealCarSpeed&0xff);//(unsigned char)InsideData[7];//(unsigned char)RealCarSpeed;//(unsigned char)(speedTemp >> 8);
	Trans_CANpkt();
}

void Gas_Fired_Req(void) 
{
	send_can_packet.identifier = 0x18ea002B;
	send_can_packet.byte_count = 8;
	send_can_packet.data[0] = 0xaf;
	send_can_packet.data[1] = 0xfe;
	send_can_packet.data[2] = 0x00;
	send_can_packet.data[3] = 0xff;
	send_can_packet.data[4] = 0xff;
	send_can_packet.data[5] = 0xff;
	send_can_packet.data[6] = 0xff;
	send_can_packet.data[7] = 0xff;
	Trans_CANpkt();
}
void battery_req(UINT8 n) 
{
	send_can_packet.identifier = 0x1800f328;
	send_can_packet.byte_count = 8;
 	send_can_packet.data[0] = n;
	send_can_packet.data[1] = 0xff;
	send_can_packet.data[2] = 0xff;
	send_can_packet.data[3] = 0xff;
	send_can_packet.data[4] = 0xff;
	send_can_packet.data[5] = 0xff;
	send_can_packet.data[6] = 0xff;
	send_can_packet.data[7] = 0xff;
	Trans_CANpkt();
}

UINT8 testJ1939IdNum(void)
{
    UINT8 i,cnt;
    cnt = 0;
    for(i=0;i<200;i++) 
    { 
      if((J1939FindIndexConfig[i].J1939ID!=0xffffffff)&&(J1939FindIndexConfig[i].J1939ID!=0))  cnt++;
      else break;
    }
    return cnt;
} 


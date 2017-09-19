#define		LOADDEAL_C
#include	"load_deal.h"
#include        "IS31FL3733_I2C.h" 


#define LOAD_NUM 6

#define LOAD_CHK_CLOSE              0x00  
#define LOAD_CHK_RUNING             0x06
#define LOAD_CHK_SHORT              0x05
#define LOAD_CHK_OPEN               0x04
#define LOAD_CHK_OVERCURRENT        0x07  

/****************************************************************************************************
����������̶��������3�ֽڣ�VirtualLoad[3]����
����      ���ߡ����ѡ�ACC�����ON�����ǰ��δ�ء�����δ�ء�����δ��
           0      1        2      3         4         5     6
          ������Ƶ��������Ƶ��������N���źš�R���źš�L���ź�
              7        8       9         10       11    12
          ����������N��δ�ء��ظ������������С�һ������������������������
             13        14       15         16          17        18
          ���ָʾ��5��
             19                       
ע�⣺���š�ǰ��δ�����������������౨��������ڲ��Զ���ɡ�                     
****************************************************************************************************/
//#pragma CODE_SEG USER_ROM                                                      
DIAGNOSE_STATUS_STRUCT DashboardDiagnose[18];
DIAGNOSE_STATUS_STRUCT FrontDriverDiagnose[13];
DIAGNOSE_STATUS_STRUCT BackDriverDiagnose[13];
DIAGNOSE_STATUS_STRUCT MiddleDriverDiagnose[13];    
DIAGNOSE_STATUS_STRUCT TopDriverDiagnose[13];
DIAGNOSE_STATUS_STRUCT PowerDiagnose[13];
DIAGNOSE_STATUS_STRUCT PowerContorolDiagnose[6];
PutLoadStruct LogicOutPut[20];
PutLoadStruct AllLogicOutPut[20];
PutLoadStruct LoadLogicResult[20];
PutLoadStruct VirtualLoad[5];
LoadStruct Cat4016Data;
LoadStruct Cat4016OldData;
UINT8 StartStatus;
LoadStStruct    PwmboardLoad;
UINT8  Bts724St[6];
St_LoadDealInfo  LoadDealInfo[6];
UINT8 LoadSleepCnt=0;
UINT8 PwmLoadShortCnt[2]={0,0};

UINT8 LoadsAbnormal[1];
UINT8 NewLoadsStatus0[1];
UINT8 NewLoadsStatus[1];
UINT8 OldLoadsStatus[1];
UINT8 DashboardLoadsStatus[1];
UINT8 LoadsCnt[8];
UINT8 LoadsSt[1];
UINT8 LoadsInput[1];

LoadStStruct    PwmboardLoad;

const  uchar TftDutyCONST[4]= {0,32,42,58};//{0,50,62,77}; //{0,60,72,80}; {0,45,55,66};//
const  uchar BuzzerDutyCONST[4]={0,38,50,70};//{0,34,38,45};{0,38,45,50};
const  uchar LedDutyCONST[4]={0,15,20,25}; 

unsigned char	WaterTempFlahTime;
PutLoadStruct LeftRightLoadLogic[2];

void setVolumeBuzzer(unsigned char val)
{

}
/*********************************************************************************************** 
��������void driverLoad(void) 
���ܣ��������� 
#define DASHBOARD_LOAD_START_ADDR       0
#define LCD_INDICATOR_START_ADDR        4
#define LED_INDICATOR_START_ADDR        28
#define BOARD_LED_START_ADDR            44
#define VIRTUAL_LOAD_START_ADDR         52

0  - 3  �����ء�     logicOutPutImage[0].Load
8  - 31 24·LCDָʾ��  logicOutPutImage[1].Load   logicOutPutImage[2].Load logicOutPutImage[3].Load
32 - 47 16·LED        logicOutPutImage[4].Load   logicOutPutImage[5].Load
48 - 49 bit0���ӱ���1  bit1���ӱ���2  ǰ�� + ���� +���ٱ����� + ת�ٱ����� logicOutPutImage[6].Load
56 - 63 8·����ͼ��                               logicOutPutImage[7].Load
64 - 92 ���29·���⸺��
***********************************************************************************************/
void getDriverLoadImage(void)
{
    PutLoadStruct logicOutPutImage[20];
    UINT8 i;
	
    for(i=0;i<20;i++)
    {
        logicOutPutImage[i].Load = 0x00; 
		LoadLogicResult[i].Load = 0x00;
    }
   
    ProcessLoadOutput(NULL,&logicOutPutImage[0].Load,DASHBOARD_LOAD_START_ADDR,/*DASHBOARD_LOAD_NUM*/NUMBER_OF_OUTPUT_CHANNELS);//�������8·����
         
    for(i=0;i<20;i++)
    {  
        LogicOutPut[i].Load = logicOutPutImage[i].Load;
    }

    LeftRightLoadLogic[0].Load =  LoadLogicResult[4].Load;
    LeftRightLoadLogic[1].Load =  LoadLogicResult[5].Load;
    ///Cat4016Data.Data = ((UINT16)logicOutPutImage[4].Load << 0x08) + logicOutPutImage[5].Load;
    
    if(SleepFlag==1) 
    {
		LoadSleepCnt = 0;
		///�ر�ָʾ��
		LogicOutPut[4].Load = 0;LogicOutPut[5].Load = 0;LogicOutPut[6].Load &= 0x80;//55��Ϊ�������
		///Cat4016Data.Data=0;
		
		///MIDDLE_DOOR_LED ;//= 0;
		///FRONT_DOOR_LED ;//= 0;
    } 
    else 
    {
     	LoadSleepCnt++ ;
    }
   
    PwmLoadLogic();
    
    if(LoadSleepCnt>20) 
    {
		LoadSleepCnt = 21;
		//LogicOutPut[0].Load = logicOutPutImage[0].Load;
		//LogicOutPut[6].Load = logicOutPutImage[6].Load; 
    }

	
	
    for(i=0;i<5;i++)
    {
        VirtualLoad[i].Load = logicOutPutImage[i+8].Load;
    }
}


/*********************************************************************************************** 
��������void driverLoads(void) 
���ܣ���������  
//ap:20160310 LED���и��ġ������ڵ� 
***********************************************************************************************/
void driverLoads(void)
{
	ledInterface();
	
}



//ap: 2016/3/10 Ӳ������������ͨ��PWM�����ʱ�޷�ʵ��
//ap:2016/6/3	����״̬ 0x00,  (��·0x01),  0x02,0x03
void PwmLoadLogic(void) 
{
	/*if(PwmLoadOutType&0x02)    //pwm 1
	{
		//ap:��⸺�ض�·

		if(PWMLOADST1) 
		{		
                 PWMOUT1_H;
	        }
                else
                {
                 PWMOUT1_L;
                }
        }*/

if(PwmLoadOutType&0x02)    //pwm 1
	{
	          PWMOUT1_PORT_MUX();
                PWMOUT1_PDDR_O;
		//ap:��⸺�ض�·
		if(PWMLOADST1) 
		{
			if((PwmShortSt.b.Key1)&&(PwmboardLoad.St.S1!=0x01))   //��·��...  //
			{
				PwmLoadShortCnt[1]++; 
				if(PwmLoadShortCnt[1]>50) 
				{
					PwmboardLoad.St.S1 = 0x01;//��·  
					PWMLOADST1 = 0;
				}
			}   
		}
		else
		{
			PwmLoadShortCnt[1] = 0;
		} 
		//ap:��·���������л�
		if(PWMLOADST1!=OLDPWMLOADST1)       //ja23     pw1
		{
			if(PWMLOADST1) //ap:�������� 0 ->1
			{
				if(PwmboardLoad.St.S1 != 0x01) //�Ƕ�·
				{  
					//PWMPER45=0xfffe;                    ///ap:IO�����
					//PWMDTY45=0xffff; 
					//PWMOUT1_PORT_MUX();
					//FTM_PWM_init(PWMOUT1_MODULE, PWMOUT1_CH, 100,100);
					PWMOUT1_H;
					PwmboardLoad.St.S1 = 0x03;  
				} 
				else//ap ��·
				{
					//PWMOUT1_PORT_MUX();
					//FTM_PWM_init(PWMOUT1_MODULE, PWMOUT1_CH, 100,0);
					PWMOUT1_L;
					//PWMPER45=0xfffe;                    ///ap:IO�����
					//PWMDTY45=1; 
				} 
			} 
			else //ap:�����
			{
				if(PwmboardLoad.St.S1!=0x01)  PwmboardLoad.St.S1 = 0x00;
				//PWMOUT1_PORT_MUX();
				//FTM_PWM_init(PWMOUT1_MODULE, PWMOUT1_CH, 100,0);
				PWMOUT1_L;
				//PWMPER45=0xfffe;                    ///�����
				//PWMDTY45=1;   
			}  
			OLDPWMLOADST1 =  PWMLOADST1;
		}
		
		if((POWER_12V_READ==1) && PWMLOADST1) //ap:�µ�·û V_POWER3_CTRL��������V_POWER3_CTRL==0
		{
			//ap: 2016/3/10��Ӧ���ǿ�·��?? 
			PwmboardLoad.St.S1  = 1;
		}
		else 
			USR_s_OV.DashboardLoad[0].St.S3 = PwmboardLoad.St.S1;  //modify by apo:DashboardLoad[0].St.S3->DashboardLoad[2].St.S1
	}



								
               
	//ap:pwm2
                
               
	if(PwmLoadOutType&0x01)    //pwm 1           //ja13  --  pw2
	{
		if(PWMLOADST0) 
		{
			if((PwmShortSt.b.Key0)&&(PwmboardLoad.St.S0!=0x01))   //��·��...
			{
				PwmLoadShortCnt[0]++; 
				if(PwmLoadShortCnt[0]>8)
				{
					PwmboardLoad.St.S0 = 0x01;//��· 
					PWMLOADST0 = 0; 
				}
			} 
			else 
			{
				PwmLoadShortCnt[0] = 0;
			}
		} 
		
		if(PWMLOADST0!=OLDPWMLOADST0) 
		{
			if(PWMLOADST0)      //   OLDPWMLOADST0	//ap :0 -> 1
			{                                 //   OLDPWMLOAD1 
				if(PwmboardLoad.St.S0!=0x01) 
				{  
					//PWMOUT2_PORT_MUX();
					FTM_PWM_init(PWMOUT2_MODULE, PWMOUT2_CH, 100,100);
					//PWMPER23=0xfffe;                    ///���
					//PWMDTY23=0xffff;   
					PwmboardLoad.St.S0 = 0x03;
				} 
				else
				{
					//PWMOUT2_PORT_MUX();
					FTM_PWM_init(PWMOUT2_MODULE, PWMOUT2_CH, 100,0);
					//PWMPER23=0xfffe;                    ///���
					//PWMDTY23=1; 
				}
			} 
			else 
			{
				
				if(PwmboardLoad.St.S0!=0x01)  PwmboardLoad.St.S0 = 0x00;
				//PWMOUT2_PORT_MUX();
				FTM_PWM_init(PWMOUT2_MODULE, PWMOUT2_CH, 100,0);
				//PWMPER23=0xfffe;                    ///���
				//PWMDTY23=1;   
			}  
			OLDPWMLOADST0 =  PWMLOADST0;
		}
		USR_s_OV.DashboardLoad[0].St.S2 = PwmboardLoad.St.S0; //modify by apo:DashboardLoad[0].St.S2->DashboardLoad[2].St.S0
	}
}



/*************************************************************************************************
��������void startStatus(void)
���ܣ�����״̬
*************************************************************************************************/
void startStatus(void)
{
    if(START_TOO_LONGER_LOAD)
    {
        StartStatus = 0x01;
    }
    else if(N_GEAR_UNOPEN_LOAD)
    {
        StartStatus = 0x02;
    }
    else if(RESTART_LOAD)
    {
        StartStatus = 0x03;
    } 
    else if(STARTING_LOAD)
    {
        StartStatus = 0x04;
    }
    else 
    {
        StartStatus = 0x00;
    }   
}
/*********************************************************************************************** 
��������void initDashboardOutputPin(void) 
���ܣ���ʼ���Ǳ�����˿�����,�������趨Ϊ�����ͬʱ��ʼ���0.
��������
����ֵ����
***********************************************************************************************/
void initDashboardLoadOutputPin(void) 
{
    JA1_LOAD_OUTPUT_MUX();JA1_LOAD_OUTPUT_OUT;	 								//JA1_LOAD_OUTPUT_DIR = OUT_PUT;
    JA2_LOAD_OUTPUT_MUX();JA2_LOAD_OUTPUT_OUT;									//JA2_LOAD_OUTPUT_DIR = OUT_PUT;
    LOAD_DSEL_MUX();LOAD_DSEL_OUT;												//LOAD_DSEL_DIR =  OUT_PUT;
    JA1_LOAD_OUTPUT_L;															//JA1_LOAD_DATA = 0;
    JA2_LOAD_OUTPUT_L;															//JA2_LOAD_DATA = 0;

}

/**********************************************************************************
��������static void GetSTProcess(void) 
���ܣ����ST��״̬�� 
* @brief			ad���תJA1,JA2���״̬
* @parameter		*loadst:		�����JA1,JA2���״̬�洢����λ��ʾ
**********************************************************************************/
void GetIsProcess(UINT8 *loadst)
{   
    UINT8 i,status;
	unsigned char row ,line;

	LOAD_DSEL_L;//LOAD_DSEL = 0; 
	delay_us(500);
	Bts724St[0] = AD_Trans(LOAD_ADC_MOUDLE,LOAD_ADC_CHANNL);//PTE8
	LOAD_DSEL_H;// LOAD_DSEL = 1;
	delay_us(500);
	Bts724St[1] = AD_Trans(LOAD_ADC_MOUDLE,LOAD_ADC_CHANNL);

	for(i=0;i<2;i++) 
    {
    	row = i >> 2;
		line = i & 0x03;
		if(Bts724St[i] < 3)  status =  0;  //��·
		else if(Bts724St[i] > 150) status = 1;  //nbapo:��·
		else                     status = 2;//nbapo:����
		loadst[row] |= (status << (2 * line));
    }    
} 

/**********************************************************************************
�������void LoadStatusCheck(UINT8 *loadIn,UINT8 *loadSt2BitIn, St_LoadDealInfo *LoadDealInfo,UINT8 loadMax)
���ܣ���⸺��״̬
* @parameter:			*loadIn		�����Ƹ������״̬
*						*loadSt2BitIn	
*						*LoadDealInfo
*						loadMax			ѭ���������
**********************************************************************************/ 
void LoadStatusCheck(UINT8 *loadIn,UINT8 *loadSt2BitIn, St_LoadDealInfo *LoadDealInfo,UINT8 loadMax)
{
	UINT8 InStatus,cnt;
	UINT8 row,line;
	UINT8 loadDestSt;
	UINT8 loadSrcSt;
	St_LoadDealInfo *LoadDealData;
	  
	for(cnt=0;cnt<loadMax;cnt++)
	{
		LoadDealData = &LoadDealInfo[cnt];
		loadDestSt = LoadDealData->loadDestSrcSt>>4;
		loadSrcSt  = LoadDealData->loadDestSrcSt&0x0f;
		row = cnt>>3;
		line= cnt&0x07;
		InStatus  = (loadIn[row]>>line)&0x01;
		InStatus <<= 2;
		row = cnt>>2;
		line= cnt&0x03;
		InStatus |= (loadSt2BitIn[row]>>(2*line))&0x03;
		//״̬ In  st(00->0V 01->5V 10->�������� 11->�����趨����) 
		//	   0	00		������
		//	   1	10		������
		//	   1	11		�����趨����
		//	   1	00		��·
		//	   1	01		��·
		if((loadDestSt != LOAD_CHK_SHORT)&&(loadDestSt != LOAD_CHK_OVERCURRENT))
		{
			switch(InStatus)
			{
				case LOAD_CHK_CLOSE:
					LoadDealData->loadDestSrcSt = 0;
					LoadDealData->openShortCnt  = 0;
					break;
				case LOAD_CHK_RUNING:
					if((loadSrcSt != LOAD_CHK_RUNING)&&(loadSrcSt != LOAD_CHK_CLOSE))
					{
						if(LoadDealData->openShortCnt<6)
							LoadDealData->openShortCnt++;
						else
						{
							LoadDealData->loadDestSrcSt = (LOAD_CHK_RUNING<<4)|LOAD_CHK_RUNING;
							LoadDealData->openShortCnt = 0;
						}
					}
					else 
						LoadDealData->loadDestSrcSt = (LOAD_CHK_RUNING<<4)|LOAD_CHK_RUNING;
					break;
				case LOAD_CHK_SHORT:
					if(loadSrcSt != LOAD_CHK_SHORT)
					{
						LoadDealData->loadDestSrcSt &= 0xf0;
						LoadDealData->loadDestSrcSt |= LOAD_CHK_SHORT;
						LoadDealData->openShortCnt  = 4;
					}
					else
					{
						if(LoadDealData->openShortCnt>0)
						{
							LoadDealData->openShortCnt--;
						}
						else
						{
							LoadDealData->loadDestSrcSt = (LOAD_CHK_SHORT<<4)|LOAD_CHK_SHORT;
						}
					}
					break;
				case LOAD_CHK_OPEN:
					if(loadSrcSt != LOAD_CHK_OPEN)
					{
						LoadDealData->loadDestSrcSt &= 0xf0;
						LoadDealData->loadDestSrcSt |= LOAD_CHK_OPEN;
						LoadDealData->openShortCnt  = 4;
					}
					else
					{
						if(LoadDealData->openShortCnt>0)
						{
							LoadDealData->openShortCnt--;
						}
						else
						{
							LoadDealData->loadDestSrcSt = (LOAD_CHK_OPEN<<4)|LOAD_CHK_OPEN;
						}
					}
					break;
				case LOAD_CHK_OVERCURRENT:
					if(loadSrcSt != LOAD_CHK_OVERCURRENT)
					{
						LoadDealData->loadDestSrcSt &= 0xf0;
						LoadDealData->loadDestSrcSt |= LOAD_CHK_OPEN;
						LoadDealData->openShortCnt  = 4;
					}
					else
					{
						if(LoadDealData->openShortCnt>0)
						{
							LoadDealData->openShortCnt--;
						}
						else
						{
							LoadDealData->loadDestSrcSt = (LOAD_CHK_OVERCURRENT<<4)|LOAD_CHK_OVERCURRENT;
						}
					}
					break;
				default:break;    
			}  
		} 
		else  
		{
			*loadIn  &= ~(0x01<<cnt);  //�رո���	
		}
	}
}
void GetLoadMaskStatus(St_LoadDealInfo *LoadDealInfo,UINT8 loadMax) 
{
	UINT8 i;
	UINT8 row,line;
	UINT8 loadst;
	UINT8 dsrSt,shortm,Openm;
	
	for(i=0;i<loadMax;i++) 
	{
		loadst = LoadDealInfo[i].loadDestSrcSt>>4;
		row = i>>3; 
		line= i&0x07;
		shortm = (OpenShortMask.shortMask>>line)&0x01;
		Openm  = (OpenShortMask.openMask>>line)&0x01;
		switch(loadst)
		{
			case    LOAD_CHK_CLOSE:    dsrSt = 0x00;break;            
			case    LOAD_CHK_RUNING:   dsrSt = 0x03;break;         
			case    LOAD_CHK_SHORT:    
				if(shortm)	dsrSt = 0x00; 
				else      dsrSt = 0x01; 
				break;   
			case    LOAD_CHK_OPEN:     
				if(Openm) dsrSt = 0x00;  
				else      dsrSt = 0x02;  
				break;           
			default:dsrSt = 0x00;break;            
		}
		row = i >> 2; 
		line= i & 0x03;
		USR_s_OV.DashboardLoad[row].Data &= ~(0x03<<(line*2));
		USR_s_OV.DashboardLoad[row].Data |=   dsrSt<<(line*2);
	}

}
void  StartLoadOutCheck(UINT8 *loadIn,St_LoadDealInfo *LoadDealInfo) 
{

  UINT8 loadst;
  UINT8 dsrSt;

    dsrSt = LoadDealInfo[0].loadDestSrcSt>>4;
    loadst =loadIn[0]&0x01;
    switch(dsrSt)
    {
      case    LOAD_CHK_CLOSE:                
      case    LOAD_CHK_RUNING:            
      case    LOAD_CHK_OPEN:  
	  			if(loadst)	JA1_LOAD_OUTPUT_H;								//JA1_LOAD_DATA =InStatus;
				else	JA1_LOAD_OUTPUT_L;
	  			break;  
      case    LOAD_CHK_SHORT:    break;              
      default:break;            
    }
    
    dsrSt = LoadDealInfo[1].loadDestSrcSt>>4;
    loadst = (loadIn[0]>>1)&0x01;
    switch(dsrSt)
    {
      case    LOAD_CHK_CLOSE:        
      case    LOAD_CHK_RUNING:            
      case    LOAD_CHK_OPEN:     
	  			if(loadst)	JA2_LOAD_OUTPUT_H;								//JA1_LOAD_DATA =InStatus;
				else	JA2_LOAD_OUTPUT_L;
	  			break;  
      case    LOAD_CHK_SHORT:    break;              
      default:break;            
    }  

}

void doLoads(void) 
{
    UINT8  loadst[2] = {0,0};
    UINT8 temp;
	
    temp = LogicOutPut[0].Load;//attention!!!
    ///StartLoadOutCheck(&temp,LoadDealInfo);//nbapo:JA1,JA2�������
    GetIsProcess(loadst);//���JA1,JA2���״̬
    LoadStatusCheck(&temp,loadst, LoadDealInfo,2);
	if(temp&0x01)	JA1_LOAD_OUTPUT_H;
	else	JA1_LOAD_OUTPUT_L;
	if((temp>>1)&0x01)	JA2_LOAD_OUTPUT_H;
	else	JA2_LOAD_OUTPUT_L;
    GetLoadMaskStatus(LoadDealInfo,2);     
}


extern UINT16 ShowDir,ShowCnt;
void LoadsLedPost(void) 
{
	UINT8 temp[3] = {0xff,0xff,0xff};
/*
	//����ѹ
	if((Show&0x3ff)>0x3C0) //green
	{
		#if PRESSURE_LED == OLD_pressure
		temp[4] = (UINT8)Show;//H
		temp[6] = 0x03 & (Show >> 8);
		#elif PRESSURE_LED == NEW_pressure
		temp[4] =  (UINT8)(0x0f & Show); 
		temp[6] = (UINT8)(0xfc & (Show>>2));
		#endif
	}
	else //red
	{
		#if PRESSURE_LED == OLD_pressure
		temp[4] = 0;
		temp[6] = 0xFC & (Show >> 2); 
		#elif PRESSURE_LED == NEW_pressure
		temp[4] = (UINT8)(0xf0 & (Show));
		temp[6] = (UINT8)(0x03 & (Show >> 8));
		#endif
	}
	if((Show&0x3ff)>0x3C0) //green
	{
		#if PRESSURE_LED == OLD_pressure
		temp[5] = (UINT8)Show;//H
		temp[7] = 0x03 & (Show >> 8);
		#elif PRESSURE_LED == NEW_pressure
		temp[5] = (UINT8)(0x0f & Show); 
		temp[7] = (UINT8)(0xfc & (Show>>2));
		#endif
	}
	else //red
	{
		#if PRESSURE_LED == OLD_pressure
		temp[5] = 0;
		temp[7] = 0xFC & (Show >> 2); 
		#elif PRESSURE_LED == NEW_pressure
		temp[5] = (UINT8)(0xf0 & (Show));
		temp[7] = (UINT8)(0x03 & (Show >> 8));
		#endif
	}*/


      
       if(ShowDir==0)
       {
         Out595(temp);
         IS31fL_SetLeftLeds(ShowCnt,0 , 0x80);  	
         IS31fL_SetRightLeds(ShowCnt,0, 0x80);
       }
        else if(ShowDir==1)
	 {
           IS31fL_SetLeftLeds(ShowCnt,7 , 0x80);  	
           IS31fL_SetRightLeds(ShowCnt,7, 0x80);
      
	    if(ShowCnt==0) 
		{
                    temp[0] =0;temp[1] =0;temp[2] =0;
			Out595(temp);
		}

	   }
         
         
	//Out595(temp);
}


/**********************************************************************************************
��������void dashboardStatus(void)
**********************************************************************************************/
void PowerContorolStatus(void)
{
    PowerContorolDiagnose[0].d.d0 = USR_s_OV.PowerContorolLoadState[2].St.S0;           //J101              ����           
    PowerContorolDiagnose[0].d.d1 = USR_s_OV.PowerContorolLoadState[1].St.S3;           //J102              ����             
    PowerContorolDiagnose[0].d.d2 = USR_s_OV.PowerContorolLoadState[2].St.S1;           //J103              ����  
    
    if(InsideData[POWER_CONTOROL_OFFSET+2] == 0xfffe)
    {  
        PowerContorolDiagnose[0].d.d3 = 0x01;         //J104              ������   
    }
    else
    {
        PowerContorolDiagnose[0].d.d3 = 0x03;
    }           
                  
    PowerContorolDiagnose[1].d.d0 = USR_s_OV.PowerContorolLoadState[2].St.S2;           //J105              ����
    
    if(InsideData[POWER_CONTOROL_OFFSET+1] == 0xfffe)
    {  
        PowerContorolDiagnose[1].d.d1 = 0x01;        //J106              ������
    }
    else
    {
        PowerContorolDiagnose[1].d.d1 = 0x03;
    }                                 
    PowerContorolDiagnose[1].d.d2 = USR_s_OV.PowerContorolLoadState[2].St.S3;           //J107              ����           
    if(InsideData[POWER_CONTOROL_OFFSET+0] == 0xfffe)
    {  
        PowerContorolDiagnose[1].d.d3 = 0x01;        //J108              ������   
    }
    else
    {
        PowerContorolDiagnose[1].d.d3 = 0x03;
    }                  
    PowerContorolDiagnose[2].d.d0 = USR_s_OV.PowerContorolLoadState[3].St.S0;           //J109              ����            
    PowerContorolDiagnose[2].d.d1 = USR_s_OV.PowerContorolKey[0].b.Key4;            //J110              ����            
    PowerContorolDiagnose[2].d.d2 = USR_s_OV.PowerContorolKey[0].b.Key3;            //J112              ����            
    PowerContorolDiagnose[2].d.d3 = USR_s_OV.PowerContorolKey[0].b.Key2;            //J114              ����            
    PowerContorolDiagnose[3].d.d0 = USR_s_OV.PowerContorolKey[0].b.Key0;            //J116              ����             
    PowerContorolDiagnose[3].d.d1 = USR_s_OV.PowerContorolKey[0].b.Key1;            //J118              ����            
    PowerContorolDiagnose[3].d.d2 = USR_s_OV.PowerContorolLoadState[1].St.S0;           //JA02              ����           
    PowerContorolDiagnose[3].d.d3 = USR_s_OV.PowerContorolLoadState[0].St.S3;           //JB01              ����             
    PowerContorolDiagnose[4].d.d0 = USR_s_OV.PowerContorolLoadState[0].St.S2;           //JB02              ����            
    PowerContorolDiagnose[4].d.d1 = USR_s_OV.PowerContorolLoadState[0].St.S1;           //JB03              ����       
    PowerContorolDiagnose[4].d.d2 = USR_s_OV.PowerContorolLoadState[0].St.S0;           //JB04              ����
    PowerContorolDiagnose[4].d.d3 = USR_s_OV.PowerContorolLoadState[1].St.S1;           //JC02              ����
    PowerContorolDiagnose[5].d.d0 = USR_s_OV.PowerContorolLoadState[1].St.S2;           //JD01              ����
    PowerContorolDiagnose[5].d.d1 = USR_s_OV.PowerContorolLoadState[3].St.S1;           //JD02  ����˵�� 
    PowerContorolDiagnose[5].d.d2 = USR_s_OV.PowerContorolLoadState[3].St.S2;          // JD03    ����˵��
    PowerContorolDiagnose[5].d.d3 = USR_s_OV.PowerContorolLoadState[3].St.S3;           //JD04    ����˵��
   
}

/**********************************************************************************************
��������void dashboardStatus(void)
**********************************************************************************************/
void dashboardStatus(void)                                             //�˿�                  //����          //���
{
	DashboardDiagnose[0].d.d0 = USR_s_OV.DashboardLoad[0].St.S0;			  //JA01				  ����				   1
	DashboardDiagnose[0].d.d1 = USR_s_OV.DashboardLoad[0].St.S1;			  //JA02				  ����				   2
	DashboardDiagnose[0].d.d2 = 0x01;									   //JA11 �ն˵���		   ����˵��
	if(PwmLoadOutType&0x01) 
	{
	 // if(PWMLOADST0)
	 // DashboardDiagnose[0].d.d3 = 0x03;
	 // else 
	 // DashboardDiagnose[0].d.d3 = 0x00;
	 DashboardDiagnose[0].d.d3 = PwmboardLoad.St.S0;
	}
	else DashboardDiagnose[0].d.d3 = 0x01;										//JA13					����
	
	
	DashboardDiagnose[1].d.d0 = USR_s_OV.DashboardKey[3].b.Key3;		   //JA14				   ���� 				  28
	DashboardDiagnose[1].d.d1 = USR_s_OV.DashboardKey[2].b.Key3;		   //JA15				   ���� 				  20
	DashboardDiagnose[1].d.d2 = USR_s_OV.DashboardKey[3].b.Key2;		   //JA16				   ���� 				  27
	DashboardDiagnose[1].d.d3 = USR_s_OV.DashboardKey[2].b.Key2;		   //JA17				   ���� 				  19
	DashboardDiagnose[2].d.d0 = USR_s_OV.DashboardKey[3].b.Key1;		   //JA18				   ���� 				  26							 
	DashboardDiagnose[2].d.d1 = 0x01;										//JA19				   �жϵ��� 			  25
	DashboardDiagnose[2].d.d2 = USR_s_OV.DashboardKey[3].b.Key0;		   //JA20				   ���� 
	DashboardDiagnose[2].d.d3 = USR_s_OV.DashboardKey[2].b.Key7;		   //JA22				   ���� 				  24
	if(PwmLoadOutType&0x02) 
	{
	 // if(PWMLOADST1)
	 // DashboardDiagnose[3].d.d0= 0x03;
	 // else 
	 // DashboardDiagnose[3].d.d0 = 0x00;
	 DashboardDiagnose[3].d.d0 = PwmboardLoad.St.S1;
	} 
	else 
	{
	  if(RealCarSpeed>0)   DashboardDiagnose[3].d.d0 = 0x03;   
	  else				   DashboardDiagnose[3].d.d0 = 0x00;	//JA23					�������
	}
	
	
	DashboardDiagnose[3].d.d1 = USR_s_OV.DashboardKey[2].b.Key6;		   //JA24				   ���� 				  23											
	DashboardDiagnose[3].d.d2 = USR_s_OV.DashboardKey[2].b.Key0;		   //JA25				   ���� 				  17
	DashboardDiagnose[3].d.d3 = USR_s_OV.DashboardKey[2].b.Key5;		   //JA26				   ���� 				  22
	DashboardDiagnose[4].d.d0 = USR_s_OV.DashboardKey[2].b.Key1;		   //JA27				   ���� 				  18
	DashboardDiagnose[4].d.d1 = USR_s_OV.DashboardKey[2].b.Key4;		   //JA28				   ���� 				  21
	DashboardDiagnose[4].d.d2 = USR_s_OV.DashboardKey[3].b.Key6;		   //JB01				   ���� 				  31
	DashboardDiagnose[4].d.d3 = USR_s_OV.DashboardKey[3].b.Key7;		   //JB03				   ���� 				  32
	DashboardDiagnose[5].d.d0 = USR_s_OV.DashboardKey[3].b.Key4;		   //JB04				   ���� 				  29
	DashboardDiagnose[5].d.d1 = USR_s_OV.DashboardKey[0].b.Key0;		   //JB05				   ���� 				  1
	DashboardDiagnose[5].d.d2 = USR_s_OV.DashboardKey[1].b.Key7;		   //JB06				   ���� 				  16
	DashboardDiagnose[5].d.d3 = USR_s_OV.DashboardKey[0].b.Key1;		   //JB07				   ���� 				  2
	DashboardDiagnose[6].d.d0 = USR_s_OV.DashboardKey[1].b.Key6;		   //JB08				   ���� 				  15
	DashboardDiagnose[6].d.d1 = USR_s_OV.DashboardKey[0].b.Key2;		   //JB09				   ���� 				  3
	DashboardDiagnose[6].d.d2 = USR_s_OV.DashboardKey[1].b.Key5;		   //JB10				   ���� 				  14
	DashboardDiagnose[6].d.d3 = USR_s_OV.DashboardKey[0].b.Key3;		   //JB11				   ���� 				  4
	DashboardDiagnose[7].d.d0 = USR_s_OV.DashboardKey[1].b.Key4;		   //JB12				   ���� 				  13
	DashboardDiagnose[7].d.d1 = USR_s_OV.DashboardKey[0].b.Key4;		   //JB13				   ���� 				  5
	DashboardDiagnose[7].d.d2 = USR_s_OV.DashboardKey[1].b.Key3;		   //JB14				   ���� 				  12
	DashboardDiagnose[7].d.d3 = USR_s_OV.DashboardKey[0].b.Key5;		   //JB15				   ���� 				  6
	DashboardDiagnose[8].d.d0 = USR_s_OV.DashboardKey[1].b.Key2;		   //JB16				   ���� 				  11
	DashboardDiagnose[8].d.d1 = USR_s_OV.DashboardKey[0].b.Key6;		   //JB17				   ���� 				  7
	DashboardDiagnose[8].d.d2 = USR_s_OV.DashboardKey[1].b.Key1;		   //JB18				   ���� 				  10
	DashboardDiagnose[8].d.d3 = USR_s_OV.DashboardKey[0].b.Key7;		   //JB19				   ���� 				  8
	DashboardDiagnose[9].d.d0 = USR_s_OV.DashboardKey[1].b.Key0;		   //JB20				   ���� 				  9
	DashboardDiagnose[9].d.d1 = 0x01;									   //JC01 12V��Դ		   ����˵��
	DashboardDiagnose[9].d.d2 = USR_s_OV.DashboardKey[4].b.Key6;		   //JC02				   ���� 				  39
   
	DashboardDiagnose[9].d.d3 = 0x01;									   //JC03				   ����
   
	DashboardDiagnose[10].d.d0 = USR_s_OV.DashboardKey[4].b.Key5;		   //JC04				   ���� 				  38
	
	if((SysConfigData.CarSpeedSrc==1)&&(RealCarSpeed>0))
	DashboardDiagnose[10].d.d1 = 0x03;									   //JC05				   ����
	else  DashboardDiagnose[10].d.d1 = 0x00; 
   
	DashboardDiagnose[10].d.d2 = USR_s_OV.DashboardKey[4].b.Key4;		   //JC06				   ���� 				  37
  
	if(InsideData[0] == 0xfffe)
	{  
		DashboardDiagnose[10].d.d3 = 0x01;								   //JC07				   ������
	}
	else
	{
		DashboardDiagnose[10].d.d3 = 0x03; 
	}
	DashboardDiagnose[11].d.d0 = USR_s_OV.DashboardKey[4].b.Key3;		   //JC08				   ���� 				  36
   
   
	if(InsideData[1] == 0xfffe)
	{  
		DashboardDiagnose[11].d.d1 = 0x01;								  //JC09				  ������
	}
	else
	{
		DashboardDiagnose[11].d.d1 = 0x03;
	}
	DashboardDiagnose[11].d.d2 = USR_s_OV.DashboardKey[3].b.Key5;		   //JC10				   ���� 				  30
	if(InsideData[2] == 0xfffe)
	{  
		DashboardDiagnose[11].d.d3 = 0x01;								  //JC11				  ������		  
	}
	else
	{
		DashboardDiagnose[11].d.d3 = 0x03;
	}
	DashboardDiagnose[12].d.d0 = USR_s_OV.DashboardKey[4].b.Key1;		   //JC12				   ���� 				  34 
	if(InsideData[3] == 0xfffe)  //0
	{  
		DashboardDiagnose[12].d.d1 = 0x01;								   //JC13				   ������
	}
	else
	{
		DashboardDiagnose[12].d.d1 = 0x03;
	}
	DashboardDiagnose[12].d.d2 = USR_s_OV.DashboardKey[4].b.Key0;		   //JC14				   ���� 				  33	
	if(InsideData[4] == 0xfffe)    //3
	{  
		DashboardDiagnose[12].d.d3 = 0x01;								  //JC15				  ������
	}
	else
	{
		DashboardDiagnose[12].d.d3 = 0x03;
	}
	DashboardDiagnose[13].d.d0 = USR_s_OV.DashboardKey[4].b.Key2;		   //JC16				   ���� 				  35
}

/**********************************************************************************************
��������void FrontDriverStatus(void)
���ܣ�����LCDǰ���������ʾ˳���ȡ���˿ڵ�״̬
ע�⣺������Key8��Key15Ϊ�޶��尴����
**********************************************************************************************/
void FrontDriverStatus(void)
{
    FrontDriverDiagnose[0].d.d0 = USR_s_OV.FrontDriveLoad[4].St.S0;        //JA01            ����            OUT16
    FrontDriverDiagnose[0].d.d1 = USR_s_OV.FrontDriveLoad[4].St.S1;        //JA02            ����            OUT17
    FrontDriverDiagnose[0].d.d2 = USR_s_OV.FrontDriveLoad[3].St.S3;        //JA13            ����            OUT15
    FrontDriverDiagnose[0].d.d3 = USR_s_OV.FrontDriveLoad[3].St.S2;        //JA17            ����            OUT14
    FrontDriverDiagnose[1].d.d0 = USR_s_OV.FrontDriveLoad[0].St.S2;        //JA08            ����            OUT2
    FrontDriverDiagnose[1].d.d1 = USR_s_OV.FrontDriveLoad[0].St.S3;        //JA10            ����            OUT3
    FrontDriverDiagnose[1].d.d2 = USR_s_OV.FrontDriveKey[2].b.Key1;        //JA12            ����            17
    FrontDriverDiagnose[1].d.d3 = USR_s_OV.FrontDriveLoad[0].St.S0;        //JA13            ����            OUT0
    
    FrontDriverDiagnose[2].d.d0 = USR_s_OV.FrontDriveLoad[0].St.S1;        //JA14            ����            OUT1
    FrontDriverDiagnose[2].d.d1 = USR_s_OV.FrontDriveLoad[1].St.S0;        //JA15            ����            OUT4
    FrontDriverDiagnose[2].d.d2 = USR_s_OV.FrontDriveLoad[1].St.S1;        //JA16            ����            OUT5
    FrontDriverDiagnose[2].d.d3 = USR_s_OV.FrontDriveKey[0].b.Key0;        //JA17            ����            0
    FrontDriverDiagnose[3].d.d0 = USR_s_OV.FrontDriveKey[0].b.Key1;        //JA18            ����            1    
    FrontDriverDiagnose[3].d.d1 = USR_s_OV.FrontDriveKey[0].b.Key2;        //JA19            ����            2   
    FrontDriverDiagnose[3].d.d2 = USR_s_OV.FrontDriveKey[0].b.Key3;        //JA20            ����            3 
    FrontDriverDiagnose[3].d.d3 = USR_s_OV.FrontDriveKey[1].b.Key5;        //JA21            ����            13  
    FrontDriverDiagnose[4].d.d0 = USR_s_OV.FrontDriveKey[1].b.Key6;        //JA22            ����            14 
    FrontDriverDiagnose[4].d.d1 = USR_s_OV.FrontDriveKey[0].b.Key4;        //JA23            ����            4 
    FrontDriverDiagnose[4].d.d2 = USR_s_OV.FrontDriveKey[0].b.Key5;        //JA24            ����            5  
    
    if(InsideData[FRONT_INSIDE_OFFSET] == 0xfffe)
    {  
        FrontDriverDiagnose[4].d.d3 = 0x01;        //JA25            ������          sensor1
    }
    else
    {
        FrontDriverDiagnose[4].d.d3 = 0x03; 
    }
    if(InsideData[FRONT_INSIDE_OFFSET+1] == 0xfffe)
    {  
        FrontDriverDiagnose[5].d.d0 = 0x01;        //JA26            ������          sensor2
    }
    else
    {
        FrontDriverDiagnose[5].d.d0 = 0x03;
    }
    if(InsideData[FRONT_INSIDE_OFFSET+2] == 0xfffe)
    {  
        FrontDriverDiagnose[5].d.d1 = 0x01;        //JA27            ������          sensor3
    }
    else
    {
        FrontDriverDiagnose[5].d.d1 = 0x03;
    }
    if(InsideData[FRONT_INSIDE_OFFSET+3] == 0xfffe)
    {  
        FrontDriverDiagnose[5].d.d2 = 0x01;        //JA28            ������          sensor4
    }
    else
    {
        FrontDriverDiagnose[5].d.d2 = 0x03;
    }
       
    if(InsideData[FRONT_INSIDE_OFFSET+4] == 0xfffe)
    {  
        FrontDriverDiagnose[5].d.d3 = 0x01;        //JA29            ������          sensor5
    }
    else
    {
        FrontDriverDiagnose[5].d.d3 = 0x03; 
    }
    if(InsideData[FRONT_INSIDE_OFFSET+5] == 0xfffe)
    {  
        FrontDriverDiagnose[6].d.d0 = 0x01;        //JA30            ������          sensor6
    }
    else
    {
        FrontDriverDiagnose[6].d.d0 = 0x03;
    }
    if(InsideData[FRONT_INSIDE_OFFSET+6] == 0xfffe)
    {  
        FrontDriverDiagnose[6].d.d1 = 0x01;        //JA31           ������          sensor7
    }
    else
    {
        FrontDriverDiagnose[6].d.d1 = 0x03;
    }
    if(InsideData[FRONT_INSIDE_OFFSET+7] == 0xfffe)
    {  
        FrontDriverDiagnose[6].d.d2 = 0x01;        //JA32           ������          sensor4
    }
    else
    {
        FrontDriverDiagnose[6].d.d2 = 0x03;
    }
     
    FrontDriverDiagnose[6].d.d3 = USR_s_OV.FrontDriveKey[0].b.Key6;        //JA33            ����            6
    FrontDriverDiagnose[7].d.d0 = USR_s_OV.FrontDriveLoad[1].St.S2;        //JA37            ����            OUT6
    FrontDriverDiagnose[7].d.d1 = USR_s_OV.FrontDriveLoad[1].St.S3;        //JA38            ����            OUT7
    FrontDriverDiagnose[7].d.d2 = USR_s_OV.FrontDriveLoad[2].St.S0;        //JA39            ����            OUT8
    FrontDriverDiagnose[7].d.d3 = USR_s_OV.FrontDriveLoad[2].St.S1;        //JA40            ����            OUT9
    
        
    FrontDriverDiagnose[8].d.d0 = USR_s_OV.FrontDriveKey[0].b.Key7;        //JA41            ����            7
    FrontDriverDiagnose[8].d.d1 = USR_s_OV.FrontDriveKey[1].b.Key0;        //JA42            ����            8
    FrontDriverDiagnose[8].d.d2 = USR_s_OV.FrontDriveKey[1].b.Key1;        //JA43            ����            9
    FrontDriverDiagnose[8].d.d3 = USR_s_OV.FrontDriveKey[1].b.Key2;        //JA44            ����            10
    FrontDriverDiagnose[9].d.d0 = USR_s_OV.FrontDriveKey[1].b.Key3;        //JA45            ����            11
    FrontDriverDiagnose[9].d.d1 = USR_s_OV.FrontDriveKey[1].b.Key4;        //JA46            ����            12
    FrontDriverDiagnose[9].d.d2 = USR_s_OV.FrontDriveKey[1].b.Key7;        //JA47            ����            15
    FrontDriverDiagnose[9].d.d3 = USR_s_OV.FrontDriveKey[2].b.Key0;        //JA48            ����            16
    
    FrontDriverDiagnose[10].d.d0 = USR_s_OV.FrontDriveLoad[2].St.S2;        //JA49            ����            OUT10
    FrontDriverDiagnose[10].d.d1 = USR_s_OV.FrontDriveLoad[2].St.S3;        //JA50            ����            OUT11
    FrontDriverDiagnose[10].d.d2 = USR_s_OV.FrontDriveLoad[3].St.S0;        //JA51            ����            OUT12
    FrontDriverDiagnose[10].d.d3 = USR_s_OV.FrontDriveLoad[3].St.S1;        //JA52            ����            OUT13
    
   // FrontDriverDiagnose[11].d.d0 = 1;                                     //JA53            ��ַ1            
   // FrontDriverDiagnose[11].d.d1 = 1;                                     //JA54            ��ַ2            
    FrontDriverDiagnose[11].d.d0 = 1;                                       //JA55            ת��
    if(RealCarSpeed>0)
    FrontDriverDiagnose[11].d.d1 = 1;                                       //JA56            ����  
    else 
    FrontDriverDiagnose[11].d.d1 = 0;
    
    FrontDriverDiagnose[11].d.d2 = 1;                                       //JA57            +12v         
    FrontDriverDiagnose[11].d.d3 = USR_s_OV.FrontDriveLoad[4].St.S3;        //JA58            ����            OUT19
    FrontDriverDiagnose[12].d.d0 = USR_s_OV.FrontDriveLoad[4].St.S2;        //JA59            ����            OUT18 
    FrontDriverDiagnose[12].d.d1 = 1;                                       //JA60            ����      
    FrontDriverDiagnose[12].d.d2 = 0;      
    FrontDriverDiagnose[12].d.d3 = 0;
}
/**********************************************************************************************
��������void BackDriverStatus(void)
���ܣ�����LCDǰ���������ʾ˳���ȡ���˿ڵ�״̬
ע�⣺������Key8��Key15Ϊ�޶��尴����
**********************************************************************************************/
void BackDriverStatus(void)
{
    BackDriverDiagnose[0].d.d0 = USR_s_OV.BackDriveLoad[4].St.S0;        //JA01            ����            OUT16
    BackDriverDiagnose[0].d.d1 = USR_s_OV.BackDriveLoad[4].St.S1;        //JA02            ����            OUT17
    BackDriverDiagnose[0].d.d2 = USR_s_OV.BackDriveLoad[3].St.S3;        //JA13            ����            OUT15
    BackDriverDiagnose[0].d.d3 = USR_s_OV.BackDriveLoad[3].St.S2;        //JA17            ����            OUT14
    BackDriverDiagnose[1].d.d0 = USR_s_OV.BackDriveLoad[0].St.S2;        //JA08            ����            OUT2
    BackDriverDiagnose[1].d.d1 = USR_s_OV.BackDriveLoad[0].St.S3;        //JA10            ����            OUT3
    BackDriverDiagnose[1].d.d2 = USR_s_OV.BackDriveKey[2].b.Key1;        //JA12            ����            17
    BackDriverDiagnose[1].d.d3 = USR_s_OV.BackDriveLoad[0].St.S0;        //JA13            ����            OUT0
    
    BackDriverDiagnose[2].d.d0 = USR_s_OV.BackDriveLoad[0].St.S1;        //JA14            ����            OUT1
    BackDriverDiagnose[2].d.d1 = USR_s_OV.BackDriveLoad[1].St.S0;        //JA15            ����            OUT4
    BackDriverDiagnose[2].d.d2 = USR_s_OV.BackDriveLoad[1].St.S1;        //JA16            ����            OUT5
    BackDriverDiagnose[2].d.d3 = USR_s_OV.BackDriveKey[0].b.Key0;        //JA17            ����            0
    BackDriverDiagnose[3].d.d0 = USR_s_OV.BackDriveKey[0].b.Key1;        //JA18            ����            1    
    BackDriverDiagnose[3].d.d1 = USR_s_OV.BackDriveKey[0].b.Key2;        //JA19            ����            2   
    BackDriverDiagnose[3].d.d2 = USR_s_OV.BackDriveKey[0].b.Key3;        //JA20            ����            3 
    BackDriverDiagnose[3].d.d3 = USR_s_OV.BackDriveKey[1].b.Key5;        //JA21            ����            13  
    BackDriverDiagnose[4].d.d0 = USR_s_OV.BackDriveKey[1].b.Key6;        //JA22            ����            14 
    BackDriverDiagnose[4].d.d1 = USR_s_OV.BackDriveKey[0].b.Key4;        //JA23            ����            4 
    BackDriverDiagnose[4].d.d2 = USR_s_OV.BackDriveKey[0].b.Key5;        //JA24            ����            5  
    
    if(InsideData[BACK_INSIDE_OFFSET] == 0xfffe)
    {  
        BackDriverDiagnose[4].d.d3 = 0x01;        //JA25            ������          sensor1
    }
    else
    {
        BackDriverDiagnose[4].d.d3 = 0x03; 
    }
    if(InsideData[BACK_INSIDE_OFFSET+1] == 0xfffe)
    {  
        BackDriverDiagnose[5].d.d0 = 0x01;        //JA26            ������          sensor2
    }
    else
    {
        BackDriverDiagnose[5].d.d0 = 0x03;
    }
    if(InsideData[BACK_INSIDE_OFFSET+2] == 0xfffe)
    {  
        BackDriverDiagnose[5].d.d1 = 0x01;        //JA27            ������          sensor3
    }
    else
    {
        BackDriverDiagnose[5].d.d1 = 0x03;
    }
    if(InsideData[BACK_INSIDE_OFFSET+3] == 0xfffe)
    {  
        BackDriverDiagnose[5].d.d2 = 0x01;        //JA28            ������          sensor4
    }
    else
    {
        BackDriverDiagnose[5].d.d2 = 0x03;
    }
       
    if(InsideData[BACK_INSIDE_OFFSET+4] == 0xfffe)
    {  
        BackDriverDiagnose[5].d.d3 = 0x01;        //JA29            ������          sensor5
    }
    else
    {
        BackDriverDiagnose[5].d.d3 = 0x03; 
    }
    if(InsideData[BACK_INSIDE_OFFSET+5] == 0xfffe)
    {  
        BackDriverDiagnose[6].d.d0 = 0x01;        //JA30            ������          sensor6
    }
    else
    {
        BackDriverDiagnose[6].d.d0 = 0x03;
    }
    if(InsideData[BACK_INSIDE_OFFSET+6] == 0xfffe)
    {  
        BackDriverDiagnose[6].d.d1 = 0x01;        //JA31           ������          sensor7
    }
    else
    {
        BackDriverDiagnose[6].d.d1 = 0x03;
    }
    if(InsideData[BACK_INSIDE_OFFSET+7] == 0xfffe)
    {  
        BackDriverDiagnose[6].d.d2 = 0x01;        //JA32           ������          sensor4
    }
    else
    {
        BackDriverDiagnose[6].d.d2 = 0x03;
    }
     
    BackDriverDiagnose[6].d.d3 = USR_s_OV.BackDriveKey[0].b.Key6;        //JA33            ����            6
    BackDriverDiagnose[7].d.d0 = USR_s_OV.BackDriveLoad[1].St.S2;        //JA37            ����            OUT6
    BackDriverDiagnose[7].d.d1 = USR_s_OV.BackDriveLoad[1].St.S3;        //JA38            ����            OUT7
    BackDriverDiagnose[7].d.d2 = USR_s_OV.BackDriveLoad[2].St.S0;        //JA39            ����            OUT8
    BackDriverDiagnose[7].d.d3 = USR_s_OV.BackDriveLoad[2].St.S1;        //JA40            ����            OUT9
    
        
    BackDriverDiagnose[8].d.d0 = USR_s_OV.BackDriveKey[0].b.Key7;        //JA41            ����            7
    BackDriverDiagnose[8].d.d1 = USR_s_OV.BackDriveKey[1].b.Key0;        //JA42            ����            8
    BackDriverDiagnose[8].d.d2 = USR_s_OV.BackDriveKey[1].b.Key1;        //JA43            ����            9
    BackDriverDiagnose[8].d.d3 = USR_s_OV.BackDriveKey[1].b.Key2;        //JA44            ����            10
    BackDriverDiagnose[9].d.d0 = USR_s_OV.BackDriveKey[1].b.Key3;        //JA45            ����            11
    BackDriverDiagnose[9].d.d1 = USR_s_OV.BackDriveKey[1].b.Key4;        //JA46            ����            12
    BackDriverDiagnose[9].d.d2 = USR_s_OV.BackDriveKey[1].b.Key7;        //JA47            ����            15
    BackDriverDiagnose[9].d.d3 = USR_s_OV.BackDriveKey[2].b.Key0;        //JA48            ����            16
    
    BackDriverDiagnose[10].d.d0 = USR_s_OV.BackDriveLoad[2].St.S2;        //JA49            ����            OUT10
    BackDriverDiagnose[10].d.d1 = USR_s_OV.BackDriveLoad[2].St.S3;        //JA50            ����            OUT11
    BackDriverDiagnose[10].d.d2 = USR_s_OV.BackDriveLoad[3].St.S0;        //JA51            ����            OUT12
    BackDriverDiagnose[10].d.d3 = USR_s_OV.BackDriveLoad[3].St.S1;        //JA52            ����            OUT13
    
    //BackDriverDiagnose[11].d.d0 = 1;                                       //JA53            ��ַ1            
    //BackDriverDiagnose[11].d.d1 = 1;                                       //JA54            ��ַ2            
    if(RealCarSpeed>0)         
    BackDriverDiagnose[11].d.d0 = 1;                                       //JA55            ����
    else 
    BackDriverDiagnose[11].d.d0 = 0;
    
    if(RealCarSpeed>0)
    BackDriverDiagnose[11].d.d1 = 1;                                       //JA56            ת��  
    else 
    BackDriverDiagnose[11].d.d1 = 0; 
    
    BackDriverDiagnose[11].d.d2 = 1;                                       //JA57            +12v         
    BackDriverDiagnose[11].d.d3 = USR_s_OV.BackDriveLoad[4].St.S3;        //JA58            ����            OUT19
    BackDriverDiagnose[12].d.d0 = USR_s_OV.BackDriveLoad[4].St.S2;        //JA59            ����            OUT18 
    BackDriverDiagnose[12].d.d1 = 1;                                       //JA60            ����            
    
}

/**********************************************************************************************
��������void dashboardStatus(void)
**********************************************************************************************/
void MiddleDriverStatus(void)
{
    MiddleDriverDiagnose[0].d.d0 = USR_s_OV.MiddleDriveLoad[4].St.S0;        //JA01            ����            OUT16
    MiddleDriverDiagnose[0].d.d1 = USR_s_OV.MiddleDriveLoad[4].St.S1;        //JA02            ����            OUT17
    MiddleDriverDiagnose[0].d.d2 = USR_s_OV.MiddleDriveLoad[3].St.S3;        //JA13            ����            OUT15
    MiddleDriverDiagnose[0].d.d3 = USR_s_OV.MiddleDriveLoad[3].St.S2;        //JA17            ����            OUT14
    MiddleDriverDiagnose[1].d.d0 = USR_s_OV.MiddleDriveLoad[0].St.S2;        //JA08            ����            OUT2
    MiddleDriverDiagnose[1].d.d1 = USR_s_OV.MiddleDriveLoad[0].St.S3;        //JA10            ����            OUT3
    MiddleDriverDiagnose[1].d.d2 = USR_s_OV.MiddleDriveKey[2].b.Key1;        //JA12            ����            17
    MiddleDriverDiagnose[1].d.d3 = USR_s_OV.MiddleDriveLoad[0].St.S0;        //JA13            ����            OUT0
    
    MiddleDriverDiagnose[2].d.d0 = USR_s_OV.MiddleDriveLoad[0].St.S1;        //JA14            ����            OUT1
    MiddleDriverDiagnose[2].d.d1 = USR_s_OV.MiddleDriveLoad[1].St.S0;        //JA15            ����            OUT4
    MiddleDriverDiagnose[2].d.d2 = USR_s_OV.MiddleDriveLoad[1].St.S1;        //JA16            ����            OUT5
    MiddleDriverDiagnose[2].d.d3 = USR_s_OV.MiddleDriveKey[0].b.Key0;        //JA17            ����            0
    MiddleDriverDiagnose[3].d.d0 = USR_s_OV.MiddleDriveKey[0].b.Key1;        //JA18            ����            1    
    MiddleDriverDiagnose[3].d.d1 = USR_s_OV.MiddleDriveKey[0].b.Key2;        //JA19            ����            2   
    MiddleDriverDiagnose[3].d.d2 = USR_s_OV.MiddleDriveKey[0].b.Key3;        //JA20            ����            3 
    MiddleDriverDiagnose[3].d.d3 = USR_s_OV.MiddleDriveKey[1].b.Key5;        //JA21            ����            13  
    MiddleDriverDiagnose[4].d.d0 = USR_s_OV.MiddleDriveKey[1].b.Key6;        //JA22            ����            14 
    MiddleDriverDiagnose[4].d.d1 = USR_s_OV.MiddleDriveKey[0].b.Key4;        //JA23            ����            4 
    MiddleDriverDiagnose[4].d.d2 = USR_s_OV.MiddleDriveKey[0].b.Key5;        //JA24            ����            5  
    
    if(InsideData[MIDDLE_INSIDE_OFFSET] == 0xfffe)
    {  
        MiddleDriverDiagnose[4].d.d3 = 0x01;        //JA25            ������          sensor1
    }
    else
    {
        MiddleDriverDiagnose[4].d.d3 = 0x03; 
    }
    if(InsideData[MIDDLE_INSIDE_OFFSET+1] == 0xfffe)
    {  
        MiddleDriverDiagnose[5].d.d0 = 0x01;        //JA26            ������          sensor2
    }
    else
    {
        MiddleDriverDiagnose[5].d.d0 = 0x03;
    }
    if(InsideData[MIDDLE_INSIDE_OFFSET+2] == 0xfffe)
    {  
        MiddleDriverDiagnose[5].d.d1 = 0x01;        //JA27            ������          sensor3
    }
    else
    {
        MiddleDriverDiagnose[5].d.d1 = 0x03;
    }
    if(InsideData[MIDDLE_INSIDE_OFFSET+3] == 0xfffe)
    {  
        MiddleDriverDiagnose[5].d.d2 = 0x01;        //JA28            ������          sensor4
    }
    else
    {
        MiddleDriverDiagnose[5].d.d2 = 0x03;
    }
       
    if(InsideData[MIDDLE_INSIDE_OFFSET+4] == 0xfffe)
    {  
        MiddleDriverDiagnose[5].d.d3 = 0x01;        //JA29            ������          sensor5
    }
    else
    {
        MiddleDriverDiagnose[5].d.d3 = 0x03; 
    }
    if(InsideData[MIDDLE_INSIDE_OFFSET+5] == 0xfffe)
    {  
        MiddleDriverDiagnose[6].d.d0 = 0x01;        //JA30            ������          sensor6
    }
    else
    {
        MiddleDriverDiagnose[6].d.d0 = 0x03;
    }
    if(InsideData[MIDDLE_INSIDE_OFFSET+6] == 0xfffe)
    {  
        MiddleDriverDiagnose[6].d.d1 = 0x01;        //JA31           ������          sensor7
    }
    else
    {
        MiddleDriverDiagnose[6].d.d1 = 0x03;
    }
    if(InsideData[MIDDLE_INSIDE_OFFSET+7] == 0xfffe)
    {  
        MiddleDriverDiagnose[6].d.d2 = 0x01;        //JA32           ������          sensor4
    }
    else
    {
        MiddleDriverDiagnose[6].d.d2 = 0x03;
    }
     
    MiddleDriverDiagnose[6].d.d3 = USR_s_OV.MiddleDriveKey[0].b.Key6;        //JA33            ����            6
    MiddleDriverDiagnose[7].d.d0 = USR_s_OV.MiddleDriveLoad[1].St.S2;        //JA37            ����            OUT6
    MiddleDriverDiagnose[7].d.d1 = USR_s_OV.MiddleDriveLoad[1].St.S3;        //JA38            ����            OUT7
    MiddleDriverDiagnose[7].d.d2 = USR_s_OV.MiddleDriveLoad[2].St.S0;        //JA39            ����            OUT8
    MiddleDriverDiagnose[7].d.d3 = USR_s_OV.MiddleDriveLoad[2].St.S1;        //JA40            ����            OUT9
    
        
    MiddleDriverDiagnose[8].d.d0 = USR_s_OV.MiddleDriveKey[0].b.Key7;        //JA41            ����            7
    MiddleDriverDiagnose[8].d.d1 = USR_s_OV.MiddleDriveKey[1].b.Key0;        //JA42            ����            8
    MiddleDriverDiagnose[8].d.d2 = USR_s_OV.MiddleDriveKey[1].b.Key1;        //JA43            ����            9
    MiddleDriverDiagnose[8].d.d3 = USR_s_OV.MiddleDriveKey[1].b.Key2;        //JA44            ����            10
    MiddleDriverDiagnose[9].d.d0 = USR_s_OV.MiddleDriveKey[1].b.Key3;        //JA45            ����            11
    MiddleDriverDiagnose[9].d.d1 = USR_s_OV.MiddleDriveKey[1].b.Key4;        //JA46            ����            12
    MiddleDriverDiagnose[9].d.d2 = USR_s_OV.MiddleDriveKey[1].b.Key7;        //JA47            ����            15
    MiddleDriverDiagnose[9].d.d3 = USR_s_OV.MiddleDriveKey[2].b.Key0;        //JA48            ����            16
    
    MiddleDriverDiagnose[10].d.d0 = USR_s_OV.MiddleDriveLoad[2].St.S2;        //JA49            ����            OUT10
    MiddleDriverDiagnose[10].d.d1 = USR_s_OV.MiddleDriveLoad[2].St.S3;        //JA50            ����            OUT11
    MiddleDriverDiagnose[10].d.d2 = USR_s_OV.MiddleDriveLoad[3].St.S0;        //JA51            ����            OUT12
    MiddleDriverDiagnose[10].d.d3 = USR_s_OV.MiddleDriveLoad[3].St.S1;        //JA52            ����            OUT13
    
    //MiddleDriverDiagnose[11].d.d0 = 1;                                       //JA53            ��ַ1            
    //MiddleDriverDiagnose[11].d.d1 = 1;                                       //JA54            ��ַ2   
    if(RealCarSpeed>0)         
    MiddleDriverDiagnose[11].d.d0 = 1;                                       //JA55            ����
    else 
    MiddleDriverDiagnose[11].d.d0 = 0;
   
    if(RealCarSpeed>0)
    MiddleDriverDiagnose[11].d.d1 = 1;                                       //JA56            ת��  
    else 
    MiddleDriverDiagnose[11].d.d1 = 0;  
    
    MiddleDriverDiagnose[11].d.d2 = 1;                                       //JA57            +12v         
    MiddleDriverDiagnose[11].d.d3 = USR_s_OV.MiddleDriveLoad[4].St.S3;        //JA58            ����            OUT19
    MiddleDriverDiagnose[12].d.d0 = USR_s_OV.MiddleDriveLoad[4].St.S2;        //JA59            ����            OUT18 
    MiddleDriverDiagnose[12].d.d1 = 1;                                       //JA60            ����            
}
/**********************************************************************************************
��������void dashboardStatus(void)
**********************************************************************************************/
void TopDriverStatus(void)
{
    TopDriverDiagnose[0].d.d0 = USR_s_OV.TopDriveLoad[4].St.S0;        //JA01            ����            OUT16
    TopDriverDiagnose[0].d.d1 = USR_s_OV.TopDriveLoad[4].St.S1;        //JA02            ����            OUT17
    TopDriverDiagnose[0].d.d2 = USR_s_OV.TopDriveLoad[3].St.S3;        //JA13            ����            OUT15
    TopDriverDiagnose[0].d.d3 = USR_s_OV.TopDriveLoad[3].St.S2;        //JA17            ����            OUT14
    TopDriverDiagnose[1].d.d0 = USR_s_OV.TopDriveLoad[0].St.S2;        //JA08            ����            OUT2
    TopDriverDiagnose[1].d.d1 = USR_s_OV.TopDriveLoad[0].St.S3;        //JA10            ����            OUT3
    TopDriverDiagnose[1].d.d2 = USR_s_OV.TopDriveKey[2].b.Key1;        //JA12            ����            17
    TopDriverDiagnose[1].d.d3 = USR_s_OV.TopDriveLoad[0].St.S0;        //JA13            ����            OUT0
    
    TopDriverDiagnose[2].d.d0 = USR_s_OV.TopDriveLoad[0].St.S1;        //JA14            ����            OUT1
    TopDriverDiagnose[2].d.d1 = USR_s_OV.TopDriveLoad[1].St.S0;        //JA15            ����            OUT4
    TopDriverDiagnose[2].d.d2 = USR_s_OV.TopDriveLoad[1].St.S1;        //JA16            ����            OUT5
    TopDriverDiagnose[2].d.d3 = USR_s_OV.TopDriveKey[0].b.Key0;        //JA17            ����            0
    TopDriverDiagnose[3].d.d0 = USR_s_OV.TopDriveKey[0].b.Key1;        //JA18            ����            1    
    TopDriverDiagnose[3].d.d1 = USR_s_OV.TopDriveKey[0].b.Key2;        //JA19            ����            2   
    TopDriverDiagnose[3].d.d2 = USR_s_OV.TopDriveKey[0].b.Key3;        //JA20            ����            3 
    TopDriverDiagnose[3].d.d3 = USR_s_OV.TopDriveKey[1].b.Key5;        //JA21            ����            13  
    TopDriverDiagnose[4].d.d0 = USR_s_OV.TopDriveKey[1].b.Key6;        //JA22            ����            14 
    TopDriverDiagnose[4].d.d1 = USR_s_OV.TopDriveKey[0].b.Key4;        //JA23            ����            4 
    TopDriverDiagnose[4].d.d2 = USR_s_OV.TopDriveKey[0].b.Key5;        //JA24            ����            5  
    
    if(InsideData[TOP_INSIDE_OFFSET] == 0xfffe)
    {  
        TopDriverDiagnose[4].d.d3 = 0x01;        //JA25            ������          sensor1
    }
    else
    {
        TopDriverDiagnose[4].d.d3 = 0x03; 
    }
    if(InsideData[TOP_INSIDE_OFFSET+1] == 0xfffe)
    {  
        TopDriverDiagnose[5].d.d0 = 0x01;        //JA26            ������          sensor2
    }
    else
    {
        TopDriverDiagnose[5].d.d0 = 0x03;
    }
    if(InsideData[TOP_INSIDE_OFFSET+2] == 0xfffe)
    {  
        TopDriverDiagnose[5].d.d1 = 0x01;        //JA27            ������          sensor3
    }
    else
    {
        TopDriverDiagnose[5].d.d1 = 0x03;
    }
    if(InsideData[TOP_INSIDE_OFFSET+3] == 0xfffe)
    {  
        TopDriverDiagnose[5].d.d2 = 0x01;        //JA28            ������          sensor4
    }
    else
    {
        TopDriverDiagnose[5].d.d2 = 0x03;
    }
       
    if(InsideData[TOP_INSIDE_OFFSET+4] == 0xfffe)
    {  
        TopDriverDiagnose[5].d.d3 = 0x01;        //JA29            ������          sensor5
    }
    else
    {
        TopDriverDiagnose[5].d.d3 = 0x03; 
    }
    if(InsideData[TOP_INSIDE_OFFSET+5] == 0xfffe)
    {  
        TopDriverDiagnose[6].d.d0 = 0x01;        //JA30            ������          sensor6
    }
    else
    {
        TopDriverDiagnose[6].d.d0 = 0x03;
    }
    if(InsideData[TOP_INSIDE_OFFSET+6] == 0xfffe)
    {  
        TopDriverDiagnose[6].d.d1 = 0x01;        //JA31           ������          sensor7
    }
    else
    {
        TopDriverDiagnose[6].d.d1 = 0x03;
    }
    if(InsideData[TOP_INSIDE_OFFSET+7] == 0xfffe)
    {  
        TopDriverDiagnose[6].d.d2 = 0x01;        //JA32           ������          sensor4
    }
    else
    {
        TopDriverDiagnose[6].d.d2 = 0x03;
    }
     
    TopDriverDiagnose[6].d.d3 = USR_s_OV.TopDriveKey[0].b.Key6;        //JA33            ����            6
    TopDriverDiagnose[7].d.d0 = USR_s_OV.TopDriveLoad[1].St.S2;        //JA37            ����            OUT6
    TopDriverDiagnose[7].d.d1 = USR_s_OV.TopDriveLoad[1].St.S3;        //JA38            ����            OUT7
    TopDriverDiagnose[7].d.d2 = USR_s_OV.TopDriveLoad[2].St.S0;        //JA39            ����            OUT8
    TopDriverDiagnose[7].d.d3 = USR_s_OV.TopDriveLoad[2].St.S1;        //JA40            ����            OUT9
    
        
    TopDriverDiagnose[8].d.d0 = USR_s_OV.TopDriveKey[0].b.Key7;        //JA41            ����            7
    TopDriverDiagnose[8].d.d1 = USR_s_OV.TopDriveKey[1].b.Key0;        //JA42            ����            8
    TopDriverDiagnose[8].d.d2 = USR_s_OV.TopDriveKey[1].b.Key1;        //JA43            ����            9
    TopDriverDiagnose[8].d.d3 = USR_s_OV.TopDriveKey[1].b.Key2;        //JA44            ����            10
    TopDriverDiagnose[9].d.d0 = USR_s_OV.TopDriveKey[1].b.Key3;        //JA45            ����            11
    TopDriverDiagnose[9].d.d1 = USR_s_OV.TopDriveKey[1].b.Key4;        //JA46            ����            12
    TopDriverDiagnose[9].d.d2 = USR_s_OV.TopDriveKey[1].b.Key7;        //JA47            ����            15
    TopDriverDiagnose[9].d.d3 = USR_s_OV.TopDriveKey[2].b.Key0;        //JA48            ����            16
    
    TopDriverDiagnose[10].d.d0 = USR_s_OV.TopDriveLoad[2].St.S2;        //JA49            ����            OUT10
    TopDriverDiagnose[10].d.d1 = USR_s_OV.TopDriveLoad[2].St.S3;        //JA50            ����            OUT11
    TopDriverDiagnose[10].d.d2 = USR_s_OV.TopDriveLoad[3].St.S0;        //JA51            ����            OUT12
    TopDriverDiagnose[10].d.d3 = USR_s_OV.TopDriveLoad[3].St.S1;        //JA52            ����            OUT13
    
    //TopDriverDiagnose[11].d.d0 = 1;                                     //JA53            ��ַ1            
    //TopDriverDiagnose[11].d.d1 = 1;                                     //JA54            ��ַ2            
    TopDriverDiagnose[11].d.d0 = 1;                                       //JA55            ����
    if(RealCarSpeed>0)
    TopDriverDiagnose[11].d.d1 = 1;                                       //JA56            ת��  
    else 
    TopDriverDiagnose[11].d.d1 = 0;
    
    TopDriverDiagnose[11].d.d2 = 1;                                       //JA57            +12v         
    TopDriverDiagnose[11].d.d3 = USR_s_OV.TopDriveLoad[4].St.S3;        //JA58            ����            OUT19
    TopDriverDiagnose[12].d.d0 = USR_s_OV.TopDriveLoad[4].St.S2;        //JA59            ����            OUT18 
    TopDriverDiagnose[12].d.d1 = 1;                                       //JA60            ����            
}
/**********************************************************************************************
��������void dashboardStatus(void)
**********************************************************************************************/
void Power205Status(void)
{
   
    
   // PowerDiagnose[0].d.d0  = 0;           //JA01              ����              OUT13
    PowerDiagnose[0].d.d0  = USR_s_OV.PowerFuse[1].St.S1;           //JA02              ����              OUT14
    //PowerDiagnose[0].d.d2  = 0;           //JA03              ����              OUT15
    PowerDiagnose[0].d.d1  = USR_s_OV.PowerFuse[1].St.S0;           //JA04              ����              OUT16
    PowerDiagnose[0].d.d2  = USR_s_OV.PowerFuse[0].St.S0;           //JA05              ����              OUT17
    PowerDiagnose[0].d.d3  = USR_s_OV.PowerFuse[0].St.S3;           //JA06              ����              OUT18 
    PowerDiagnose[1].d.d0  = USR_s_OV.PowerFuse[0].St.S1;           //JA07              ����              OUT13
    PowerDiagnose[1].d.d1  = USR_s_OV.PowerFuse[0].St.S2;           //JA08              ����              OUT14 
    PowerDiagnose[1].d.d2  = 0;                                     //Jb01              ��ε���          OUT12
    PowerDiagnose[1].d.d3  = USR_s_OV.PowerFuse[2].St.S1;           //Jb02              ��θ���          OUT11 
    PowerDiagnose[2].d.d0  = 0;                                     //Jb03              ����              OUT10
    PowerDiagnose[2].d.d1  = USR_s_OV.PowerFuse[2].St.S0;           //Jb04              ����              OUT3
    PowerDiagnose[2].d.d2  = USR_s_OV.PowerFuse[1].St.S2;           //Jb05              ����              OUT10
    PowerDiagnose[2].d.d3  = USR_s_OV.PowerFuse[1].St.S3;           //Jb06              ����              OUT3    
    PowerDiagnose[3].d.d0  = USR_s_OV.PowerFuse[2].St.S2;           //Jc01              ����              OUT9  
    PowerDiagnose[3].d.d1  = USR_s_OV.PowerFuse[2].St.S3;           //Jc02              ����              OUT4
    PowerDiagnose[3].d.d2  = 0;//             ����              OUT8
    PowerDiagnose[3].d.d3  = USR_s_OV.PowerFuse[3].St.S0;           //Jc04               ����              OUT5
    PowerDiagnose[4].d.d0  = 0;           //Jc05              ����              OUT9  
    PowerDiagnose[4].d.d1  = USR_s_OV.PowerFuse[3].St.S1;           //Jc06              ����              OUT4
    PowerDiagnose[4].d.d2  = 0;           //Jc07              ����              OUT8
    PowerDiagnose[4].d.d3  = USR_s_OV.PowerFuse[3].St.S2;           //Jc08              ����              OUT9  
    PowerDiagnose[5].d.d0  = 0;           //Jc09             ����              OUT4
    PowerDiagnose[5].d.d1  = USR_s_OV.PowerFuse[3].St.S3;           //Jc10              ����              OUT8
    PowerDiagnose[5].d.d2 = USR_s_OV.PowerFuse[4].St.S0;           //JD01              ����              OUT7
    PowerDiagnose[5].d.d3 = USR_s_OV.PowerFuse[4].St.S1;           //JD02              ����              OUT6  
    PowerDiagnose[6].d.d0 = USR_s_OV.PowerFuse[4].St.S3;           //JD03              ����              OUT2
    PowerDiagnose[6].d.d1 = USR_s_OV.PowerFuse[4].St.S2;           //JD04              ����              OUT1
    PowerDiagnose[6].d.d2 = 0x00;           //��
    PowerDiagnose[6].d.d3 = 0x00;           //��
    PowerDiagnose[7].d.d0 = 0x00;           //��
}
/**********************************************************************************************
��������void dashboardStatus(void)
**********************************************************************************************/
void PowerStatus(void)
{

    PowerDiagnose[0].d.d0 = USR_s_OV.PowerFuse[4].St.S0;           //JA02              ����˿             F17
    PowerDiagnose[0].d.d1 = USR_s_OV.PowerFuse[3].St.S3;           //JA04              ����˿             F16 
    PowerDiagnose[0].d.d2 = USR_s_OV.PowerFuse[3].St.S2;           //JA06              ����˿             F15
    PowerDiagnose[0].d.d3 = USR_s_OV.PowerFuse[3].St.S0;           //JA07              ����˿             F13
    PowerDiagnose[1].d.d0 = USR_s_OV.PowerFuse[3].St.S1;           //JA08              ����˿             F14 
    PowerDiagnose[1].d.d1 = USR_s_OV.PowerFuse[2].St.S3;           //JB01              ����˿             F12
    PowerDiagnose[1].d.d2 = USR_s_OV.PowerFuse[2].St.S2;           //JB02              ����˿             F11
    PowerDiagnose[1].d.d3 = USR_s_OV.PowerFuse[2].St.S1;           //JB04              ����˿             F10
    PowerDiagnose[2].d.d0 = USR_s_OV.PowerFuse[1].St.S3;           //JB05              ����˿             F8
    PowerDiagnose[2].d.d1 = USR_s_OV.PowerFuse[2].St.S0;           //JB06              ����˿             F9
    PowerDiagnose[2].d.d2 = USR_s_OV.PowerFuse[1].St.S2;           //JC01              ����˿             F7
    PowerDiagnose[2].d.d3 = USR_s_OV.PowerFuse[1].St.S1;           //JC02              ����˿             F6
    PowerDiagnose[3].d.d0 = USR_s_OV.PowerFuse[1].St.S0;           //JC04              ����˿             F5 
    PowerDiagnose[3].d.d1 = USR_s_OV.PowerFuse[0].St.S3;           //JC06              ����˿             F4
    PowerDiagnose[3].d.d2 = USR_s_OV.PowerFuse[0].St.S2;           //JC08              ����˿             F3
    PowerDiagnose[3].d.d3 = USR_s_OV.PowerFuse[0].St.S0;           //JC09              ����˿             F1
    PowerDiagnose[4].d.d0 = USR_s_OV.PowerFuse[0].St.S1;           //JC10              ����˿             F2
    PowerDiagnose[4].d.d1 = 0x01;           //     ����˵�� 
    PowerDiagnose[4].d.d2 = 0x01;           //     ����˵��
    PowerDiagnose[4].d.d3 = 0x01;           //     ����˵��
    PowerDiagnose[5].d.d0 = 0x01;           //     ����˵��
    PowerDiagnose[5].d.d1 = 0x01;           //     ����˵�� 
    PowerDiagnose[5].d.d2 = 0x01;           //     ����˵��
    PowerDiagnose[5].d.d3 = 0x01;           //     ����˵��
    PowerDiagnose[6].d.d0 = 0x01;           //     ����˵��
    PowerDiagnose[6].d.d1 = 0x01;           //     ����˵��
  
    PowerDiagnose[6].d.d2  = USR_s_OV.PowerLoadState[3].St.S0;           //JA03              ����              OUT13
    PowerDiagnose[6].d.d3  = USR_s_OV.PowerLoadState[3].St.S1;           //JA05              ����              OUT14
    PowerDiagnose[7].d.d0  = USR_s_OV.PowerLoadState[3].St.S2;           //JB03              ����              OUT15
    PowerDiagnose[7].d.d1  = USR_s_OV.PowerLoadState[3].St.S3;           //JC03              ����              OUT16
    PowerDiagnose[7].d.d2  = USR_s_OV.PowerLoadState[4].St.S0;           //JC05              ����              OUT17
    PowerDiagnose[7].d.d3  = USR_s_OV.PowerLoadState[4].St.S1;           //JC07              ����              OUT18 
    PowerDiagnose[8].d.d0  = USR_s_OV.PowerLoadState[2].St.S3;           //JD01              ��ε���          OUT12
    PowerDiagnose[8].d.d1  = USR_s_OV.PowerLoadState[2].St.S2;           //JD02              ��θ���          OUT11 
    PowerDiagnose[8].d.d2  = USR_s_OV.PowerLoadState[2].St.S1;           //JE01              ����              OUT10
    PowerDiagnose[8].d.d3  = USR_s_OV.PowerLoadState[0].St.S2;           //JE02              ����              OUT3
    PowerDiagnose[9].d.d0  = USR_s_OV.PowerLoadState[2].St.S0;           //JE03              ����              OUT9
    PowerDiagnose[9].d.d1  = USR_s_OV.PowerLoadState[0].St.S3;           //JE04              ����              OUT4
    PowerDiagnose[9].d.d2  = USR_s_OV.PowerLoadState[1].St.S3;           //JE09              ����              OUT8
    PowerDiagnose[9].d.d3 =  USR_s_OV.PowerLoadState[1].St.S0;           //JE10              ����              OUT5
    PowerDiagnose[10].d.d0 = USR_s_OV.PowerLoadState[1].St.S2;           //JE11              ����              OUT7
    PowerDiagnose[10].d.d1 = USR_s_OV.PowerLoadState[1].St.S1;           //JE12              ����              OUT6  
    PowerDiagnose[10].d.d2 = USR_s_OV.PowerLoadState[0].St.S1;           //JE13              ����              OUT2
    PowerDiagnose[10].d.d3 = USR_s_OV.PowerLoadState[0].St.S0;           //JE14              ����              OUT1
    PowerDiagnose[11].d.d0 = USR_s_OV.PowerDriveKey[0].b.Key0;           //JD04              ��θ�λ          key1
    PowerDiagnose[11].d.d1 = 0x01;           //��
    PowerDiagnose[11].d.d2 = 0x01;           //��
    PowerDiagnose[11].d.d3 = 0x01;           //��
}
/************************************************************************************************
��������void doDiagnoseStatus(void)
���ܣ����״̬
************************************************************************************************/
void doDiagnoseStatus(void)
{
    dashboardStatus();
    if(ModuleConfigTemp & 0x01)//bit0 ǰģ��
    {
        FrontDriverStatus(); 
    }
    if(ModuleConfigTemp & 0x02)//bit1 ��ģ��
    {
        BackDriverStatus();
    }
    if(ModuleConfigTemp & 0x04)//bit2 ��ģ��
    {
        MiddleDriverStatus();
    }
    if(ModuleConfigTemp & 0x08)//bit3��ģ��
    {
        TopDriverStatus();
    }
    if(ModuleConfigTemp & 0x10)//bit4���
    {
        if(SysConfigData.selectPowerCtr==1)  Power205Status();
        else PowerStatus();
    }
    if(ModuleConfigTemp & 0x40)  //bit6 ��Դ����
    {
        PowerContorolStatus();
    }
}
void doAccOff(void)
{
	if(ACC_KEY != OLD_ACC_KEY)
	{
		OLD_ACC_KEY = ACC_KEY;
		if(!ACC_KEY)
		{  
			//PowerOffSave();                                                            p
			clearData(ComGear1Warn,sizeof(ComGear1Warn));
			clearData(ComGear2Warn,sizeof(ComGear2Warn));
			clearData(ComGear3Warn,sizeof(ComGear3Warn));
			clearData(ComGear4Warn,sizeof(ComGear4Warn));
			clearData(ComSensorFaultSound,sizeof(ComSensorFaultSound));
			clearData(&ComModuleErrorFlagWarn,1);  
			clearData(OldGear1Warn,sizeof(ComGear1Warn));
			clearData(OldGear2Warn,sizeof(ComGear2Warn));
			clearData(OldGear3Warn,sizeof(ComGear3Warn));
			clearData(OldGear4Warn,sizeof(ComGear4Warn));
			clearData(OldSensorFaultSound,sizeof(ComSensorFaultSound));
			clearData(&OldModuleErrorFlagWarn,1);
			PwmboardLoad.Data = 0x00;  

			if( CountDistanceSave !=   USR_s_DeviceConfigData.TotalDistance) 
			{
				PowerOffSave(); 
				CountDistanceSave =   USR_s_DeviceConfigData.TotalDistance; 
			}
			initLoadsParameter();
			MUTE_SOUNDS_F = 0;
		} 
		else 
		{
			CountDistanceSave =   USR_s_DeviceConfigData.TotalDistance;
			//CAN1_RS = 0;
			//CAN0_RS = 0;
			//SleepFlag = 0;

			//WakeUpInit();
		}
	}
} 
/**********************************************************************************************
��������void initLoadsParameter(void)
���ܣ���ʼ���븺����صı��� 
**********************************************************************************************/
void initLoadsParameter(void)
{
    UINT8 i;
    LoadsAbnormal[0] = 0x00;
    DashboardLoadsStatus[0] = 0x00;
    LoadsSt[0] = 0x00;
    LoadsInput[0] = 0x00;
    for(i=0;i<8;i++)
    {  
        LoadsCnt[i] = 0x00;        
    }
    DashboardLoadsStatus[0] = 0x00;
	DashboardLoadsStatus[1] = 0x00;
    USR_s_OV.DashboardLoad[0].Data = 0x00;
	
    NewLoadsStatus[0] = 0x00;
    OldLoadsStatus[0] = 0x00; 
    LoadDealInfo[0].loadDestSrcSt =0; 
    LoadDealInfo[1].loadDestSrcSt =0;  
	LoadDealInfo[2].loadDestSrcSt =0; 
	LoadDealInfo[3].loadDestSrcSt =0; 
	LoadDealInfo[4].loadDestSrcSt =0; 
	LoadDealInfo[5].loadDestSrcSt =0; 
       
}
/************************************************************************************************
��������void doAccOff(void)
���ܣ�������ش��󣨿�·����·��
************************************************************************************************/
void doOnOff(void)
{
    if(ON_KEY != OLD_ON_SAVE_KEY)
	{
		OLD_ON_SAVE_KEY = ON_KEY;
		if(!ON_KEY)
		{  
			if( CountDistanceSave !=   USR_s_DeviceConfigData.TotalDistance) 
			{
				PowerOffSave(); //�ϵ籣��
				CountDistanceSave =   USR_s_DeviceConfigData.TotalDistance; 
			}
		} 
		else 
		{
			CountDistanceSave =   USR_s_DeviceConfigData.TotalDistance;
		}
	}
    if(ON_KEY)	doDistance();  //��ON���ۻ����
} 

void doLCDBKLight(void){
	if(BK_LCD_refreshFlag){
		UINT8 pv = 0;
		static UINT8 lcdBK = 80;
		pv = TftDutyCONST[USR_s_DeviceConfigData.LightLevel]+InsideData[10];
		if(pv > 80) pv = 80;
		if(lcdBK < pv) lcdBK++;
		else if(lcdBK > pv) lcdBK = pv;

		LCDBACKLIGHT_DUTY =FTM_PWM_Duty(TFT_LINGHT_FRQ,lcdBK);
		BK_LCD_refreshFlag = 0;
	}
	
}


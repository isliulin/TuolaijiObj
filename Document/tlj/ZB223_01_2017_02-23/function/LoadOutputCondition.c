#include "LoadOutputCondition.h"

#define	CONDITION_V_BIT				0x8000
#define CONDITION_L_BIT				0x4000//与或位
#define	CONDITION_N_BIT				0x2000//非
#define CONDITION_VER_INDEX_MARSK	0x1FFF

#define CLOSE_CONDITION_COUNT       11//when condition type is: holdon x miliseconds to open and close by other conditon 
#define CONDITION_1_COUNT           12
#define CONDITION_2_COUNT           11
#define OR_NEXT     0x4000 
#define NEG         0x2000 
#define VALID       0x8000

#define TIME_1S  1000
#define TIME_60S   60000
#define TIME_3600S  3600000
#define TIME_50MS  50


//The last unit of the condition formula defines the output type and the output time features: 
//15 14 13 12 11 10 9 8       7 6 5 4 3 2 1 0
//T   T (miliseconds:max 16383ms= 16.383s   )
//miliseconds--0:正常；非零：时间
//T:
/*del
del//0)    00-postpone(hold on) x miliseconds then open; if condition true postpone some seconds then output the current value,if condition false close 
del//              during and after the waiting time, if the user want to open the load, the condition should hold true. 
del//              any time when condition is false, the output will close 
del//  ---output close (postpone time)----___output open______________
del//  |condition true ======if key off here then output close========|key off_____output close___
del*/  
// 0)   00-open no more than x ms
//        __________________x ms_ _ _ _ _ _ _ _ _ _ _  _ _ _           
// ______| (condition true) |(conditon false)_ _ _close _ _|____close(although condition on)_____


//1)    01-postpone(hold on) x miliseconds then close  
//  --condition true------________holdon time_____
//                       |condition false         |output close 
//              the hold on timer will start when condition transform from true to false; 
#define OUTPUT_TYPE_POSTPONE_CLOSE  0x2800                                                  
//2)    10-Toggle output at a cycle of H_time(7bits)+L_Time(7bits) time 
//              time unit is 50ms: so Max H_time and L_Time is 128*50ms=6.4s
//             _________H_time______                        ____< H_time__ 
// ______key on|                   |_________L_time_________|            |__key off___ 

//3） 11-holdon true at least x miliseconds then open and close by other conditon 
//    (_____Must Hold on true At Least x ms to open___)____________
// ___| (on condition true)            |_ _if on conditon off then close_ _  
//                                                          _______out put open__                 
//                                                                              |___if off condition true then close____
// 
//the conditions(CONDITION_MAX_CONDITION_UNITS)=open condition(CONDITION_MAX_CONDITION_UNITS-CLOSE_CONDITION_COUNT)+close condition(CLOSE_CONDITION_COUNT) 
//bit7-bit0:the time,0-normal type, no postpone or hold on
//输出功能：
#define LoadNormalOut         1 //负载正常输出
#define LoadOutDelayClose     2 //有效延时N秒断电       开关有效时，延时N秒断电（输出N秒断电(N秒))  开关有效延时断电（N秒）
#define LoadFlickerOut        3 //闪烁:(NHZ)            闪烁(高N秒,低N秒)
#define LoadDelayOut          4 //延时N秒输出           停N秒后输出  (延时N秒输出(N秒)）延时输出（N秒）
#define LoadInvalidDelayOut   5 //无效延时输出          负载满足条件输出，不满足条件后延时N秒关闭开关无效时，延时N秒断电（延时N秒断电(N秒))开关无效延时断电(N秒)
#define LoadInterzoneOut      6 //区间输出(1~30分钟)
#define LoadConstlOut         7 //常电输出
#define LoadOnetTriggerOut    8 //一次触发
#define LoadCloseOut          9 //负载关闭
#define LoadIntermissionOut   10//间歇输出
#define LoadIntermissionOutNs 11//间隔输出（N秒）
//01100：负载N小时后输出N单位(8,2s)               



//#pragma CONST_SEG    DEFAULT
UINT32 OutPutTimer[NUMBER_OF_OUTPUT_CHANNELS];//miliseconds       //注：把NUMBER_OF_OUTPUT_CHANNELS 改为 43
UINT8  OutPutCycleCnt[NUMBER_OF_OUTPUT_CHANNELS];                 //注：把NUMBER_OF_OUTPUT_CHANNELS 改为 43
UINT8  OldOutPutData[22];

#define OUTPUT_STATE_00_TIMER_STARTED           0x01      
#define OUTPUT_STATE_HOLDON_CONDITION_TRUE      0x02
#define OUTPUT_STATE_HOLDON_TIMER_STARTED       0x04
#define OUTPUT_STATE_CYCLIC_STARTED             0x08


#define OUTPUT_STATE_11_TIMER_STARTED           0x10
#define OUTPUT_STATE_11_CONDITION_ON_OPEN       0x20
#define OUTPUT_STATE_00_TIMER_OVER              0x40
#define OUTPUT_STATE_REGION_STARTED              0x80

UINT8   OutPutState[NUMBER_OF_OUTPUT_CHANNELS];


/******************************************************************************************************
函数名：void loadTovirtualKey(void)
******************************************************************************************************/
void loadTovirtualKey(void)
{
	UINT16 virtualKey;
	UINT8 port;
	UINT8 index;
	UINT8 i;
	UINT8 style;
	UINT8 load;
	UINT8 row,line,indexRow,indexLine;
	// UINT8 offset;
	UINT8 oddCnt=0xff;
	UINT8 evenCnt = 0xff;

	{
		for(i=0;i<150;i++)
		{
			FeedWatchDog();
			virtualKey = LoadOutputConditions[i][25];
			port = (UINT8)LoadOutputConditions[i][23];
			if(port == 0x3b)
			{
				__asm("nop");
			}
			if(port == 0xff)  continue;
			
			index = (UINT8)virtualKey;
			style = (UINT8)(virtualKey >> 8);
			if(!style)
			{
				continue;
			}
			
			if(port < 4)//L : 0,1:load;2,3:pwm ;4\5\6\7:none
			{
				indexRow = index / 8;
				indexLine = index % 8;

				//row = port >> 2;//add by apo
				//line = port & 0x03;
				load = (USR_s_OV.DashboardLoad[0].Data >> (2 * line)) & 0x03;
				if(style == 0x05)//L:异常转虚拟开关
				{  
					if((load == LOAD_OPEN) || (load == LOAD_SHORT))
					{
						USR_s_OV.DashboardVirtualKey[indexRow].Data |= 0x01 << indexLine;
					}         
					else
					{
						USR_s_OV.DashboardVirtualKey[indexRow].Data &= ~(0x01 << indexLine); 
					} 
				}
			}
			else
			{
				row = port/8;
				line = port%8;
				indexRow = index/8;
				indexLine = index%8;
				if(style == 0x03)
				{                  
					if(LogicOutPut[row].Load & (0x01 << line))
					{
						// row = i/8;
						// line = i%8;
						//  if(AllLogicOutPut[row].Load&(0x01<<line))      //注kk;
						// {
						USR_s_OV.DashboardVirtualKey[indexRow].Data |= 0x01 << indexLine;   
						//  }
						//  else
						//  {
						////     USR_s_OV.DashboardVirtualKey[indexRow].Data &= ~(0x01 << indexLine);
						//}
					}
					else
					{
						USR_s_OV.DashboardVirtualKey[indexRow].Data &= ~(0x01 << indexLine);  
					}
				}
				else if(style == 0x04)//奇数按键
				{
					oddCnt++;
					if(LogicOutPut[row].Load & (0x01 << line))
					{
						OddKey |= 1 << oddCnt;
					}
					else
					{
						OddKey &= ~(1 << oddCnt);
					}
					
					if((OddKey&(1<<oddCnt)) && !(OldOddKey&(1<<oddCnt)))//nbapo:上升沿跳变
					{
						if(OddKeyValue&(1<<oddCnt))
						{
							OddKeyValue &= ~(1<<oddCnt);
						}
						else
						{
							OddKeyValue |= 1<<oddCnt;
						}
					}
					
					if((OddKey & (1<<oddCnt)) && (OddKeyValue&(1<<oddCnt)))
					{
						USR_s_OV.DashboardVirtualKey[indexRow].Data |= 0x01 << indexLine; 
					}
					else
					{
						USR_s_OV.DashboardVirtualKey[indexRow].Data &= ~(0x01 << indexLine);
					}
					
					if(OddKey & (1<<oddCnt))
					{
						OldOddKey |= 1<<oddCnt;
					}
					else
					{
						OldOddKey &= ~(1<<oddCnt);
					}
				} 
				else if(style == 0x05)
				{ 
					evenCnt++;
					if(LogicOutPut[row].Load & (0x01 << line))
					{
						EvenKey |= 1 << evenCnt;
					}
					else
					{
						EvenKey &= ~(1 << evenCnt);
					}
					if((EvenKey&(1<<evenCnt)) && !(OldEvenKey&(1<<evenCnt)))
					{
						if(EvenKeyValue&(1<<evenCnt))
						{
							EvenKeyValue &= ~(1<<evenCnt);
						}
						else
						{
							EvenKeyValue |= 1<<evenCnt;
						}
					}
					if((EvenKey&(1<<evenCnt)) && (EvenKeyValue&(1<<evenCnt)))
					{
						USR_s_OV.DashboardVirtualKey[indexRow].Data |= 0x01 << indexLine; 
					}
					else
					{
						USR_s_OV.DashboardVirtualKey[indexRow].Data &= ~(0x01 << indexLine);
					}
					if(EvenKey&(1<<evenCnt))
					{
						OldEvenKey |= 1<<evenCnt;
					}
					else
					{
						OldEvenKey &= ~(1<<evenCnt);
					}    
				}
			}
		}
	}
}



/*
The condition is a UINT16[],which stores the logic expression:
The Logic Unit is defined as:
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
V  L  N  (            VerIndex      )
V:Valid 1-this unit is a valid one; 0:mark the end of the conditions
R:reserved;
L:logic operation with the next variable, 0-&,1-|; 
N:negative operation to the current variable, 0-not negative,1-to negative 
VerIndex:verriable index(start from 0), 
    bits(12 11 10) is the module index(same as node id),identifies the source of the key
    bits(9 8 7 6 5 4 3 2 1 0)-the key index of the module;
                            the bit9:the private bit.                                                                  
                                  1:which means this key is only used by the module itself.
                                  0:public global key, when a key is a public key, it must be broadcasted in the CAN network,
                                  so the value of the key of  all the nodes are synchronized.
                            the bit8:Virtual key bit.
                                  1:virtual key
                                  0:physical key
 
the first uint: 
    0x0000-always false(close);
    0x0001-always true(open);
some test examples:(CONDITION_MAX_CONDITION_UNITS=3)
0x0000 0x0000 0x0000	no condition(not configged) return false,always close 
0x0001 0x0000 0x0000    always true(open) 
0x8000 0x0000 0x0000	just test VerIndex 0;
0x8000 0x8001 0x0000	Ver0&Ver1
0x8000 0xE001 0x8003 	Ver0&~Ver1|Ver3
0x8000 0xC001 0xA003 	Ver0&Ver1|~Ver3
 
Note: 
1.  there is no need to configuration the short cut condition for the physical output, 
    we have already  processed the short cut condition in source code. 
2.  要求逻辑表达式是最小项表达式或最简式（比如通过卡诺图消项后的）.

*/
//IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS)
                                                
BOOLEAN IsConditionTrue( UINT16  ROMCONST GENPTR*GENPTR_B pCondition,UINT8 startIndex, UINT8 maxCount)
{
    UINT8 i; UINT16 conditionUnit;BOOLEAN tempVerValue;
    BOOLEAN verValue=TRUE;
	
    FeedWatchDog();
    conditionUnit = *(pCondition+startIndex);
    if (!(conditionUnit & CONDITION_V_BIT))//fist uint valid bit is false
    {
        if (conditionUnit & 0x0001)
        {
            return TRUE;
        }                               
        return FALSE;
    }
	
    for (i=startIndex; i<startIndex+maxCount; i++)
    {
        conditionUnit = *(pCondition+i);
		
        if (!(conditionUnit & CONDITION_V_BIT))//无效
        {  
            return verValue;   
        }
		
        if(verValue == FALSE)//lastlogic is Is & and  verValue=0 
        {
            if (conditionUnit & CONDITION_L_BIT)//and currentlogic is |    
            {
				if(i == startIndex+maxCount-1 || (((*(pCondition+i+1))&CONDITION_V_BIT) == 0))	 	//下一个为逻辑项无效
				{
					//in case of the last one is wrongly added a | symble 
					//or the next is the end
					return FALSE;
				}
                verValue = TRUE;
            }
            continue;
        }
		
        tempVerValue = Key_IndexToValue(conditionUnit&CONDITION_VER_INDEX_MARSK);  //
        if (conditionUnit & CONDITION_N_BIT)//to Negative? //执行非逻辑
        {
            tempVerValue = !tempVerValue;
        }
		
        verValue &= tempVerValue; // &
        if (conditionUnit & CONDITION_L_BIT)//Is | 
        {
            if (verValue)//no need to proceed 
            {
                return TRUE;
            } 
            else
            {
                if (i==startIndex+maxCount-1 || (((*(pCondition+i+1))&CONDITION_V_BIT)==0))
                {
                    //in case of the last one is wrongly added a | symble 
                    //or the next is the end
                    return FALSE;
                }
                verValue=TRUE;
            }
        }
    }
    return verValue;
}    
//                         bit0   bit1
//unsigned long  keyStutas[100];    acc    on   1 1
/** 
 * 该函数依据LoadOutputConditions中的配置对输出进行处理 
 * 参数说明： 
 *  outPutData：RAM中的存储输出状态的字节数组，每一个bit代表一个输出通道状态。如：outPutData[1]的第0位代表其索引为8的通道
 *  outPutConditionArrayIndex_FirstDimension：
 *      为LoadOutputConditions中的输出通道配置索引；其对应outPutData的第零输出通道。
 *  channelsCount：
 *      从索引outPutConditionArrayIndex_FirstDimension开始，要处理的通道数。
 * 例子：ProcessLoadOutput(OutPutChannelData,0,NUMBER_OF_OUTPUT_CHANNELS)
 */
void ProcessLoadOutput(UINT8 *conditionData,UINT8 *outPutData,UINT8 outPutConditionArrayIndex_FirstDimension,UINT8 channelsCount)
{
    UINT8 i,row,line,row1,line1;
    UINT16 outPutTypeAndSeconds,miliSecondsType;
    UINT32 miliSeconds;
   	UINT32 flickermiliSecondsLow,flickermiliSecondsHigh;
    BOOLEAN conditionResult;
    UINT8 port;
    UINT8 outPutType;
	
    FeedWatchDog();
    for (i=outPutConditionArrayIndex_FirstDimension;i<channelsCount+outPutConditionArrayIndex_FirstDimension;i++)
	{ 
        outPutTypeAndSeconds =LoadOutputConditions[i][CONDITION_MAX_CONDITION_UNITS]; //功能+时间单位+端口
        port = (UINT8)outPutTypeAndSeconds;                                          //负载端口
        if(port == 0xff)
        {
            continue;     
        }
        if(port==36)   Stop_Time_Cnt = 1;// __DI();
       	miliSecondsType = outPutTypeAndSeconds & OUTPUT_TIME_MARSK;
       	miliSecondsType = (UINT8)(miliSecondsType>>8);                   //时间单位
       	outPutType = (UINT8)(outPutTypeAndSeconds>>11);                  //功能
       	miliSeconds=LoadOutputConditions[i][CONDITION_MAX_CONDITION_UNITS+1]; //时间值	
   		if (outPutType == 0x03)//闪烁
		{
			flickermiliSecondsLow=(UINT8)LoadOutputConditions[i][CONDITION_MAX_CONDITION_UNITS+1];
			flickermiliSecondsHigh=(UINT8)(LoadOutputConditions[i][CONDITION_MAX_CONDITION_UNITS+1]>>8);
			switch(miliSecondsType)		//时间单位
			{
				case 0:
					flickermiliSecondsLow = flickermiliSecondsLow;
					flickermiliSecondsHigh = flickermiliSecondsHigh;
					break;
				case 1:
					flickermiliSecondsLow = flickermiliSecondsLow*TIME_1S;
					flickermiliSecondsHigh = flickermiliSecondsHigh*TIME_1S;
					break;		
				case 2:
					flickermiliSecondsLow = flickermiliSecondsLow*TIME_60S;	
					flickermiliSecondsHigh = flickermiliSecondsHigh*TIME_60S;	
					break;		
				case 3:
					flickermiliSecondsLow=flickermiliSecondsLow*TIME_3600S;				
					flickermiliSecondsHigh=flickermiliSecondsHigh*TIME_3600S;				
					break;		
				case 4:
					flickermiliSecondsLow=flickermiliSecondsLow*TIME_50MS; 			
					flickermiliSecondsHigh=flickermiliSecondsHigh*TIME_50MS; 
					break;						
				default:
					break;
			}

		}
		else
		{
			switch(miliSecondsType)
			{
				case 0:
					miliSeconds=miliSeconds;
					break;
				case 1:
					miliSeconds=miliSeconds*TIME_1S;
					break;		
				case 2:
					miliSeconds=miliSeconds*TIME_60S;	
					break;		
				case 3:
					miliSeconds=miliSeconds*TIME_3600S;				
					break;		
				case 4:
					miliSeconds=miliSeconds*TIME_50MS;				
					break;						
				default:
					break;
			}
		}

		
        row = port / 8;
        line = port % 8;
        row1 = i / 8;
        line1 = i % 8;			
        //00001：负载正常输出
        if(outPutType==LoadNormalOut)//负载正常输出
        {
            conditionResult = IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
            if (conditionResult)
            {
                LoadLogicResult[row].Load|= 1<<line;
                outPutData[row] |= 1<<line;//open
                AllLogicOutPut[row1].Load |= 1<<line1;
            } 
            else
            {
                LoadLogicResult[row].Load&= ~(1<<line);
                AllLogicOutPut[row1].Load &= ~(1<<line1);//close
            }                     
        }
        //0010 开关有效延时断电
        else if (outPutType==LoadOutDelayClose)//00:max open x ms
            {
                UINT32  seconds_temp;
                conditionResult = IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
                if(conditionResult)
                {
                     
                    if (OutPutState[i]&OUTPUT_STATE_00_TIMER_STARTED)
                    {
                        if (OutPutState[i]&OUTPUT_STATE_00_TIMER_OVER)
                        {
                            ;//outPutData[i]&=~(1<<j);//close
                            AllLogicOutPut[row1].Load &= ~(1<<line1);
                            LoadLogicResult[row].Load&= ~(1<<line);
                        } 
                        else
                        {
                            if (WaitTimerCheckOver(OutPutTimer[i]))
                            {
                                ;//outPutData[i]&=~(1<<j);//close
                                AllLogicOutPut[row1].Load &= ~(1<<line1);
                                OutPutState[i]|=OUTPUT_STATE_00_TIMER_OVER;
                                 LoadLogicResult[row].Load&= ~(1<<line);
                            }
                            else
                            {
                                outPutData[row]|=1<<line;//open
                                AllLogicOutPut[row1].Load |= 1<<line1;
                                
                                LoadLogicResult[row].Load|= 1<<line;
                            }
                        }
                    } 
                    else//not start
                    {
                        LoadLogicResult[row].Load|= 1<<line;
                        
                        outPutData[row]|=1<<line;//open
                        AllLogicOutPut[row1].Load |= 1<<line1;
                        seconds_temp = (UINT32)miliSeconds;// * (UINT32)1000;
                        WaitTimerStart(&OutPutTimer[i],seconds_temp);
                        OutPutState[i]|=OUTPUT_STATE_00_TIMER_STARTED;
                        OutPutState[i]&=~OUTPUT_STATE_00_TIMER_OVER;
                    }
                } 
                else
                {  
                    LoadLogicResult[row].Load&= ~(1<<line);
                    //outPutData[i]&=~(1<<j);//close
                    AllLogicOutPut[row1].Load &= ~(1<<line1);
                    OutPutState[i]&=~OUTPUT_STATE_00_TIMER_STARTED;
                    OutPutState[i]&=~OUTPUT_STATE_00_TIMER_OVER;
                }
            } 
      //00011：闪烁:
            else if (outPutType==LoadFlickerOut)//cyclic
            {
                    conditionResult=IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
                    if (conditionResult)
                    {       LoadLogicResult[row].Load|= 1<<line;
                        if (OutPutState[i]&OUTPUT_STATE_CYCLIC_STARTED)//起始周期
                        {
                            if (WaitTimerCheckOver(OutPutTimer[i]))//timer over
                            {
                                //if (outPutData[row]&(1<<line))//last is high
                                if(LogicOutPut[row].Load & (0x01 << line))
                                {
                                    //miliSeconds=(miliSeconds & OUTPUT_TIMER_MARSK_TOGGLE_LOW)*50;
                                    WaitTimerStart(&OutPutTimer[i],flickermiliSecondsLow); 
                                    outPutData[row]&=~(1<<line);//c
                                    AllLogicOutPut[row1].Load &= ~(1<<line1);
                                } 
                                else
                                {
                                    //miliSeconds=(miliSeconds>>6)*50;
                                    WaitTimerStart(&OutPutTimer[i],flickermiliSecondsHigh); 
                                    outPutData[row]|=1<<line;//o
                                    AllLogicOutPut[row1].Load |= 1<<line1;
                                }
                            }
                            else
                            {
                                if(LogicOutPut[row].Load & (0x01 << line))
                                {
                                    outPutData[row]|=1<<line;//o
                                    AllLogicOutPut[row1].Load |= 1<<line1;
                                }
                                else
                                {
                                    outPutData[row]&=~(1<<line);//c
                                    AllLogicOutPut[row1].Load &= ~(1<<line1);
                                }
                            }
                        } 
                        else
                        {
                            outPutData[row]|=1<<line;//open 
                            AllLogicOutPut[row1].Load |= 1<<line1;
                            OutPutState[i]|=OUTPUT_STATE_CYCLIC_STARTED;
                           // miliSeconds=(miliSeconds>>6)*50;
                            WaitTimerStart(&OutPutTimer[i],flickermiliSecondsLow);   
                        }

                    } 
                    else
                    {   //if(port>=32)           //当大于 32
                        // LoadLogicResult[row].Load&= ~(1<<line);
                        //outPutData[row]&=~(1<<line);//close
                        AllLogicOutPut[row1].Load &= ~(1<<line1);
                        OutPutState[i]&=~OUTPUT_STATE_CYCLIC_STARTED;
                    }
                } 
            ////0100停N秒后动作
            else if(outPutType==LoadDelayOut) //停N秒后动作
            {
                conditionResult=IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
                if (conditionResult) 
                {
                     
                    if((OutPutState[i]&OUTPUT_STATE_HOLDON_CONDITION_TRUE))
                    {
                        if(WaitTimerCheckOver(OutPutTimer[i]))
                        {
                            outPutData[row]|=(1<<line);
                            AllLogicOutPut[row1].Load |= 1<<line1;
                            LoadLogicResult[row].Load|= 1<<line;
                        }
                        
                        //else
                        //{
                        //    outPutData[i]&=~(1<<j);
                        //}
                    }
                    else
                    {
                        LoadLogicResult[row].Load&= ~(1<<line);
                        OutPutState[i]|=OUTPUT_STATE_HOLDON_CONDITION_TRUE;
                        outPutData[row]&=~(1<<line);
                        AllLogicOutPut[row1].Load &= ~(1<<line1);
                        if(miliSeconds > 3200)
                        {
                            miliSeconds = 3200;  //最大延迟32秒
                        }
                        WaitTimerStart(&OutPutTimer[i],miliSeconds);//*10);
                    }

                } 
                else 
                {
                    outPutData[row]&=~(1<<line); 
                    AllLogicOutPut[row1].Load &= ~(1<<line1);
                    OutPutState[i]&=~OUTPUT_STATE_HOLDON_CONDITION_TRUE;
                    LoadLogicResult[row].Load&= ~(1<<line);
                }              
            } 
                        //00101：无效延时输出 
        else if (outPutType==LoadInvalidDelayOut)//01:postpone x miliseconds then close
        {
            conditionResult=IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
            if (conditionResult)
            {
                LoadLogicResult[row].Load|= 1<<line;
                AllLogicOutPut[row1].Load |= 1<<line1;
                outPutData[row]|=1<<line; 
                OutPutState[i]|=OUTPUT_STATE_HOLDON_CONDITION_TRUE; //0x02
                OutPutState[i]&=~OUTPUT_STATE_HOLDON_TIMER_STARTED; //0x04
            } 
            else
            {
               
               
                if ((OutPutState[i]&OUTPUT_STATE_HOLDON_TIMER_STARTED)!=0)   //延时xms
                {                               
                    if (WaitTimerCheckOver(OutPutTimer[i]))//timer over
                    {
                        //outPutData[i]&=~(1<<j);//close
                        AllLogicOutPut[row1].Load &= ~(1<<line1);
                        OutPutState[i]&=~OUTPUT_STATE_HOLDON_CONDITION_TRUE;
                        OutPutState[i]&=~OUTPUT_STATE_HOLDON_TIMER_STARTED;
                        LoadLogicResult[row].Load&= ~(1<<line);
                    } 
                    else
                    {
                        outPutData[row]|=1<<line;//open
                        AllLogicOutPut[row1].Load |= 1<<line1;
                        LoadLogicResult[row].Load |= 1<<line;
                    }
                } 
                else
                {
                    if (OutPutState[i]&OUTPUT_STATE_HOLDON_CONDITION_TRUE)
                    {
                        //timer start when condition transform from true to false
                       // miliSeconds *= 10;
                        WaitTimerStart(&OutPutTimer[i],miliSeconds);
                        OutPutState[i]|=OUTPUT_STATE_HOLDON_TIMER_STARTED;
                        outPutData[row]|=1<<line;//open
                        AllLogicOutPut[row1].Load |= 1<<line1;
                        LoadLogicResult[row].Load |= 1<<line;
                    } 
                    else
                    {
                        //outPutData[i]&=~(1<<j);//close
                        AllLogicOutPut[row1].Load &= ~(1<<line1);
                    }
                }
            }

        }
        else if(outPutType==LoadInterzoneOut) // 区间
      		{
      			conditionResult=IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
      			if (conditionResult)
      			{
      				if (OutPutState[i]&OUTPUT_STATE_REGION_STARTED)
      				{
      					if (WaitTimerCheckOver(OutPutTimer[i])&&OutPutState[i]&OUTPUT_STATE_00_TIMER_OVER)//timer over
      					{
      						if(LogicOutPut[row].Load & (0x01 << line))
      						{
      							outPutData[row]&=~(1<<line);//c
      							AllLogicOutPut[row1].Load &= ~(1<<line1);
      							
      							OutPutState[i]&=~OUTPUT_STATE_00_TIMER_OVER;
      						} 
      						else
      						{
      							WaitTimerStart(&OutPutTimer[i],flickermiliSecondsLow); 
      							outPutData[row]|=1<<line;//o
      							AllLogicOutPut[row1].Load |= 1<<line1;
      							
      							
      						}
      					}
      					else
      					{
      						if(LogicOutPut[row].Load & (0x01 << line))
      						{
      							outPutData[row]|=1<<line;//o
      							AllLogicOutPut[row1].Load |= 1<<line1;
      						}
      						else
      						{
      							outPutData[row]&=~(1<<line);//c
      							AllLogicOutPut[row1].Load &= ~(1<<line1);
      						}
      					}
      				} 
      				else
      				{
      					OutPutState[i]|=OUTPUT_STATE_REGION_STARTED;
      					WaitTimerStart(&OutPutTimer[i],flickermiliSecondsHigh );	
      					OutPutState[i]|=OUTPUT_STATE_00_TIMER_OVER;					
      				}
      	
      			} 
      			else
      			{
      				//outPutData[row]&=~(1<<line);//close				
      				AllLogicOutPut[row1].Load &= ~(1<<line1);
      				OutPutState[i]&=~OUTPUT_STATE_REGION_STARTED;
      			}
      		}
      		            //0111 //常电输出
        else if(outPutType==LoadConstlOut) ////常电     
        {
            outPutData[row]|=1<<line;//open
            AllLogicOutPut[row1].Load |= 1<<line1;
                   
        }	

    		else if(outPutType==LoadOnetTriggerOut)//一次触发
    		{
    			BOOLEAN onConditionResult=IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
    			if (onConditionResult)
    			{
    				outPutData[row]|=1<<line;//open
    				AllLogicOutPut[row1].Load |= 1<<line1;
    			  LoadLogicResult[row].Load |= 1<<line;
    			}
    			else
    			{
    				if(LogicOutPut[row].Load & (0x01 << line))
    				{
    					outPutData[row]|=1<<line;//open
    					AllLogicOutPut[row1].Load |= 1<<line1;
    				  LoadLogicResult[row].Load |= 1<<line;
    				}
    			}
    		} 
    	
    		else if(outPutType==LoadCloseOut)//关闭
    		{
    			BOOLEAN onConditionResult=IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
    			if (onConditionResult)
    			{
    				outPutData[row]&=~(1<<line);//close
    			  LoadLogicResult[row].Load &= ~(1<<line);
    				AllLogicOutPut[row1].Load &= ~(1<<line1);
    			}
    		}
    		
    		 
     	else if(outPutType==LoadIntermissionOut) // 间歇输出
        {
            conditionResult=IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
            if (conditionResult)
            {
            
			        	miliSeconds=USR_s_OV.rain_scr*1000;
                if (OutPutState[i]&OUTPUT_STATE_CYCLIC_STARTED)
                {
                    if (WaitTimerCheckOver(OutPutTimer[i]))//timer over
                    {
                        if(LogicOutPut[row].Load & (0x01 << line))
                        {
                            WaitTimerStart(&OutPutTimer[i],miliSeconds); 
                            outPutData[row]&=~(1<<line);//c
                            AllLogicOutPut[row1].Load &= ~(1<<line1);
                        } 
                        else
                        {
                            WaitTimerStart(&OutPutTimer[i],500); 
                            outPutData[row]|=1<<line;//o
                            AllLogicOutPut[row1].Load |= 1<<line1;
                        }
                    }
                    else
                    {
                        if(LogicOutPut[row].Load & (0x01 << line))
                        {
                            outPutData[row]|=1<<line;//o
                            AllLogicOutPut[row1].Load |= 1<<line1;
                        }
                        else
                        {
                            outPutData[row]&=~(1<<line);//c
                            AllLogicOutPut[row1].Load &= ~(1<<line1);
                        }
                    }
                } 
                else
                {
                    outPutData[row]|=1<<line;//open            
                    AllLogicOutPut[row1].Load |= 1<<line1;
                    OutPutState[i]|=OUTPUT_STATE_CYCLIC_STARTED;
                    WaitTimerStart(&OutPutTimer[i],500);   
                }

            } 
            else
            {
                //outPutData[row]&=~(1<<line);//close               
                AllLogicOutPut[row1].Load &= ~(1<<line1);
                OutPutState[i]&=~OUTPUT_STATE_CYCLIC_STARTED;
            }
        }
        		else if(outPutType==LoadIntermissionOutNs)  //间隔输出
        		{
                    conditionResult=IsConditionTrue(LoadOutputConditions[i],0,CONDITION_MAX_CONDITION_UNITS);
                    if (conditionResult)
                    {
        				if(WaitTimerCheckOver(OutPutTimer[i]))
        				{
        					outPutData[row]|=1<<line;//open 		   
        					AllLogicOutPut[row1].Load |= 1<<line1;
        					OutPutState[i] |= OUTPUT_STATE_00_TIMER_STARTED;		
        				}
        				else
        				{
        					outPutData[row]&=~(1<<line);//close
        					AllLogicOutPut[row1].Load &= ~(1<<line1);			
        				}       
                    } 
                    else
                    {  
                        if (OutPutState[i]&OUTPUT_STATE_00_TIMER_STARTED)
        					WaitTimerStart(&OutPutTimer[i],miliSeconds);

        				AllLogicOutPut[row1].Load &= ~(1<<line1);
                        OutPutState[i]&=~OUTPUT_STATE_00_TIMER_STARTED;				
                    }
              }	
		
            


           
               if(port>44)   Stop_Time_Cnt = 0;// __EI();
    	}
       // __EI();
}


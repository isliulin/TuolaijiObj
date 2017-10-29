#define		TftDisplay_C
#include	"TW_FontOsd_Picture.h"
#include	"TW_SpiOsd_Picture.h"
#pragma CODE_SEG  USER_ROM
#pragma MESSAGE DISABLE C12056///屏蔽无必要的编译提示 
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、数字显示转换
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Tft_Display(void)
{
	 if(SleepFlag==1)		
		 return;
   if(OnKeyBit==KeyOn)
   {	
   		First_dis_control();	 
			if(DelayForDispNum!=3)
				return;
			MainMenuSpecialStateDetect();
			Screen_display();
   }
	 else
	 {
	 		BuzzerLevelSetFlag=0;
			LightLevelSetFlag=0;
	 		MainMenuDis_countDelay=0;
			MainMenuDis_count=0;
			MainMenuLogoTime=0;
	 		LcdOffDeal();
	 }
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、数字显示转换
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static	void First_dis_control(void)//判断屏幕显示不显示，蜂鸣器叫不叫
{
	uchar i;

	switch(DelayForDispNum)///when first on,delay 2 cycle to wait stability state
	{
		case 0:
			TW88XX_Power(1);
			WaitTW8825SystemStability();
			if(TVPowerStability_Flag==1)
				DelayForDispNum++;
			break;
		case 1:
		#if		VIDOE_NUMBER==0
				VideoEnableFlag=0;
		#else
			#if  VIDOE_NUMBER==1
					if(RShieldSignalKeyBit==KeyOn)
						VideoEnableFlag=1;
			#elif		VIDOE_NUMBER==2	
					if(RShieldSignalKeyBit==KeyOn||MiddleDoorOpenSignalKeyBit==KeyOn||MiddleDoorOpenSignalDelayFlag==1)
						VideoEnableFlag=1;
			#elif		VIDOE_NUMBER==3	
					if(RShieldSignalKeyBit==KeyOn||MiddleDoorOpenSignalKeyBit==KeyOn||UpVideoSignalKeyBit==KeyOn) 
						VideoEnableFlag=1;
			#endif
		#endif
			if(OnKeyBit==KeyOn)///如果上电时ON档已经在开启状态时的情况
				lcd_flag=1;
			if(lcd_flag==1)
			{
				lcd_flag=0; 
				///对视频板FLASH的配置,必须配置否则将出现无法与外挂FLASH的通讯异常
				InitCPU();
				InitCore();
				InitSystem(0);	
				///当刚上到ON档时，对之前的报警状态还原
				for(i=0;i<FIRST_ERROR_NUM;i++)
					SmallError[i]=SmallErrorOld[i];
				for(i=0;i<sizeof(First_Error);i++)
					First_Error[i]=First_ErrorOld[i];
				for(i=0;i<SECOND_ERROR_NUM;i++)
					Second_Error[i]=Second_ErrorOld[i];
				for(i=0;i<sizeof(SpecialSecond_Error);i++)
					SpecialSecond_Error[i]=SpecialSecond_ErrorOld[i];
				for(i=0;i<sizeof(SpecialThird_Error);i++)
					SpecialThird_Error[i]=SpecialThird_ErrorOld[i];
			} 
			DelayForDispNum++;
			break;
		case 2:
			Init_PictureState();
			BackLightCmd(BackLightOpen);
			DelayForDispNum++;	
			break;
		case 3:
			RShieldSignal_MiddleDoorOpen_VideoDisplay();
			break;
		default:
			break;
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、数字显示转换
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	LcdOffDeal(void)
{
	if(lcd_flag==0)
	{			
	  lcd_flag=1;	
		count_st=0;///clear count start key number ram
		Switch_Video_Cursor=SWITCH_VIDEO_OSD;///clear switch video change when power off
		VideoOperationFlag=0;		
		VideoEnableFlag=0;
		menu_id=LogoMenuNum;
		MainMenuDispIndex=0;
		Display_MainMenu_Flag=1;
		OsdCmd(OsdClose);
 		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
		BackLightCmd(BackLightOff);
		TW88XX_Power(0);
		TVPowerStability_Flag=0;
		DelayForDispNum=0;
  }
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、数字显示转换
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void RShieldSignal_MiddleDoorOpen_VideoDisplay(void)
{
	#if 	VIDOE_NUMBER==0
	if(0)
	#elif		VIDOE_NUMBER==1
	if(RShieldSignalKeyBit==KeyOn)
	#elif		VIDOE_NUMBER==2	
	if(RShieldSignalKeyBit==KeyOn||MiddleDoorOpenSignalKeyBit==KeyOn||MiddleDoorOpenSignalDelayFlag==1)
	#elif		VIDOE_NUMBER==3	
	if(RShieldSignalKeyBit==KeyOn||MiddleDoorOpenSignalKeyBit==KeyOn||UpVideoSignalKeyBit==KeyOn)
	#endif
	{
	 	if(VideoEnableFlag==1)
	 	{
	 		VideoEnableFlag=0;
			OsdCmd(OsdClose);
			if(MiddleDoorOpenSignalKeyBit==KeyOn||MiddleDoorOpenSignalDelayFlag==1)
			{
				if(MiddleDoorOpenSignalDelayFlag==0)
					MiddleDoorOpenSignalDelayFlag=1;	
				if(MiddleDoorOpenSignalKeyBit==KeyOn)
					MiddleDoorOpenSignalDelayNum=0;					
				VideoChannelSelect(MiddleDoorVideoChannel);
				Switch_Video_Cursor=SWITCH_VIDEO_MIDDLE_DOOR;
				VideoChannelTemp =VIDEO_CHANNEL_MIDDLE_DOOR;
			}
			else if(RShieldSignalKeyBit==KeyOn)
			{
				VideoChannelSelect(RShieldVideoChannel);
				Switch_Video_Cursor=SWITCH_VIDEO_R_SHIELD;
				VideoChannelTemp =VIDEO_CHANNEL_R_SHIELD;
			}
			else if(UpVideoSignalKeyBit==KeyOn)
			{
				VideoChannelSelect(UpVideoChannel);
				Switch_Video_Cursor=SWITCH_VIDEO_UP_VIDEO;
				VideoChannelTemp =VIDEO_CHANNEL_UP_VIDEO;
			}
			Osd_Video_Change(Video_State);
			
			menu_id=VideoMenuNum;
			Display_VideoMenu_Flag=1;//显示主界面的标志位
	 	}
	}
	else
	{
		#if		VIDOE_NUMBER==3	
		////if(UpVideoDispDelayFlag==0)
		#endif
		{
		 	if(VideoEnableFlag==0)
		 	{
		 		VideoEnableFlag=1;
				if(MainMenuLogoTime>800)
					menu_id=MainMenuNum;
				else
					menu_id=LogoMenuNum;
				MainMenuDispIndex=0;
				Display_LogoMenu_Flag=1;
				Display_MainMenu_Flag=1;
				SpiOsdWinAllOff();
				FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
				Osd_Video_Change(Osd_State);
		 	}
	  }
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、数字显示转换
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void st_process(void)
{
  switch(st_index)
  {
    case 0:
      if(((StartKeyBit==KeyOn)&&(BackRoomKeyBit==KeyOn))||((BackStartKeyBit==KeyOn)&&(OnKeyBit==KeyOn)/*&&(BackRoomKeyBit==KeyOff)*/)/*&&(ENGINE_ECU_CANErrorFlag==0)*/)			//start key on
      {	       
        St_Count_Empty_Shield=0;
        St_Count_Normal=0;
				St_Count_Safty=0;
        delay15s=0;
        st_index=1;
				count_st++;				///start number count 
      }
      else
      {
        st_status=EngineIdleNum;
      }
      break;
    case 1:
      if(((StartKeyBit==KeyOn)&&(BackRoomKeyBit==KeyOn))||((BackStartKeyBit==KeyOn)&&(OnKeyBit==KeyOn)/*&&(BackRoomKeyBit==KeyOff)*/)/*&&(ENGINE_ECU_CANErrorFlag==0)*/)
      {
        if(motor_st_flag==1)			///restart alarm											
        {         
          delay15s=0;
          St_Count_Normal=0;
          St_Count_Empty_Shield=0;
					St_Count_Safty=0;
          st_status=EngineRestartStateNum;
				}
	      else if(NShieldSignalKeyBit==KeyOff)	  ///empty shelve key not open
	      {	          
	        delay15s=0;
	        St_Count_Normal=0;
					St_Count_Safty=0;
					St_Count_Empty_Shield++;
	        if(St_Count_Empty_Shield>EmptyDetectNum)
	        {
	          St_Count_Empty_Shield=102;
	          st_status=EngineStartNShieldNum;							
	        }
		  		count_st=0;
	      }
				/*else if(SaftyKeyBit==KeyOff)///empty shelve key or safty key not open MainMenu_AnQuanKaiGuanWeiGuan
				{
					delay15s=0;
	        St_Count_Empty_Shield=0;;
	        St_Count_Normal=0;
					St_Count_Safty++;
	        if(St_Count_Safty>SaftySwitchDetectNum)
	        {
	          St_Count_Safty=102;
	          st_status=EngineStartSaftyNum;							
	        }
		  		count_st=0;
				}*/
	      else if(delay15s>EngineStartDelayNum)
	      {
	        delay15s=3500;
	        St_Count_Normal=0;
	        St_Count_Empty_Shield=0;
					St_Count_Safty=0;
	        st_status=EngineStartTooLongNum;          ///engine start too long          
	        if(engSpeed>EngineStartRotationNum||EngineChargeIndicateKeyBit==KeyOff)						
		  	 		count_st=1;
		  		else 
						count_st=0;	  
	      }
	      else 
	      {
	        St_Count_Empty_Shield=0;
					St_Count_Safty=0;
					St_Count_Normal++;
	        if(St_Count_Normal>NormalDetectNum)
	        {
	          St_Count_Normal=7;
	          st_status=EngineStartProcessNum;       ///engine start process            
	        }
	      }
      }
      else
      {
        st_status=EngineIdleNum;
        st_index=0;
      }
      break;
    default:
      st_index=0;
      break;
  }
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、数字显示转换
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void motor_st(void)
{
  if(count_st>=0x02)
	{
		///restart judge engine state
		if(engSpeed>EngineStartRotationNum||EngineChargeIndicateKeyBit==KeyOff)			
      motor_st_flag=1;
		else 
		{
			count_st=0x00;
		}
  }
	else 
	{
		motor_st_flag=0;
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、数字显示转换
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void MainMenuSpecialStateDetect(void)
{
	if(OnKeyBit==KeyOff)
		return;
	st_process();
	motor_st();
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、数字显示转换
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Digit_Data_Conversation_To_Real_Display
(ulong Digit_Data,uchar *Digit_Data_Real_Disp,
 uchar Digit_Data_Real_Disp_Number,uchar Dot_Positon_Disp
)
{
	ulong	Digit_Data_Temp;
	ulong	Digit_Data_Conversation_Temp;///
	uchar	Digit_Conversation_Num;///共几位
	uchar	Digit_Coversation_Data[10];///以32位的数据最大是10位数

	Digit_Data_Temp=(ulong)Digit_Data;///不管其数值大小，都将其转换为32位
	///以下的操作是取各位的值，然后放到一个临时变量中
	Digit_Coversation_Data[9]=(uchar)(Digit_Data_Temp/1000000000);
	Digit_Data_Conversation_Temp=Digit_Data_Temp/100000000;
	Digit_Coversation_Data[8]=(uchar)(Digit_Data_Conversation_Temp%10);
	Digit_Data_Conversation_Temp=Digit_Data_Temp/10000000;
	Digit_Coversation_Data[7]=(uchar)(Digit_Data_Conversation_Temp%10);
	Digit_Data_Conversation_Temp=Digit_Data_Temp/1000000;
	Digit_Coversation_Data[6]=(uchar)(Digit_Data_Conversation_Temp%10);
	Digit_Data_Conversation_Temp=Digit_Data_Temp/100000;
	Digit_Coversation_Data[5]=(uchar)(Digit_Data_Conversation_Temp%10);
	Digit_Data_Conversation_Temp=Digit_Data_Temp/10000;
	Digit_Coversation_Data[4]=(uchar)(Digit_Data_Conversation_Temp%10);
	Digit_Data_Conversation_Temp=Digit_Data_Temp/1000;
	Digit_Coversation_Data[3]=(uchar)(Digit_Data_Conversation_Temp%10);
	Digit_Data_Conversation_Temp=Digit_Data_Temp/100;
	Digit_Coversation_Data[2]=(uchar)(Digit_Data_Conversation_Temp%10);
	Digit_Data_Conversation_Temp=Digit_Data_Temp/10;
	Digit_Coversation_Data[1]=(uchar)(Digit_Data_Conversation_Temp%10);
	Digit_Coversation_Data[0]=(uchar)(Digit_Data_Temp%10);
	
	if(Dot_Positon_Disp==DIGIT_DATA_NO_DOT)
	{
		for(Digit_Conversation_Num=0;Digit_Conversation_Num<Digit_Data_Real_Disp_Number;Digit_Conversation_Num++)
		{
			*Digit_Data_Real_Disp=Digit_Coversation_Data[Digit_Data_Real_Disp_Number-1-Digit_Conversation_Num];
			Digit_Data_Real_Disp++;
		}		
	}
	else
	{
		if(Dot_Positon_Disp==DIGIT_DATA_0_DOT)
		{
			*Digit_Data_Real_Disp=0;///第1个位置为0，即在0位
			Digit_Data_Real_Disp++;
			*Digit_Data_Real_Disp=_16_26_DIGIT_DOT_POS;
			Digit_Data_Real_Disp++;
			for(Digit_Conversation_Num=2;Digit_Conversation_Num<Digit_Data_Real_Disp_Number;Digit_Conversation_Num++)
			{
				*Digit_Data_Real_Disp=Digit_Coversation_Data[Digit_Data_Real_Disp_Number-1-Digit_Conversation_Num];
				Digit_Data_Real_Disp++;
			}		
		}
		else
		{
			for(Digit_Conversation_Num=0;Digit_Conversation_Num<Digit_Data_Real_Disp_Number;Digit_Conversation_Num++)
			{
				if(Digit_Conversation_Num<Dot_Positon_Disp)
					*Digit_Data_Real_Disp=Digit_Coversation_Data[Digit_Data_Real_Disp_Number-2-Digit_Conversation_Num];
				else	if(Digit_Conversation_Num==Dot_Positon_Disp)
					*Digit_Data_Real_Disp=_16_26_DIGIT_DOT_POS;
				else
					*Digit_Data_Real_Disp=Digit_Coversation_Data[Digit_Data_Real_Disp_Number-1-Digit_Conversation_Num];
				Digit_Data_Real_Disp++;
			}
		}		
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、将ASC转换为对应字符库的数据
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
七、备注:
1、仅实现数字和字符，及部分的ASC2转换
***************************************************************************/

void	ASC_Data_Conversation_To_Real_Display(uchar *ASC_Data,uchar *Real_Disp,uchar count)
{
	uchar i,temp;
	for(i=0;i<count;i++)
	{
		if(('0'<=(*ASC_Data))&&((*ASC_Data)<='9'))
		{
			*Real_Disp =*ASC_Data-'0';
		}
		else if(('A'<=(*ASC_Data))&&((*ASC_Data)<='Z'))
		{
			temp =*ASC_Data-'A';
			*Real_Disp =temp+FONT_WORD_A;
		}
		else if(('a'<=*ASC_Data)&&(*ASC_Data<='z'))		
		{
			temp =*ASC_Data-'a';
			*Real_Disp =temp+FONT_WORD_a;
		}
		else if(*ASC_Data=='-')
			*Real_Disp =FONT_WORD_HENG;
		else if(*ASC_Data==':')
			*Real_Disp =FONT_WORD_ZHUANG_DIAN;
		else if(*ASC_Data=='.')
			*Real_Disp =FONT_WORD_DIAN;
		else
			*Real_Disp =*ASC_Data;
		Real_Disp++;
		ASC_Data++;
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、将数字转换为16进制显示
三、形参数：
1、
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
七、备注:
1、未检测数组长度，调用时需确保数组长度足够
***************************************************************************/
void	Digit_Data_Conversation_To_Hex_Display(ulong Digit_Data,uchar *Hex_Disp,uchar count)
{
	uchar i,temp;
	for(i=0;i<count;i++)
	{
		temp =(uchar)(Digit_Data&0x0f);
		if(temp>9)
		{
			temp-=10;
			temp+=FONT_WORD_A;
		}
		Hex_Disp[count-i-1]=temp;
		Digit_Data>>=4;
	}
}

/*******************************************************************************************************
funtion operation:the process of display the tft lcd content
funtion call:void menu_main_1(void);
				void st_display(void);void error_display(void);void menu_main_2(void);void menu_main_3(void);
				void menu_dis2(void);void menu_dis3_error(void);void menu_main_3(void);void menu_dis3_1(void);
				void menu_dis3_2(void);void menu_dis3_3(void); 
input parameter:void
return:void  
********************************************************************************************************/
static	void Screen_display(void)
{
	if(menu_id==LogoMenuNum)
		Display_LogoMenu();   //LOGO
  else if(menu_id==MainMenuNum)	
		Display_MainMenu();   //显示主界面，LOGO，22个小图标等  
  else if(menu_id==SecondMenuNum)
   	Display_SubMenu();     //显示二级菜单界面，故障查询等
  else if(menu_id==ThirdMenuNum)
  {
		switch(SubMenu_Cursor)
		{
			case MalfunctionMenuNum:
				Display_ThirdMenu_Malfunction();
				break;
			case DiagnoseMenuNum:
				Display_ThirdMenu_DiagnoseQuery();	
				break;
			case SystemParameterMenuNum:
				Display_ThirdMenu_SystemParameter();
				break;
			case DistanceFualConsumeMenuNum:
				Display_ThirdMenu_DistanceFualConsume();
				break;
			case PictureAndChannleMenuNum:
				Display_ThirdMenu_PictureAndChannleSet();
				break;
			case	TimeSetMenuNum:
				Display_ThirdMenu_SystemTimeSet();
				break;
			case	TyrePressMenuNum:
				Display_ThirdMenu_TyrePress();
				break;
			case	HelpInfomationMenuNum:
				Display_ThirdMenu_HelpInfomation();
				break;
			default:
				break;
		}
  }  
	else if(menu_id==VideoMenuNum)
		Display_NoVideoMenu();
	
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void Display_NoVideoMenu(void)
{	
	if(Display_VideoMenu_Flag==1)
	{
		VideoSignalExistOld_Flag=1;
		DelayMs(30);
		Display_VideoMenu_Flag=0;
	}
	if(VideoSignalDetect()==SignalExist)
		VideoSignalExist_Flag=1;
	else
	{
		VideoSignalExist_Flag=0;
		VideoChannelSelect(VideoChannelTemp);
	}
	
  if(VideoSignalExist_Flag==0&&VideoSignalExistOld_Flag==1)
  {
		Display_VideoMenu_Flag=1;
		Osd_Video_Change(Osd_State);
		OsdCmd(OsdClose);
		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
  }
	else if(VideoSignalExistOld_Flag==0&&VideoSignalExist_Flag==1)
	{
		OsdCmd(OsdClose);
		VideoChannelSelect(VideoChannelTemp);
		Osd_Video_Change(Video_State);
	}
	VideoSignalExistOld_Flag=VideoSignalExist_Flag;
	
	///视频不存在
	if(Display_VideoMenu_Flag==1)
	{
		Display_Layer_Graphics
		(&img_NoVideoMenu[VideoChannelTemp],&header_table,&NoVideoMenu_Info,
		&NoVideoMenu_Header[VideoChannelTemp],SPI_OSD_WIN_2);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);

		Display_VideoMenu_Flag=0;
		OsdCmd(OsdOpen);
		SOsdHwBuffWrite2Hw();
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void Display_LogoMenu(void)
{	
 	if(Display_LogoMenu_Flag==1)
 	{
 		OsdCmd(OsdClose);
 		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
	}
	///spi Osd 
	if(Display_LogoMenu_Flag==1)
	{
		Display_LogoMenu_Flag =0;
		Display_Layer_Graphics
		(&img_MainMenu_LOGO[LOGO_ShenLong],&header_table,&MainMenu_LOGO_Info,
		&MainMenu_LOGO_Header[LOGO_ShenLong],SPI_OSD_WIN_2);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);

		OsdCmd(OsdOpen);
		SOsdHwBuffWrite2Hw();
	}
	if(MainMenuLogoTime>800)
	{
		Display_MainMenu_Flag=1;
		menu_id=MainMenuNum;
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void Display_MainMenu(void)
{		
 	if(Display_MainMenu_Flag==1)
 	{
 		if(MainMenuDispIndex==0)
 		{
	 		OsdCmd(OsdClose);
	 		SpiOsdWinAllOff();
			FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
			///font osd 字符的特性
			FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT+1,FONT_OSD_PIXELS_16);///+1是为了小图标能覆盖完整		
 		}
	}
	///spi Osd 
	if(Display_MainMenu_Flag==1)
	{
		if(MainMenuDispIndex==1)
		{
			Display_Layer_Graphics
			(&img_LedIndicate,&header_table,&MainMenu_LedIndicate_Info,
			&MainMenu_LedIndicate_Header,SPI_OSD_WIN_2);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);
			
			Display_Layer_Graphics
			(&img_SmallModuleIndicate,&header_table,&MainMenu_SmallModuleIndicate_Info,
			&MainMenu_SmallModuleIndicate_Header,SPI_OSD_WIN_4);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_4, OFF);
		}
	}
	switch(MainMenuDispIndex)
	{
		case 0:
			MainMenu_LedIndicate_Disp();
			MainMenu_VehicleModule_Disp();
			break;
		case 1:
			MainMenu_OilPress_Disp();
//			MainMenu_SOC_Disp();
			break;
		case 2:
			MainMenu_SystemVoltage_Disp();
			break;
		case 3:
			MainMenu_SmallModuleIndicate_Disp();
			break;
		case 4:	
			MainMenu_Malfunction_Disp();	
			break;
		default:
			break;
	}
	
	if(Display_MainMenu_Flag==1)
 	{
 		if(MainMenuDispIndex==4)
 		{
			Display_MainMenu_Flag = 0;
			FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
			FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
			FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
			FontOsdWinDisplay(FONT_OSD_WIN_4,ON);
			OsdCmd(OsdOpen);
			SOsdHwBuffWrite2Hw();
 		}
	}
	MainMenuDispIndex++;
	if(MainMenuDispIndex==5)///总共4个步骤
		MainMenuDispIndex=0;
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	MainMenu_VehicleModule_Disp(void)
{	
	VehicleModuleIndicateNum=0xff;
	if(FrontDoorOpenSignalKeyBit==KeyOn&&MiddleDoorOpenSignalKeyBit==KeyOn)
	{
		VehicleModuleIndicateNum=0;
		Display_Layer_Graphics
		(&img_VehicleModuleIndicate_AllOpen,&header_table,&MainMenu_VehicleModuleIndicate_AllOpen_Info,
		&MainMenu_VehicleModuleIndicate_AllOpen_Header,SPI_OSD_WIN_3);				
	}
	else if(FrontDoorOpenSignalKeyBit==KeyOn)
	{
		VehicleModuleIndicateNum=1;
		Display_Layer_Graphics
		(&img_VehicleModuleIndicate_FrontOpen,&header_table,&MainMenu_VehicleModuleIndicate_FrontOpen_Info,
		&MainMenu_VehicleModuleIndicate_FrontOpen_Header,SPI_OSD_WIN_3);	
	}
	else if(MiddleDoorOpenSignalKeyBit==KeyOn)
	{
		VehicleModuleIndicateNum=2;
		Display_Layer_Graphics
		(&img_VehicleModuleIndicate_MiddleOpen,&header_table,&MainMenu_VehicleModuleIndicate_MiddleOpen_Info,
		&MainMenu_VehicleModuleIndicate_MiddleOpen_Header,SPI_OSD_WIN_3); 	
	}
	else
	{
		VehicleModuleIndicateNum=3;
		Display_Layer_Graphics
		(&img_VehicleModuleIndicate_AllClose,&header_table,&MainMenu_VehicleModuleIndicate_AllClose_Info,
		&MainMenu_VehicleModuleIndicate_AllClose_Header,SPI_OSD_WIN_3); 	
	}
	SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, OFF);
	if(VehicleModuleIndicateNum!=VehicleModuleIndicateNumOld)
	{
		VehicleModuleIndicateNumOld=VehicleModuleIndicateNum;
		if(Display_MainMenu_Flag==0)
		{
			SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,0);
		} 
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void MainMenu_LedIndicate_Disp(void)
{
	///font osd win1
	if(Display_MainMenu_Flag==1)
	{
		///先把在其他界面下（这里指次界面）做透明色的还原为不透明
		FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_NO_TRANS_VALUE);
		///再设定其此界面的透明色
		FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLUE,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_1, 15,0, 12, 2);
		FontOsdWinZoom(FONT_OSD_WIN_1,3,1);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);
	}
	///font osd win1
	///第一行的图标
	if((EngineRoomTemperatureKeyBit==KeyOn))
		WriteColor_StringToAddr(0,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(0,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//	if(LeftFrontStopHoofKeyBit==KeyOff||RightFrontStopHoofKeyBit==KeyOff
//		 ||LeftBackStopHoofKeyBit==KeyOff||RightBackStopHoofKeyBit==KeyOff)
//		WriteColor_StringToAddr(1,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(1,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(EngineChargeIndicateKeyBit==KeyOn)
		WriteColor_StringToAddr(2,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(2,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(DelaySpeedWorkIndicateKeyBit==KeyOn)
		WriteColor_StringToAddr(3,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(3,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(FootStopKeyBit==KeyOn||DelaySpeedWorkIndicateKeyBit==KeyOn)
		WriteColor_StringToAddr(4,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(4,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(GearBoxWarnKeyBit==KeyOn)
		WriteColor_StringToAddr(5,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(5,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(LocationLampKeyBit==KeyOn)
		WriteColor_StringToAddr(6,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(6,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(NShieldSignalKeyBit==KeyOn)
		WriteColor_StringToAddr(7,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(7,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if((NShieldSignalKeyBit==KeyOff)&&(RShieldSignalKeyBit==KeyOff))
		WriteColor_StringToAddr(8,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(8,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(RShieldSignalKeyBit==KeyOn)
		WriteColor_StringToAddr(9,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(9,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(buzzer_flag==0)
		WriteColor_StringToAddr(10,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(10,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(buzzer_flag==1)
		WriteColor_StringToAddr(11,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(11,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	///第二行的图标
//	if(DoorPumpLowAirKeyBit==KeyOn)
//		WriteColor_StringToAddr(12,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(12,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(BackDriveState_15==Load_On||BackDriveState_16==Load_On)//干燥剂
		WriteColor_StringToAddr(13,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(13,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//	if(Engine_Diagnose_Lamp.b.Red_Lamp==Lamp_On)
//		WriteColor_StringToAddr(14,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(14,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(AirFiltrateWarnKeyBit==KeyOn)
		WriteColor_StringToAddr(15,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(15,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(FrontAirPressWarnKeyBit==KeyOn||BackAirPressWarnKeyBit==KeyOn)
		WriteColor_StringToAddr(16,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(16,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(ASRMulfunctionIndicateKeyBit==KeyOn)
		WriteColor_StringToAddr(17,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(17,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
/*	if(CatalystTankLevelFlag2==1)
		WriteColor_StringToAddr(18,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,1);
	else if(CatalystTankLevelFlag==1)
		WriteColor_StringToAddr(18,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else*/
		WriteColor_StringToAddr(18,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
/*	if(SCR_Diagnose_Lamp.b.Red_Lamp==Lamp_On||SCR_Diagnose_Lamp.b.Amber_Lamp==Lamp_On)
		WriteColor_StringToAddr(19,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else*/
		WriteColor_StringToAddr(19,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//	if(GotOffDoorBellKeyBit==KeyOn)
//		WriteColor_StringToAddr(20,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(20,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
/*	if(SCR_Diagnose_Lamp.b.MIL_Lamp==Lamp_On)
		WriteColor_StringToAddr(21,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else*/
		WriteColor_StringToAddr(21,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(EngineLowerWaterKeyBit==KeyOn)
		WriteColor_StringToAddr(22,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(22,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	
	if(SmallFuelConsumeBlinkFlag==1&&SmallDistanceClearFlash_Flag==1)
		WriteColor_StringToAddr(23,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(23,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
}



#if 0
static void	MainMenu_LedIndicate_Disp(void)
{
	///font osd win1
	if(Display_MainMenu_Flag==1)
 	{
 		///先把在其他界面下（这里指次界面）做透明色的还原为不透明
		FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_NO_TRANS_VALUE);
		///再设定其此界面的透明色
		FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLUE,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_1, 15,0, 12, 2);
		FontOsdWinZoom(FONT_OSD_WIN_1,3,1);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);
	}
	///font osd win1
	///第一行的图标 1
	if((EngineRoomTemperatureKeyBit==KeyOn))
		WriteColor_StringToAddr(0,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(0,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	
//	if(LeftFrontStopHoofKeyBit==KeyOff||RightFrontStopHoofKeyBit==KeyOff
//		 ||LeftBackStopHoofKeyBit==KeyOff||RightBackStopHoofKeyBit==KeyOff)
//		WriteColor_StringToAddr(1,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(1,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
// 高压上电	
//	if(EngineChargeIndicateKeyBit==KeyOn)
//		WriteColor_StringToAddr(2,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(2,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//	P档信号
	if(0)
		WriteColor_StringToAddr(3,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(3,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);

	if(RShieldSignalKeyBit)
		WriteColor_StringToAddr(4,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(4,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	
	if(0)
		WriteColor_StringToAddr(5,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(5,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);

	if(NShieldSignalKeyBit==KeyOn)
		WriteColor_StringToAddr(6,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(6,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(LocationLampKeyBit==KeyOn)
		WriteColor_StringToAddr(7,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(7,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(FootStopKeyBit==KeyOn)
		WriteColor_StringToAddr(8,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(8,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//  电池切断	
//	if(GearBoxWarnKeyBit)
//		WriteColor_StringToAddr(9,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(9,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(buzzer_flag==0)
		WriteColor_StringToAddr(10,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(10,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(buzzer_flag==1)
		WriteColor_StringToAddr(11,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(11,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	///第二行的图标
//  绝缘电阻过低	
//	if(DoorPumpLowAirKeyBit==KeyOn)
//		WriteColor_StringToAddr(12,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(12,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(BackDriveState_15==Load_On||BackDriveState_16==Load_On)//干燥剂 
		WriteColor_StringToAddr(13,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(13,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//  电机过热	
//	if(Engine_Diagnose_Lamp.b.Red_Lamp==Lamp_On)
//		WriteColor_StringToAddr(14,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(14,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//  电机超速
//	if(AirFiltrateWarnKeyBit==KeyOn)
//		WriteColor_StringToAddr(15,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(15,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(FrontAirPressWarnKeyBit==KeyOn||BackAirPressWarnKeyBit==KeyOn||
		pres_value<AIR_PRESS_LOWER_NUM||gaspress2<AIR_PRESS_LOWER_NUM)
		WriteColor_StringToAddr(16,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(16,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	if(ASRMulfunctionIndicateKeyBit==KeyOn)
		WriteColor_StringToAddr(17,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(17,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//  离合器放开	
/*	if(CatalystTankLevelFlag2==1)
		WriteColor_StringToAddr(18,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,1);
	else if(CatalystTankLevelFlag==1)
		WriteColor_StringToAddr(18,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else*/
		WriteColor_StringToAddr(18,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//  离合器结合
/*	if(SCR_Diagnose_Lamp.b.Red_Lamp==Lamp_On)
		WriteColor_StringToAddr(19,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else*/
		WriteColor_StringToAddr(19,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//  下客铃
//	if(GotOffDoorBellKeyBit==KeyOn)
//		WriteColor_StringToAddr(20,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
//	else
		WriteColor_StringToAddr(20,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
//  充电线连接
/*	if(SCR_Diagnose_Lamp.b.MIL_Lamp==Lamp_On)
		WriteColor_StringToAddr(21,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else*/
		WriteColor_StringToAddr(21,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	//READY图标
	if(0)
		WriteColor_StringToAddr(22,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(22,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
	
	if(0)
		WriteColor_StringToAddr(23,BG_COLOR_BLUE|CH_COLOR_BLUE,ClearFontOsd,1,0);
	else
		WriteColor_StringToAddr(23,BG_COLOR_BLUE|CH_COLOR_BLACK,ClearFontOsd,1,0);
}
#endif
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void MainMenu_SmallModuleIndicate_Disp(void)
{
	unsigned long	Temp;
	unsigned char LiChenXiaoJi[7],LiChenZongJi[8];///显示小计与总计里程
	unsigned char VehicleSpeed_Disp[7];///显示车速
	unsigned char HundredKilometerFuelConsume_Disp[6];///显示百公里油耗
	unsigned char FualConsume_Instantaneous_Disp[8];///显示瞬时油耗
	unsigned char	Catalyst_Tank_Level_Disp[4];///显示尿素液位
	unsigned char	Date_Display[8],Time_Display[8];///显示日期时间

	///font osd win4
	if(Display_MainMenu_Flag==1)
 	{
		FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLUE,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_4, 12,414, 48, 2);
		FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,314);
	}
	///change to date display
	Date_Display[0]=year_h;
	Date_Display[1]=year_l;
	Date_Display[2]=FONT_WORD_HENG;
	Date_Display[3]=month_h;
	Date_Display[4]=month_l;
	Date_Display[5]=FONT_WORD_HENG;
	Date_Display[6]=date_h;
	Date_Display[7]=date_l;
	///change to time display
	Time_Display[0]=hour_h;
	Time_Display[1]=hour_l;
	Time_Display[2]=FONT_WORD_ZHUANG_DIAN;
	Time_Display[3]=minute_h;
	Time_Display[4]=minute_l;
	Time_Display[5]=FONT_WORD_ZHUANG_DIAN;
	Time_Display[6]=second_h;
	Time_Display[7]=second_l;
	///里程总计
	Temp=VehicleOverallDistance/10;
	Digit_Data_Conversation_To_Real_Display(Temp,LiChenZongJi,sizeof(LiChenZongJi)-2,DIGIT_DATA_NO_DOT);
	LiChenZongJi[6]=FONT_WORD_a+10;///k
	LiChenZongJi[7]=FONT_WORD_a+12;///m
	///里程小计
	Temp=VehicleSmallDistance/100;
	Digit_Data_Conversation_To_Real_Display(Temp,LiChenXiaoJi,sizeof(LiChenXiaoJi)-2,3);
	LiChenXiaoJi[5]=FONT_WORD_a+10;///k
	LiChenXiaoJi[6]=FONT_WORD_a+12;///m
	///车速
	Digit_Data_Conversation_To_Real_Display(carspeed,VehicleSpeed_Disp,sizeof(VehicleSpeed_Disp)-4,DIGIT_DATA_NO_DOT);
	VehicleSpeed_Disp[3]=FONT_WORD_a+10;///k
	VehicleSpeed_Disp[4]=FONT_WORD_a+12;///m
	VehicleSpeed_Disp[5]=FONT_WORD_PIE;/// /
	VehicleSpeed_Disp[6]=FONT_WORD_a+7;///h
	///百公里油耗
	Digit_Data_Conversation_To_Real_Display(HundredKilometerFuelConsume,HundredKilometerFuelConsume_Disp,sizeof(HundredKilometerFuelConsume_Disp)-1,2);
	HundredKilometerFuelConsume_Disp[5]=FONT_WORD_A+11;///L
	///瞬时油耗
	Temp=(ulong)fual_value*5;
	Digit_Data_Conversation_To_Real_Display(Temp,FualConsume_Instantaneous_Disp,sizeof(FualConsume_Instantaneous_Disp)-3,2);
	FualConsume_Instantaneous_Disp[5]=FONT_WORD_A+11;///L
	FualConsume_Instantaneous_Disp[6]=FONT_WORD_PIE;/// /
	FualConsume_Instantaneous_Disp[7]=FONT_WORD_a+7;///h
	///尿素液位
	Temp=CatalystTankLevel/10;
	Digit_Data_Conversation_To_Real_Display(Temp,Catalyst_Tank_Level_Disp,sizeof(Catalyst_Tank_Level_Disp)-1,DIGIT_DATA_NO_DOT);
	Catalyst_Tank_Level_Disp[3]=FONT_WORD_BAI_FEN;///%
	
	///font osd win4
	///第1行
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(314,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,1,0);
	WriteColor_StringToAddr(315,BG_COLOR_CYAN|CH_COLOR_BLUE,LiChenXiaoJi,sizeof(LiChenXiaoJi),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(322,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,32,0);
	
	WriteColor_StringToAddr(354,BG_COLOR_CYAN|CH_COLOR_BLUE,Date_Display,sizeof(Date_Display),0);
	///第2行
	WriteColor_StringToAddr(362,BG_COLOR_CYAN|CH_COLOR_BLUE,LiChenZongJi,sizeof(LiChenZongJi),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(370,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,1,0);
	
	WriteColor_StringToAddr(371,BG_COLOR_CYAN|CH_COLOR_BLUE,VehicleSpeed_Disp,sizeof(VehicleSpeed_Disp),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(378,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,2,0);
	
	WriteColor_StringToAddr(380,BG_COLOR_CYAN|CH_COLOR_BLUE,HundredKilometerFuelConsume_Disp,sizeof(HundredKilometerFuelConsume_Disp),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(386,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,1,0);
	
	WriteColor_StringToAddr(387,BG_COLOR_CYAN|CH_COLOR_BLUE,FualConsume_Instantaneous_Disp,sizeof(FualConsume_Instantaneous_Disp),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(395,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,2,0);
	
	WriteColor_StringToAddr(397,BG_COLOR_CYAN|CH_COLOR_BLUE,Catalyst_Tank_Level_Disp,sizeof(Catalyst_Tank_Level_Disp),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(401,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,1,0);
	
	WriteColor_StringToAddr(402,BG_COLOR_CYAN|CH_COLOR_BLUE,Time_Display,sizeof(Time_Display),0);
}


#if 0
static void	MainMenu_SmallModuleIndicate_Disp(void)
{
	unsigned long	Temp;
	unsigned char LiChenXiaoJi[7],LiChenZongJi[8];///显示小计与总计里程
	unsigned char VehicleSpeed_Disp[7];///显示车速
//	unsigned char HundredKilometerFuelConsume_Disp[6];///显示百公里油耗
//	unsigned char FualConsume_Instantaneous_Disp[8];///显示瞬时油耗
//	unsigned char	Catalyst_Tank_Level_Disp[4];///显示尿素液位
	unsigned char	Date_Display[8],Time_Display[8];///显示日期时间

	unsigned char BatteryPresentCurr_Disp[6];
	unsigned char BatteryPresentVolt_Disp[6];
	unsigned char EngineSpeed_Disp[6];
	uint WordTemp1;
	///font osd win4
	if(Display_MainMenu_Flag==1)
 	{
		FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLUE,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_4, 12,414, 48, 2);
		FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,314);
	}
	///change to date display
	Date_Display[0]=year_h;
	Date_Display[1]=year_l;
	Date_Display[2]=FONT_WORD_HENG;
	Date_Display[3]=month_h;
	Date_Display[4]=month_l;
	Date_Display[5]=FONT_WORD_HENG;
	Date_Display[6]=date_h;
	Date_Display[7]=date_l;
	///change to time display
	Time_Display[0]=hour_h;
	Time_Display[1]=hour_l;
	Time_Display[2]=FONT_WORD_ZHUANG_DIAN;
	Time_Display[3]=minute_h;
	Time_Display[4]=minute_l;
	Time_Display[5]=FONT_WORD_ZHUANG_DIAN;
	Time_Display[6]=second_h;
	Time_Display[7]=second_l;
	///里程总计
	Temp=VehicleOverallDistance/10;
	Digit_Data_Conversation_To_Real_Display(Temp,LiChenZongJi,sizeof(LiChenZongJi)-2,DIGIT_DATA_NO_DOT);
	LiChenZongJi[6]=FONT_WORD_a+10;///k
	LiChenZongJi[7]=FONT_WORD_a+12;///m
	///里程小计
	Temp=VehicleSmallDistance/100;
	Digit_Data_Conversation_To_Real_Display(Temp,LiChenXiaoJi,sizeof(LiChenXiaoJi)-2,3);
	LiChenXiaoJi[5]=FONT_WORD_a+10;///k
	LiChenXiaoJi[6]=FONT_WORD_a+12;///m
	///车速
	Digit_Data_Conversation_To_Real_Display(carspeed,VehicleSpeed_Disp,sizeof(VehicleSpeed_Disp)-4,DIGIT_DATA_NO_DOT);
	VehicleSpeed_Disp[3]=FONT_WORD_a+10;///k
	VehicleSpeed_Disp[4]=FONT_WORD_a+12;///m
	VehicleSpeed_Disp[5]=FONT_WORD_PIE;/// /
	VehicleSpeed_Disp[6]=FONT_WORD_a+7;///h
	///电池组电压
	
	WordTemp1=BatteryPresentVolt/10;			
	Digit_Data_Conversation_To_Real_Display(WordTemp1,BatteryPresentVolt_Disp,4,DIGIT_DATA_NO_DOT);
	BatteryPresentVolt_Disp[4]=FONT_WORD_A+'V'-'A';
	BatteryPresentVolt_Disp[5]=FONT_WORD_QING;

	///电池组电流
	WordTemp1=BatteryPresentCurr;		
	if(WordTemp1>=5000)
	{
		WordTemp1=WordTemp1-5000;
		WordTemp1=WordTemp1/10;
		Digit_Data_Conversation_To_Real_Display(WordTemp1,BatteryPresentCurr_Disp,3,DIGIT_DATA_NO_DOT);
		BatteryPresentCurr_Disp[3]=FONT_WORD_A;
		BatteryPresentCurr_Disp[4]=FONT_WORD_QING;
		BatteryPresentCurr_Disp[5]=FONT_WORD_QING;
	}
	else
	{
		WordTemp1=5000-WordTemp1;
		WordTemp1=WordTemp1/10;
		BatteryPresentCurr_Disp[0]=FONT_WORD_HENG;
		Digit_Data_Conversation_To_Real_Display(WordTemp1,&BatteryPresentCurr_Disp[1],4,DIGIT_DATA_NO_DOT); 	
		BatteryPresentCurr_Disp[5]=FONT_WORD_A;
	}		

	///电机转速
	WordTemp1=engSpeed;
	EngineSpeed_Disp[0]=FONT_WORD_QING;
	Digit_Data_Conversation_To_Real_Display(WordTemp1,&EngineSpeed_Disp[1],4,DIGIT_DATA_NO_DOT); 	
	EngineSpeed_Disp[5]=FONT_WORD_QING;

	///font osd win4
	///第1行
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(314,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,1,0);
	WriteColor_StringToAddr(315,BG_COLOR_CYAN|CH_COLOR_BLUE,LiChenXiaoJi,sizeof(LiChenXiaoJi),0);

	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(322,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,25,0);
	//电机转速
	WriteColor_StringToAddr(347,BG_COLOR_CYAN|CH_COLOR_BLUE,EngineSpeed_Disp,sizeof(EngineSpeed_Disp),0);
	if(Display_MainMenu_Flag==1)	
		WriteColor_StringToAddr(353,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,1,0);
	//日期
	WriteColor_StringToAddr(354,BG_COLOR_CYAN|CH_COLOR_BLUE,Date_Display,sizeof(Date_Display),0);
	///第2行
	EngineSpeed_Disp[0]=FONT_WORD_QING;
	EngineSpeed_Disp[1]=FONT_WORD_A+'R'-'A';
	EngineSpeed_Disp[2]=FONT_WORD_A+'P'-'A';
	EngineSpeed_Disp[3]=FONT_WORD_A+'M'-'A';
	EngineSpeed_Disp[4]=FONT_WORD_QING;
	EngineSpeed_Disp[5]=FONT_WORD_QING;	
	WriteColor_StringToAddr(362,BG_COLOR_CYAN|CH_COLOR_BLUE,LiChenZongJi,sizeof(LiChenZongJi),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(370,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,1,0);
	
	WriteColor_StringToAddr(371,BG_COLOR_CYAN|CH_COLOR_BLUE,VehicleSpeed_Disp,sizeof(VehicleSpeed_Disp),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(378,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,2,0);
	
	WriteColor_StringToAddr(380,BG_COLOR_CYAN|CH_COLOR_BLUE,BatteryPresentVolt_Disp,sizeof(BatteryPresentVolt_Disp),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(386,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,1,0);
	
	WriteColor_StringToAddr(387,BG_COLOR_CYAN|CH_COLOR_BLUE,BatteryPresentCurr_Disp,sizeof(BatteryPresentCurr_Disp),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(393,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,2,0);
	
	WriteColor_StringToAddr(395,BG_COLOR_CYAN|CH_COLOR_BLUE,EngineSpeed_Disp,sizeof(EngineSpeed_Disp),0);
	if(Display_MainMenu_Flag==1)
		WriteColor_StringToAddr(401,BG_COLOR_MAGENTA|CH_COLOR_BLUE,ClearFontOsd,1,0);
	
	WriteColor_StringToAddr(402,BG_COLOR_CYAN|CH_COLOR_BLUE,Time_Display,sizeof(Time_Display),0);
}
#endif
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	MainMenu_Malfunction_Disp(void)
{	
	const SLIDEIMAGE *img_MainMenu_Malfunction;///单页面显示的位置
	uchar Temp;
	
	SOsdWinBuffClean(SPI_OSD_WIN_7);
	SOsdWinBuffClean(SPI_OSD_WIN_8);

	if(dis_count==0)
	{
		img_MainMenu_Malfunction=&img_ThirdMenu_GuZhan_Content[0];
		Display_Layer_Graphics
		(img_MainMenu_Malfunction,&header_table,&MainMenu_GuZhanChaXun_Disp_Address[0],
		&ThirdMenu_GuZhan_Header,SPI_OSD_WIN_7);///i+2表示从win3开始
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_7, OFF);		
	}
	else if(dis_count==1)
	{
		img_MainMenu_Malfunction=&img_ThirdMenu_GuZhan_Content[dis_content[0]];
		Display_Layer_Graphics
		(img_MainMenu_Malfunction,&header_table,&MainMenu_GuZhanChaXun_Disp_Address[0],
		&ThirdMenu_GuZhan_Header,SPI_OSD_WIN_7);///i+2表示从win3开始
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_7, OFF);
	}
	else if(dis_count==2)
	{
		img_MainMenu_Malfunction=&img_ThirdMenu_GuZhan_Content[dis_content[0]];
		Display_Layer_Graphics
		(img_MainMenu_Malfunction,&header_table,&MainMenu_GuZhanChaXun_Disp_Address[0],
		&ThirdMenu_GuZhan_Header,SPI_OSD_WIN_7);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_7, OFF);

		img_MainMenu_Malfunction=&img_ThirdMenu_GuZhan_Content[dis_content[1]];
		Display_Layer_Graphics
		(img_MainMenu_Malfunction,&header_table,&MainMenu_GuZhanChaXun_Disp_Address[1],
		&ThirdMenu_GuZhan_Header,SPI_OSD_WIN_8);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_8, OFF);		

	}
	else if(dis_count>2)
	{
		if(MainMenuDis_count>=dis_count)
			MainMenuDis_count=0;
		img_MainMenu_Malfunction=&img_ThirdMenu_GuZhan_Content[dis_content[MainMenuDis_count]];
		Display_Layer_Graphics
		(img_MainMenu_Malfunction,&header_table,&MainMenu_GuZhanChaXun_Disp_Address[0],
		&ThirdMenu_GuZhan_Header,SPI_OSD_WIN_7);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_7, OFF);

		Temp=MainMenuDis_count+1;	
		if(Temp>=dis_count)
			Temp=0;
		img_MainMenu_Malfunction=&img_ThirdMenu_GuZhan_Content[dis_content[Temp]];
		Display_Layer_Graphics
		(img_MainMenu_Malfunction,&header_table,&MainMenu_GuZhanChaXun_Disp_Address[1],
		&ThirdMenu_GuZhan_Header,SPI_OSD_WIN_8);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_8, OFF);		
	}
	if(Display_MainMenu_Flag==0)
	{
		SOsdHwBuffWrite_Win(SPI_OSD_WIN_7,1);
		SOsdHwBuffWrite_Win(SPI_OSD_WIN_8,1);
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	MainMenu_OilPress_Disp(void)
{
	uchar JiYouYaLi_Disp[4];///显示机油压力值
	uchar oilPres_Temp;///机油压力当前值转换临时变量
	
	if(Display_MainMenu_Flag==1)
	{
		///font osd win2
		FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLUE,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_2, 15,160, 4, 1);
		FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,306);
	}
	if(oilPres<FualLowNum)
	{
		oilPres_Temp=oilPres/10;
		Display_Layer_Graphics
		(&img_FualIndicate[0],&header_table,&MainMenu_FualIndicate_Info,
		&MainMenu_FualIndicate_Header,SPI_OSD_WIN_5);
	}
	else if(oilPres>FualHighNum)
	{
		oilPres_Temp=oilPres/10+2;

		Display_Layer_Graphics
		(&img_FualIndicate[oilPres_Temp],&header_table,&MainMenu_FualIndicate_Info,
		&MainMenu_FualIndicate_Header,SPI_OSD_WIN_5);

	}
	else
	{
		oilPres_Temp=oilPres/10+1;
		if(oilPres_Temp==1)
			oilPres_Temp++;

		Display_Layer_Graphics
		(&img_FualIndicate[oilPres_Temp],&header_table,&MainMenu_FualIndicate_Info,
		&MainMenu_FualIndicate_Header,SPI_OSD_WIN_5);
	}

	SpiOsdWinPixelAlpha( SPI_OSD_WIN_5, OFF);
	if(Display_MainMenu_Flag==0)
	{
		SOsdHwBuffWrite_Win(SPI_OSD_WIN_5,1);
	}
	///font osd win2
	Digit_Data_Conversation_To_Real_Display
	(oilPres,JiYouYaLi_Disp,sizeof(JiYouYaLi_Disp),1);
	WriteColor_StringToAddr(306,BG_COLOR_MAGENTA|CH_COLOR_BLUE,JiYouYaLi_Disp,4,0);
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	MainMenu_SOC_Disp(void)
{
	uchar SOC_Disp[4];///显示SOC值
	uchar SOC_Temp;		///SOC当前值转换临时变量
	uchar Temp;
	if(Display_MainMenu_Flag==1)
	{
		///font osd win2
		FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLUE,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_2, 15,160, 4, 1);
		FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,306);
	}

	SOC_Temp=(uchar)(BatteryPresentSOC/2);
	if(SOC_Temp<30)
	{
		Temp=0;
	}
	else
	{
		Temp =SOC_Temp/10;
		Temp =Temp-2;
	}
	Display_Layer_Graphics
	(&img_SOCIndicate[Temp],&header_table,&MainMenu_SOCIndicate_Info,
	&MainMenu_SOCIndicate_Header,SPI_OSD_WIN_5);
	

	SpiOsdWinPixelAlpha( SPI_OSD_WIN_5, OFF);
	if(Display_MainMenu_Flag==0)
	{
		SOsdHwBuffWrite_Win(SPI_OSD_WIN_5,1);
	}
	///font osd win2
	SOC_Disp[3]=FONT_WORD_BAI_FEN;
	Digit_Data_Conversation_To_Real_Display
	(SOC_Temp,SOC_Disp,sizeof(SOC_Disp)-1,DIGIT_DATA_NO_DOT);
	WriteColor_StringToAddr(306,BG_COLOR_MAGENTA|CH_COLOR_BLUE,SOC_Disp,4,0);
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
#define	BATTER_DISP_LOW_DATA		23//24///电压显示最低值
#define	BATTER_DISP_HIGH_DATA	30//32///电压显示最高值
static void	MainMenu_SystemVoltage_Disp(void)
{
	uchar DianChiDianYa_Disp[4];///显示电池电压
	unsigned char	SystemVolatage_Temp;

	if(Display_MainMenu_Flag==1)
	{
		///font osd win3
		FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLUE,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_3, 720,160, 4, 1);
		FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,310);
	}	

	SystemVolatage_Temp=(uchar)(SystemVolatage/10);
	if(SystemVolatage_Temp<BATTER_DISP_LOW_DATA)
		SystemVolatage_Temp=BATTER_DISP_LOW_DATA;
	else	if(SystemVolatage_Temp>BATTER_DISP_HIGH_DATA)
		SystemVolatage_Temp=BATTER_DISP_HIGH_DATA;
	else
		;
	SystemVolatage_Temp=SystemVolatage_Temp-BATTER_DISP_LOW_DATA;
	if(SystemVolatage_Temp==0&&lvi_flag==1)
		SystemVolatage_Temp=SystemVolatage_Temp;
	else if(lvi_flag==1&&SystemVolatage_Temp!=0)
		SystemVolatage_Temp=SystemVolatage_Temp+2;
	else
		SystemVolatage_Temp++;
	Display_Layer_Graphics
	(&img_SystemVoltageIndicate[SystemVolatage_Temp],&header_table,&MainMenu_SystemVoltageIndicate_Info,
	&MainMenu_SystemVoltageIndicate_Header,SPI_OSD_WIN_6);
	SpiOsdWinPixelAlpha( SPI_OSD_WIN_6, OFF);
	///font osd win3
	Digit_Data_Conversation_To_Real_Display
	(SystemVolatage,DianChiDianYa_Disp,sizeof(DianChiDianYa_Disp),2);
	WriteColor_StringToAddr(310,BG_COLOR_MAGENTA|CH_COLOR_BLUE,DianChiDianYa_Disp,4,0);
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
#define		SubCircleLineWidth		12///光标位置图标的边框的宽度
#define		SubCircleLineHeight		5///光标位置图标的边框的长度
#define		SubCircleLineCharacterHeight	0x06///光标位置图标的边框的字的大小
void	Display_SubMenu(void)
{
	unsigned char i;
	
	if(Display_SubMenu_Flag==1)
	{
		OsdCmd(OsdClose);
		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
	}
	if(Display_SubMenu_Flag==1)
	{
		///spi osd
		Display_Layer_Graphics(&img_SubMenuBackGround,&header_table,&SubMenu_BackGround_Info,
															&SubMenu_BackGround_Header,SPI_OSD_WIN_2);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);
		///font osd
		FontOsd_Interface_CharacterHeight_Pixels(SubCircleLineCharacterHeight,FONT_OSD_PIXELS_16);
		FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);
		FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
	}
	///font osd
	switch(SubMenu_Cursor)
	{
		case 0:
			FontOsdWinScreen(FONT_OSD_WIN_1, 7,180, SubCircleLineWidth, SubCircleLineHeight);
			break;
		case 1:
			FontOsdWinScreen(FONT_OSD_WIN_1, 207,180, SubCircleLineWidth, SubCircleLineHeight);
			break;
		case 2:
			FontOsdWinScreen(FONT_OSD_WIN_1, 407,180, SubCircleLineWidth, SubCircleLineHeight);
			break;
		case 3:
			FontOsdWinScreen(FONT_OSD_WIN_1, 607,180, SubCircleLineWidth, SubCircleLineHeight);
			break;
		case 4:
			FontOsdWinScreen(FONT_OSD_WIN_1, 7,400, SubCircleLineWidth, SubCircleLineHeight);
			break;
		case 5:
			FontOsdWinScreen(FONT_OSD_WIN_1, 207,400, SubCircleLineWidth, SubCircleLineHeight);
			break;
		case 6:
			FontOsdWinScreen(FONT_OSD_WIN_1, 407,400, SubCircleLineWidth, SubCircleLineHeight);
			break;
		case 7:
			FontOsdWinScreen(FONT_OSD_WIN_1, 607,400, SubCircleLineWidth, SubCircleLineHeight);
			break;
		default:
			break;
	}	
	
	for(i=0;i<SubCircleLineHeight;i++)
	{
		if(i==0)
			WriteColor_StringToAddr(SubCircleLineWidth*i,BG_COLOR_BLACK|CH_COLOR_RED,ClearFontOsd,SubCircleLineWidth,0);
		else if(i==SubCircleLineHeight-1)
			WriteColor_StringToAddr(SubCircleLineWidth*i,BG_COLOR_BLACK|CH_COLOR_RED,ClearFontOsd,SubCircleLineWidth,0);
		else 
		{
			WriteColor_StringToAddr(SubCircleLineWidth*i,BG_COLOR_BLACK|CH_COLOR_RED,ClearFontOsd,1,0);
			WriteColor_StringToAddr(SubCircleLineWidth*i+1,BG_COLOR_BLACK,ClearFontOsd,SubCircleLineWidth-2,0);
			WriteColor_StringToAddr(SubCircleLineWidth*i+SubCircleLineWidth-1,BG_COLOR_BLACK|CH_COLOR_RED,ClearFontOsd,1,0);
		}
	}	
	if(Display_SubMenu_Flag==1)
	{
		Display_SubMenu_Flag=0;
		FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
		OsdCmd(OsdOpen);		
		SOsdHwBuffWrite2Hw();
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Display_ThirdMenu_Malfunction(void)
{
	unsigned char i;
	const SLIDEIMAGE *img_ThirdMenu_GuZhan[ThirdMenu_PageDispNum];///单页面显示的位置
	unsigned char PageNumberTemp;///页面数
	unsigned char PageSpareNumber;///页面所剩故障个数
	unsigned char Mulfunction_Total_Number_Disp[3];///显示总故障数
	unsigned char Mulfunction_Total_Page_Disp[2];///显示总页数
	unsigned char Mulfunction_Query_Page_Disp[2];///显示查询页数
	
	if(Display_ThirdMenu_Mulfunction_Flag==1)
	{
		OsdCmd(OsdClose);
		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
	}
	
	if(Display_ThirdMenu_Mulfunction_Flag==1)
	{
		///spi osd
		Display_Layer_Graphics
		(&img_ThirdMenu_GuZhanChaXun,&header_table,&ThirdMenu_GuZhanChaXun_Item_Info,
		&ThirdMenu_GuZhanChaXun_Header,SPI_OSD_WIN_2);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);

		///font osd
		FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
		///win1
		FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_1, 300,105, 3, 1);
		FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);
		///win2
		FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_2, 329,402, 2, 1);
		FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,3);
		///win3
		FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_3, 442,402, 2, 1);
		FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,5);
	}
	///spi osd
	if(ThirdMenu_Mal_OperatFlag==1)
	{
		for(i=SPI_OSD_WIN_3;i<=SPI_OSD_WIN_8;i++)///把故障内容关闭
			SpiOsdWinHWEnable(i, OFF);
		ThirdMenu_Mal_OperatFlag=0;
	}
	if(dis_count!=0)
	{
		PageNumberTemp=dis_count/ThirdMenu_PageDispNum;
		PageSpareNumber=dis_count%ThirdMenu_PageDispNum;
		if(MulFunctionQueryCursor<PageNumberTemp)///当多于一页，且不是最后一页
		{
			PageSpareNumber=ThirdMenu_PageDispNum;///显示的故障个数为5个
		}
		if(dis_count%ThirdMenu_PageDispNum==0)///当故障个数是5的整数倍时
			PageNumberTemp -= 1;
		for(i=SPI_OSD_WIN_3;i<=SPI_OSD_WIN_8;i++)///先清再显示
			SOsdWinBuffClean(i);
		for(i=0;i<PageSpareNumber;i++)
		{
			img_ThirdMenu_GuZhan[i]=&img_ThirdMenu_GuZhan_Content[dis_content[MulFunctionQueryCursor*ThirdMenu_PageDispNum+i]];
			Display_Layer_Graphics
			(img_ThirdMenu_GuZhan[i],&header_table,&ThirdMenu_GuZhanChaXun_Disp_Address[i],
			&ThirdMenu_GuZhan_Header,i+2);///i+2表示从win3开始
			SpiOsdWinPixelAlpha( i+2, OFF);
		}
		if(Display_ThirdMenu_Mulfunction_Flag==0)
			SOsdHwBuffWrite2Hw();
	}
	else
	{
		PageNumberTemp=0;///页面数
		PageSpareNumber=0;///页面所剩故障个数
	}
	///font osd win1
	Digit_Data_Conversation_To_Real_Display(dis_count,Mulfunction_Total_Number_Disp,sizeof(Mulfunction_Total_Number_Disp),DIGIT_DATA_NO_DOT);
	WriteColor_StringToAddr(0,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Mulfunction_Total_Number_Disp,sizeof(Mulfunction_Total_Number_Disp),0);
	///font osd win2
	Digit_Data_Conversation_To_Real_Display(PageNumberTemp+1,Mulfunction_Total_Page_Disp,sizeof(Mulfunction_Total_Page_Disp),DIGIT_DATA_NO_DOT);
	WriteColor_StringToAddr(3,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Mulfunction_Total_Page_Disp,sizeof(Mulfunction_Total_Page_Disp),0);
	///font osd win3
	Digit_Data_Conversation_To_Real_Display(MulFunctionQueryCursor+1,Mulfunction_Query_Page_Disp,sizeof(Mulfunction_Query_Page_Disp),DIGIT_DATA_NO_DOT);
	WriteColor_StringToAddr(5,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Mulfunction_Query_Page_Disp,sizeof(Mulfunction_Query_Page_Disp),0);
	if(Display_ThirdMenu_Mulfunction_Flag==1)
	{
		Display_ThirdMenu_Mulfunction_Flag=0;
		FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
		FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
		FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
		OsdCmd(OsdOpen);		
		SOsdHwBuffWrite2Hw();
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：初始化
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Display_ThirdMenu_DiagnoseQuery(void)
{
	unsigned char Disp_Page[5];///显示页码
	
	if(Display_ThirdMenu_DiagnoseQuery_Flag==1)
	{
		OsdCmd(OsdClose);
		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
	}
	if(Display_ThirdMenu_DiagnoseQuery_Flag==1)
	{
		///spi osd
		Display_Layer_Graphics
		(&img_ThirdMenu_XiTongZhenDuan_MoKuaiXuanZhe[ThirdMenu_DiagnoseQuery_Cursor],&header_table,&ThirdMenu_XiTongZhenDuan_MoKuaiXuanZhe_Item_Info,
		&ThirdMenu_XiTongZhenDuan_MoKuaiXuanZhe_Header[ThirdMenu_DiagnoseQuery_Cursor],SPI_OSD_WIN_2);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);
		///font osd
		FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
		
		switch(ThirdMenu_DiagnoseQuery_Cursor)
		{
			case	0:
			case	1:
			case	2:
			case	3:
			case	4:
				///win2
				FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinScreen(FONT_OSD_WIN_2, 72,85, 1, 11);
				FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0);
				///win3
				FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinScreen(FONT_OSD_WIN_3, 460,85, 1, 11);
				FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,11);
				///win4
				FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinScreen(FONT_OSD_WIN_4, 362,395, 5, 1);
				FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,22);
				break;
			case	5:
/*				if(ThirdMenu_DiagnoseQuery_PowerManage_Cursor==0)
				{
					///win2
					FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
					FontOsdWinScreen(FONT_OSD_WIN_2, 72,85, 1, 11);
					FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
					FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0);
					///win3
					FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
					FontOsdWinScreen(FONT_OSD_WIN_3, 460,85, 1, 11);
					FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
					FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,11);
					///win4
					FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
					FontOsdWinScreen(FONT_OSD_WIN_4, 362,395, 5, 1);
					FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
					FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,22);
				}
				else
				{
					///win1
					FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
					FontOsdWinScreen(FONT_OSD_WIN_1, 362,100, 6, 1);
					FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
					FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);			
					///win4
					FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
					FontOsdWinScreen(FONT_OSD_WIN_4, 362,395, 5, 1);
					FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
					FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,22);
					///win2
					FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
					FontOsdWinScreen(FONT_OSD_WIN_2, 32,286, 47, 1);
					FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
					FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,30);
					///win3
					FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
					FontOsdWinScreen(FONT_OSD_WIN_3, 32,344,47, 1);
					FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
					FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,80);			
				}*/
				break;				
			case	6:
				///win1
				FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinScreen(FONT_OSD_WIN_1, 156,175, 33, 1);
				FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);
				///win2
				FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinScreen(FONT_OSD_WIN_2, 156,320, 33, 1);//285
				FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,33);
				
				///win4
				FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinScreen(FONT_OSD_WIN_4, 362,395, 5, 1);
				FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,66);				
				break;
			case	7:
/*				///win2
				FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinScreen(FONT_OSD_WIN_2, 285,111, 6, 10);
				FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,30);
				///win3
				FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinScreen(FONT_OSD_WIN_3, 670,111, 6, 10);
				FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,100);				
				///win4
				FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinScreen(FONT_OSD_WIN_4, 362,395, 5, 1);
				FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,22);			*/	
				break;
			default:
				break;		
		}
	}
	if(Display_ThirdMenu_DiagnoseQuery_Flag==1)
	{
		if(ThirdMenu_DiagnoseQuery_Cursor==0)///仪表模块诊断的页面切换
		{
			Display_Layer_Graphics
			(&img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[ThirdMenu_DiagnoseQuery_Panal_Cursor],&header_table,&ThirdMenu_XiTongZhenDuanNeiRong_Item_Info,
			&ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[ThirdMenu_DiagnoseQuery_Panal_Cursor],SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
		}
		else if(ThirdMenu_DiagnoseQuery_Cursor==1)///前控模块诊断界面
		{
			Display_Layer_Graphics
			(&img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[ThirdMenu_DiagnoseQuery_FrontDrive_Cursor+3],&header_table,&ThirdMenu_XiTongZhenDuanNeiRong_Item_Info,
			&ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[ThirdMenu_DiagnoseQuery_FrontDrive_Cursor+3],SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
		}		
		else if(ThirdMenu_DiagnoseQuery_Cursor==2)///后控模块诊断界面
		{
			Display_Layer_Graphics
			(&img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[ThirdMenu_DiagnoseQuery_BackDrive_Cursor+6],&header_table,&ThirdMenu_XiTongZhenDuanNeiRong_Item_Info,
			&ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[ThirdMenu_DiagnoseQuery_BackDrive_Cursor+6],SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
		}			
/*		else if(ThirdMenu_DiagnoseQuery_Cursor==3)///顶控模块诊断界面
		{
			Display_Layer_Graphics
			(&img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[ThirdMenu_DiagnoseQuery_TopDrive_Cursor+9],&header_table,&ThirdMenu_XiTongZhenDuanNeiRong_Item_Info,
			&ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[ThirdMenu_DiagnoseQuery_TopDrive_Cursor+9],SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
		}	*/		
		else if(ThirdMenu_DiagnoseQuery_Cursor==4)///配电模块诊断界面
		{
			Display_Layer_Graphics
			(&img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[ThirdMenu_DiagnoseQuery_Power_Cursor+9],&header_table,&ThirdMenu_XiTongZhenDuanNeiRong_Item_Info,
			&ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[ThirdMenu_DiagnoseQuery_Power_Cursor+9],SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
		}	
/*		else if(ThirdMenu_DiagnoseQuery_Cursor==5)///电源管理模块诊断界面
		{
			if(ThirdMenu_DiagnoseQuery_PowerManage_Cursor==0)
			{
				Display_Layer_Graphics
				(&img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[ThirdMenu_DiagnoseQuery_PowerManage_Cursor+15],&header_table,&ThirdMenu_XiTongZhenDuanNeiRong_Item_Info,
				&ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[ThirdMenu_DiagnoseQuery_PowerManage_Cursor+15],SPI_OSD_WIN_3);
			}
			else
			{
				Display_Layer_Graphics
				(&img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[ThirdMenu_DiagnoseQuery_PowerManage_Cursor+15],&header_table,&ThirdMenu_XiTongZhenDuanNeiRong_PowerContrl_Item_Info,
				&ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[ThirdMenu_DiagnoseQuery_PowerManage_Cursor+15],SPI_OSD_WIN_3);
			}
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
		}	*/		
		else if(ThirdMenu_DiagnoseQuery_Cursor==6)///模块查询模块的显示
		{
			Display_Layer_Graphics
			(&img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[12],&header_table,&ThirdMenu_XiTongZhenDuanNeiRong_Item_Info,
			&ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[12],SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
		}
/*		else if(ThirdMenu_DiagnoseQuery_Cursor==7)///模块查询模块的显示
		{
			Display_Layer_Graphics
			(&img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[ThirdMenu_DiagnoseQuery_BMSPower_Cursor+18],&header_table,&ThirdMenu_XiTongZhenDuanNeiRong_Item_Info,
			&ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[ThirdMenu_DiagnoseQuery_BMSPower_Cursor+18],SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
		}	*/	
	}
	///FONT OSD
	switch(ThirdMenu_DiagnoseQuery_Cursor)
	{
		case	0:
			///win2
			Diagnose_Panal_LeftMenu();
			///win3
			Diagnose_Panal_RightMenu();
			///win4
			Disp_Page[0]=3;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=ThirdMenu_DiagnoseQuery_Panal_Cursor+1;
			WriteColor_StringToAddr(22,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Disp_Page,sizeof(Disp_Page),0);
			break;
		case	1:
			Diagnose_FrontDriver_LeftMenu();
			Diagnose_FrontDriver_RightMenu();
			Disp_Page[0]=3;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=ThirdMenu_DiagnoseQuery_FrontDrive_Cursor+1;
			WriteColor_StringToAddr(22,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Disp_Page,sizeof(Disp_Page),0);
			break;
		case	2:
			Diagnose_BackDriver_LeftMenu();
			Diagnose_BackDriver_RightMenu();
			Disp_Page[0]=3;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=ThirdMenu_DiagnoseQuery_BackDrive_Cursor+1;
			WriteColor_StringToAddr(22,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Disp_Page,sizeof(Disp_Page),0);
			break;
		case	3:
			Diagnose_Module_Clear();
			Disp_Page[0]=FONT_WORD_QING;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=FONT_WORD_QING;			
/*
			Diagnose_TopDriver_LeftMenu();
			Diagnose_TopDriver_RightMenu();
			Disp_Page[0]=3;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=ThirdMenu_DiagnoseQuery_TopDrive_Cursor+1;
			WriteColor_StringToAddr(22,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Disp_Page,sizeof(Disp_Page),0);
			*/
			break;
		case	4:
			Diagnose_Power_LeftMenu();
			Diagnose_Power_RightMenu();
			Disp_Page[0]=2;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=ThirdMenu_DiagnoseQuery_Power_Cursor+1;
			WriteColor_StringToAddr(22,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Disp_Page,sizeof(Disp_Page),0);
			break;
		case	5:
			Diagnose_Module_Clear();
			Disp_Page[0]=FONT_WORD_QING;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=FONT_WORD_QING;
			WriteColor_StringToAddr(22,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Disp_Page,sizeof(Disp_Page),0);
/*			
			Diagnose_PowerManage_LeftMenu();
			Diagnose_PowerManage_RightMenu();
			Disp_Page[0]=2;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=ThirdMenu_DiagnoseQuery_PowerManage_Cursor+1;
			WriteColor_StringToAddr(22,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Disp_Page,sizeof(Disp_Page),0);
			*/
			break;			
		case	6:
			Diagnose_ModuleTemperature();
			Disp_Page[0]=1;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=1;
			WriteColor_StringToAddr(66,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Disp_Page,sizeof(Disp_Page),0);
			break;
		case	7:
			Diagnose_Module_Clear();
			Disp_Page[0]=FONT_WORD_QING;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=FONT_WORD_QING;
/*
			Diagnose_BMS_DMC_LeftMenu();
			Diagnose_BMS_DMC_RightMenu();

			Disp_Page[0]=7;
			Disp_Page[1]=FONT_WORD_QING;
			Disp_Page[2]=FONT_WORD_QING;
			Disp_Page[3]=FONT_WORD_QING;
			Disp_Page[4]=ThirdMenu_DiagnoseQuery_BMSPower_Cursor+1;*/
			WriteColor_StringToAddr(22,BG_COLOR_MAGENTA|CH_COLOR_BLACK,Disp_Page,sizeof(Disp_Page),0);
			break;
		default:
			break;		
	}
	
	if(Display_ThirdMenu_DiagnoseQuery_Flag==1)
	{
		Display_ThirdMenu_DiagnoseQuery_Flag=0;
		switch(ThirdMenu_DiagnoseQuery_Cursor)
		{
			case	0:
			case	1:
			case	2:
			case	3:
			case	4:
				FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_4,ON);
				break;
			case	5:
				if(ThirdMenu_DiagnoseQuery_PowerManage_Cursor==0)
				{
					FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
					FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
					FontOsdWinDisplay(FONT_OSD_WIN_4,ON);					
				}
				else
				{
					FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
					FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
					FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
					FontOsdWinDisplay(FONT_OSD_WIN_4,ON);	
				}
				break;
			case	6:
					FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
					FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
					FontOsdWinDisplay(FONT_OSD_WIN_4,ON);					
				break;
			case	7:
				FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_4,ON);	
				break;
			default:
				break;		
		}
		OsdCmd(OsdOpen);		
		SOsdWinBuffWrite2Hw(SPI_OSD_WIN_2,SPI_OSD_WIN_3);
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void	Diagnose_State
(unsigned char Diagnose_Module,unsigned char Diagnose_Type,unsigned int	Diagnose_Content,
 unsigned char Diagnose_Content_Position)
{
	unsigned char Disp_Diagnose_Flag;
	if(Diagnose_Type==DIAGNOSE_LOAD)
	{
		switch(Diagnose_Content)
		{
			case Load_On: 
				Disp_Diagnose_Flag=FONT_WORD_LIANG;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_GREEN|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
			case Load_Short: 
				Disp_Diagnose_Flag=FONT_WORD_DUAN_LU;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_RED|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
			case Load_Off: 
				Disp_Diagnose_Flag=FONT_WORD_BU_LIANG;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_WHITE|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
			case Load_Open: 
				Disp_Diagnose_Flag=FONT_WORD_KAI_LU;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_RED|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
			default:
				Disp_Diagnose_Flag=FONT_WORD_QING;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_BLACK|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
		}
	}
	else if(Diagnose_Type==DIAGNOSE_SWITCH)
	{
		switch(Diagnose_Content)
		{
			case KeyOn: 
				Disp_Diagnose_Flag=FONT_WORD_JIE_TONG;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_GREEN|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
			case KeyOff: 
				Disp_Diagnose_Flag=FONT_WORD_BU_TONG;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_WHITE|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
			default:
				Disp_Diagnose_Flag=FONT_WORD_QING;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_BLACK|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
		}
	}
	else if(Diagnose_Type==DIAGNOSE_SENSOR)
	{
		unsigned char ModuleError=0;
		
		switch(Diagnose_Content)
		{
			case SensorErorrData: 					
				Disp_Diagnose_Flag=FONT_WORD_DUAN_LU;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_RED|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);	
				break;
			default:
				if(Diagnose_Module==DIAGNOSE_FRONT_DRIVE)
					ModuleError=FrontDriveErrorFlag;
				else if(Diagnose_Module==DIAGNOSE_BACK_DRIVE)
					ModuleError=BackDriveErrorFlag;
				else if(Diagnose_Module==DIAGNOSE_TOP_DRIVE)
					ModuleError=TopDriveErrorFlag;
				else if(Diagnose_Module==DIAGNOSE_POWER_MANAGE)
					ModuleError=PowerContrlErrorFlag;
				if(ModuleError)
				{
					Disp_Diagnose_Flag=FONT_WORD_DUAN_LU;
					WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_WHITE|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				}
				else
				{
					Disp_Diagnose_Flag=FONT_WORD_LIANG;
					WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_GREEN|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				}
				break;
		}
	}
	else if(Diagnose_Type==DIAGNOSE_CATCH)
	{
		switch(Diagnose_Content)
		{
			case CATCH_SENSOR_SIGNAL_NO:
				Disp_Diagnose_Flag=FONT_WORD_BU_LIANG;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_WHITE|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
			default:
				Disp_Diagnose_Flag=FONT_WORD_LIANG;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_GREEN|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
		}
	}
	else if(Diagnose_Type==DIAGNOSE_ENABLE_UNABLE)
	{
		switch(Diagnose_Content)
		{
			case STATE_UNABLE:
				Disp_Diagnose_Flag=FONT_WORD_BU_LIANG;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_WHITE|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
			case STATE_ENABLE:
				Disp_Diagnose_Flag=FONT_WORD_LIANG;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_GREEN|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
			default:
				Disp_Diagnose_Flag=FONT_WORD_QING;
				WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_BLACK|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
				break;
		}
	}
	else
	{
		Disp_Diagnose_Flag=FONT_WORD_QING;
		WriteColor_StringToAddr(Diagnose_Content_Position,BG_COLOR_BLACK|CH_COLOR_BLACK,&Disp_Diagnose_Flag,1,0);
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_Panal_LeftMenu(void)
{
	unsigned char	i;
	
	if(ThirdMenu_DiagnoseQuery_Panal_Cursor==0)
	{
		i=0;///JA01
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_LOAD,PANNAL_OUT_STATE_1,i);
		i=1;///JA02
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_LOAD,PANNAL_OUT_STATE_2,i);
		i=2;///JA11				
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_ENABLE_UNABLE,1,i);			
		i=3;///JA13
		#if		J13_OUT==0
			Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_LOAD,PANNAL_OUT_STATE_3,i);
		#elif		J13_OUT==1
			Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_CATCH,carspeed,i);
		#endif	
		i=4;///JA14
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey28,i);
		i=5;///JA15
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey20,i);
		i=6;///JA16
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey27,i);
		i=7;///JA17
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey19,i);
		i=8;///JA18
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey26,i);
		i=9;///JA19
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_ENABLE_UNABLE,1,i);			
		i=10;///JA20
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey25,i);
	}
	else if(ThirdMenu_DiagnoseQuery_Panal_Cursor==1)
	{
		i=0;///JB06
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey16,i);
		i=1;///JB07
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey2,i);		
		i=2;///JB08
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey15,i);
		i=3;///JB09
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey3,i);
		i=4;///JB10
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey14,i);
		i=5;///JB11
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey4,i);
		i=6;///JB12
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey13,i);
		i=7;///JB13
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey5,i);
		i=8;///JB14
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey12,i);
		i=9;///JB15
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey6,i);
		i=10;///JB16
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey11,i);
	}
	else if(ThirdMenu_DiagnoseQuery_Panal_Cursor==2)
	{
		i=0;///JC08
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey36,i);
		i=1;///JC09；当此端口要用时附DIAGNOSE_SENSOR，否则DIAGNOSE_BEI_YONG PannalSensorData[1]
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_ENABLE_UNABLE,0,i);		
		i=2;///JC10
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey30,i);
		i=3;///JC11；当此端口要用时附DIAGNOSE_SENSOR，否则DIAGNOSE_BEI_YONG
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=4;///JC12
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey34,i);
		i=5;///JC13；当此端口要用时附DIAGNOSE_SENSOR，否则DIAGNOSE_BEI_YONG
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=6;///JC14
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey33,i);
		i=7;///JC15；当此端口要用时附DIAGNOSE_SENSOR，否则DIAGNOSE_BEI_YONG
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=8;///JC16
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey35,i);
		for(i=9;i<=10;i++)
			Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_Panal_RightMenu(void)
{
	unsigned char	i;
	uchar Disp_Icon;
	if(ThirdMenu_DiagnoseQuery_Panal_Cursor==0)
	{
		i=11;///JA22
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey24,i);
		i=12;///JA23；当此端口要用时附DIAGNOSE_CATCH，否则附DIAGNOSE_BEI_YONG
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_CATCH,carspeed,i);
		i=13;///JA24
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey23,i);
		i=14;///JA25
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey17,i);
		i=15;///JA26
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey22,i);
		i=16;///JA27
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey18,i);
		i=17;///JA28
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey21,i);
		i=18;///JB01
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey31,i);
		i=19;///JB03
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey32,i);
		i=20;///JB04
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey29,i);
		i=21;///JB05
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey1,i);
	}
	else if(ThirdMenu_DiagnoseQuery_Panal_Cursor==1)
	{
		i=11;///JB17
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey7,i);
		i=12;///JB18
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey10,i);		
		i=13;///JB19
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey8,i);
		i=14;///JB20
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey9,i);
		i=15;///JC01；当此端口要用时附STATE_ENABLE，否则STATE_UNABLE
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=16;///JC02
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey39,i);
		i=17;///JC03；当此端口要用时附DIAGNOSE_SENSOR，否则DIAGNOSE_BEI_YONG
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=18;///JC04
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey38,i);
		i=19;///JC05
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_CATCH,carspeed,i);
		i=20;///JC06
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_SWITCH,MeterKey37,i);
		i=21;///JC07；当此端口要用时附DIAGNOSE_SENSOR，否则DIAGNOSE_BEI_YONG
		Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_ENABLE_UNABLE,0,i);
	}
	else if(ThirdMenu_DiagnoseQuery_Panal_Cursor==2)
	{
		for(i=11;i<=12;i++)
			Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);
		i=13;///开关关
		Disp_Icon=FONT_WORD_BU_TONG;
		WriteColor_StringToAddr(i,BG_COLOR_WHITE|CH_COLOR_BLACK,&Disp_Icon,1,0);
		i=14;///开关开
		Disp_Icon=FONT_WORD_JIE_TONG;
		WriteColor_StringToAddr(i,BG_COLOR_GREEN|CH_COLOR_BLACK,&Disp_Icon,1,0);
		i=15;///负载关
		Disp_Icon=FONT_WORD_BU_LIANG;
		WriteColor_StringToAddr(i,BG_COLOR_WHITE|CH_COLOR_BLACK,&Disp_Icon,1,0);
		i=16;///负载开
		Disp_Icon=FONT_WORD_LIANG;
		WriteColor_StringToAddr(i,BG_COLOR_GREEN|CH_COLOR_BLACK,&Disp_Icon,1,0);
		i=17;///负载短路
		Disp_Icon=FONT_WORD_DUAN_LU;
		WriteColor_StringToAddr(i,BG_COLOR_RED|CH_COLOR_BLACK,&Disp_Icon,1,0);
		i=18;///负载开路
		Disp_Icon=FONT_WORD_KAI_LU;
		WriteColor_StringToAddr(i,BG_COLOR_RED|CH_COLOR_BLACK,&Disp_Icon,1,0);
		i=19;///传感器关
		Disp_Icon=FONT_WORD_DUAN_LU;
		WriteColor_StringToAddr(i,BG_COLOR_WHITE|CH_COLOR_BLACK,&Disp_Icon,1,0);
		i=20;///传感器开
		Disp_Icon=FONT_WORD_LIANG;
		WriteColor_StringToAddr(i,BG_COLOR_GREEN|CH_COLOR_BLACK,&Disp_Icon,1,0);
		i=21;///传感器故障
		Disp_Icon=FONT_WORD_DUAN_LU;
		WriteColor_StringToAddr(i,BG_COLOR_RED|CH_COLOR_BLACK,&Disp_Icon,1,0);	
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_FrontDriver_LeftMenu(void)
{
	unsigned char	i;
	
	if(ThirdMenu_DiagnoseQuery_FrontDrive_Cursor==0)
	{
		i=0;///JA01
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_18,i);
		i=1;///JA04
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_17,i);
		i=2;///JA14				
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_15,i);			
		i=3;///JA16
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_16,i);
		i=4;///JA17
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_1,i);
		i=5;///JA18
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_2,i);
		i=6;///JA19
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_3,i);
		i=7;///JA20
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_4,i);
		i=8;///JA21
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_5,i);
		i=9;///JA22
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_6,i);			
		i=10;///JA23
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_7,i);
	}
	else if(ThirdMenu_DiagnoseQuery_FrontDrive_Cursor==1)
	{
		i=0;///JA35
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey5,i);
		i=1;///JA36
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey7,i);		
		i=2;///JA37
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey9,i);
		i=3;///JA38
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey11,i);
		i=4;///JA39
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_9,i);
		i=5;///JA40
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_10,i);
		i=6;///JA41
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey2,i);
		i=7;///JA42
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey4,i);
		i=8;///JA43
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey6,i);	
		i=9;///JA44
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey8,i);
		i=10;///JA45
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey10,i);
	}
	else if(ThirdMenu_DiagnoseQuery_FrontDrive_Cursor==2)
	{
		i=0;///JA57
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey15,i);
		i=1;///JA58
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);		
		i=2;///JA59
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=3;///JA60
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		for(i=4;i<=10;i++)
			Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_FrontDriver_RightMenu(void)
{
	unsigned char	i;
	if(ThirdMenu_DiagnoseQuery_FrontDrive_Cursor==0)
	{
		i=11;///JA24
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_8,i);
		i=12;///JA25
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		i=13;///JA26
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		i=14;///JA27
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=15;///JA28
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SENSOR,FrontSensorData[FRONT_AIR_POSITION_IN],i);
		i=16;///JA29
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SENSOR,FrontSensorData[BACK_AIR_POSITION_IN],i);
		i=17;///JA30
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=18;///JA31
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=19;///JA32
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=20;///JA33
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey1,i);
		i=21;///JA34
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey3,i);
	}
	else if(ThirdMenu_DiagnoseQuery_FrontDrive_Cursor==1)
	{
		i=11;///JA46
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey12,i);
		i=12;///JA47
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_SWITCH,FrontKey13,i);			
		i=13;///JA48
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=14;///JA49
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_11,i);
		i=15;///JA50
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_11,i);
		i=16;///JA51
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_12,i);
		i=17;///JA52
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_12,i);
		i=18;///JA53
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_13,i);
		i=19;///JA54
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_13,i);
		i=20;///JA55
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_14,i);
		i=21;///JA56
		Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_LOAD,FrontDriveState_14,i);
	}
	else if(ThirdMenu_DiagnoseQuery_FrontDrive_Cursor==2)
	{
		for(i=11;i<=21;i++)
			Diagnose_State(DIAGNOSE_FRONT_DRIVE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_BackDriver_LeftMenu(void)
{
	unsigned char	i;
	
	if(ThirdMenu_DiagnoseQuery_BackDrive_Cursor==0)
	{
		i=0;///JA01
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_18,i);
		i=1;///JA04
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_17,i);
		i=2;///JA14				
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_15,i);			
		i=3;///JA16
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_16,i);
		i=4;///JA17
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_1,i);
		i=5;///JA18
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_2,i);
		i=6;///JA19
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_3,i);
		i=7;///JA20
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_4,i);
		i=8;///JA21
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_5,i);
		i=9;///JA22
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_6,i);			
		i=10;///JA23
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_7,i);
	}
	else if(ThirdMenu_DiagnoseQuery_BackDrive_Cursor==1)
	{
		i=0;///JA35
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey5,i);
		i=1;///JA36
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey7,i);		
		i=2;///JA37
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey9,i);
		i=3;///JA38
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey11,i);
		i=4;///JA39
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_9,i);
		i=5;///JA40
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_10,i);
		i=6;///JA41
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey2,i);
		i=7;///JA42
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey4,i);
		i=8;///JA43
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey6,i);	
		i=9;///JA44
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey8,i);
		i=10;///JA45
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey10,i);
	}
	else if(ThirdMenu_DiagnoseQuery_BackDrive_Cursor==2)
	{
		i=0;///JA57
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey15,i);
		i=1;///JA58
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);		
		i=2;///JA59
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=3;///JA60
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		for(i=4;i<=10;i++)
			Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_BackDriver_RightMenu(void)
{
	unsigned char	i;
	if(ThirdMenu_DiagnoseQuery_BackDrive_Cursor==0)
	{
		i=11;///JA24
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_8,i);
		i=12;///JA25
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		i=13;///JA26
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		i=14;///JA27
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=15;///JA28
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=16;///JA29
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=17;///JA30
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=18;///JA31
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=19;///JA32
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=20;///JA33
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey1,i);
		i=21;///JA34
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey3,i);
	}
	else if(ThirdMenu_DiagnoseQuery_BackDrive_Cursor==1)
	{		
		i=11;///JA46
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey12,i);
		i=12;///JA47
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_SWITCH,BackKey13,i);			
		i=13;///JA48
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=14;///JA49
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_11,i);
		i=15;///JA50
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_11,i);
		i=16;///JA51
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_12,i);
		i=17;///JA52
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_12,i);
		i=18;///JA53
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_13,i);
		i=19;///JA54
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_13,i);
		i=20;///JA55
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_14,i);
		i=21;///JA56
		Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_LOAD,BackDriveState_14,i);
	}
	else if(ThirdMenu_DiagnoseQuery_BackDrive_Cursor==2)
	{
		for(i=11;i<=21;i++)
			Diagnose_State(DIAGNOSE_BACK_DRIVE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_TopDriver_LeftMenu(void)
{
	unsigned char	i;
	
	if(ThirdMenu_DiagnoseQuery_TopDrive_Cursor==0)
	{
		i=0;///JA01
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_18,i);
		i=1;///JA04
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_17,i);
		i=2;///JA14				
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_15,i);			
		i=3;///JA16
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_16,i);
		i=4;///JA17
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_1,i);
		i=5;///JA18
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_2,i);
		i=6;///JA19
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_3,i);
		i=7;///JA20
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_4,i);
		i=8;///JA21
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_5,i);
		i=9;///JA22
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_6,i);			
		i=10;///JA23
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_7,i);
	}
	else if(ThirdMenu_DiagnoseQuery_TopDrive_Cursor==1)
	{
		i=0;///JA35
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey5,i);
		i=1;///JA36
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey7,i);		
		i=2;///JA37
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey9,i);
		i=3;///JA38
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey11,i);
		i=4;///JA39
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_9,i);
		i=5;///JA40
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_10,i);
		i=6;///JA41
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey2,i);
		i=7;///JA42
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey4,i);
		i=8;///JA43
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey6,i);	
		i=9;///JA44
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey8,i);
		i=10;///JA45
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey10,i);
	}
	else if(ThirdMenu_DiagnoseQuery_TopDrive_Cursor==2)
	{
		i=0;///JA57
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey15,i);
		i=1;///JA58
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);		
		i=2;///JA59
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=3;///JA60
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		for(i=4;i<=10;i++)
			Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_TopDriver_RightMenu(void)
{
	unsigned char	i;
	if(ThirdMenu_DiagnoseQuery_TopDrive_Cursor==0)
	{
		i=11;///JA24
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_8,i);
		i=12;///JA25
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		i=13;///JA26
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		i=14;///JA27
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=15;///JA28
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=16;///JA29
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=17;///JA30
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=18;///JA31
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=19;///JA32
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=20;///JA33
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey1,i);
		i=21;///JA34
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey3,i);
	}
	else if(ThirdMenu_DiagnoseQuery_TopDrive_Cursor==1)
	{		
		i=11;///JA46
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey12,i);
		i=12;///JA47
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_SWITCH,TopKey13,i);			
		i=13;///JA48
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=14;///JA49
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_11,i);
		i=15;///JA50
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_11,i);
		i=16;///JA51
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_12,i);
		i=17;///JA52
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_12,i);
		i=18;///JA53
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_13,i);
		i=19;///JA54
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_13,i);
		i=20;///JA55
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_14,i);
		i=21;///JA56
		Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_LOAD,TopDriveState_14,i);
	}
	else if(ThirdMenu_DiagnoseQuery_TopDrive_Cursor==2)
	{
		for(i=11;i<=21;i++)
			Diagnose_State(DIAGNOSE_TOP_DRIVE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);
	}
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_BMS_DMC_LeftMenu(void)
{
	unsigned char	i;
	uchar Disp_temp[6];
	uint WordTemp1,WordTemp2;
	uint CharTemp1,CharTemp2;
	if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==0)
	{
		i=0;
		WordTemp1=HighestChargeVolt/10;		
		Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,sizeof(Disp_temp)-2,DIGIT_DATA_NO_DOT);		
		Disp_temp[4]=FONT_WORD_A+'V'-'A';
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);
		
		i=1;
		WordTemp1=BatteryNominalVolt;		
		Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,sizeof(Disp_temp)-2,DIGIT_DATA_NO_DOT);
		Disp_temp[4]=FONT_WORD_A+'V'-'A';
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);

		i=2;
		if(HighestWorkTemp>=40)
		{
			CharTemp1=HighestWorkTemp-40;			
			Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			Disp_temp[0]=FONT_WORD_HENG;
			CharTemp1=40-HighestWorkTemp;
			Digit_Data_Conversation_To_Real_Display(CharTemp1,&Disp_temp[1],2,DIGIT_DATA_NO_DOT);
		}
		Disp_temp[3]=FONT_WORD_ZHE_DU;
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);

		i=3;
		if(LowestWorkTemp>=40)
		{
			CharTemp1=LowestWorkTemp-40;			
			Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			Disp_temp[0]=FONT_WORD_HENG;
			CharTemp1=40-LowestWorkTemp;
			Digit_Data_Conversation_To_Real_Display(CharTemp1,&Disp_temp[1],2,DIGIT_DATA_NO_DOT);
		}
		Disp_temp[3]=FONT_WORD_ZHE_DU;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=4;
		CharTemp1=BatteryPitchNum;			
		Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		Disp_temp[3]=FONT_WORD_QING;
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=5;
		CharTemp1=LECUnumber;			
		Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,2,DIGIT_DATA_NO_DOT);
		Disp_temp[2]=FONT_WORD_QING;
		Disp_temp[3]=FONT_WORD_QING;
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=6;
		WordTemp1=SingleBatHighestVolt;			
		Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,5,1);
		Disp_temp[5]=FONT_WORD_A+'V'-'A';
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=7;
		WordTemp1=SingleBatLowestVolt;			
		Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,5,1);
		Disp_temp[5]=FONT_WORD_A+'V'-'A';
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=8;
		WordTemp1=BatteryNominalCurr/10;			
		Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);
		Disp_temp[3]=FONT_WORD_A;
		Disp_temp[4]=FONT_WORD_a+'H'-'A';
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=9;
		CharTemp1=BatterySOH;			
		Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);
		Disp_temp[3]=FONT_WORD_BAI_FEN;
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);	
	}
	else if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==1)
	{
		i=0;
		Disp_temp[0]=FONT_HanZi_Zheng;
		Disp_temp[1]=FONT_HanZi_Chang;		
		Disp_temp[2]=FONT_WORD_QING;
		Disp_temp[3]=FONT_WORD_QING;
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;		
		if(BatteryBMSStatus1.b.BMSSystemStatusFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Gu;
			Disp_temp[1]=FONT_HanZi_Zhang;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=1;
		if(BatteryBMSStatus1.b.BMUStatusFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Gu;
			Disp_temp[1]=FONT_HanZi_Zhang;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=2;
		if(BatteryBMSStatus1.b.LECUStatusFlag==0x01)
		{
			Disp_temp[0]=FONT_HanZi_Xiu;
			Disp_temp[1]=FONT_HanZi_Mian;
		}		
		else if(BatteryBMSStatus1.b.LECUStatusFlag==0x02)
		{
			Disp_temp[0]=FONT_HanZi_Gu;
			Disp_temp[1]=FONT_HanZi_Zhang;
		}
		else if(BatteryBMSStatus1.b.LECUStatusFlag==0x03)
		{
			Disp_temp[0]=FONT_HanZi_Wu;
			Disp_temp[1]=FONT_HanZi_Xiao;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=3;
		if(BatteryBMSStatus1.b.BatterFualFlag==0x01)
		{
			Disp_temp[0]=FONT_HanZi_Guo;
			Disp_temp[1]=FONT_HanZi_Ya;
		}
		else if(BatteryBMSStatus1.b.BatterFualFlag==0x02)
		{
			Disp_temp[0]=FONT_HanZi_Qian;
			Disp_temp[1]=FONT_HanZi_Ya;
		}
		else if(BatteryBMSStatus1.b.BatterFualFlag==0x03)
		{
			Disp_temp[0]=FONT_HanZi_Gu;
			Disp_temp[1]=FONT_HanZi_Zhang;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=4;
		if(BatteryBMSStatus1.b.BatterWorkFlag==0x01)
		{
			Disp_temp[0]=FONT_HanZi_Fang;
			Disp_temp[1]=FONT_HanZi_Dian;
		}
		else if(BatteryBMSStatus1.b.BatterWorkFlag==0x02)
		{
			Disp_temp[0]=FONT_HanZi_Chong;
			Disp_temp[1]=FONT_HanZi_Dian;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Wu;
			Disp_temp[1]=FONT_HanZi_Xiao;
		}
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		
		
		for(i=5;i<=9;i++)
		{
			WriteColor_StringToAddr(i*6+30,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,6,0);
		}

	}
	else if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==2)
	{
		i=0;
		Disp_temp[0]=FONT_HanZi_Zheng;
		Disp_temp[1]=FONT_HanZi_Chang;		
		Disp_temp[2]=FONT_WORD_QING;
		Disp_temp[3]=FONT_WORD_QING;
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;		
		if(BatteryBMSStatus2.b.SingleBatHigherVoltFlag_2==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=1;
		if(BatteryBMSStatus2.b.SingleBatHigherVoltFlag_1==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=2;
		if(BatteryBMSStatus2.b.SingleBatLowerVoltFlag_2==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=3;
		if(BatteryBMSStatus2.b.SingleBatLowerVoltFlag_1==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=4;
		if(BatteryBMSStatus2.b.BatterHigherTempFlag_2==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=5;
		if(BatteryBMSStatus2.b.BatterHigherTempFlag_1==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=6;
		if(BatteryBMSStatus2.b.SingleBatDiffentVoltFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=7;
		if(BatteryBMSStatus2.b.SingleBatDiffentTempFlag_1==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=8;
		if(BatteryBMSStatus3.b.IntranetCanErrFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=9;
		if(BatteryBMSStatus3.b.OuternetCanErrFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		
	}
	else if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==3)
	{
		i=0;
		CharTemp1=BMSContrllerFaultNumber;
		Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT); 
		Disp_temp[3]=FONT_WORD_QING;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=1;
		CharTemp1=BMSContrllerWarnNumber;
		Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT); 
		Disp_temp[3]=FONT_WORD_QING;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=2;
		CharTemp1=DriverSystemFaultNumber;
		Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT); 
		Disp_temp[3]=FONT_WORD_QING;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				
						
		i=3;
		CharTemp1=InverterTemperature;
		if(CharTemp1>=40)
		{
			CharTemp1=CharTemp1-40;			
			Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			Disp_temp[0]=FONT_WORD_HENG;
			CharTemp1=40-CharTemp1;
			Digit_Data_Conversation_To_Real_Display(CharTemp1,&Disp_temp[1],2,DIGIT_DATA_NO_DOT);
		}
		Disp_temp[3]=FONT_WORD_ZHE_DU;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=4;
		CharTemp1=Motor1InverterRadiatorTemp;
		if(CharTemp1>=40)
		{
			CharTemp1=CharTemp1-40;			
			Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			Disp_temp[0]=FONT_WORD_HENG;
			CharTemp1=40-CharTemp1;
			Digit_Data_Conversation_To_Real_Display(CharTemp1,&Disp_temp[1],2,DIGIT_DATA_NO_DOT);
		}
		Disp_temp[3]=FONT_WORD_ZHE_DU;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=5;
		CharTemp1=Motor1WindingResTemp;
		if(CharTemp1>=40)
		{
			CharTemp1=CharTemp1-40;			
			Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			Disp_temp[0]=FONT_WORD_HENG;
			CharTemp1=40-CharTemp1;
			Digit_Data_Conversation_To_Real_Display(CharTemp1,&Disp_temp[1],2,DIGIT_DATA_NO_DOT);
		}
		Disp_temp[3]=FONT_WORD_ZHE_DU;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=6;
		CharTemp1=Motor2InverterRadiatorTemp;
		if(CharTemp1>=40)
		{
			CharTemp1=CharTemp1-40;			
			Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			Disp_temp[0]=FONT_WORD_HENG;
			CharTemp1=40-CharTemp1;
			Digit_Data_Conversation_To_Real_Display(CharTemp1,&Disp_temp[1],2,DIGIT_DATA_NO_DOT);
		}
		Disp_temp[3]=FONT_WORD_ZHE_DU;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=7;
		CharTemp1=Motor2WindingResTemp;
		if(CharTemp1>=40)
		{
			CharTemp1=CharTemp1-40;			
			Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			Disp_temp[0]=FONT_WORD_HENG;
			CharTemp1=40-CharTemp1;
			Digit_Data_Conversation_To_Real_Display(CharTemp1,&Disp_temp[1],2,DIGIT_DATA_NO_DOT);
		}
		Disp_temp[3]=FONT_WORD_ZHE_DU;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=8;
		Disp_temp[2]=FONT_WORD_QING;			
		Disp_temp[3]=FONT_WORD_QING;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;		
		if((ConnecterStatus&0x0c)==0x00)//00
		{
			Disp_temp[0]=FONT_HanZi_Wei;
			Disp_temp[1]=FONT_HanZi_Xi;			
			Disp_temp[2]=FONT_HanZi_He;
		}
		else if((ConnecterStatus&0x0c)==0x04)//01
		{
			Disp_temp[0]=FONT_HanZi_Xi;			
			Disp_temp[1]=FONT_HanZi_He;
		}
		else if((ConnecterStatus&0x0c)==0x08)//10
		{
			Disp_temp[0]=FONT_HanZi_Gu;			
			Disp_temp[1]=FONT_HanZi_Zhang;
		}
		else if((ConnecterStatus&0x0c)==0x0C)//11		
		{
			Disp_temp[0]=FONT_HanZi_Wu;			
			Disp_temp[1]=FONT_HanZi_Xiao;
		}
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				

		i=9;
		Disp_temp[2]=FONT_WORD_QING;			
		Disp_temp[3]=FONT_WORD_QING;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;		
		if((ConnecterStatus&0x03)==0x00)//00
		{
			Disp_temp[0]=FONT_HanZi_Wei;
			Disp_temp[1]=FONT_HanZi_Xi;			
			Disp_temp[2]=FONT_HanZi_He;
		}
		else if((ConnecterStatus&0x03)==0x01)//01
		{
			Disp_temp[0]=FONT_HanZi_Xi;			
			Disp_temp[1]=FONT_HanZi_He;
		}
		else if((ConnecterStatus&0x03)==0x02)//10
		{
			Disp_temp[0]=FONT_HanZi_Gu;			
			Disp_temp[1]=FONT_HanZi_Zhang;
		}
		else if((ConnecterStatus&0x03)==0x03)//11		
		{
			Disp_temp[0]=FONT_HanZi_Wu;			
			Disp_temp[1]=FONT_HanZi_Xiao;
		}
		WriteColor_StringToAddr(i*6+30,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);				
	}	
	else
	{
		for(i=0;i<=9;i++)
		{
			WriteColor_StringToAddr(i*6+30,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,6,0);
		}
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_BMS_DMC_RightMenu(void)
{
	unsigned char	i;
	uchar Disp_temp[6];
	uint WordTemp1,WordTemp2;
	uint CharTemp1,CharTemp2;	
	if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==0)
	{
		i=0;
		WordTemp1=BatteryPresentVolt/10;			
		Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,4,DIGIT_DATA_NO_DOT);
		Disp_temp[4]=FONT_WORD_A+'V'-'A';
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);

		i=1;
		WordTemp1=BatteryPresentCurr;		
		if(WordTemp1>=5000)
		{
			WordTemp1=WordTemp1-5000;
			WordTemp1=WordTemp1/10;
			Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);
			Disp_temp[3]=FONT_WORD_A;
			Disp_temp[4]=FONT_WORD_QING;
			Disp_temp[5]=FONT_WORD_QING;
		}
		else
		{
			WordTemp1=5000-WordTemp1;
			WordTemp1=WordTemp1/10;
			Disp_temp[0]=FONT_WORD_HENG;
			Digit_Data_Conversation_To_Real_Display(WordTemp1,&Disp_temp[1],4,DIGIT_DATA_NO_DOT); 	
			Disp_temp[5]=FONT_WORD_A;
		}		
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=2;
		WordTemp1=BatteryPresentSOC*5;			
		Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,5,3);
		Disp_temp[5]=FONT_WORD_BAI_FEN;
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);
		
		i=3;
		if(BatteryPresentHighestTemp>=40)
		{
			CharTemp1=BatteryPresentHighestTemp-40;			
			Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			Disp_temp[0]=FONT_WORD_HENG;
			CharTemp1=40-BatteryPresentHighestTemp;
			Digit_Data_Conversation_To_Real_Display(CharTemp1,&Disp_temp[1],2,DIGIT_DATA_NO_DOT);
		}
		Disp_temp[3]=FONT_WORD_ZHE_DU;
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);

		i=4;
		if(BatteryPresentLowestTemp>=40)
		{
			CharTemp1=BatteryPresentLowestTemp-40;			
			Digit_Data_Conversation_To_Real_Display(CharTemp1,Disp_temp,3,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			Disp_temp[0]=FONT_WORD_HENG;
			CharTemp1=40-BatteryPresentLowestTemp;
			Digit_Data_Conversation_To_Real_Display(CharTemp1,&Disp_temp[1],2,DIGIT_DATA_NO_DOT);
		}
		Disp_temp[3]=FONT_WORD_ZHE_DU;
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);

		i=5;
		WordTemp1=SingleBatPresentHighestVolt;			
		Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,5,1);
		Disp_temp[5]=FONT_WORD_A+'V'-'A';
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=6;
		WordTemp1=SingleBatPresentLowestVolt;			
		Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,5,1);
		Disp_temp[5]=FONT_WORD_A+'V'-'A';
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=7;
		WordTemp1=PositiveBusInsulationRES;			
		if(WordTemp1<1000)
		{
			WordTemp1=WordTemp1*10;
			Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,4,DIGIT_DATA_NO_DOT);
			Disp_temp[4]=FONT_WORD_A+'K'-'A';
			Disp_temp[5]=FONT_HanZi_OuMu;
		}
		else
		{
			WordTemp1=WordTemp1/10;
			Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,4,1);
			Disp_temp[4]=FONT_WORD_A+'M'-'A';
			Disp_temp[5]=FONT_HanZi_OuMu;
		}
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		

		i=8;
		WordTemp1=MinusBusInsulationRES;			
		if(WordTemp1<1000)
		{
			WordTemp1=WordTemp1*10;
			Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,4,DIGIT_DATA_NO_DOT);
			Disp_temp[4]=FONT_WORD_A+'K'-'A';
			Disp_temp[5]=FONT_HanZi_OuMu;
		}
		else
		{
			WordTemp1=WordTemp1/10;
			Digit_Data_Conversation_To_Real_Display(WordTemp1,Disp_temp,4,1);
			Disp_temp[4]=FONT_WORD_A+'M'-'A';
			Disp_temp[5]=FONT_HanZi_OuMu;
		}
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		
		
		i=9;
		WriteColor_StringToAddr(i*6+100,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,6,0);

	}
	else if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==1)
	{		
		for(i=0;i<=9;i++)
		{
			WriteColor_StringToAddr(i*6+100,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,6,0);
		}
	}
	else if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==2)
	{
		i=0;
		Disp_temp[0]=FONT_HanZi_Zheng;
		Disp_temp[1]=FONT_HanZi_Chang;		
		Disp_temp[2]=FONT_WORD_QING;
		Disp_temp[3]=FONT_WORD_QING;
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;		
		if(BatteryBMSStatus3.b.HighVoltLeakageFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);

		i=1;
		if(BatteryBMSStatus3.b.DischargeRelayAdhereFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);

		i=2;
		if(BatteryBMSStatus3.b.DischargeCurrentBreakFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);

		i=3;
		if(BatteryBMSStatus3.b.DischargeOverCurrentFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);

		i=4;
		if(BatteryBMSStatus3.b.ChargeOverCurrentFlag==1)
		{
			Disp_temp[0]=FONT_HanZi_Bao;
			Disp_temp[1]=FONT_HanZi_Jin;
		}
		else
		{
			Disp_temp[0]=FONT_HanZi_Zheng;
			Disp_temp[1]=FONT_HanZi_Chang;	
		}		
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);
					
		for(i=5;i<=9;i++)
		{
			WriteColor_StringToAddr(i*6+100,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,6,0);
		}
	}
	else if(ThirdMenu_DiagnoseQuery_BMSPower_Cursor==3)
	{
		i=0;
		Disp_temp[2]=FONT_WORD_QING;			
		Disp_temp[3]=FONT_WORD_QING;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;		
		if((ConnecterStatus&0x04)==0x00)//00
		{
			Disp_temp[0]=FONT_HanZi_Guan;
			Disp_temp[1]=FONT_HanZi_Bi;			
		}
		else// if((ConnecterStatus&0x04)=0x01)//01
		{
			Disp_temp[0]=FONT_HanZi_Qi1;
			Disp_temp[1]=FONT_HanZi_Yong;
		}
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		
		i=1;
		Disp_temp[2]=FONT_WORD_QING;			
		Disp_temp[3]=FONT_WORD_QING;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;		
		if((OperationStatus&0x03)==0x00)//00
		{
			Disp_temp[0]=FONT_WORD_A+'O'-'A';
			Disp_temp[1]=FONT_WORD_A+'F'-'A';			
			Disp_temp[2]=FONT_WORD_A+'F'-'A';	
		}
		else if((OperationStatus&0x03)==0x01)//01
		{
			Disp_temp[0]=FONT_WORD_A;			
			Disp_temp[1]=FONT_WORD_A+'C'-'A';	
			Disp_temp[2]=FONT_WORD_A+'C'-'A';	
		}
		else if((OperationStatus&0x03)==0x02)//10
		{
			Disp_temp[0]=FONT_WORD_A+'O'-'A';		
			Disp_temp[1]=FONT_WORD_A+'N'-'A';
		}
		else if((OperationStatus&0x03)==0x03)//11		
		{
			Disp_temp[0]=FONT_WORD_A+'S'-'A';		
			Disp_temp[1]=FONT_WORD_a+'T'-'A';
			Disp_temp[2]=FONT_WORD_a;		
			Disp_temp[3]=FONT_WORD_a+'R'-'A';
			Disp_temp[4]=FONT_WORD_a+'T'-'A';		
		}		
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);		
		i=2;
		Disp_temp[2]=FONT_WORD_QING;			
		Disp_temp[3]=FONT_WORD_QING;		
		Disp_temp[4]=FONT_WORD_QING;
		Disp_temp[5]=FONT_WORD_QING;		
		if((BMSContrllerGears&0x10)==0x00)//00
		{
			Disp_temp[0]=FONT_HanZi_Wei;
			Disp_temp[1]=FONT_HanZi_Qi1;			
			Disp_temp[2]=FONT_HanZi_Yong;
		}
		else// if((BMSContrllerGears&0x03)==0x01)//01
		{
			Disp_temp[0]=FONT_HanZi_Qi1;			
			Disp_temp[1]=FONT_HanZi_Yong;
		}
		WriteColor_StringToAddr(i*6+100,BG_COLOR_RED|CH_COLOR_BLACK,Disp_temp,sizeof(Disp_temp),0);	
		for(i=3;i<=9;i++)
		{
			WriteColor_StringToAddr(i*6+100,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,6,0);
		}
	}	
	else
	{
		for(i=0;i<=9;i++)
		{
			WriteColor_StringToAddr(i*6+100,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,6,0);
		}
	}
}


/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_Power_LeftMenu(void)
{
	unsigned char	i;
	
	if(ThirdMenu_DiagnoseQuery_Power_Cursor==0)
	{
		for(i=0;i<=10;i++)
			Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);	
	}
	else if(ThirdMenu_DiagnoseQuery_Power_Cursor==1)
	{
		i=0;///JA01
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=1;///JA02
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_17,i);		
		i=2;///JA03
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_13,i);
		i=3;///JA04
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_16,i);
		i=4;///JA05
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_14,i);
		i=5;///JA06
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_15,i);
		i=6;///JA07
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_13,i);
		i=7;///JA08
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_14,i);
		i=8;///JB01
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_12,i);
		i=9;///JB02
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_11,i);
		i=10;///JB03
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_15,i);
	}
	else if(ThirdMenu_DiagnoseQuery_Power_Cursor==2)
	{
		i=0;///JC09
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_01,i);
		i=1;///JC10
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_02,i);		
		i=2;///JD01
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_12,i);
		i=3;///JD02
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_11,i);
		i=4;///JD03
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		i=5;///JD04
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_SWITCH,PowerKey1,i);
		i=6;///JE01
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_10,i);
		i=7;///JE02
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_03,i);
		i=8;///JE03
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_09,i);
		i=9;///JE04
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_04,i);
		i=10;///JE08
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_ENABLE_UNABLE,0,i);
	}	
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_Power_RightMenu(void)
{
	unsigned char	i;
	if(ThirdMenu_DiagnoseQuery_Power_Cursor==0)
	{
		for(i=11;i<=21;i++)
			Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);		
	}
	else if(ThirdMenu_DiagnoseQuery_Power_Cursor==1)
	{
		i=11;///JB04
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_10,i);
		i=12;///JB05
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_08,i);		
		i=13;///JB06
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_09,i);
		i=14;///JC01
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_07,i);
		i=15;///JC02
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_06,i);
		i=16;///JC03
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_16,i);
		i=17;///JC04
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_05,i);
		i=18;///JC05
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_17,i);
		i=19;///JC06
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_04,i);
		i=20;///JC07
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_18,i);
		i=21;///JC08
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerFuseState_03,i);
	}
	else if(ThirdMenu_DiagnoseQuery_Power_Cursor==2)
	{
		i=11;///JE09
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_08,i);
		i=12;///JE10
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_05,i);		
		i=13;///JE11
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_07,i);
		i=14;///JE12
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_06,i);
		i=15;///JE13
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_02,i);
		i=16;///JE14
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_LOAD,PowerState_01,i);
		i=17;///PB01
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		i=18;///PB02
		Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_ENABLE_UNABLE,1,i);
		for(i=19;i<=21;i++)
			Diagnose_State(DIAGNOSE_POWER_DRIVE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);		
	}	
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_PowerManage_LeftMenu(void)
{
	unsigned char	i;
	unsigned char	StDetectNum,temp,index;
	
	uint PowerTotalCurrent=0;
	uchar PowerCurrentDsp[6];
	if(ThirdMenu_DiagnoseQuery_PowerManage_Cursor==0)
	{
		i=0;///JA03
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_SWITCH,PowerContrlKey3,i);
		i=1;///JA04
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_SWITCH,PowerContrlKey4,i);
		i=2;///JA05				
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_SWITCH,PowerContrlKey1,i);			
		i=3;///JA06
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_SWITCH,PowerContrlKey2,i);
		i=4;///JA07//out13
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_11,i);
		i=5;///JA09//out12
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_10,i);
		i=6;///JA11//out14
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_12,i);
		i=7;///JA12
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_ENABLE_UNABLE,0,i);
		i=8;///JA13//out11
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_09,i);
		i=9;///JP02
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_01,i);			
		i=10;///JP03
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_02,i);
	}
	else if(ThirdMenu_DiagnoseQuery_PowerManage_Cursor==1)
	{		
		for(i=0;i<7;i++)
			PowerTotalCurrent +=PowerContrlCurrent[i];		
		// 总电流		
		Digit_Data_Conversation_To_Real_Display(PowerTotalCurrent,PowerCurrentDsp,sizeof(PowerCurrentDsp)-1,3);
		PowerCurrentDsp[5]=FONT_WORD_A;
		WriteColor_StringToAddr(0,BG_COLOR_MAGENTA|CH_COLOR_BLACK,PowerCurrentDsp,sizeof(PowerCurrentDsp),0);
		// 各路电流
		PowerCurrentDsp[4]=FONT_WORD_A;
		for(i=0;i<7;i++)
		{
			// JP2---JP7
			Digit_Data_Conversation_To_Real_Display(PowerContrlCurrent[i],PowerCurrentDsp,sizeof(PowerCurrentDsp)-2,2);		
			WriteColor_StringToAddr(30+i*7,BG_COLOR_MAGENTA|CH_COLOR_BLACK,PowerCurrentDsp,5,0);		
			WriteColor_StringToAddr(30+i*7+5,BG_COLOR_MAGENTA|CH_COLOR_BLACK,ClearFontOsd,2,0);	
		}
		PowerCurrentDsp[0]=FONT_WORD_QING;
		PowerCurrentDsp[1]=FONT_WORD_QING;
		PowerCurrentDsp[4]=FONT_WORD_QING;
		PowerCurrentDsp[5]=FONT_WORD_QING;
		index=0;
		StDetectNum=0;
		for(i=0;i<7;i++)
		{
			// JP2---JP7 故障类型
			temp =PowerContrlLoad[index].Data>>StDetectNum;
			temp=temp&0x03;
			if(temp==Load_Open)
			{
				PowerCurrentDsp[2]=FONT_HanZi_Kai;
				PowerCurrentDsp[3]=FONT_HanZi_Lu;
			}
			else if((temp==Load_Short)&&((PowerContrlOverLoadFlag.Data&(0x01<<i))!=0))//过载
			{
				PowerCurrentDsp[2]=FONT_HanZi_Gao;
				PowerCurrentDsp[3]=FONT_HanZi_Liu;
			}
			else if(temp==Load_Short)
			{
				PowerCurrentDsp[2]=FONT_HanZi_Duan;
				PowerCurrentDsp[3]=FONT_HanZi_Lu;
			}
			else
			{
				PowerCurrentDsp[2]=FONT_HanZi_Zheng;
				PowerCurrentDsp[3]=FONT_HanZi_Chang;
			}
			WriteColor_StringToAddr(80+i*7,BG_COLOR_MAGENTA|CH_COLOR_BLACK,PowerCurrentDsp,5,0);		
			WriteColor_StringToAddr(80+i*7+5,BG_COLOR_MAGENTA|CH_COLOR_BLACK,ClearFontOsd,2,0);	
			StDetectNum=StDetectNum+2;
			if(StDetectNum>=8)
			{
				StDetectNum=0;
				index++;
			}
		}			
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_PowerManage_RightMenu(void)
{
	unsigned char	i;
	if(ThirdMenu_DiagnoseQuery_PowerManage_Cursor==0)
	{
		i=11;///JP04
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_03,i);
		i=12;///JP05
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_04,i);
		i=13;///JP06
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_05,i);
		i=14;///JP07
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_06,i);
		i=15;///JP08
		Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_LOAD,PowerContrlState_07,i);
		for(i=16;i<=21;i++)
			Diagnose_State(DIAGNOSE_POWER_MANAGE,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);

	}
	else if(ThirdMenu_DiagnoseQuery_PowerManage_Cursor==1)
	{
		;
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_Module_Clear(void)
{
	unsigned char i;
	
	for(i=0;i<=21;i++)
			Diagnose_State(DIAGNOSE_PANEL,DIAGNOSE_BEI_YONG,DIAGNOSE_BEI_YONG,i);
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
static void Diagnose_ModuleTemperature(void)
{
	unsigned char i;
	unsigned char ModuleTemp_Lin1Disp[33];///模块温度第1行显示
	unsigned char ModuleTemp_Lin2Disp[33];///模块温度第2行显示
	
	#ifndef		FRONTMODULE_CONNECT
		ModuleTemp_Lin1Disp[0]=FONT_WORD_QING;
		ModuleTemp_Lin1Disp[1]=FONT_WORD_QING;
		ModuleTemp_Lin1Disp[2]=FONT_WORD_QING;
	#else
		if(FrontDriveModuleTemperatureFlag==0)
		{
			ModuleTemp_Lin1Disp[0]=FONT_WORD_QING;
			ModuleTemp_Lin1Disp[1]=QianQuWenDu[0];
			ModuleTemp_Lin1Disp[2]=QianQuWenDu[1];
		}
		else
		{
			ModuleTemp_Lin1Disp[0]=FONT_WORD_HENG;
			ModuleTemp_Lin1Disp[1]=QianQuWenDu[0];
			ModuleTemp_Lin1Disp[2]=QianQuWenDu[1];
		}
	#endif

	for(i=3;i<15;i++)
		ModuleTemp_Lin1Disp[i]=FONT_WORD_QING;

	#ifndef		BACKMODULE_CONNECT
		ModuleTemp_Lin1Disp[15]=FONT_WORD_QING;
		ModuleTemp_Lin1Disp[16]=FONT_WORD_QING;
		ModuleTemp_Lin1Disp[17]=FONT_WORD_QING;
	#else
		if(BackDriveModuleTemperatureFlag==0)
		{
			ModuleTemp_Lin1Disp[15]=FONT_WORD_QING;
			ModuleTemp_Lin1Disp[16]=HouQuWenDu[0];
			ModuleTemp_Lin1Disp[17]=HouQuWenDu[1];
		}
		else
		{
			ModuleTemp_Lin1Disp[15]=FONT_WORD_HENG;
			ModuleTemp_Lin1Disp[16]=HouQuWenDu[0];
			ModuleTemp_Lin1Disp[17]=HouQuWenDu[1];
		}
	#endif

	for(i=18;i<30;i++)
		ModuleTemp_Lin1Disp[i]=FONT_WORD_QING;

	#ifndef		TOPMODULE_CONNECT
		ModuleTemp_Lin1Disp[30]=FONT_WORD_QING;
		ModuleTemp_Lin1Disp[31]=FONT_WORD_QING;
		ModuleTemp_Lin1Disp[32]=FONT_WORD_QING;
	#else
		if(TopDriveModuleTemperatureFlag==0)
		{
			ModuleTemp_Lin1Disp[30]=FONT_WORD_QING;
			ModuleTemp_Lin1Disp[31]=DingQuWenDu[0];
			ModuleTemp_Lin1Disp[32]=DingQuWenDu[1];
		}
		else
		{
			ModuleTemp_Lin1Disp[30]=FONT_WORD_HENG;
			ModuleTemp_Lin1Disp[31]=DingQuWenDu[0];
			ModuleTemp_Lin1Disp[32]=DingQuWenDu[1];
		}
	#endif
	
	WriteColor_StringToAddr(0,BG_COLOR_MAGENTA|CH_COLOR_BLACK,ModuleTemp_Lin1Disp,sizeof(ModuleTemp_Lin1Disp),0);

	if(PanelModuleTemperatureFlag==0)
	{
		ModuleTemp_Lin2Disp[0]=FONT_WORD_QING;
		ModuleTemp_Lin2Disp[1]=YiBiaoWenDu[0];
		ModuleTemp_Lin2Disp[2]=YiBiaoWenDu[1];
	}
	else
	{
		ModuleTemp_Lin2Disp[0]=FONT_WORD_HENG;
		ModuleTemp_Lin2Disp[1]=YiBiaoWenDu[0];
		ModuleTemp_Lin2Disp[2]=YiBiaoWenDu[1];
	}
	
	for(i=3;i<15;i++)
		ModuleTemp_Lin2Disp[i]=FONT_WORD_QING;
	
	#ifndef		POWERMODULE_CONNECT
		ModuleTemp_Lin2Disp[15]=FONT_WORD_QING;
		ModuleTemp_Lin2Disp[16]=FONT_WORD_QING;
		ModuleTemp_Lin2Disp[17]=FONT_WORD_QING;
	#else
		if(PowerModuleTemperatureFlag==0)
		{
			ModuleTemp_Lin2Disp[15]=FONT_WORD_QING;
			ModuleTemp_Lin2Disp[16]=PeiDianWenDu[0];
			ModuleTemp_Lin2Disp[17]=PeiDianWenDu[1];
		}
		else
		{
			ModuleTemp_Lin2Disp[15]=FONT_WORD_HENG;
			ModuleTemp_Lin2Disp[16]=PeiDianWenDu[0];
			ModuleTemp_Lin2Disp[17]=PeiDianWenDu[1];
		}
	#endif
	
	for(i=18;i<30;i++)
		ModuleTemp_Lin2Disp[i]=FONT_WORD_QING;	
	#ifndef		POWERCONTRLMODULE_CONNECT
		ModuleTemp_Lin2Disp[30]=FONT_WORD_QING;
		ModuleTemp_Lin2Disp[31]=FONT_WORD_QING;
		ModuleTemp_Lin2Disp[32]=FONT_WORD_QING;
	#else
		if(PowerContrlModuleTemperatureFlag==0)
		{
			ModuleTemp_Lin2Disp[30]=FONT_WORD_QING;
			ModuleTemp_Lin2Disp[31]=PowerContrlWenDu[0];
			ModuleTemp_Lin2Disp[32]=PowerContrlWenDu[1];
		}
		else
		{
			ModuleTemp_Lin2Disp[30]=FONT_WORD_HENG;
			ModuleTemp_Lin2Disp[31]=PowerContrlWenDu[0];
			ModuleTemp_Lin2Disp[32]=PowerContrlWenDu[1];
		}
	#endif	
	WriteColor_StringToAddr(33,BG_COLOR_MAGENTA|CH_COLOR_BLACK,ModuleTemp_Lin2Disp,sizeof(ModuleTemp_Lin2Disp),0);
}
/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
//参数设置
void	Display_ThirdMenu_SystemParameter(void)
{
	unsigned char i,Temp;
	unsigned char DispTemp[7];
	
	if(Display_ThirdMenu_SystemParameter_Flag==1)
	{
		OsdCmd(OsdClose);
		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
	}

	///spi osd
	//系统参数设置
	if(Display_ThirdMenu_SystemParameter_Flag==1)
	{
		Display_Layer_Graphics(&img_SystemParameterMenuBackGround,&header_table,&SystemParameterMenuBackGround_Info,
															&SystemParameterMenuBackGround_Header,SPI_OSD_WIN_2);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);

		///font osd
		FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
		FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_1,527,145, 1, 1);
		FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);

		FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_2,492,222, 6, 1);
		FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0x40);

		FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdWinScreen(FONT_OSD_WIN_3,492,298, 6, 1);
		FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,0x60);		
									
	}
	if(SystemParameter_CursorChange_Flag==1||Display_ThirdMenu_SystemParameter_Flag==1)
	{
		SystemParameter_CursorChange_Flag=0;
		switch(ThirdMenu_SystemParameter_Cursor)
		{
			case 0://雨刮
				Display_Layer_Graphics(&img_SystemParameterMenuItemRainWiper,&header_table,&SystemParameterMenuItemRainWiper_Info,
																	&SystemParameterMenuItem_Header,SPI_OSD_WIN_3);
				SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
				break;
			case 1://仪表背光
				Display_Layer_Graphics(&img_SystemParameterMenuItemBackLight,&header_table,&SystemParameterMenuItemBackLight_Info,
																	&SystemParameterMenuItem_Header,SPI_OSD_WIN_3);
				SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
				break;					
			case 2://蜂鸣器
				Display_Layer_Graphics(&img_SystemParameterMenuItemBuzzer,&header_table,&SystemParameterMenuItemBuzzer_Info,
																	&SystemParameterMenuItem_Header,SPI_OSD_WIN_3);
				SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
				break;
				break;	
			default:					
				break;	
		}
		if(Display_ThirdMenu_SystemParameter_Flag==0)
		{
			if(ThirdMenu_SystemParameter_Cursor==0xff)
			{
				SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
			}
			else
			{
				SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
				if(ThirdMenu_SystemParameter_Cursor==1)
					SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,0); 
				else
					SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,1); 			
			}					
		}			
	}
	
	///font osd 
	//雨刮设置
	Temp=rain_scr;
	Digit_Data_Conversation_To_Real_Display(Temp,DispTemp,1,DIGIT_DATA_NO_DOT);
	WriteColor_StringToAddr(0x00,BG_COLOR_RED|CH_COLOR_BLACK,DispTemp,1,0);	
	//仪表背光
	Temp=FONT_WORD_SHU;
	for(i=0;i<6;i++)
	{
		if(i<(LightLevel*2))
			WriteColor_StringToAddr(0x40+i,BG_COLOR_RED|CH_COLOR_BLACK,&Temp,1,0);
		else
			WriteColor_StringToAddr(0x40+i,BG_COLOR_WHITE|CH_COLOR_BLACK,&Temp,1,0);
	}
	//蜂鸣器声音
	Temp=FONT_WORD_SHU;
	for(i=0;i<6;i++)
	{
		if(i<(BuzzerLevel*2))
			WriteColor_StringToAddr(0x60+i,BG_COLOR_RED|CH_COLOR_BLACK,&Temp,1,0);
		else
			WriteColor_StringToAddr(0x60+i,BG_COLOR_WHITE|CH_COLOR_BLACK,&Temp,1,0);
	}

	if(Display_ThirdMenu_SystemParameter_Flag==1)
	{
		Display_ThirdMenu_SystemParameter_Flag=0;
		FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
		FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
		FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
		OsdCmd(OsdOpen);	
		SOsdWinBuffWrite2Hw(SPI_OSD_WIN_2,SPI_OSD_WIN_3);
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
//里程油耗查询
void	Display_ThirdMenu_DistanceFualConsume(void)
{
	unsigned char Temp;
	unsigned char LiChenXiaoJi[7],LiChenZongJi[8];
	unsigned char FualConsume_Small_Disp[6];
	unsigned char FualConsume_100km_Disp[6];
	unsigned char FualConsume_Instantaneous_Disp[8];		
	unsigned char FualConsume_Total_Disp[10];
	unsigned long DwordTemp;

	if(Display_ThirdMenu_DistanceFualConsume_Flag==1)
	{
		OsdCmd(OsdClose);
		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
	}
	if(Display_ThirdMenu_DistanceFualConsume_Flag==1)
	{
		///spi osd
		//旅程油耗背景图
		Display_Layer_Graphics(&img_DistanceFualConsumeMenu,&header_table,&DistanceFualConsumeMenu_Info,
															&DistanceFualConsumeMenu_Header,SPI_OSD_WIN_2);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);
		
		Display_Layer_Graphics(&img_SmallDistanceClearMenu,&header_table,&SmallDistanceClearMenu_Info,
															&SmallDistanceClearMenu_Header,SPI_OSD_WIN_3);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, OFF);

		SmallDistanceClearFlash1s_Flag=0;
		///font osd
		FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
		FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);

		FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0x30);

		FontOsdWinScreen(FONT_OSD_WIN_1, 72,182, 41, 1);
		FontOsdWinScreen(FONT_OSD_WIN_2, 72,322, 41, 1);
		
	}

	if((SmallDistanceClearFlash1s_Flag==1)&&(Display_ThirdMenu_DistanceFualConsume_Flag==0))
	{
		SmallDistanceClearFlash1s_Flag=0;
		if(SmallDistanceClearFlash_Flag==0&&SmallFuelConsumeBlinkFlag==1)
		{
			Display_Layer_Graphics(&img_SmallDistanceClearSelMenu,&header_table,&SmallDistanceClearMenu_Info,
																&SmallDistanceClearMenu_Header,SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, OFF);		
			SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,0);
		}
		else
		{
			Display_Layer_Graphics(&img_SmallDistanceClearMenu,&header_table,&SmallDistanceClearMenu_Info,
																&SmallDistanceClearMenu_Header,SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, OFF);		
			SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,0);
		}
	}

	///font osd 
	///瞬时油耗
	Digit_Data_Conversation_To_Real_Display(fual_value,FualConsume_Instantaneous_Disp,sizeof(FualConsume_Instantaneous_Disp)-3,2);
	FualConsume_Instantaneous_Disp[5]=FONT_WORD_A+11;///L
	FualConsume_Instantaneous_Disp[6]=FONT_WORD_PIE;/// /
	FualConsume_Instantaneous_Disp[7]=FONT_WORD_a+7;///h
	WriteColor_StringToAddr(0x00,BG_COLOR_RED|CH_COLOR_BLACK,FualConsume_Instantaneous_Disp,sizeof(FualConsume_Instantaneous_Disp),0);//最低可显示10个字符
	WriteColor_StringToAddr(0x00+8,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,3,0);	//未用透过
	///不可用透过区域
	WriteColor_StringToAddr(0x00+11,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);
	///总油耗
	DwordTemp =total_fual_used/2;
	Digit_Data_Conversation_To_Real_Display(DwordTemp,FualConsume_Total_Disp,sizeof(FualConsume_Total_Disp)-3,DIGIT_DATA_NO_DOT);
	FualConsume_Total_Disp[7]=FONT_WORD_DIAN;///.	
	Temp=total_fual_used%2;
	if(Temp==0)		
		FualConsume_Total_Disp[8]=0;///0
	else
		FualConsume_Total_Disp[8]=5;///0
	FualConsume_Total_Disp[9]=FONT_WORD_A+11;///L	
	WriteColor_StringToAddr(0x00+15,BG_COLOR_RED|CH_COLOR_BLACK,FualConsume_Total_Disp,sizeof(FualConsume_Total_Disp),0);//最低可显示10个字符
	WriteColor_StringToAddr(0x00+15+10,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,1,0);//未用透过
	///不可用透过区域
	WriteColor_StringToAddr(0x00+26,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);
	///百公里油耗
	Digit_Data_Conversation_To_Real_Display(HundredKilometerFuelConsume,FualConsume_100km_Disp,sizeof(FualConsume_100km_Disp)-1,2);
	FualConsume_100km_Disp[5]=FONT_WORD_A+11;///L	
	WriteColor_StringToAddr(0x00+30,BG_COLOR_RED|CH_COLOR_BLACK,FualConsume_100km_Disp,sizeof(FualConsume_100km_Disp),0);//最低可显示10个字符
	WriteColor_StringToAddr(0x00+30+6,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,5,0);//未用透过
	///短途油耗
	Digit_Data_Conversation_To_Real_Display(SmallDistanceFuelConsume,FualConsume_Small_Disp,sizeof(FualConsume_Small_Disp)-1,3);
	FualConsume_Small_Disp[5]=FONT_WORD_A+11;///L	
	WriteColor_StringToAddr(0x30 ,BG_COLOR_RED|CH_COLOR_BLACK,FualConsume_Small_Disp,sizeof(FualConsume_Small_Disp),0);//最低可显示10个字符
	WriteColor_StringToAddr(0x30+6,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,5,0);//未用透过
	///不可用透过区域 
	WriteColor_StringToAddr(0x30+11,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);

	///里程小计
	DwordTemp=VehicleSmallDistance/100;
	Digit_Data_Conversation_To_Real_Display(DwordTemp,LiChenXiaoJi,sizeof(LiChenXiaoJi)-2,3);
	LiChenXiaoJi[5]=FONT_WORD_a+10;///k
	LiChenXiaoJi[6]=FONT_WORD_a+12;///m
	WriteColor_StringToAddr(0x30+15,BG_COLOR_RED|CH_COLOR_BLACK,LiChenXiaoJi,sizeof(LiChenXiaoJi),0);//最多可显示11个字符
	WriteColor_StringToAddr(0x30+15+7,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);	
	///不可用透过区域 
	WriteColor_StringToAddr(0x30+26,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);
	///里程总计
	DwordTemp =VehicleOverallDistance/10;
	Digit_Data_Conversation_To_Real_Display(DwordTemp,LiChenZongJi,sizeof(LiChenZongJi)-2,DIGIT_DATA_NO_DOT);
	LiChenZongJi[6]=FONT_WORD_a+10;///k
	LiChenZongJi[7]=FONT_WORD_a+12;///m	
	WriteColor_StringToAddr(0x30+30,BG_COLOR_RED|CH_COLOR_BLACK,LiChenZongJi,sizeof(LiChenZongJi),0);//最多可显示11个字符
	WriteColor_StringToAddr(0x30+30+8,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,3,0);
	
	if(Display_ThirdMenu_DistanceFualConsume_Flag==1)
	{
		Display_ThirdMenu_DistanceFualConsume_Flag=0;
		FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
		FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
		OsdCmd(OsdOpen);	
		SOsdWinBuffWrite2Hw(SPI_OSD_WIN_2,SPI_OSD_WIN_3);		
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Display_ThirdMenu_PictureAndChannleSet(void)
{
	unsigned char i,Temp,TempNum;
	if(Display_ThirdMenu_PictureChannle_Flag==1)
	{
		if(PictureChannleSelChange_Flag==0)
		{
			OsdCmd(OsdClose);
			SpiOsdWinAllOff();
			FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
		}
	}
	if(Display_ThirdMenu_PictureChannle_Flag==1||PictureChannleSelChange_Flag==1||PictureChannleNumChange_Flag==1)
	{
		///spi osd
		if(ThirdMenu_Cursor==ChannleSetSubMenuNum)
		{
			///font osd			
			if(Display_ThirdMenu_PictureChannle_Flag==1)
			{
				//通道选择底层
				Display_Layer_Graphics(&img_ThirdMenu_Channle,&header_table,&ThirdMenu_Channle_Item_Info,
																	&ThirdMenu_Channle_Header,SPI_OSD_WIN_2);
				if(FourMenu_Cursor!=0xff)
				{
					SOsdHwBuffSetAlpha(0x000+8);
					SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, 0x7f);				
				}
				else
				{
					SOsdHwBuffSetAlpha(0x000+8);
					SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);	
				}
				//font osd
				FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
				FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_1,1,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);

				FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_2,1,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0x10);

				FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_3,1,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,0x20);			

				FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_4,1,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,0x30);					
			
				FontOsdWinScreen(FONT_OSD_WIN_1, 410,104, 1, 1);	//前门通道
				FontOsdWinScreen(FONT_OSD_WIN_2, 410,186, 1, 1);	//中门通道
				FontOsdWinScreen(FONT_OSD_WIN_3, 410,268, 1, 1);	//倒车通道				
				FontOsdWinScreen(FONT_OSD_WIN_4, 410,350, 1, 1);	//顶部通道	

				FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_4,ON);
			}
			else if(PictureChannleSelChange_Flag==1)
			{
				PictureChannleSelChange_Flag=0;
				SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, OFF);

				switch(FourMenu_Cursor)
				{
					case 0:
						//前门
						Display_Layer_Graphics(&img_ThirdMenu_ChannleFrontDoorSel,&header_table,&ThirdMenu_ChannleFrontDoor_Item_Info,
																			&ThirdMenu_ChannleItem_Header,SPI_OSD_WIN_3);
						break;
					case 1:
						//中门
						Display_Layer_Graphics(&img_ThirdMenu_ChannleMiddleDoorSel,&header_table,&ThirdMenu_ChannleMiddleDoor_Item_Info,
																			&ThirdMenu_ChannleItem_Header,SPI_OSD_WIN_3);
						break;						
						break;
					case 2:
						//倒车
						Display_Layer_Graphics(&img_ThirdMenu_ChannleBackCarSel,&header_table,&ThirdMenu_ChannleBackCar_Item_Info,
																			&ThirdMenu_ChannleItem_Header,SPI_OSD_WIN_3);
						break;
					case 3:
						//顶部
						Display_Layer_Graphics(&img_ThirdMenu_ChannleTOPSel,&header_table,&ThirdMenu_ChannleTOP_Item_Info,
																			&ThirdMenu_ChannleItem_Header,SPI_OSD_WIN_3);							
						break;
					default:						
						break;
				} 					
				if(FourMenu_Cursor==0xff)
				{
					SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
				}
				else
				{
					SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
					SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,1); 			
				}
			}
			PictureChannleNumChange_Flag=0;
			DelayMs(30);
			//前门通道号	
			TempNum=FrontDoorVideoChannel+1;
			if(FourMenu_Cursor==0)
			{				
				WriteColor_StringToAddr(0,BG_COLOR_BLACK|CH_COLOR_MAGENTA,&TempNum,1,1);
				if(VideoSignalDetect()==SignalExist)
					FrontDoorVideoSignalNum=SignalExist;
				else
				{
					FrontDoorVideoSignalNum=SignalNo;				
//					Osd_Video_Change(Osd_State);
				}
			}
			else
				WriteColor_StringToAddr(0,BG_COLOR_MAGENTA|CH_COLOR_BLACK,&TempNum,1,0);
			//中门通道号	
			TempNum=MiddleDoorVideoChannel+1;
			if(FourMenu_Cursor==1)
			{
				WriteColor_StringToAddr(0x10,BG_COLOR_BLACK|CH_COLOR_MAGENTA,&TempNum,1,1);
				if(VideoSignalDetect()==SignalExist)
					MiddleDoorVideoSignalNum=SignalExist;
				else
				{
					MiddleDoorVideoSignalNum=SignalNo;				
//					Osd_Video_Change(Osd_State);
				}
			}
			else
				WriteColor_StringToAddr(0x10,BG_COLOR_MAGENTA|CH_COLOR_BLACK,&TempNum,1,0);
			//倒车通道号	
			TempNum=RShieldVideoChannel+1;
			if(FourMenu_Cursor==2)
			{
				WriteColor_StringToAddr(0x20,BG_COLOR_BLACK|CH_COLOR_MAGENTA,&TempNum,1,1);		
				if(VideoSignalDetect()==SignalExist)
					RShieldVideoSignalNum=SignalExist;
				else
				{
					RShieldVideoSignalNum=SignalNo; 	
//					Osd_Video_Change(Osd_State);
				}
			}
			else
				WriteColor_StringToAddr(0x20,BG_COLOR_MAGENTA|CH_COLOR_BLACK,&TempNum,1,0);	
			//顶部通道号	
			TempNum=TopVideoChannel+1;
			if(FourMenu_Cursor==3)
			{
				WriteColor_StringToAddr(0x30,BG_COLOR_BLACK|CH_COLOR_MAGENTA,&TempNum,1,1);		
				if(VideoSignalDetect()==SignalExist)
					TopVideoSignalNum=SignalExist;
				else
				{
					TopVideoSignalNum=SignalNo; 	
//					Osd_Video_Change(Osd_State);
				}
			}
			else
				WriteColor_StringToAddr(0x30,BG_COLOR_MAGENTA|CH_COLOR_BLACK,&TempNum,1,0);			
		}
		else
		{	//图像参数设置					
			if(Display_ThirdMenu_PictureChannle_Flag==1)
			{
				//参数设置底层
				Display_Layer_Graphics(&img_ThirdMenu_Picture,&header_table,&ThirdMenu_Picture_Item_Info,
																	&ThirdMenu_Picture_Header,SPI_OSD_WIN_2);	
				SOsdHwBuffSetAlpha(0x000+7);
				SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, 0x7f);		

				///font osd
				FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
				FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);

				FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0x20);
				
				FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,0x40);
				
				FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);				
				FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,0x60);	

				
				FontOsdWinScreen(FONT_OSD_WIN_1, 370, 97, 20, 1);	//对比度
				FontOsdWinScreen(FONT_OSD_WIN_2, 370,180, 20, 1);	//亮度
				FontOsdWinScreen(FONT_OSD_WIN_3, 370,264, 20, 1);	//色度
				FontOsdWinScreen(FONT_OSD_WIN_4, 370,348, 20, 1);	//清晰度
				
				FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_4,ON);			
			}
			else if(PictureChannleSelChange_Flag==1)
			{
				PictureChannleSelChange_Flag=0;
				switch(FourMenu_Cursor)
				{
					case 0:
						//对比度
						Display_Layer_Graphics(&img_ThirdMenu_PictureContrastSel,&header_table,&ThirdMenu_PictureContrastSel_Item_Info,
																			&ThirdMenu_PictureItem_Header,SPI_OSD_WIN_3);			
						break;
					case 1:
						//亮度
						Display_Layer_Graphics(&img_ThirdMenu_PictureBrightSel,&header_table,&ThirdMenu_PictureBrightSel_Item_Info,
																			&ThirdMenu_PictureItem_Header,SPI_OSD_WIN_3);
						break;
					case 2:
						//色度
						Display_Layer_Graphics(&img_ThirdMenu_PictureChromaSel,&header_table,&ThirdMenu_PictureChromaSel_Item_Info,
																			&ThirdMenu_PictureItem_Header,SPI_OSD_WIN_3);
						break;
					case 3:
						//清晰度
						Display_Layer_Graphics(&img_ThirdMenu_PictureDefinitionSel,&header_table,&ThirdMenu_PictureDefinitionSel_Item_Info,
																			&ThirdMenu_PictureItem_Header,SPI_OSD_WIN_3);
						break;
					default:
						break;
				}	
				SOsdHwBuffSetAlpha(0x100+7);
				SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, 0x7f);					
				if(FourMenu_Cursor==0xff)
				{	
					SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
//						FontOsdWinDisplay(FONT_OSD_WIN_1,OFF);
				}
				else
				{				
					SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
					SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,0); 			
				}				
			}
			else if(PictureChannleNumChange_Flag==1)
			{
				PictureChannleNumChange_Flag=0;
				switch(FourMenu_Cursor)
				{
					case 0://对比度
						ContrastCmd(PictureContrastNum);	
						break;
					case 1://亮度
						BrightnessCmd(PictureBrightNum);
						break;
					case 2://色度
						HueCmd(PictureChromaNum); 
						break;
					case 3://清晰度
						SharpnessCmd(PictureDefinitionNum); 
						break;
					default:
						break;
				}	
			}			
			///font osd 
			///对比度
			TempNum =PictureContrastNum<<1;					
			Temp =FONT_WORD_SHU;
			for(i=0;i<20;i++)
			{
				if(i<TempNum)
					WriteColor_StringToAddr(i,BG_COLOR_RED|CH_COLOR_BLACK,&Temp,1,0); 		
				else
					WriteColor_StringToAddr(i,BG_COLOR_WHITE|CH_COLOR_BLACK,&Temp,1,0); 
			}			
			///亮度
			TempNum =PictureBrightNum<<1;			
			Temp =FONT_WORD_SHU;
			for(i=0;i<20;i++)
			{
				if(i<TempNum)
					WriteColor_StringToAddr(0x20+i,BG_COLOR_RED|CH_COLOR_BLACK,&Temp,1,0); 		
				else
					WriteColor_StringToAddr(0x20+i,BG_COLOR_WHITE|CH_COLOR_BLACK,&Temp,1,0); 
			}		
			///色度
			TempNum =PictureChromaNum<<1;					
			Temp =FONT_WORD_SHU;
			for(i=0;i<20;i++)
			{
				if(i<TempNum)
					WriteColor_StringToAddr(0x40+i,BG_COLOR_RED|CH_COLOR_BLACK,&Temp,1,0); 		
				else
					WriteColor_StringToAddr(0x40+i,BG_COLOR_WHITE|CH_COLOR_BLACK,&Temp,1,0); 
			}		
			///清晰度
			TempNum =PictureDefinitionNum<<1;					
			Temp =FONT_WORD_SHU;
			for(i=0;i<20;i++)
			{
				if(i<TempNum)
					WriteColor_StringToAddr(0x60+i,BG_COLOR_RED|CH_COLOR_BLACK,&Temp,1,0); 		
				else
					WriteColor_StringToAddr(0x60+i,BG_COLOR_WHITE|CH_COLOR_BLACK,&Temp,1,0); 
			}					
		}
	}
	if(Display_ThirdMenu_PictureChannle_Flag==1)
	{
		Display_ThirdMenu_PictureChannle_Flag=0;
		OsdCmd(OsdOpen);		
		if(PictureChannleSelChange_Flag==0)
			SOsdHwBuffWrite2Hw();
		else
			SOsdHwBuffWrite_Win(SPI_OSD_WIN_2,1);
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Display_ThirdMenu_SystemTimeSet(void)
{
	unsigned char i;
	unsigned char	Date_Display[10],Time_Display[8];
	
	if(Display_ThirdMenu_TimeSet_Flag==1)
	{
		OsdCmd(OsdClose);
		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
	}
	if(Display_ThirdMenu_TimeSet_Flag==1||SystemDataTimeSelChange_Flag==1)
	{
		///spi osd
		Display_Layer_Graphics(&img_SystemTimeMenuBackGround,&header_table,&SystemTimeMenu_BackGround_Info,
														&SystemTimeMenu_BackGround_Header,SPI_OSD_WIN_2);
		SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF); 
		
		///日期设置
		if(time_error_flag==1)
		{
			Display_Layer_Graphics(&img_SystemDataTimeErr,&header_table,&SystemDataTimeErr_Info,
																&SystemDataTimeErr_Header,SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, OFF);		
		}
		else if(SystemDataTimeSet_Flag==0)
		{
			Display_Layer_Graphics(&img_SystemDataItemSel,&header_table,&SystemDataItemSel_Info,
																&SystemTimeDataItem_Header,SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, OFF);		
		}
		else
		{
			Display_Layer_Graphics(&img_SystemTimeItemSel,&header_table,&SystemTimeItemSel_Info,
																&SystemTimeDataItem_Header,SPI_OSD_WIN_3);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, OFF);	
		}
		///font osd
		FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
		FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);
		FontOsdWinZoom(FONT_OSD_WIN_1,0,0);

		FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
		FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0x10);
		FontOsdWinZoom(FONT_OSD_WIN_2,0,0);		

		FontOsdWinScreen(FONT_OSD_WIN_1, 330,180, 10, 1);
		FontOsdWinScreen(FONT_OSD_WIN_2, 330,285, 8, 1);	

		if(Display_ThirdMenu_TimeSet_Flag==0&&SystemDataTimeSelChange_Flag==1)
		{
			SystemDataTimeSelChange_Flag=0;
			SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
			SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,0);
		}
	}
	///font osd
	///change to date display
	Date_Display[0]=2;
	Date_Display[1]=0;
	Date_Display[2]=year_h;
	Date_Display[3]=year_l;
	Date_Display[4]=FONT_WORD_HENG;
	Date_Display[5]=month_h;
	Date_Display[6]=month_l;
	Date_Display[7]=FONT_WORD_HENG;
	Date_Display[8]=date_h;
	Date_Display[9]=date_l;
	///change to time display
	Time_Display[0]=hour_h;
	Time_Display[1]=hour_l;
	Time_Display[2]=FONT_WORD_ZHUANG_DIAN;
	Time_Display[3]=minute_h;
	Time_Display[4]=minute_l;
	Time_Display[5]=FONT_WORD_ZHUANG_DIAN;
	Time_Display[6]=second_h;
	Time_Display[7]=second_l;	
	//日期显示
	for(i=0;i<sizeof(Date_Display);i++)
	{
		if(i==(TimeSetCursor+2)&&SystemDataTimeSet_Flag==0)
			WriteColor_StringToAddr(i,BG_COLOR_BLACK|CH_COLOR_RED,&Date_Display[i],1,1);
		else
			WriteColor_StringToAddr(i,BG_COLOR_RED|CH_COLOR_BLACK,&Date_Display[i],1,0);
	}
	//时间显示
	for(i=0;i<sizeof(Time_Display);i++)
	{	
		if(i==TimeSetCursor&&SystemDataTimeSet_Flag==1)
			WriteColor_StringToAddr(0x10+i,BG_COLOR_BLACK|CH_COLOR_RED,&Time_Display[i],1,1);
		else
			WriteColor_StringToAddr(0x10+i,BG_COLOR_RED|CH_COLOR_BLACK,&Time_Display[i],1,0);
	}
	
	if(Display_ThirdMenu_TimeSet_Flag==1)
	{
		Display_ThirdMenu_TimeSet_Flag=0;
		FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
		FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
		OsdCmd(OsdOpen);		
		SOsdWinBuffWrite2Hw(SPI_OSD_WIN_2,SPI_OSD_WIN_3);
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、三级界面的胎压查询
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Display_ThirdMenu_TyrePress(void)
{
	unsigned char Temp,i,j;
	unsigned char DispTyreTemp[9],DispTyrePress[9];
	const struct menu_item_info *image_Disp_info;	
	const SLIDEIMAGE *img_Select;
  uint TyreDataTemp;
	unsigned char DispTPMS_Status[2];
	unsigned long DispTPMS_ID;
	img_Select=(const SLIDEIMAGE *)0;	
	if(Display_ThirdMenu_TyrePress_Flag==1)
	{
		if(TyrePressMenuItem==1)
		{
			if(TyrePressUpDataCount==0)
			{
				OsdCmd(OsdClose);
				SpiOsdWinAllOff();
				FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
			}
		}
		else
		{
			OsdCmd(OsdClose);
			SpiOsdWinAllOff();
			FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
			TyrePressUpDataCount=0;
		}
		
	}
	if(TyrePressMenuItem==0)
	{
		///spi osd
		//胎压查询
		if(Display_ThirdMenu_TyrePress_Flag==1)
		{
			Display_Layer_Graphics(&img_TyrePressMenuBackGround,&header_table,&TyrePressMenuBackGround_Info,
																&TyrePressMenuBackGround_Header,SPI_OSD_WIN_2);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);

			///font osd
			FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
			FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);

			FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0x40);

			FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,0x60);
			
			FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,0x80);					
/*
			FontOsdWinScreen(FONT_OSD_WIN_1,246,57, 19, 3);
			FontOsdWinScreen(FONT_OSD_WIN_2,439,147, 7, 3);
			FontOsdWinScreen(FONT_OSD_WIN_3,439,257, 7, 3);
			FontOsdWinScreen(FONT_OSD_WIN_4,246,347,19, 3);			
*/
			FontOsdWinScreen(FONT_OSD_WIN_1,243,80,20, 2);
			FontOsdWinScreen(FONT_OSD_WIN_2,417,164, 9, 2);
			FontOsdWinScreen(FONT_OSD_WIN_3,417,274, 9, 2);
			FontOsdWinScreen(FONT_OSD_WIN_4,243,358,20, 2);				
		}
		///font osd 
		//胎压1
		DispTyrePress[0]=FONT_WORD_QING;
		DispTyrePress[5]=FONT_WORD_A+'K'-'A';
		DispTyrePress[6]=FONT_WORD_a+'P'-'A';
		DispTyrePress[7]=FONT_WORD_QING;
		DispTyrePress[8]=FONT_WORD_QING;
		
		DispTyreTemp[0]=FONT_WORD_QING;		
//		DispTyreTemp[4]=FONT_WORD_ZHE_DU;		
//		DispTyreTemp[5]=FONT_WORD_ZHE_DU;
		DispTyreTemp[6]=FONT_WORD_QING;
		DispTyreTemp[7]=FONT_WORD_QING;
		DispTyreTemp[8]=FONT_WORD_QING;		
		TyreDataTemp=TyreGetData[0].TyrePressure;//第一行
		Digit_Data_Conversation_To_Real_Display(TyreDataTemp,&DispTyrePress[1],sizeof(DispTyrePress)-5,DIGIT_DATA_NO_DOT);
		WriteColor_StringToAddr(0x00,BG_COLOR_RED|CH_COLOR_BLACK,DispTyrePress,9,0);	
		Temp=TyreGetData[0].TyreTemperature;//第二行
		if(TyreGetData[0].TyreStateFlag.b.HuWenDuFlag==1)		
		{
			DispTyreTemp[1]=FONT_WORD_HENG;
			DispTyreTemp[5]=FONT_WORD_ZHE_DU;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[2],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			DispTyreTemp[4]=FONT_WORD_ZHE_DU;
			DispTyreTemp[5]=FONT_WORD_QING;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[1],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}
		WriteColor_StringToAddr(0x00+20,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);

		//部分未用透过	
		WriteColor_StringToAddr(0x00+9,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,2,0);	
		WriteColor_StringToAddr(0x00+9+20,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,2,0);	

		//胎压3
		TyreDataTemp=TyreGetData[2].TyrePressure;//第一行
		Digit_Data_Conversation_To_Real_Display(TyreDataTemp,&DispTyrePress[1],sizeof(DispTyrePress)-5,DIGIT_DATA_NO_DOT);
		WriteColor_StringToAddr(0x00+11,BG_COLOR_RED|CH_COLOR_BLACK,DispTyrePress,9,0);	
		Temp=TyreGetData[2].TyreTemperature;//第二行
		if(TyreGetData[2].TyreStateFlag.b.HuWenDuFlag==1)		
		{
			DispTyreTemp[1]=FONT_WORD_HENG;
			DispTyreTemp[5]=FONT_WORD_ZHE_DU;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[2],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			DispTyreTemp[4]=FONT_WORD_ZHE_DU;
			DispTyreTemp[5]=FONT_WORD_QING;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[1],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}
		WriteColor_StringToAddr(0x00+11+20,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);	


		//胎压4
		TyreDataTemp=TyreGetData[3].TyrePressure;//第一行
		Digit_Data_Conversation_To_Real_Display(TyreDataTemp,&DispTyrePress[1],sizeof(DispTyrePress)-5,DIGIT_DATA_NO_DOT);
		WriteColor_StringToAddr(0x40,BG_COLOR_RED|CH_COLOR_BLACK,DispTyrePress,9,0);
		Temp=TyreGetData[3].TyreTemperature;//第二行
		if(TyreGetData[3].TyreStateFlag.b.HuWenDuFlag==1)		
		{
			DispTyreTemp[1]=FONT_WORD_HENG;
			DispTyreTemp[5]=FONT_WORD_ZHE_DU;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[2],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			DispTyreTemp[4]=FONT_WORD_ZHE_DU;
			DispTyreTemp[5]=FONT_WORD_QING;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[1],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}		
		WriteColor_StringToAddr(0x40+9,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);

		//胎压5		
		TyreDataTemp=TyreGetData[4].TyrePressure;//第一行
		Digit_Data_Conversation_To_Real_Display(TyreDataTemp,&DispTyrePress[1],sizeof(DispTyrePress)-5,DIGIT_DATA_NO_DOT);
		WriteColor_StringToAddr(0x60,BG_COLOR_RED|CH_COLOR_BLACK,DispTyrePress,9,0);
		Temp=TyreGetData[4].TyreTemperature;//第二行
		if(TyreGetData[4].TyreStateFlag.b.HuWenDuFlag==1)		
		{
			DispTyreTemp[1]=FONT_WORD_HENG;
			DispTyreTemp[5]=FONT_WORD_ZHE_DU;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[2],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			DispTyreTemp[4]=FONT_WORD_ZHE_DU;
			DispTyreTemp[5]=FONT_WORD_QING;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[1],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}		
		WriteColor_StringToAddr(0x60+9,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);


		//胎压2	
		TyreDataTemp=TyreGetData[1].TyrePressure;//第一行
		Digit_Data_Conversation_To_Real_Display(TyreDataTemp,&DispTyrePress[1],sizeof(DispTyrePress)-5,DIGIT_DATA_NO_DOT);
		WriteColor_StringToAddr(0x80,BG_COLOR_RED|CH_COLOR_BLACK,DispTyrePress,9,0);	
		Temp=TyreGetData[1].TyreTemperature;//第二行
		if(TyreGetData[1].TyreStateFlag.b.HuWenDuFlag==1)		
		{
			DispTyreTemp[1]=FONT_WORD_HENG;
			DispTyreTemp[5]=FONT_WORD_ZHE_DU;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[2],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			DispTyreTemp[4]=FONT_WORD_ZHE_DU;
			DispTyreTemp[5]=FONT_WORD_QING;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[1],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}			
		WriteColor_StringToAddr(0x80+20,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);
	
		//透过未用部分
		WriteColor_StringToAddr(0x80+9,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,2,0);	
		WriteColor_StringToAddr(0x80+9+20,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,2,0);	
		
		//胎压6
		TyreDataTemp=TyreGetData[5].TyrePressure;//第一行
		Digit_Data_Conversation_To_Real_Display(TyreDataTemp,&DispTyrePress[1],sizeof(DispTyrePress)-5,DIGIT_DATA_NO_DOT);
		WriteColor_StringToAddr(0x80+11,BG_COLOR_RED|CH_COLOR_BLACK,DispTyrePress,9,0);	
		Temp=TyreGetData[5].TyreTemperature;//第二行
		if(TyreGetData[5].TyreStateFlag.b.HuWenDuFlag==1)		
		{
			DispTyreTemp[1]=FONT_WORD_HENG;
			DispTyreTemp[5]=FONT_WORD_ZHE_DU;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[2],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}
		else
		{
			DispTyreTemp[4]=FONT_WORD_ZHE_DU;
			DispTyreTemp[5]=FONT_WORD_QING;
			Digit_Data_Conversation_To_Real_Display(Temp,&DispTyreTemp[1],sizeof(DispTyreTemp)-6,DIGIT_DATA_NO_DOT);	
		}		
		WriteColor_StringToAddr(0x80+11+20,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);	
		
	}//胎压监测
	else if(TyrePressMenuItem==1)
	{
		///spi osd
		//胎压监测
		if(Display_ThirdMenu_TyrePress_Flag==1)
		{
			if(TyrePressUpDataCount==0)
			{
				Display_Layer_Graphics(&img_TyrePressMonitorMenuBackGround,&header_table,&TyrePressMonitorMenuBackGround_Info,
																	&TyrePressMonitorMenuBackGround_Header,SPI_OSD_WIN_2);
				SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);
				FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
			}	
		}
		if(TyrePressUpDataCount==1)
		{
			if(Display_ThirdMenu_TyrePress_Flag==1)
			{
				FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_1,1,1);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);

				FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0x02);

//				FontOsdWinScreen(FONT_OSD_WIN_1,258,83, 2, 1);		//主机状态
				FontOsdWinScreen(FONT_OSD_WIN_1,242,70, 2, 1);		//主机状态				
				FontOsdWinScreen(FONT_OSD_WIN_2,206,200,36,1);		//胎压状态
			}
			///font osd FONT_OSD_WIN_1
			Temp=0;
			for(i=0;i<6;i++)
			{
				if((TyreState_Flag[i]&0x40)!=0)
					Temp=0xff;
			}
			if(TPMS_Moudle_Err_Flag==1||Temp==0xff)
			{
				DispTPMS_Status[0]=FONT_HanZi_Duan1;
				DispTPMS_Status[1]=FONT_HanZi_Kai;
			}
			else
			{
				DispTPMS_Status[0]=FONT_HanZi_Zheng;
				DispTPMS_Status[1]=FONT_HanZi_Chang;
			}
			WriteColor_StringToAddr(0x00,BG_COLOR_CYAN|CH_COLOR_BLACK,DispTPMS_Status,2,0);
				
			//FONT_OSD_WIN_2
			j=0;
			for(i=0;i<36;i=i+6)
			{ 
				if((TyreState_Flag[j]&0x02)!=0)		//过压
				{
					DispTPMS_Status[0]=FONT_HanZi_Di;
					DispTPMS_Status[1]=FONT_HanZi_Ya;
				}
				else if((TyreState_Flag[j]&0x04)!=0)//低压
				{
					DispTPMS_Status[0]=FONT_HanZi_Gao;
					DispTPMS_Status[1]=FONT_HanZi_Ya;
				}
				else
				{
					DispTPMS_Status[0]=FONT_HanZi_Zheng;//正常
					DispTPMS_Status[1]=FONT_HanZi_Chang;			
				}
				WriteColor_StringToAddr(0x02+i,BG_COLOR_RED|CH_COLOR_BLACK,DispTPMS_Status,2,0);		//压力状态
				WriteColor_StringToAddr(0x02+i+2,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);//未用透过
				j++;
			}	
		}
		else if(TyrePressUpDataCount==2)
		{
			if(Display_ThirdMenu_TyrePress_Flag==1)
			{

				FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,38);

				FontOsdWinScreen(FONT_OSD_WIN_3,206,252,36,1);		//胎压状态
			}
			//FONT_OSD_WIN_3
			j=0;
			for(i=0;i<36;i=i+6)
			{
				if((TyreState_Flag[j]&0x08)!=0)		//高温
				{
					DispTPMS_Status[0]=FONT_HanZi_Gao;
					DispTPMS_Status[1]=FONT_HanZi_Wen;
				}
				else
				{
					DispTPMS_Status[0]=FONT_HanZi_Zheng;//正常
					DispTPMS_Status[1]=FONT_HanZi_Chang;
				}
				WriteColor_StringToAddr(38+i,BG_COLOR_RED|CH_COLOR_BLACK,DispTPMS_Status,2,0);	//温度状态
				WriteColor_StringToAddr(38+i+2,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);//未用透过
				j++;
			}	
		}		
		else if(TyrePressUpDataCount==3)
		{
			if(Display_ThirdMenu_TyrePress_Flag==1)
			{
				FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
				FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
				FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,74);
				FontOsdWinScreen(FONT_OSD_WIN_4,206,304,36,3);		//胎压状态
			}
			//FONT_OSD_WIN_4
			for(Temp=0;Temp<3;Temp++)
			{
				if(Temp==1)
					WriteColor_StringToAddr(74+Temp*36,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,36,0);
				else if(Temp==0)
				{
					j=0;
					for(i=0;i<36;i=i+6)
					{
						if((TyreState_Flag[j]&0x01)!=0)		//漏气
						{
							DispTPMS_Status[0]=FONT_HanZi_Lou;
							DispTPMS_Status[1]=FONT_HanZi_Qi;
						}
						else
						{
							DispTPMS_Status[0]=FONT_HanZi_Zheng;//正常
							DispTPMS_Status[1]=FONT_HanZi_Chang;
						}						
						WriteColor_StringToAddr(74+Temp*36+i,BG_COLOR_RED|CH_COLOR_BLACK,DispTPMS_Status,2,0);			 //漏气状态
						WriteColor_StringToAddr(74+Temp*36+i+2,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);//未用透过
						j++;
					}
				}
				else if(Temp==2)
				{
					j=0;
					for(i=0;i<36;i=i+6)
					{
						if((TyreState_Flag[j]&0x20)!=0)		//传感器故障
						{
							DispTPMS_Status[0]=FONT_HanZi_Gu;
							DispTPMS_Status[1]=FONT_HanZi_Zhang;
						}
						else
						{
							DispTPMS_Status[0]=FONT_HanZi_Zheng;//正常
							DispTPMS_Status[1]=FONT_HanZi_Chang;
						}							
						WriteColor_StringToAddr(74+Temp*36+i,BG_COLOR_RED|CH_COLOR_BLACK,DispTPMS_Status,2,0);			 //传感器状态
						WriteColor_StringToAddr(74+Temp*36+i+2,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);//未用透过
						j++;
					}
				}				
			}
		}		
	}//胎压学习密码设置
	else	if(TyrePressMenuItem==2)
	{
		///spi osd
		//胎压密码设置
		if(Display_ThirdMenu_TyrePress_Flag==1||TyrePressSetSelChange_Flag==1)
		{
			Display_Layer_Graphics(&img_TyrePressPassWordMenuBackGround,&header_table,&TyrePressPassWordMenuBackGround_Info,
																&TyrePressPassWordMenuBackGround_Header,SPI_OSD_WIN_2);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);

			if(TyrePressPassWordErr_Flag==1)
			{
				Display_Layer_Graphics(&img_TyrePressPassWordErr,&header_table,&TyrePressPassWordErr_Info,
																	&TyrePressPassWordErr_Header,SPI_OSD_WIN_3);
				SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
			}
			///font osd
			FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
			FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);	

			FontOsdWinScreen(FONT_OSD_WIN_1,273,174,20, 1);

			if(Display_ThirdMenu_TyrePress_Flag==0)
			{
				TyrePressSetSelChange_Flag=0;
				if(TyrePressPassWordErr_Flag==0)
				{
					SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
				}
				else
				{
					SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
					SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,0); 
				}					
			}
		}
		///font osd 
		///密码
		for(i=0,Temp=5;i<4;i++)
		{	
			if((i!=TyrePressPassWordNum)||TyrePressMenuStatus==0xff)
				WriteColor_StringToAddr(0x00+Temp*i,BG_COLOR_RED|CH_COLOR_RED,ClearFontOsd,1,0);
			else
			{
				if(TyrePressPassWord[TyrePressPassWordNum]>9)
				{
					j=TyrePressPassWord[TyrePressPassWordNum]-10;
					j=FONT_WORD_A+j;
				}
				else
					j=TyrePressPassWord[TyrePressPassWordNum];
				WriteColor_StringToAddr(0x00+Temp*i,BG_COLOR_BLACK|CH_COLOR_RED,&j,1,1);
			}
			WriteColor_StringToAddr(0x00+Temp*i+1,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,4,0);
		}
	}	
	else	if(TyrePressMenuItem==3)
	{
		///spi osd
		//胎压学习
		if(Display_ThirdMenu_TyrePress_Flag==1||TyrePressSetSelChange_Flag==1)
		{
			Display_Layer_Graphics(&img_TyrePressSetMenuBackGround,&header_table,&TyrePressSetMenuBackGround_Info,
																&TyrePressSetMenuBackGround_Header,SPI_OSD_WIN_2);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);
			
			switch(TyrePressMenuLocate)
			{
				case 0://胎压1
					image_Disp_info=&TyrePressSetMenuItem1_Info;
					break;
				case 1://胎压2
					image_Disp_info=&TyrePressSetMenuItem2_Info;
					break;
				case 2://胎压3
					image_Disp_info=&TyrePressSetMenuItem3_Info;
					break;	
				case 3://胎压4
					image_Disp_info=&TyrePressSetMenuItem4_Info;
					break;
				case 4://胎压5
					image_Disp_info=&TyrePressSetMenuItem5_Info;
					break;
				case 5://胎压6
					image_Disp_info=&TyrePressSetMenuItem6_Info;
					break;						
				default:
					image_Disp_info=&TyrePressSetMenuItem1_Info;					
					break;	
			}
			
			switch(TyrePressMenuStatus)
			{
				case 0://选中
					if(TyrePressMenuLocate==0||TyrePressMenuLocate==2)
						img_Select=&img_TyrePressSetMenuItemUpSel;
					else if(TyrePressMenuLocate==3||TyrePressMenuLocate==4)
						img_Select=&img_TyrePressSetMenuItemMiddleSel;
					else if(TyrePressMenuLocate==1||TyrePressMenuLocate==5)
						img_Select=&img_TyrePressSetMenuItemDownSel;
					break;
				case 1://设置中
					if(TyrePressMenuLocate==0||TyrePressMenuLocate==2)
						img_Select=&img_TyrePressSetMenuItemUpSetting;
					else if(TyrePressMenuLocate==3||TyrePressMenuLocate==4)
						img_Select=&img_TyrePressSetMenuItemMiddleSetting;
					else if(TyrePressMenuLocate==1||TyrePressMenuLocate==5)
						img_Select=&img_TyrePressSetMenuItemDownSetting;
					break;					
				case 2://设置超时
					if(TyrePressMenuLocate==0||TyrePressMenuLocate==2)
						img_Select=&img_TyrePressSetMenuItemUpFual;
					else if(TyrePressMenuLocate==3||TyrePressMenuLocate==4)
						img_Select=&img_TyrePressSetMenuItemMiddleFual;
					else if(TyrePressMenuLocate==1||TyrePressMenuLocate==5)
						img_Select=&img_TyrePressSetMenuItemDownFual;
					break;
				case 3://设置成功
					if(TyrePressMenuLocate==0||TyrePressMenuLocate==2)
						img_Select=&img_TyrePressSetMenuItemUpsucceed;
					else if(TyrePressMenuLocate==3||TyrePressMenuLocate==4)
						img_Select=&img_TyrePressSetMenuItemMiddlesucceed;
					else if(TyrePressMenuLocate==1||TyrePressMenuLocate==5)
						img_Select=&img_TyrePressSetMenuItemDownsucceed;
					break;	
				default:					
					break;	
			}
			if(img_Select!=((const SLIDEIMAGE *)0))
			{
				Display_Layer_Graphics(img_Select,&header_table,image_Disp_info,
																	&TyrePressSetMenuItem_Header,SPI_OSD_WIN_3);
				SpiOsdWinPixelAlpha( SPI_OSD_WIN_3, ON);
			}
			///font osd
			FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
			FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,0);

			FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,0x30);

			FontOsdAplha(FONT_OSD_WIN_3,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_3,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_3,0x60);
			
			FontOsdAplha(FONT_OSD_WIN_4,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_4,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_4,0x90);					

			FontOsdWinScreen(FONT_OSD_WIN_1,241,106,20, 1);
			FontOsdWinScreen(FONT_OSD_WIN_2,416,190, 9, 1);
			FontOsdWinScreen(FONT_OSD_WIN_3,416,300, 9, 1);
			FontOsdWinScreen(FONT_OSD_WIN_4,241,384,20, 1);			

			if(Display_ThirdMenu_TyrePress_Flag==0)
			{
				TyrePressSetSelChange_Flag=0;
				if(TyrePressMenuStatus==0xff)
				{
					SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
				}
				else
				{
					SpiOsdWinHWEnable(SPI_OSD_WIN_3, OFF);
					if(TyrePressMenuLocate==1||TyrePressMenuLocate==5)
						SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,1); 	
					else
						SOsdHwBuffWrite_Win(SPI_OSD_WIN_3,0); 
				}					
			}
		}
		///font osd 
		//胎压1
		DispTyreTemp[0]='I'-'A'+FONT_WORD_A;
		DispTyreTemp[1]='D'-'A'+FONT_WORD_A;
		DispTyreTemp[2]=FONT_WORD_ZHUANG_DIAN;	

		DispTPMS_ID=TPMS_ID[0][0];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[0][1];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[0][2];
		Digit_Data_Conversation_To_Hex_Display(DispTPMS_ID,DispTyreTemp+3,6);
		WriteColor_StringToAddr(0x00,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);	
		//部分未用透过	
		WriteColor_StringToAddr(0x00+9,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,2,0);			
		//胎压3
		DispTPMS_ID=TPMS_ID[2][0];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[2][1];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[2][2];	
		Digit_Data_Conversation_To_Hex_Display(DispTPMS_ID,DispTyreTemp+3,6);		
		WriteColor_StringToAddr(0x00+11,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);	

		//胎压4
		DispTPMS_ID=TPMS_ID[3][0];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[3][1];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[3][2];		
		Digit_Data_Conversation_To_Hex_Display(DispTPMS_ID,DispTyreTemp+3,6);			
		WriteColor_StringToAddr(0x30,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);
		
		//胎压5		
		DispTPMS_ID=TPMS_ID[4][0];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[4][1];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[4][2];				
		Digit_Data_Conversation_To_Hex_Display(DispTPMS_ID,DispTyreTemp+3,6);			
		WriteColor_StringToAddr(0x60,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);

		//胎压2	
		DispTPMS_ID=TPMS_ID[1][0];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[1][1];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[1][2];					
		Digit_Data_Conversation_To_Hex_Display(DispTPMS_ID,DispTyreTemp+3,6);
		WriteColor_StringToAddr(0x90,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);	
	
		//透过未用部分
		WriteColor_StringToAddr(0x90+9,BG_COLOR_BLACK|CH_COLOR_BLACK,ClearFontOsd,2,0);	
		
		//胎压6
		DispTPMS_ID=TPMS_ID[5][0];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[5][1];
		DispTPMS_ID=DispTPMS_ID<<8;
		DispTPMS_ID+=TPMS_ID[5][2];					
		Digit_Data_Conversation_To_Hex_Display(DispTPMS_ID,DispTyreTemp+3,6);		
		WriteColor_StringToAddr(0x90+11,BG_COLOR_RED|CH_COLOR_BLACK,DispTyreTemp,9,0);	

		Temp=0;
		for(i=0;i<TYRE_COUNT;i++)
		{
			if((TyreState[i]!=TyreStateold[i])&&(TyreState[i]==TPMS_SUCCEED))
				Temp=0xff;
			TyreStateold[i]=TyreState[i];
		}
		if(Temp==0xff)
			Eeprom_Write_Fact(TPMSIDSAVEStateAddr, (uchar*)&TPMS_ID, 18);
	}	
	if(Display_ThirdMenu_TyrePress_Flag==1)
	{	//胎压
		if(TyrePressMenuItem==2)
		{
			FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
			Display_ThirdMenu_TyrePress_Flag=0;
			OsdCmd(OsdOpen);	
			SOsdHwBuffWrite2Hw();
		}
		else if(TyrePressMenuItem==1)
		{
			if(TyrePressUpDataCount==3)
			{
				FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
				FontOsdWinDisplay(FONT_OSD_WIN_4,ON);
				Display_ThirdMenu_TyrePress_Flag=0;
				OsdCmd(OsdOpen);	
				SOsdHwBuffWrite2Hw();		
			}
		}
		else 
		{
			FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
			FontOsdWinDisplay(FONT_OSD_WIN_2,ON);
			FontOsdWinDisplay(FONT_OSD_WIN_3,ON);
			FontOsdWinDisplay(FONT_OSD_WIN_4,ON);	
			Display_ThirdMenu_TyrePress_Flag=0;
			OsdCmd(OsdOpen);	
			SOsdHwBuffWrite2Hw();					
		}
	}
	if(TyrePressMenuItem==1)
	{
		TyrePressUpDataCount++;
		if(TyrePressUpDataCount>3)
			TyrePressUpDataCount=0;
	}
}

/***************************************************************************
一、程序类型（底层驱动、应用层、初始化）：应用层
二、功能描述：
1、
三、形参数：无
四、返回值：无
五、更改记录（如果是底层驱动、初始化类型程序时）：
1、
六、引用操作方式：
1、
***************************************************************************/
void	Display_ThirdMenu_HelpInfomation(void)
{
	unsigned char YiBiaoBanBen[8],VersionTemp[8];
	
	if(Display_ThirdMenu_HelpInfomation_Flag==1)
	{
		OsdCmd(OsdClose);
		SpiOsdWinAllOff();
		FontOsdWinDisplay(FONT_OSD_ALL_WIN,OFF);
	}

	if(VersionQuery_Flag==0)
	{
		if(Display_ThirdMenu_HelpInfomation_Flag==1||HelpInfomationTurnPage_Flag==1)
		{
			///font osd
			FontOsd_Interface_CharacterHeight_Pixels(COMMON_CHARACTER_HEIGHT,FONT_OSD_PIXELS_16);
			FontOsdAplha(FONT_OSD_WIN_1,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_1,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_1,2);
			FontOsdWinScreen(FONT_OSD_WIN_1, 102,180, 38, 1);

			FontOsdAplha(FONT_OSD_WIN_2,CH_COLOR_BLACK,ALPHA_TRANS_VALUE);
			FontOsdWinZoom(FONT_OSD_WIN_2,0,0);
			FontOsdSetOsdRamAddrAttr(FONT_OSD_WIN_2,42);					
			FontOsdWinScreen(FONT_OSD_WIN_2, 102,320, 38, 1);
			//版本查询背景
			Display_Layer_Graphics(&img_VersionQueryMenuBackGround,&header_table,&VersionQueryMenuBackGround_Info,
																&VersionQueryMenuBackGround_Header,SPI_OSD_WIN_2);
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);
		}
		///font osd 
		//前驱
#ifdef	FRONTMODULE_CONNECT			
		ASC_Data_Conversation_To_Real_Display(QianQuBanBen,VersionTemp,sizeof(QianQuBanBen));			
		WriteColor_StringToAddr(0x02,BG_COLOR_RED|CH_COLOR_BLACK,VersionTemp,sizeof(VersionTemp),0);
#else
		WriteColor_StringToAddr(0x02,BG_COLOR_RED|CH_COLOR_BLACK,ClearFontOsd,8,0);
#endif
		WriteColor_StringToAddr(10,BG_COLOR_RED|CH_COLOR_BLACK,ClearFontOsd,7,0);
		//后驱
#ifdef	BACKMODULE_CONNECT			
		ASC_Data_Conversation_To_Real_Display(HouQuBanBen,VersionTemp,sizeof(HouQuBanBen));				
		WriteColor_StringToAddr(17,BG_COLOR_RED|CH_COLOR_BLACK,VersionTemp,sizeof(VersionTemp),0);
#else
		WriteColor_StringToAddr(17,BG_COLOR_RED|CH_COLOR_BLACK,ClearFontOsd,8,0);
#endif			
		WriteColor_StringToAddr(25,BG_COLOR_RED|CH_COLOR_BLACK,ClearFontOsd,7,0);
		//顶驱
#ifdef	TOPMODULE_CONNECT			
		ASC_Data_Conversation_To_Real_Display(DingQuBanBen,VersionTemp,sizeof(DingQuBanBen));				
		WriteColor_StringToAddr(32,BG_COLOR_RED|CH_COLOR_BLACK,VersionTemp,sizeof(VersionTemp),0);		
#else
		WriteColor_StringToAddr(32,BG_COLOR_RED|CH_COLOR_BLACK,ClearFontOsd,8,0);
#endif
		///仪表
		YiBiaoBanBen[0]='B';YiBiaoBanBen[1]='D';YiBiaoBanBen[2]='S';YiBiaoBanBen[3]='L';
		YiBiaoBanBen[4]='J';YiBiaoBanBen[5]='R';YiBiaoBanBen[6]='0';YiBiaoBanBen[7]='3';
//		YiBiaoBanBen[0]='Z';YiBiaoBanBen[1]='B';YiBiaoBanBen[2]='2';YiBiaoBanBen[3]='0';
//		YiBiaoBanBen[4]='9';YiBiaoBanBen[5]='D';YiBiaoBanBen[6]='T';YiBiaoBanBen[7]='T';		
		ASC_Data_Conversation_To_Real_Display(YiBiaoBanBen,VersionTemp,sizeof(YiBiaoBanBen));
		WriteColor_StringToAddr(42,BG_COLOR_RED|CH_COLOR_BLACK,VersionTemp,sizeof(VersionTemp),0);	
		WriteColor_StringToAddr(50,BG_COLOR_RED|CH_COLOR_BLACK,ClearFontOsd,7,0);
		//配电
#ifdef POWERMODULE_CONNECT			
		ASC_Data_Conversation_To_Real_Display(PeiDianBanBen,VersionTemp,sizeof(PeiDianBanBen));				
		WriteColor_StringToAddr(57,BG_COLOR_RED|CH_COLOR_BLACK,VersionTemp,sizeof(VersionTemp),0);
#else
		WriteColor_StringToAddr(57,BG_COLOR_RED|CH_COLOR_BLACK,ClearFontOsd,8,0);
#endif
		WriteColor_StringToAddr(65,BG_COLOR_RED|CH_COLOR_BLACK,ClearFontOsd,7,0);
		//电源管理模块
#ifdef POWERCONTRLMODULE_CONNECT			
		ASC_Data_Conversation_To_Real_Display(PowerContrlBanBen,VersionTemp,sizeof(PowerContrlBanBen));				
		WriteColor_StringToAddr(72,BG_COLOR_RED|CH_COLOR_BLACK,VersionTemp,sizeof(VersionTemp),0);
#else
		WriteColor_StringToAddr(72,BG_COLOR_RED|CH_COLOR_BLACK,ClearFontOsd,8,0);
#endif		
		if(Display_ThirdMenu_HelpInfomation_Flag==1)
		{
			FontOsdWinDisplay(FONT_OSD_WIN_1,ON);
			FontOsdWinDisplay(FONT_OSD_WIN_2,ON);	
		}
	}
	else
	{					
		if(HelpInfomationTurnPage_Flag==1||Display_ThirdMenu_HelpInfomation_Flag==1)
		{
			HelpInfomationTurnPage_Flag=0;
			//帮助信息背景层
			Display_Layer_Graphics(&img_HelpInfomationMenuBackGround,&header_table,&HelpInfomationMenuBackGround_Info,
																&HelpInfomationMenuBackGround_Header,SPI_OSD_WIN_2);	
			SpiOsdWinPixelAlpha( SPI_OSD_WIN_2, OFF);
		}
	}
	
	if(Display_ThirdMenu_HelpInfomation_Flag==1)
	{
		Display_ThirdMenu_HelpInfomation_Flag=0;
		OsdCmd(OsdOpen);	
		SOsdHwBuffWrite2Hw();
	}
}
#pragma CODE_SEG DEFAULT



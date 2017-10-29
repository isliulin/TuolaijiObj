#undef	EXT
#ifdef	TftDisplay_C
#define	EXT
#else
#define	EXT extern
#endif
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz512.h>
/*******************parameter macros**********************/
///������ʱ����
#define	EmptyDetectNum							1//5
#define	EngineStartDelayNum					977
#define	EngineStartRotationNum			550
#define	NormalDetectNum							2
#define	SaftySwitchDetectNum				5
///����״̬
#define	EngineRestartStateNum				1
#define	EngineStartTooLongNum				2
#define	EngineStartNShieldNum				3
#define	EngineStartProcessNum				4
#define	EngineStartSaftyNum						5
#define	EngineIdleNum									0xff

#define	ONDisplayDelayNum							200///����ʾǰ����ʱ
///������ʾ�Ĳμ�
#define	DIGIT_DATA_NO_DOT							0xff	///����Ҫ��"."������
#define	DIGIT_DATA_0_DOT							0			///������ǰ���"0"��"."
#define	_16_26_DIGIT_DOT_POS					0x17	///"."���ű�������Ϊ23

#define	ThirdMenu_PageDispNum				5///��ҳ���������
///�������
#define	DIAGNOSE_BEI_YONG		0///����
#define	DIAGNOSE_LOAD				1///����
#define	DIAGNOSE_SWITCH			2///����
#define	DIAGNOSE_SENSOR			3///������
#define	DIAGNOSE_CATCH			4///�����ź�
#define	DIAGNOSE_ENABLE_UNABLE	5///�Ƿ�ʹ��
///�˿�ʹ�����
#define	STATE_UNABLE				0
#define	STATE_ENABLE				1
///���ģ��
#define	DIAGNOSE_NO_MODULE			0
#define	DIAGNOSE_PANEL					1
#define	DIAGNOSE_FRONT_DRIVE		2
#define	DIAGNOSE_BACK_DRIVE			3
#define	DIAGNOSE_TOP_DRIVE			4
#define	DIAGNOSE_POWER_DRIVE		5
#define	DIAGNOSE_POWER_MANAGE		6


/******************funtion macros************************/
#pragma CODE_SEG  USER_ROM
void	Tft_Display(void);
static 	void RShieldSignal_MiddleDoorOpen_VideoDisplay(void);
static 	void LcdOffDeal(void);
static	void First_dis_control(void);
static	void mainmenu_back_delay(void);
static	void Screen_display(void);
static 	void st_process(void);
static 	void motor_st(void);
static 	void MainMenuSpecialStateDetect(void);
//////�¶���ĺ���
void Display_NoVideoMenu(void);
void  Display_LogoMenu(void);
void 	Display_MainMenu(void);
void	Display_SubMenu(void);
void	Display_ThirdMenu_Malfunction(void);
void	Display_ThirdMenu_DiagnoseQuery(void);
void	Digit_Data_Conversation_To_Real_Display
(ulong Digit_Data,uchar *Digit_Data_Real_Disp,
 uchar Digit_Data_Real_Disp_Number,uchar Dot_Positon_Disp
);
static void	Diagnose_State
(unsigned char Diagnose_Module,unsigned char Diagnose_Type,unsigned int	Diagnose_Content,
 unsigned char Diagnose_Content_Position);
static void Diagnose_Panal_LeftMenu(void);
static void Diagnose_Panal_RightMenu(void);
static void Diagnose_FrontDriver_LeftMenu(void);
static void Diagnose_FrontDriver_RightMenu(void);
static void Diagnose_BackDriver_LeftMenu(void);
static void Diagnose_BackDriver_RightMenu(void);
static void Diagnose_TopDriver_LeftMenu(void);
static void Diagnose_TopDriver_RightMenu(void);
static void Diagnose_Power_LeftMenu(void);
static void Diagnose_Power_RightMenu(void);
static void Diagnose_PowerManage_LeftMenu(void);
static void Diagnose_PowerManage_RightMenu(void);
static void Diagnose_Module_Clear(void);
static void	MainMenu_LedIndicate_Disp(void);
static void	MainMenu_SmallModuleIndicate_Disp(void);
static void Diagnose_BMS_DMC_LeftMenu(void);
static void Diagnose_BMS_DMC_RightMenu(void);
static void	MainMenu_Malfunction_Disp(void);
static void	MainMenu_OilPress_Disp(void);
static void	MainMenu_SystemVoltage_Disp(void);
static void	MainMenu_VehicleModule_Disp(void);
static void Diagnose_ModuleTemperature(void);
void	ASC_Data_Conversation_To_Real_Display(uchar *ASC_Data,uchar *Real_Disp,uchar count);
void	Digit_Data_Conversation_To_Hex_Display(ulong Digit_Data,uchar *Hex_Disp,uchar count);
void	Display_ThirdMenu_SystemParameter(void);
void	Display_ThirdMenu_DistanceFualConsume(void);
void	Display_ThirdMenu_PictureAndChannleSet(void);
void	Display_ThirdMenu_SystemTimeSet(void);
void	Display_ThirdMenu_TyrePress(void);
void	Display_ThirdMenu_HelpInfomation(void);
static void	MainMenu_SOC_Disp(void);
#pragma CODE_SEG DEFAULT
/*******************const macros**************************/

/*******************ram macros*****************************/
EXT unsigned char	SubMenu_Cursor,ThirdMenu_Cursor,FourMenu_Cursor;///�˵���ѯ��ָ��
EXT unsigned char	YiBiaoWenDu[2],QianQuWenDu[2],HouQuWenDu[2],DingQuWenDu[2],PeiDianWenDu[2],PowerContrlWenDu[2];///ģ���¶�
EXT unsigned char	QianQuBanBen[8],HouQuBanBen[8],DingQuBanBen[8],PeiDianBanBen[8],PowerContrlBanBen[8];///ģ��汾
EXT	unsigned char	RShieldVideoChannel,MiddleDoorVideoChannel,UpVideoChannel;///��Ƶͨ��
EXT	unsigned char	FrontDoorVideoChannel,TopVideoChannel,VideoChannelTemp;
EXT unsigned char	st_index,st_status,St_Count_Empty_Shield,St_Count_Normal,count_st,St_Count_Safty;
EXT unsigned char	st_statusOld;
EXT unsigned int	delay15s;	
EXT	unsigned char	MiddleDoorVideoSignalNum,RShieldVideoSignalNum,UpVideoSignalNum;
EXT	unsigned char	FrontDoorVideoSignalNum,TopVideoSignalNum;
EXT unsigned char	DelayForDispNum;///��OFF��ON��������ʾ����
EXT	uint	MiddleDoorOpenSignalDelayNum;

///zb209�¶���
EXT unsigned char MainMenuDispIndex;///������ֲ�������

//GAO �¶���

EXT unsigned char 	PictureContrastNum;
EXT unsigned char 	PictureBrightNum;
EXT unsigned char 	PictureChromaNum;
EXT unsigned char 	PictureDefinitionNum;

////////////////////////////
//ʱ����������

//ϵͳ����
EXT unsigned char 	HelpInfomationPageNum;
//�ͺĲ�ѯ
EXT unsigned char SmallDistanceClearFlash1s;
//̥ѹ��
EXT unsigned char TyrePressMenuItem;
EXT unsigned char TyrePressPassWord[4];
EXT unsigned char TyrePressPassWordNum;
EXT unsigned char	TyrePressUpDataCount;
//̥ѹ��ѯ
EXT unsigned char TyrePressMenuStatus;
EXT unsigned char TyrePressMenuLocate;
EXT unsigned char TyrePressMenuTimeOut;
//��������
EXT unsigned char ThirdMenu_SystemParameter_Cursor;

EXT unsigned char VehicleModuleIndicateNum;
EXT unsigned char VehicleModuleIndicateNumOld;

/*********************IO macros****************************/	





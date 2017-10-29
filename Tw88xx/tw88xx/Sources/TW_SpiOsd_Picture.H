#ifndef _TW_SpiOsd_Picture_H
#define _TW_SpiOsd_Picture_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* common defines and macros */
#include <hidef.h>      	
#include <mc9s12xhz256.h>
#include	"typedef.h"
#define MeterPictureStartFlahAddress	0x000000
/*
const SLIDEIMAGE MeterPictureAllFlahAddress[] = { 
	{MeterPictureStartFlahAddress+ 0x000000, 0x0A8E74 },		// 地址1-主界面效果 
	{MeterPictureStartFlahAddress+ 0x0A8E74, 0x0402D3 },		// 地址2-SubMenu 
	{MeterPictureStartFlahAddress+ 0x0E9147, 0x00F728 },		// 地址3-故障报警查询 
	{MeterPictureStartFlahAddress+ 0x0F886F, 0x058ACA },    // 地址4-模块选择
	{MeterPictureStartFlahAddress+ 0x151339, 0x011626 },    // 地址5-参数设置背景
	{MeterPictureStartFlahAddress+ 0x16295F, 0x00B900 },    // 地址6-图像设置选项 
	{MeterPictureStartFlahAddress+ 0x16E25F, 0x017070 },    // 地址7-通道设置背景 
	{MeterPictureStartFlahAddress+ 0x1852CF, 0x00B900 },    // 地址8-通道设置选项 
	{MeterPictureStartFlahAddress+ 0x190BCF, 0x00A5D0 },    // 地址9-系统时间设置背景 
	{MeterPictureStartFlahAddress+ 0x19B19F, 0x011F60 },    // 地址10-系统时间设置选项 
	{MeterPictureStartFlahAddress+ 0x1AD0FF, 0x018ACC },    // 地址11-系统帮助
	{MeterPictureStartFlahAddress+ 0x1C5BCB, 0x0105D7 },    // 地址12-版本信息背景 
	{MeterPictureStartFlahAddress+ 0x1D61A2, 0x021F4B },    // 地址13-胎压查询背景 
	{MeterPictureStartFlahAddress+ 0x1F80ED, 0x010C49 },    // 地址14-胎压监测背景 
	{MeterPictureStartFlahAddress+ 0x208D36, 0x00CAFB },    // 地址15-胎压学习密码背景
	{MeterPictureStartFlahAddress+ 0x215831, 0x003AC0 },    // 地址16-密码错误 
	{MeterPictureStartFlahAddress+ 0x2192F1, 0x02314E },    // 地址17-胎压学习背景   
	{MeterPictureStartFlahAddress+ 0x23C43F, 0x0222D8 },    // 地址18-胎压设置选项 
	{MeterPictureStartFlahAddress+ 0x25E717, 0x00EA4C },    // 地址19-里程油耗背景
	{MeterPictureStartFlahAddress+ 0x26D163, 0x0023A0 },    // 地址20-清除 
	{MeterPictureStartFlahAddress+ 0x26F503, 0x01474D },    // 地址21-系统参数设置背景 
	{MeterPictureStartFlahAddress+ 0x283C50, 0x015ED0 },    // 地址22-系统参数设置选项
	{MeterPictureStartFlahAddress+ 0x299B20, 0x029D5C },    // 地址23-视频无信号   
 }; 
*/
//定义各图片开始地址
#define	MeterPictureStartFlahAddress_1			0x000000
#define	MeterPictureStartFlahAddress_2			0x0A8E74
#define	MeterPictureStartFlahAddress_3			0x0CA294
#define	MeterPictureStartFlahAddress_4			0x0DA070
#define	MeterPictureStartFlahAddress_5			0x135A95
#define	MeterPictureStartFlahAddress_6			0x146AEB
#define	MeterPictureStartFlahAddress_7			0x1519B3
#define	MeterPictureStartFlahAddress_8			0x16715E
#define	MeterPictureStartFlahAddress_9			0x172026
#define	MeterPictureStartFlahAddress_10			0x17CC29
#define	MeterPictureStartFlahAddress_11			0x18F70F
#define	MeterPictureStartFlahAddress_12			0x1AAAF0
#define	MeterPictureStartFlahAddress_13			0x1B9E2B
#define	MeterPictureStartFlahAddress_14			0x1DB554
#define	MeterPictureStartFlahAddress_15			0x1EC0DF
#define	MeterPictureStartFlahAddress_16			0x1F87F9
#define	MeterPictureStartFlahAddress_17			0x1FDA30
#define	MeterPictureStartFlahAddress_18			0x21ED15
#define	MeterPictureStartFlahAddress_19			0x240FED
#define	MeterPictureStartFlahAddress_20			0x2521E9
#define	MeterPictureStartFlahAddress_21			0x254BC9
#define	MeterPictureStartFlahAddress_22			0x26976D
#define	MeterPictureStartFlahAddress_23			0x2813C5

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define		offset_NoChangeFlahAddress			MeterPictureStartFlahAddress_1///0x00000
///一级菜单,电池电压
const SLIDEIMAGE img_SystemVoltageIndicate[]=
{
	{offset_NoChangeFlahAddress+0x000000,0x002D50 },
	{offset_NoChangeFlahAddress+0x002D50,0x002D50 },
	{offset_NoChangeFlahAddress+0x005AA0,0x002D50 },
	{offset_NoChangeFlahAddress+0x0087F0,0x002D50 },
	{offset_NoChangeFlahAddress+0x00B540,0x002D50 },
	{offset_NoChangeFlahAddress+0x00E290,0x002D50 },
	{offset_NoChangeFlahAddress+0x010FE0,0x002D50 },
	{offset_NoChangeFlahAddress+0x013D30,0x002D50 },
	{offset_NoChangeFlahAddress+0x016A80,0x002D50 },
	{offset_NoChangeFlahAddress+0x0197D0,0x002D50 },		
};
const struct menu_item_info MainMenu_SystemVoltageIndicate_Info={720,190,0x100};
const struct RLE2_HEADER MainMenu_SystemVoltageIndicate_Header = {  0x88, 0x00, 0x003C, 0x00B0, 0x00002940, 0x01, 0xFF }; 
///一级菜单,机油压力
const SLIDEIMAGE img_FualIndicate[]	=
{
	{offset_NoChangeFlahAddress+0x01C520,0x002D50},//报警 0
	{offset_NoChangeFlahAddress+0x01C520,0x002D50},//报警 0.1
	{offset_NoChangeFlahAddress+0x01F270,0x002D50},//正常 0.1
	{offset_NoChangeFlahAddress+0x021FC0,0x002D50},
	{offset_NoChangeFlahAddress+0x024D10,0x002D50},
	{offset_NoChangeFlahAddress+0x027A60,0x002D50},
	{offset_NoChangeFlahAddress+0x02A7B0,0x002D50},
	{offset_NoChangeFlahAddress+0x02D500,0x002D50},
	{offset_NoChangeFlahAddress+0x030250,0x002D50},//8-0.7
	{offset_NoChangeFlahAddress+0x032FA0,0x002D50},//9-机油报警7-1 
	{offset_NoChangeFlahAddress+0x035CF0,0x002D50},//0.8
	{offset_NoChangeFlahAddress+0x038A40,0x002D50},
	{offset_NoChangeFlahAddress+0x03B790,0x002D50},		
};
const struct menu_item_info MainMenu_FualIndicate_Info={15,190,0x100};
const struct RLE2_HEADER MainMenu_FualIndicate_Header = {  0x88, 0x00, 0x003C, 0x00B0, 0x00002940, 0x01, 0xFF }; 

///一级菜单,SOC
const SLIDEIMAGE img_SOCIndicate[]	=
{		
  { offset_NoChangeFlahAddress+0x01C520, 0x002D8C },    // SOC1 
  { offset_NoChangeFlahAddress+0x01F2AC, 0x002D8C },    // SOC2 
  { offset_NoChangeFlahAddress+0x022038, 0x002D8C },    // SOC3 
  { offset_NoChangeFlahAddress+0x024DC4, 0x002D8C },    // SOC4 
  { offset_NoChangeFlahAddress+0x027B50, 0x002D8C },    // SOC5 
  { offset_NoChangeFlahAddress+0x02A8DC, 0x002D8C },    // SOC6 
  { offset_NoChangeFlahAddress+0x02D668, 0x002D8C },    // SOC7 
  { offset_NoChangeFlahAddress+0x0303F4, 0x002D8C },    // SOC8 
  { offset_NoChangeFlahAddress+0x033180, 0x002D8C },    // SOC9 		
};
const struct menu_item_info MainMenu_SOCIndicate_Info={15,190,0x100};
const struct RLE2_HEADER MainMenu_SOCIndicate_Header = { 0x88, 0x00, 0x003C, 0x00B1, 0x0000297C, 0x01, 0xFF }; 

///一级菜单,车模
///前门中门关
const SLIDEIMAGE img_VehicleModuleIndicate_AllClose	= {offset_NoChangeFlahAddress+0x03E4E0, 0x011481 };    // 前门关后门关
const struct menu_item_info MainMenu_VehicleModuleIndicate_AllClose_Info={160,130,0x100};
const struct RLE2_HEADER MainMenu_VehicleModuleIndicate_AllClose_Header = {0x88, 0x00, 0x01E1, 0x0091, 0x00011071, 0x01, 0xFF }; 
///前门关中门开
const SLIDEIMAGE img_VehicleModuleIndicate_MiddleOpen	= {offset_NoChangeFlahAddress+0x04F961, 0x011481 };    // 前门关后门开 
const struct menu_item_info MainMenu_VehicleModuleIndicate_MiddleOpen_Info={160,130,0x100};
const struct RLE2_HEADER MainMenu_VehicleModuleIndicate_MiddleOpen_Header = {0x88, 0x00, 0x01E1, 0x0091, 0x00011071, 0x01, 0xFF }; 
///前门开中门关
const SLIDEIMAGE img_VehicleModuleIndicate_FrontOpen	= {offset_NoChangeFlahAddress+0x060DE2, 0x011481 };    // 前门开后门关 
const struct menu_item_info MainMenu_VehicleModuleIndicate_FrontOpen_Info={160,130,0x100};
const struct RLE2_HEADER MainMenu_VehicleModuleIndicate_FrontOpen_Header = {0x88, 0x00, 0x01E1, 0x0091, 0x00011071, 0x01, 0xFF }; 
///前门中门开
const SLIDEIMAGE img_VehicleModuleIndicate_AllOpen	= {offset_NoChangeFlahAddress+0x072263, 0x011481 };    // 前门开后门开 
const struct menu_item_info MainMenu_VehicleModuleIndicate_AllOpen_Info={160,130,0x100};
const struct RLE2_HEADER MainMenu_VehicleModuleIndicate_AllOpen_Header = {0x88, 0x00, 0x01E1, 0x0091, 0x00011071, 0x01, 0xFF }; 
///一级菜单,里程、油耗、尿素、日期时间
const SLIDEIMAGE img_SmallModuleIndicate	= {offset_NoChangeFlahAddress+0x0836E4, 0x025790 };    // 首界面 
const struct menu_item_info MainMenu_SmallModuleIndicate_Info={0,280,0x100};
const struct RLE2_HEADER MainMenu_SmallModuleIndicate_Header = { 0x88, 0x00, 0x031A, 0x00C0, 0x00025380, 0x01, 0xFF }; 
///一级菜单故障显示
const struct menu_item_info		MainMenu_GuZhanChaXun_Disp_Address[2]=
{
	{190,286,0x100},
	{190,336,0x100}	
};
//二级菜单
#define		SubMenuFlashAddress			MeterPictureStartFlahAddress_2		//0x100000
const SLIDEIMAGE img_SubMenuBackGround	= {SubMenuFlashAddress+0x000000,0x021420};
const struct menu_item_info SubMenu_BackGround_Info={0,0,0x000};
const struct RLE2_HEADER SubMenu_BackGround_Header = { 0x88, 0x84, 0x0320, 0x01E0, 0x00021010, 0x01, 0xFF};  
///三级故障查询界面
#define		ThirdMenu_GuZhanChaXun_FlashAddress			MeterPictureStartFlahAddress_3//0x150000
const SLIDEIMAGE img_ThirdMenu_GuZhanChaXun	= {ThirdMenu_GuZhanChaXun_FlashAddress+0x000000,0x00FDDC};
const struct menu_item_info ThirdMenu_GuZhanChaXun_Item_Info={0,0,0x000};
const struct RLE2_HEADER ThirdMenu_GuZhanChaXun_Header = { 0x88, 0x86, 0x0320, 0x01E0, 0x0000F9CC, 0x01, 0xFF };    // 故障报警查询 
///三级系统诊断界面
#define	ThirdMenu_XiTongZhenDuan_FlashAddress		MeterPictureStartFlahAddress_4//0x200000

const SLIDEIMAGE img_ThirdMenu_XiTongZhenDuan_MoKuaiXuanZhe[ ]	=
{
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x000000, 0x00B561 },    // 仪表 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x00B561, 0x00B263 },    // 前控 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x0167C4, 0x00B238 },    // 后控 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x0219FC, 0x00B5D4 },    // 顶控 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x02CFD0, 0x00B914 },    // 配电 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x0388E4, 0x00BC42 },    // 电源管理模块 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x044526, 0x00BBBC },    // 模块温度   
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x0500E2, 0x00B943 },    // 底盘动力 
};
const struct menu_item_info ThirdMenu_XiTongZhenDuan_MoKuaiXuanZhe_Item_Info={0,0,0x000};
const struct RLE2_HEADER ThirdMenu_XiTongZhenDuan_MoKuaiXuanZhe_Header[ ] = 
{
	  { 0x88, 0x87, 0x0320, 0x01E0, 0x0000B151, 0x01, 0xFF },     // 仪表 
    { 0x88, 0x87, 0x0320, 0x01E0, 0x0000AE53, 0x01, 0xFF },     // 前控 
    { 0x88, 0x87, 0x0320, 0x01E0, 0x0000AE28, 0x01, 0xFF },     // 后控 
    { 0x88, 0x87, 0x0320, 0x01E0, 0x0000B1C4, 0x01, 0xFF },     // 顶控 
    { 0x88, 0x87, 0x0320, 0x01E0, 0x0000B504, 0x01, 0xFF },     // 配电 
    { 0x88, 0x87, 0x0320, 0x01E0, 0x0000B832, 0x01, 0xFF },     // 电源管理模块 
    { 0x88, 0x87, 0x0320, 0x01E0, 0x0000B7AC, 0x01, 0xFF },     // 模块温度 
    { 0x88, 0x87, 0x0320, 0x01E0, 0x0000B533, 0x01, 0xFF },     // 发动机 
};
///8位图片
///图像画面及通道设置
#define		TestThirdMenuPictureFlashAddress			MeterPictureStartFlahAddress_5//0x400000
//画面设置底层图
const SLIDEIMAGE img_ThirdMenu_Picture	= {TestThirdMenuPictureFlashAddress+0x000000, 0x011056  };
const struct menu_item_info ThirdMenu_Picture_Item_Info={0,0,0x000};
const struct RLE2_HEADER ThirdMenu_Picture_Header = {0x88, 0x86, 0x0320, 0x01E0, 0x00010C46, 0x01, 0xFF }; 
//画面设置参数选项
#define		TestThirdMenuPictureItemFlashAddress			MeterPictureStartFlahAddress_6//0x413000
const struct RLE2_HEADER ThirdMenu_PictureItem_Header = { 0x88, 0x00, 0x00B2, 0x0039, 0x000027A2, 0x01, 0xFF}; 
//对比度选中
const SLIDEIMAGE img_ThirdMenu_PictureContrastSel	= {TestThirdMenuPictureItemFlashAddress+0x000000, 0x002BB2 };
const struct menu_item_info ThirdMenu_PictureContrastSel_Item_Info={69,87,0x100};
//亮度选中
const SLIDEIMAGE img_ThirdMenu_PictureBrightSel	= {TestThirdMenuPictureItemFlashAddress+0x002BB2, 0x002BB2 };
const struct menu_item_info ThirdMenu_PictureBrightSel_Item_Info={69,169,0x100};
//色度选中
const SLIDEIMAGE img_ThirdMenu_PictureChromaSel	= {TestThirdMenuPictureItemFlashAddress+0x005764, 0x002BB2 };
const struct menu_item_info ThirdMenu_PictureChromaSel_Item_Info={69,251,0x100};
//清晰度选中
const SLIDEIMAGE img_ThirdMenu_PictureDefinitionSel	= {TestThirdMenuPictureItemFlashAddress+0x008316, 0x002BB2 };
const struct menu_item_info ThirdMenu_PictureDefinitionSel_Item_Info={69,334,0x100};

#define		TestThirdMenuChannleFlashAddress			MeterPictureStartFlahAddress_7//0x420000
//通道设置底层图
const SLIDEIMAGE img_ThirdMenu_Channle	= {TestThirdMenuChannleFlashAddress+0x000000, 0x0157AB };
const struct menu_item_info ThirdMenu_Channle_Item_Info={0,0,0x000};
const struct RLE2_HEADER ThirdMenu_Channle_Header = {0x88, 0x86, 0x0320, 0x01E0, 0x0001539B, 0x01, 0xFF}; 
//通道设置选项
#define		TestThirdMenuChannleItemFlashAddress			MeterPictureStartFlahAddress_8//0x460000
const struct RLE2_HEADER ThirdMenu_ChannleItem_Header = {0x88, 0x00, 0x00B4, 0x003C, 0x00002A30, 0x01, 0xFF}; 
//前门//选中
const struct menu_item_info ThirdMenu_ChannleFrontDoor_Item_Info={41,86,0x100};
const SLIDEIMAGE img_ThirdMenu_ChannleFrontDoorSel	= {TestThirdMenuChannleItemFlashAddress+0x000000, 0x002BB2 };
//中门//选中
const struct menu_item_info ThirdMenu_ChannleMiddleDoor_Item_Info={41,169,0x100};
const SLIDEIMAGE img_ThirdMenu_ChannleMiddleDoorSel	= {TestThirdMenuChannleItemFlashAddress+0x002BB2, 0x002BB2 };
//倒车//选中
const struct menu_item_info ThirdMenu_ChannleBackCar_Item_Info={41,251,0x100};
const SLIDEIMAGE img_ThirdMenu_ChannleBackCarSel	= {TestThirdMenuChannleItemFlashAddress+0x005764, 0x002BB2  };
//顶部//选中
const struct menu_item_info ThirdMenu_ChannleTOP_Item_Info={41,333,0x100};
const SLIDEIMAGE img_ThirdMenu_ChannleTOPSel	= {TestThirdMenuChannleItemFlashAddress+0x008316, 0x002BB2  };

//系统时间设置
#define		SystemTimeMenuFlashAddress			MeterPictureStartFlahAddress_9//0x470000
const SLIDEIMAGE img_SystemTimeMenuBackGround	= {SystemTimeMenuFlashAddress+0x000000, 0x00AC03};
const struct menu_item_info SystemTimeMenu_BackGround_Info={0,0,0x000};
const struct RLE2_HEADER SystemTimeMenu_BackGround_Header = {0x88, 0x87, 0x0320, 0x01E0, 0x0000A7F3, 0x01, 0xFF}; 

#define		SystemTimeDataItemFlashAddress	MeterPictureStartFlahAddress_10//0x480000
const struct RLE2_HEADER SystemTimeDataItem_Header = {  0x88, 0x00, 0x01C0, 0x003F, 0x00006E40, 0x01, 0xFF};  
//时间设置
const struct menu_item_info SystemTimeItemSel_Info={180,264,0x100};
const SLIDEIMAGE img_SystemTimeItemSel	= {SystemTimeDataItemFlashAddress+0x007250, 0x007250};
//日期设置
const struct menu_item_info SystemDataItemSel_Info={180,160,0x100};
const SLIDEIMAGE img_SystemDataItemSel	= {SystemTimeDataItemFlashAddress+0x000000, 0x007250  };
//时间设置错误
const struct menu_item_info SystemDataTimeErr_Info={180,360,0x100};
const SLIDEIMAGE img_SystemDataTimeErr	= {SystemTimeDataItemFlashAddress+0x00E4A0, 0x004646  };
const struct RLE2_HEADER SystemDataTimeErr_Header = { 0x88, 0x00, 0x0153, 0x0032, 0x00004236, 0x01, 0xFF  };  

/////////////////////////////////////////////////////
/**********************系统帮助***************************/
#define		HelpInfomationMenuBackGroundFlashAddress	MeterPictureStartFlahAddress_11//0x4A0000
const SLIDEIMAGE img_HelpInfomationMenuBackGround	= {HelpInfomationMenuBackGroundFlashAddress+0x000000, 0x01B3E1  };
const struct RLE2_HEADER HelpInfomationMenuBackGround_Header = { 0x88, 0x85, 0x0320, 0x01E0, 0x0001AFD1, 0x01, 0xFF};  
const struct menu_item_info HelpInfomationMenuBackGround_Info= {0,0,0x000};
/*
//系统说明页
#define		HelpInfomationMenuItemFlashAddress	0x4B0000
const SLIDEIMAGE img_HelpInfomationMenu_Page1	= {HelpInfomationMenuItemFlashAddress+0x000000, 0x041910 };
const struct RLE2_HEADER HelpInfomationMenu_Page1_Header = {0x88, 0x00, 0x02F8, 0x0160, 0x00041500, 0x01, 0xFF};  
const struct menu_item_info HelpInfomationMenu_Page1_Info= {32,20,0x100};

const SLIDEIMAGE img_HelpInfomationMenu_Page2	= {HelpInfomationMenuItemFlashAddress+0x000000, 0x041910 };
const struct RLE2_HEADER HelpInfomationMenu_Page2_Header = {0x88, 0x00, 0x02F8, 0x0160, 0x00041500, 0x01, 0xFF };  
const struct menu_item_info HelpInfomationMenu_Page2_Info= {0,30,0x100};*/
//整张图片
#define		VersionQueryMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_12//0x500000	
const SLIDEIMAGE img_VersionQueryMenuBackGround	= {VersionQueryMenuBackGroundFlashAddress+0x000000, 0x00F33B };
const struct RLE2_HEADER VersionQueryMenuBackGround_Header = {0x88, 0x86, 0x0320, 0x01E0, 0x0000EF2B, 0x01, 0xFF };  
const struct menu_item_info VersionQueryMenuBackGround_Info= {0,0,0x000};

//胎压查询背景图
#define		TyrePressMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_13//0x518000	
const SLIDEIMAGE img_TyrePressMenuBackGround	= {TyrePressMenuBackGroundFlashAddress+0x000000, 0x021729 };
const struct RLE2_HEADER TyrePressMenuBackGround_Header = { 0x88, 0x84, 0x0320, 0x01E0, 0x00021319, 0x01, 0xFF};  
const struct menu_item_info TyrePressMenuBackGround_Info= {0,0,0x000};
//胎压监测背景
#define		TyrePressMonitorMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_14//0x53A000	
const SLIDEIMAGE img_TyrePressMonitorMenuBackGround	= {TyrePressMonitorMenuBackGroundFlashAddress+0x000000, 0x010B8B};
const struct RLE2_HEADER TyrePressMonitorMenuBackGround_Header = { 0x88, 0x87, 0x0320, 0x01E0, 0x0001077B, 0x01, 0xFF};  
const struct menu_item_info TyrePressMonitorMenuBackGround_Info= {0,0,0x000};

//胎压学习密码
#define		TyrePressPassWordMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_15//0x550000	
const SLIDEIMAGE img_TyrePressPassWordMenuBackGround	= {TyrePressPassWordMenuBackGroundFlashAddress+0x000000, 0x00C71A   };
const struct RLE2_HEADER TyrePressPassWordMenuBackGround_Header = { 0x88, 0x87, 0x0320, 0x01E0, 0x0000C30A, 0x01, 0xFF };  
const struct menu_item_info TyrePressPassWordMenuBackGround_Info= {0,0,0x000};
//密码输入错误
#define		TyrePressPassWordErrFlashAddress		MeterPictureStartFlahAddress_16//0x560000	
const SLIDEIMAGE img_TyrePressPassWordErr	= {TyrePressPassWordErrFlashAddress+0x000000, 0x005237  };
const struct RLE2_HEADER TyrePressPassWordErr_Header = { 0x88, 0x00, 0x015F, 0x0039, 0x00004E27, 0x01, 0xFF };  
const struct menu_item_info TyrePressPassWordErr_Info= {280,238,0x100};

//胎压学习背景图
#define		TyrePressSetMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_17//0x570000	
const SLIDEIMAGE img_TyrePressSetMenuBackGround	= {TyrePressSetMenuBackGroundFlashAddress+0x000000, 0x02314E };
const struct RLE2_HEADER TyrePressSetMenuBackGround_Header = {0x88, 0x84, 0x0320, 0x01E0, 0x00022D3E, 0x01, 0xFF};  
const struct menu_item_info TyrePressSetMenuBackGround_Info= {0,0,0x000};
//胎压学习选项图
#define		TyrePressSetMenuItemFlashAddress		MeterPictureStartFlahAddress_18//0x5A0000	
const struct RLE2_HEADER TyrePressSetMenuItem_Header = { 0x88, 0x00, 0x009A, 0x0045, 0x00002982, 0x01, 0xFF };
const SLIDEIMAGE img_TyrePressSetMenuItemUpSel	= {TyrePressSetMenuItemFlashAddress+0x0088B6, 0x002D92};
const SLIDEIMAGE img_TyrePressSetMenuItemUpSetting	= {TyrePressSetMenuItemFlashAddress+0x005B24, 0x002D92 };
const SLIDEIMAGE img_TyrePressSetMenuItemUpFual	= {TyrePressSetMenuItemFlashAddress+0x000000, 0x002D92};
const SLIDEIMAGE img_TyrePressSetMenuItemUpsucceed	= {TyrePressSetMenuItemFlashAddress+0x002D92, 0x002D92 };

const SLIDEIMAGE img_TyrePressSetMenuItemDownSel	= {TyrePressSetMenuItemFlashAddress+0x013EFE, 0x002D92 };
const SLIDEIMAGE img_TyrePressSetMenuItemDownSetting	= {TyrePressSetMenuItemFlashAddress+0x01116C, 0x002D92  };
const SLIDEIMAGE img_TyrePressSetMenuItemDownFual	= {TyrePressSetMenuItemFlashAddress+0x00B648, 0x002D92 };
const SLIDEIMAGE img_TyrePressSetMenuItemDownsucceed	= {TyrePressSetMenuItemFlashAddress+0x00E3DA, 0x002D92 };

const SLIDEIMAGE img_TyrePressSetMenuItemMiddleSel	= {TyrePressSetMenuItemFlashAddress+0x01F546, 0x002D92  };
const SLIDEIMAGE img_TyrePressSetMenuItemMiddleSetting	= {TyrePressSetMenuItemFlashAddress+0x01C7B4, 0x002D92 };
const SLIDEIMAGE img_TyrePressSetMenuItemMiddleFual	= {TyrePressSetMenuItemFlashAddress+0x016C90, 0x002D92};
const SLIDEIMAGE img_TyrePressSetMenuItemMiddlesucceed	= {TyrePressSetMenuItemFlashAddress+0x019A22, 0x002D92 };
//胎压图标的位置
const struct menu_item_info TyrePressSetMenuItem1_Info= {238,69 ,0x100};
const struct menu_item_info TyrePressSetMenuItem2_Info= {239,346,0x100};
const struct menu_item_info TyrePressSetMenuItem3_Info= {412,69 ,0x100};
const struct menu_item_info TyrePressSetMenuItem4_Info= {412,154,0x100};
const struct menu_item_info TyrePressSetMenuItem5_Info= {412,264,0x100};
const struct menu_item_info TyrePressSetMenuItem6_Info= {412,346,0x100};
//油耗查询
#define		DistanceFualConsumeMenuFlashAddress		MeterPictureStartFlahAddress_19//0x600000	
const SLIDEIMAGE img_DistanceFualConsumeMenu	= {DistanceFualConsumeMenuFlashAddress+0x000000, 0x0111FC };
const struct RLE2_HEADER DistanceFualConsumeMenu_Header = { 0x88, 0x86, 0x0320, 0x01E0, 0x00010DEC, 0x01, 0xFF  };  
const struct menu_item_info DistanceFualConsumeMenu_Info= {0,0,0x000};
//小计清零图标
#define		SmallDistanceClearMenuFlashAddress		MeterPictureStartFlahAddress_20//0x610000	
const SLIDEIMAGE img_SmallDistanceClearMenu	= {SmallDistanceClearMenuFlashAddress+0x000000, 0x0014F0   };
const SLIDEIMAGE img_SmallDistanceClearSelMenu	= {SmallDistanceClearMenuFlashAddress+0x0014F0, 0x0014F0   };
const struct RLE2_HEADER SmallDistanceClearMenu_Header = { 0x88, 0x00, 0x006C, 0x0028, 0x000010E0, 0x01, 0xFF  };  
const struct menu_item_info SmallDistanceClearMenu_Info= {362,420,0x100};
//参数设置背景
#define		SystemParameterMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_21//0x620000	
const SLIDEIMAGE img_SystemParameterMenuBackGround	= {SystemParameterMenuBackGroundFlashAddress+0x000000, 0x014BA4 };
const struct RLE2_HEADER SystemParameterMenuBackGround_Header = {0x88, 0x86, 0x0320, 0x01E0, 0x00014794, 0x01, 0xFF};  
const struct menu_item_info SystemParameterMenuBackGround_Info= {0,0,0x000};
//参数设置选项
#define		SystemParameterMenuItemFlashAddress		MeterPictureStartFlahAddress_22//0x640000	
const struct RLE2_HEADER SystemParameterMenuItem_Header = {0x88, 0x00, 0x0231, 0x0038, 0x00007AB8, 0x01, 0xFF};  
const SLIDEIMAGE img_SystemParameterMenuItemRainWiper	= {SystemParameterMenuItemFlashAddress+0x000000, 0x007EC8 };
const SLIDEIMAGE img_SystemParameterMenuItemBackLight	= {SystemParameterMenuItemFlashAddress+0x007EC8, 0x007EC8 };
const SLIDEIMAGE img_SystemParameterMenuItemBuzzer		= {SystemParameterMenuItemFlashAddress+0x00FD90, 0x007EC8 };

const struct menu_item_info SystemParameterMenuItemRainWiper_Info= {152,131,0x100};
const struct menu_item_info SystemParameterMenuItemBackLight_Info= {152,208,0x100};
const struct menu_item_info SystemParameterMenuItemBuzzer_Info= {152,284,0x100};

#define		NoVideoMenuItemFlashAddress	 MeterPictureStartFlahAddress_23
const SLIDEIMAGE img_NoVideoMenu[]=
{
	{ NoVideoMenuItemFlashAddress+0x000000, 0x00A805  },    // 前门
  { NoVideoMenuItemFlashAddress+0x00A805, 0x00A7AD  },    // 中门
  { NoVideoMenuItemFlashAddress+0x014FB2, 0x00A793  },    // 倒车
  { NoVideoMenuItemFlashAddress+0x01F745, 0x00A731  },    // 顶部 
};
const struct menu_item_info NoVideoMenu_Info={215,150,0x000};
const struct RLE2_HEADER NoVideoMenu_Header[] =	{
	{ 0x88, 0x82, 0x0171, 0x0085, 0x0000A3F5, 0x01, 0xFF }, 		// 前门视频无信号 
	{ 0x88, 0x82, 0x0171, 0x0085, 0x0000A39D, 0x01, 0xFF }, 		// 中门视频无信号 
	{ 0x88, 0x82, 0x0171, 0x0085, 0x0000A383, 0x01, 0xFF }, 		// 倒车视频无信号 
	{ 0x88, 0x82, 0x0171, 0x0085, 0x0000A321, 0x01, 0xFF }, 		// 顶部视频无信号 
};

/************************************************************************************************************************************
1、新项目需要更改的界面
2、其所能用的空间为0X800000~0xFE0000,其后面被定义为字符库的空间
*************************************************************************************************************************************/
#define		MainMenuConfigStart_FlahAddress			0x800000

// 地址1-仪表图标 
#define		MainMenuConfigStart_FlahAddress1		(MainMenuConfigStart_FlahAddress+0x000000)
// 地址2-申龙LOGO 
#define		MainMenuConfigStart_FlahAddress2		(MainMenuConfigStart_FlahAddress+0x006F8E)

#define		NewProjectChange_FlahAddress			MainMenuConfigStart_FlahAddress1
///一级菜单，LED指示灯
const SLIDEIMAGE img_LedIndicate	= {NewProjectChange_FlahAddress+0x000000, 0x006F8E };    // 仪表图标 
const struct menu_item_info MainMenu_LedIndicate_Info={0,0,0x000};
const struct RLE2_HEADER MainMenu_LedIndicate_Header = { 0x88, 0x84, 0x0320, 0x0070, 0x00006B7E, 0x01, 0xFF };     // 仪表图标 

///LOGO图标
#define LOGO_ShenLong			0			///申龙LOGO

#define		MainMenu_LOGO_FlashAddress		MainMenuConfigStart_FlahAddress2
const SLIDEIMAGE img_MainMenu_LOGO[]=
{
	{ MainMenu_LOGO_FlashAddress+0x000000, 0x010CD2 },    // 申龙 
  { MainMenu_LOGO_FlashAddress+0x000000, 0x010CD2 },    // 
  { MainMenu_LOGO_FlashAddress+0x000000, 0x010CD2 },    // 
  
};
const struct menu_item_info MainMenu_LOGO_Info={0,0,0x00};
const struct RLE2_HEADER MainMenu_LOGO_Header[] =
{
	{ 0x88, 0x87, 0x0320, 0x01E0, 0x000108C2, 0x01, 0xFF },     // 申龙
  { 0x88, 0x87, 0x0320, 0x01E0, 0x000108C2, 0x01, 0xFF },     //
  { 0x88, 0x87, 0x0320, 0x01E0, 0x000108C2, 0x01, 0xFF },     //
};


#define		MalfunctionConfigStart_FlahAddress	0x900000
///三级故障查询内容界面
#define	offset_MulfunctionContent_Address		MalfunctionConfigStart_FlahAddress
#define BakAlarm		0x132CC0		//备用报警

const SLIDEIMAGE img_ThirdMenu_GuZhan_Content[]	=
{ 
	{ offset_MulfunctionContent_Address+0x186780, 0x004A60 },    // 祝您一路平安 
	///仪表模块,共3+3+1=7个
	{ offset_MulfunctionContent_Address+0x00DF20, 0x004A60 },    // 空调控制电源短路 
	{ offset_MulfunctionContent_Address+0x012980, 0x004A60 },    // 翘板开关背光灯短路 
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
  { offset_MulfunctionContent_Address+0x000000, 0x004A60 },    // 视频电源1短路
	{ offset_MulfunctionContent_Address+0x004A60, 0x004A60 },    // 视频电源2短路
	{ offset_MulfunctionContent_Address+0x0094C0, 0x004A60 },    // 车速信号1短路 
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 

	///前驱模块,共20个
	{offset_MulfunctionContent_Address+0x037C80, 0x004A60 },    // 洗涤器短路  
	{offset_MulfunctionContent_Address+0x033220, 0x004A60 },    // 前门踏步灯短路 
	{offset_MulfunctionContent_Address+0x0173E0, 0x004A60 },    // ON档继电器电源短路 
	{offset_MulfunctionContent_Address+0x04F060, 0x004A60 },    // 左前转向灯短路
	{offset_MulfunctionContent_Address+0x041140, 0x004A60 },    // 右前转向灯短路 
	{offset_MulfunctionContent_Address+0x01BE40, 0x004A60 },    // 倒档信号短路
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+0x04A600, 0x004A60 },    // 左前雾灯短路 
	{offset_MulfunctionContent_Address+0x03C6E0, 0x004A60 },    // 右前雾灯短路 
	{offset_MulfunctionContent_Address+0x053AC0, 0x004A60 },    // 左阅读灯短路
	{offset_MulfunctionContent_Address+0x045BA0, 0x004A60 },    // 右阅读灯短路
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+0x025300, 0x004A60 },    // 喇叭短路  
	{offset_MulfunctionContent_Address+0x0208A0, 0x004A60 },    // 发动机ECU电源短路
	{offset_MulfunctionContent_Address+0x029D60, 0x004A60 },    // 前门关电磁阀短路
	{offset_MulfunctionContent_Address+0x02E7C0, 0x004A60 },    // 前门开电磁阀短路
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	///后驱模块,共20个
	{offset_MulfunctionContent_Address+0x082280, 0x004A60 },    // 启动电源总开关短路 
	{offset_MulfunctionContent_Address+0x07D820, 0x004A60 },    // 后雾灯短路 
	{offset_MulfunctionContent_Address+0x06AEA0, 0x004A60 },    // 发动机ECU电源短路
	{offset_MulfunctionContent_Address+0x078DC0, 0x004A60 },    // 后仓灯短路
	{offset_MulfunctionContent_Address+0x0901A0, 0x004A60 },    // 左侧、后转向灯短路
	{offset_MulfunctionContent_Address+0x086CE0, 0x004A60 },    // 右侧、后转向灯短路 
	{offset_MulfunctionContent_Address+0x0619E0, 0x004A60 },    // 倒车灯短路 
	{offset_MulfunctionContent_Address+0x066440, 0x004A60 },    // 电源总开关短路 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+0x058520, 0x004A60 },    // ON档电源输出短路
	{offset_MulfunctionContent_Address+0x05CF80, 0x004A60 },    // 侧标志灯短路 
	{offset_MulfunctionContent_Address+0x08B740, 0x004A60 },    // 制动灯短路
	{offset_MulfunctionContent_Address+0x06F900, 0x004A60 },    // 发动机起动条件短路
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },    // 干燥器短路 
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },    // 干燥器短路
	
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 

	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警

	///顶驱模块,共20个
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警

	///配电模块，共18个负载，实际多占用2个的位置
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+0x0A7580, 0x004A60 },    // 右远光灯短路
	{offset_MulfunctionContent_Address+0x0A7580, 0x004A60 },    // 右远光灯短路 	
	{offset_MulfunctionContent_Address+0x0A2B20, 0x004A60 },    // 右近光灯短路
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+0x094C00, 0x004A60 },    // ABS ON档电源短路 
	{offset_MulfunctionContent_Address+0x0B54A0, 0x004A60 },    // 左近光灯短路
	{offset_MulfunctionContent_Address+0x0B9F00, 0x004A60 },    // 左远光灯短路 
	{offset_MulfunctionContent_Address+0x0B9F00, 0x004A60 },    // 左远光灯短路 
	{offset_MulfunctionContent_Address+0x0B0A40, 0x004A60 },    // 雨刮高速短路 
	{offset_MulfunctionContent_Address+0x0ABFE0, 0x004A60 },    // 雨刮低速短路
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+0x099660, 0x004A60 },    // 除霜机1档短路
	{offset_MulfunctionContent_Address+0x09E0C0, 0x004A60 },    // 除霜机2档短路
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警

	///配电模块，共20个保险丝
	{offset_MulfunctionContent_Address+0x0BE960, 0x004A60 },    // 保险丝F1开路 
	{offset_MulfunctionContent_Address+0x0C33C0, 0x004A60 },    // 保险丝F2开路
	{offset_MulfunctionContent_Address+0x0C7E20, 0x004A60 },    // 保险丝F3开路
	{offset_MulfunctionContent_Address+0x0CC880, 0x004A60 },    // 保险丝F4开路
	{offset_MulfunctionContent_Address+0x0D12E0, 0x004A60 },    // 保险丝F5开路
	{offset_MulfunctionContent_Address+0x0D5D40, 0x004A60 },    // 保险丝F6开路 
	{offset_MulfunctionContent_Address+0x0DA7A0, 0x004A60 },    // 保险丝F7开路
	{offset_MulfunctionContent_Address+0x0DF200, 0x004A60 },    // 保险丝F8开路
	{offset_MulfunctionContent_Address+0x0E3C60, 0x004A60 },    // 保险丝F9开路
	{offset_MulfunctionContent_Address+0x0E86C0, 0x004A60 },    // 保险丝F10开路
	{offset_MulfunctionContent_Address+0x0ED120, 0x004A60 },    // 保险丝F11开路
	{offset_MulfunctionContent_Address+0x0F1B80, 0x004A60 },    // 保险丝F12开路
	{offset_MulfunctionContent_Address+0x0F65E0, 0x004A60 },    // 保险丝F13开路
	{offset_MulfunctionContent_Address+0x0FB040, 0x004A60 },    // 保险丝F14开路
	{offset_MulfunctionContent_Address+0x0FFAA0, 0x004A60 },    // 保险丝F15开路
	{offset_MulfunctionContent_Address+0x104500, 0x004A60 },    // 保险丝F16开路
	{offset_MulfunctionContent_Address+0x108F60, 0x004A60 },    // 保险丝F17开路 
	{offset_MulfunctionContent_Address+0x10D9C0, 0x004A60 },    // 保险丝F18开路
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
// 电源管理模块,共30个
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警

	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警		
// 开路报警 共10个
	{offset_MulfunctionContent_Address+0x129800, 0x004A60 },    // 左远光灯开路
	{offset_MulfunctionContent_Address+0x11B8E0, 0x004A60 },    // 右远光灯开路
	{offset_MulfunctionContent_Address+0x120340, 0x004A60 },    // 左近光灯开路 
	{offset_MulfunctionContent_Address+0x112420, 0x004A60 },    // 右近光灯开路
	{offset_MulfunctionContent_Address+0x124DA0, 0x004A60 },    // 左前雾灯开路
	{offset_MulfunctionContent_Address+0x116E80, 0x004A60 },    // 右前雾灯开路
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	

	///二级故障
	{ offset_MulfunctionContent_Address+0x157FC0, 0x004A60 },    // 前气压报警
	{ offset_MulfunctionContent_Address+0x140BE0, 0x004A60 },    // 后气压报警  
	{ offset_MulfunctionContent_Address+0x16A940, 0x004A60 },    // 水温报警  
	{ offset_MulfunctionContent_Address+0x14A0A0, 0x004A60 },    // 机油压力报警 
	{ offset_MulfunctionContent_Address+0x165EE0, 0x004A60 },    // 水位报警 
	{ offset_MulfunctionContent_Address+0x137720, 0x004A60 },    // 电池电压报警 
	
	{ offset_MulfunctionContent_Address+0x15CA20, 0x004A60 },    // 前气压传感器故障
	{ offset_MulfunctionContent_Address+0x145640, 0x004A60 },    // 后气压传感器故障
	{ offset_MulfunctionContent_Address+0x161480, 0x004A60 },    // 燃油油量传感器故障 
	{ offset_MulfunctionContent_Address+0x14EB00, 0x004A60 },    // 机油压力传感器故障 
	{	offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{	offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{	offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{	offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{	offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{	offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警		
	////J1939通信故障 8个
	{ offset_MulfunctionContent_Address+0x13C180, 0x004A60 },    // 发动机ECU通讯故障
	{ offset_MulfunctionContent_Address+0x12E260, 0x004A60 },    // SCR通信故障  
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // BMS通信故障 	
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 动力系统通信故障	
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 无胎压模块报警 
	////车身模块通信故障 共8个
	{ offset_MulfunctionContent_Address+0x178860, 0x004A60 },    // 无前驱模块报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 无顶控模块报警
	{ offset_MulfunctionContent_Address+0x16F3A0, 0x004A60 },    // 无后驱模块报警
	{ offset_MulfunctionContent_Address+0x173E00, 0x004A60 },    // 无配电模块报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 无电源管理模块报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	

	//特殊二类故障 共16个
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 左刹车片报警 
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 右刹车片报警 
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 门泵低气压报警
	
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 变速箱故障报警 
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 卫生间水位报警 
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 发动机仓温报警 
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 缓速器高温报警 
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 空滤器报警 
	{ offset_MulfunctionContent_Address+0x17D2C0, 0x004A60 },    // 右后蹄片报警
	{ offset_MulfunctionContent_Address+0x181D20, 0x004A60 },    // 右前蹄片报警  
	{ offset_MulfunctionContent_Address+0x18B1E0, 0x004A60 },    // 左后蹄片报警
	{ offset_MulfunctionContent_Address+0x18FC40, 0x004A60 },    // 左前蹄片报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警	
//3类故障文字报警	8个
	{ offset_MulfunctionContent_Address+0x153560, 0x004A60 },    // 前门未关报警 		
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 中门未关报警	
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 请系好安全带
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警
	{ offset_MulfunctionContent_Address+BakAlarm, 0x004A60 },    // 备用异常报警 
//仪表提示信息
	{ offset_MulfunctionContent_Address+0x19DB60, 0x004A60 },    // 起动中		
	{ offset_MulfunctionContent_Address+0x1946A0, 0x004A60 },    // 空档末关报警	
	{ offset_MulfunctionContent_Address+0x199100, 0x004A60 },    // 起动时间过长报警 
	{ offset_MulfunctionContent_Address+0x1A25C0, 0x004A60 },    // 重复起动报警

};
const struct RLE2_HEADER ThirdMenu_GuZhan_Header = { 0x88, 0x00, 0x01C2, 0x0028, 0x00004650, 0x01, 0xFF }; 
const struct menu_item_info		ThirdMenu_GuZhanChaXun_Disp_Address[5]=
{
	{180,145,0x100},
	{180,200,0x100},	
	{180,250,0x100},
	{180,305,0x100},		
	{180,352,0x100},	
};
///系统诊断内容
#define		DiagnoseConfigStart_FlahAddress			0x380000

#define		CanDiagnoseConfigStart_FlahAddress	0xC00000

#define	ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress			DiagnoseConfigStart_FlahAddress
#define	ThirdMenu_CANXiTongZhenDuanNeiRong_FlashAddress		CanDiagnoseConfigStart_FlahAddress

const SLIDEIMAGE img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[]	=
{
	{ ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x000000, 0x03AD90 },    // 仪表_1 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x03AD90, 0x03AD90 },    // 仪表_2 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x075B20, 0x03AD90 },    // 仪表_3 

	{ ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x0B08B0, 0x03AD90 },    // 前控内容1 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x0EB640, 0x03AD90 },    // 前控内容2 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x1263D0, 0x03AD90 },    // 前控内容3 

	{ ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x161160, 0x03AD90 },    // 后控内容1 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x19BEF0, 0x03AD90 },    // 后控内容2 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x1D6C80, 0x03AD90 },    // 后控内容3 

  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x211A10, 0x03AD90 },    // 保险丝对应端口号 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x24C7A0, 0x03AD90 },    // 配电模块-1 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x287530, 0x03AD90 },    // 配电模块-2 	

	{ ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x2C22C0, 0x03AD90 },    // 模块温度-1  

	
};
const struct menu_item_info ThirdMenu_XiTongZhenDuanNeiRong_Item_Info={0,80,0x100};
const struct menu_item_info ThirdMenu_XiTongZhenDuanNeiRong_PowerContrl_Item_Info={0,60,0x100};
const struct RLE2_HEADER ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[] =
{
    { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 仪表内容1 
    { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 仪表内容2 
    { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 仪表内容3 
    
    { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 前控内容1 
    { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 前控内容2 
    { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 前控内容3 
    
    { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 后控内容1 
    { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 后控内容2 
    { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 后控内容3  
    
		{ 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF }, 		// 保险丝对应端口号 
		{ 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF }, 		// 配电模块-1 
		{ 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF }, 		// 配电模块-2 
		
		{ 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF }, 		// 模块温度 

};


#endif


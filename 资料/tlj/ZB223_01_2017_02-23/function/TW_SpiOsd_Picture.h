#ifndef _TW_SPIOSD_PICTURE_H_
#define _TW_SPIOSD_PICTURE_H_
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* common defines and macros */
#include	"Typedef.h"
#define MeterPictureStartFlahAddress	0x000000
const SLIDEIMAGE MeterPictureAllFlahAddress[] = { 
	{MeterPictureStartFlahAddress+ 0x000000, 0x0A8E74 },		// 地址1-主界面效果 
	{MeterPictureStartFlahAddress+ 0x0A8E74, 0x0402D3 },		// 地址2-SubMenu 
	{MeterPictureStartFlahAddress+ 0x0E9147, 0x00F728 },		// 地址3-故障报警查询 
	{MeterPictureStartFlahAddress+ 0x0F886F, 0x0594A4 },		// 地址4-模块选择 
	{MeterPictureStartFlahAddress+ 0x151D13, 0x011626 },		// 地址5-参数设置背景 
	{MeterPictureStartFlahAddress+ 0x163339, 0x00B900 },		// 地址6-图像设置选项 
	{MeterPictureStartFlahAddress+ 0x16EC39, 0x017070 },		// 地址7-通道设置背景 
	{MeterPictureStartFlahAddress+ 0x185CA9, 0x00B900 },		// 地址8-通道设置选项 
	{MeterPictureStartFlahAddress+ 0x1915A9, 0x00A5D0 },		// 地址9-系统时间设置背景 
	{MeterPictureStartFlahAddress+ 0x19BB79, 0x011F60 },		// 地址10-系统时间设置选项 
	{MeterPictureStartFlahAddress+ 0x1ADAD9, 0x018ACC },		// 地址11-系统帮助 
	{MeterPictureStartFlahAddress+ 0x1C65A5, 0x016694 },		// 地址12-版本信息背景 
	{MeterPictureStartFlahAddress+ 0x1DCC39, 0x021F4B },		// 地址13-胎压查询背景 
	{MeterPictureStartFlahAddress+ 0x1FEB84, 0x010C49 },		// 地址14-胎压监测背景 
	{MeterPictureStartFlahAddress+ 0x20F7CD, 0x00CAFB },		// 地址15-胎压学习密码背景 
	{MeterPictureStartFlahAddress+ 0x21C2C8, 0x003AC0 },		// 地址16-密码错误 
	{MeterPictureStartFlahAddress+ 0x21FD88, 0x02314E },		// 地址17-胎压学习背景 
	{MeterPictureStartFlahAddress+ 0x242ED6, 0x0222D8 },		// 地址18-胎压设置选项 
	{MeterPictureStartFlahAddress+ 0x2651AE, 0x00EA4C },		// 地址19-里程油耗背景 
	{MeterPictureStartFlahAddress+ 0x273BFA, 0x0023A0 },		// 地址20-清除 
	{MeterPictureStartFlahAddress+ 0x275F9A, 0x01474D },		// 地址21-系统参数设置背景 
	{MeterPictureStartFlahAddress+ 0x28A6E7, 0x015ED0 },		// 地址22-系统参数设置选项 
	{MeterPictureStartFlahAddress+ 0x2A05B7, 0x029D5C },		// 地址23-视频无信号  
 }; 
//定义各图片开始地址
#define	MeterPictureStartFlahAddress_1			0x000000
#define	MeterPictureStartFlahAddress_2			0x0A8E74
#define	MeterPictureStartFlahAddress_3			0x0E9147
#define	MeterPictureStartFlahAddress_4			0x0F886F
#define	MeterPictureStartFlahAddress_5			0x151D13
#define	MeterPictureStartFlahAddress_6			0x163339
#define	MeterPictureStartFlahAddress_7			0x16EC39
#define	MeterPictureStartFlahAddress_8			0x185CA9
#define	MeterPictureStartFlahAddress_9			0x1915A9
#define	MeterPictureStartFlahAddress_10			0x19BB79
#define	MeterPictureStartFlahAddress_11			0x1ADAD9
#define	MeterPictureStartFlahAddress_12			0x1C65A5
#define	MeterPictureStartFlahAddress_13			0x1DCC39
#define	MeterPictureStartFlahAddress_14			0x1FEB84
#define	MeterPictureStartFlahAddress_15			0x20F7CD
#define	MeterPictureStartFlahAddress_16			0x21C2C8
#define	MeterPictureStartFlahAddress_17			0x21FD88
#define	MeterPictureStartFlahAddress_18			0x242ED6
#define	MeterPictureStartFlahAddress_19			0x2651AE
#define	MeterPictureStartFlahAddress_20			0x273BFA
#define	MeterPictureStartFlahAddress_21			0x275F9A
#define	MeterPictureStartFlahAddress_22			0x28A6E7
#define	MeterPictureStartFlahAddress_23			0x2A05B7

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
///一级菜单,车模
///前门中门关
const SLIDEIMAGE img_VehicleModuleIndicate_AllClose	= {offset_NoChangeFlahAddress+0x03E4E0, 0x011481 };
const struct menu_item_info MainMenu_VehicleModuleIndicate_AllClose_Info={160,130,0x100};
const struct RLE2_HEADER MainMenu_VehicleModuleIndicate_AllClose_Header = {0x88, 0x00, 0x01E1, 0x0091, 0x00011071, 0x01, 0xFF }; 
///前门关中门开
const SLIDEIMAGE img_VehicleModuleIndicate_MiddleOpen	= {offset_NoChangeFlahAddress+0x04F961, 0x011481 };
const struct menu_item_info MainMenu_VehicleModuleIndicate_MiddleOpen_Info={160,130,0x100};
const struct RLE2_HEADER MainMenu_VehicleModuleIndicate_MiddleOpen_Header = {0x88, 0x00, 0x01E1, 0x0091, 0x00011071, 0x01, 0xFF }; 
///前门开中门关
const SLIDEIMAGE img_VehicleModuleIndicate_FrontOpen	= {offset_NoChangeFlahAddress+0x060DE2, 0x011481};
const struct menu_item_info MainMenu_VehicleModuleIndicate_FrontOpen_Info={160,130,0x100};
const struct RLE2_HEADER MainMenu_VehicleModuleIndicate_FrontOpen_Header = {0x88, 0x00, 0x01E1, 0x0091, 0x00011071, 0x01, 0xFF }; 
///前门中门开
const SLIDEIMAGE img_VehicleModuleIndicate_AllOpen	= {offset_NoChangeFlahAddress+0x072263, 0x011481 };
const struct menu_item_info MainMenu_VehicleModuleIndicate_AllOpen_Info={160,130,0x100};
const struct RLE2_HEADER MainMenu_VehicleModuleIndicate_AllOpen_Header = {0x88, 0x00, 0x01E1, 0x0091, 0x00011071, 0x01, 0xFF }; 
///一级菜单,里程、油耗、尿素、日期时间
const SLIDEIMAGE img_SmallModuleIndicate	= {offset_NoChangeFlahAddress+0x0836E4, 0x025790 };
const struct menu_item_info MainMenu_SmallModuleIndicate_Info={0,280,0x100};
const struct RLE2_HEADER MainMenu_SmallModuleIndicate_Header = { 0x88, 0x00, 0x031A, 0x00C0, 0x00025380, 0x01, 0xFF }; 
///一级菜单故障显示
const struct menu_item_info		MainMenu_GuZhanChaXun_Disp_Address[2]=
{
	{190,286,0x100},
	{190,336,0x100}	
};
///
#define MainMenuEndFlashAddress		(img_SmallModuleIndicate.start+img_SmallModuleIndicate.length)
///二级菜单
#define		SubMenuFlashAddress			MeterPictureStartFlahAddress_2		//0x100000
const SLIDEIMAGE img_SubMenuBackGround	= {SubMenuFlashAddress+0x000000,0x0402D3};
const struct menu_item_info SubMenu_BackGround_Info={0,0,0x000};
const struct RLE2_HEADER SubMenu_BackGround_Header = { 0x88, 0x82, 800, 480, 0x0003FEC3, 1,0xff};  
///三级故障查询界面
#define		ThirdMenu_GuZhanChaXun_FlashAddress			MeterPictureStartFlahAddress_3//0x150000
const SLIDEIMAGE img_ThirdMenu_GuZhanChaXun	= {ThirdMenu_GuZhanChaXun_FlashAddress+0x000000,0x00F728 };
const struct menu_item_info ThirdMenu_GuZhanChaXun_Item_Info={0,0,0x000};
const struct RLE2_HEADER ThirdMenu_GuZhanChaXun_Header = { 0x88, 0x86, 0x0320, 0x01E0, 0x0000F318, 0x01, 0xFF };    // 故障报警查询 
///三级系统诊断界面
#define	ThirdMenu_XiTongZhenDuan_FlashAddress		MeterPictureStartFlahAddress_4//0x200000

const SLIDEIMAGE img_ThirdMenu_XiTongZhenDuan_MoKuaiXuanZhe[ ]	=
{
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x000000, 0x00AC3A },    // 仪表 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x00AC3A, 0x00B403 },    // 前控 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x01603D, 0x00B190 },    // 后控 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x0211CD, 0x00AC43 },    // 顶控 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x02BE10, 0x00B586 },    // 配电 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x037396, 0x00B3C0 },    // 模块温度 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x042756, 0x00B6AE },    // 集中润滑 
  { ThirdMenu_XiTongZhenDuan_FlashAddress+0x04DE04, 0x00B6A0 },    // 发动机  
};
const struct menu_item_info ThirdMenu_XiTongZhenDuan_MoKuaiXuanZhe_Item_Info={0,0,0x000};
const struct RLE2_HEADER ThirdMenu_XiTongZhenDuan_MoKuaiXuanZhe_Header[ ] = 
{
	{ 0x88, 0x87, 0x0320, 0x01E0, 0x0000A82A, 0x01, 0xFF },     // 仪表 
  { 0x88, 0x87, 0x0320, 0x01E0, 0x0000AFF3, 0x01, 0xFF },     // 前控 
  { 0x88, 0x87, 0x0320, 0x01E0, 0x0000AD80, 0x01, 0xFF },     // 后控 
  { 0x88, 0x87, 0x0320, 0x01E0, 0x0000A833, 0x01, 0xFF },     // 顶控 
  { 0x88, 0x87, 0x0320, 0x01E0, 0x0000B176, 0x01, 0xFF },     // 配电 
  { 0x88, 0x87, 0x0320, 0x01E0, 0x0000AFB0, 0x01, 0xFF },     // 模块温度 
  { 0x88, 0x87, 0x0320, 0x01E0, 0x0000B29E, 0x01, 0xFF },     // 集中润滑 
  { 0x88, 0x87, 0x0320, 0x01E0, 0x0000B290, 0x01, 0xFF },     // 发动机 
};
///8位图片
///图像画面及通道设置
#define		TestThirdMenuPictureFlashAddress			MeterPictureStartFlahAddress_5//0x400000
//画面设置底层图
const SLIDEIMAGE img_ThirdMenu_Picture	= {TestThirdMenuPictureFlashAddress+0x000000, 0x011626  };
const struct menu_item_info ThirdMenu_Picture_Item_Info={0,0,0x000};
const struct RLE2_HEADER ThirdMenu_Picture_Header = { 0x88, 0x86, 0x0320, 0x01E0, 0x00011216, 0x01, 0xFF }; 
//画面设置参数选项
#define		TestThirdMenuPictureItemFlashAddress			MeterPictureStartFlahAddress_6//0x413000
const struct RLE2_HEADER ThirdMenu_PictureItem_Header = {0x88, 0x00, 0x00B4, 0x003C, 0x00002A30, 0x01, 0xFF }; 
//对比度选中
const SLIDEIMAGE img_ThirdMenu_PictureContrastSel	= {TestThirdMenuPictureItemFlashAddress+0x000000, 0x002E40  };
const struct menu_item_info ThirdMenu_PictureContrastSel_Item_Info={69,87,0x100};
//亮度选中
const SLIDEIMAGE img_ThirdMenu_PictureBrightSel	= {TestThirdMenuPictureItemFlashAddress+0x002E40, 0x002E40};
const struct menu_item_info ThirdMenu_PictureBrightSel_Item_Info={69,169,0x100};
//色度选中
const SLIDEIMAGE img_ThirdMenu_PictureChromaSel	= {TestThirdMenuPictureItemFlashAddress+0x005C80, 0x002E40 };
const struct menu_item_info ThirdMenu_PictureChromaSel_Item_Info={69,251,0x100};
//清晰度选中
const SLIDEIMAGE img_ThirdMenu_PictureDefinitionSel	= {TestThirdMenuPictureItemFlashAddress+0x008AC0, 0x002E40  };
const struct menu_item_info ThirdMenu_PictureDefinitionSel_Item_Info={69,334,0x100};

#define		TestThirdMenuChannleFlashAddress			MeterPictureStartFlahAddress_7//0x420000
//通道设置底层图
const SLIDEIMAGE img_ThirdMenu_Channle	= {TestThirdMenuChannleFlashAddress+0x000000, 0x017070 };
const struct menu_item_info ThirdMenu_Channle_Item_Info={0,0,0x000};
const struct RLE2_HEADER ThirdMenu_Channle_Header = {0x88, 0x85, 0x0320, 0x01E0, 0x00016C60, 0x01, 0xFF}; 
//通道设置选项
#define		TestThirdMenuChannleItemFlashAddress			MeterPictureStartFlahAddress_8//0x460000
const struct RLE2_HEADER ThirdMenu_ChannleItem_Header = {0x88, 0x00, 0x00B4, 0x003C, 0x00002A30, 0x01, 0xFF}; 
//前门//选中
const struct menu_item_info ThirdMenu_ChannleFrontDoor_Item_Info={41,86,0x100};
const SLIDEIMAGE img_ThirdMenu_ChannleFrontDoorSel	= {TestThirdMenuChannleItemFlashAddress+0x000000, 0x002E40 };
//中门//选中
const struct menu_item_info ThirdMenu_ChannleMiddleDoor_Item_Info={41,169,0x100};
const SLIDEIMAGE img_ThirdMenu_ChannleMiddleDoorSel	= {TestThirdMenuChannleItemFlashAddress+0x002E40, 0x002E40 };
//倒车//选中
const struct menu_item_info ThirdMenu_ChannleBackCar_Item_Info={41,251,0x100};
const SLIDEIMAGE img_ThirdMenu_ChannleBackCarSel	= {TestThirdMenuChannleItemFlashAddress+0x005C80, 0x002E40  };
//顶部//选中
const struct menu_item_info ThirdMenu_ChannleTOP_Item_Info={41,333,0x100};
const SLIDEIMAGE img_ThirdMenu_ChannleTOPSel	= {TestThirdMenuChannleItemFlashAddress+0x008AC0, 0x002E40  };

//系统时间设置
#define		SystemTimeMenuFlashAddress			MeterPictureStartFlahAddress_9//0x470000
const SLIDEIMAGE img_SystemTimeMenuBackGround	= {SystemTimeMenuFlashAddress+0x000000, 0x00A5D0};
const struct menu_item_info SystemTimeMenu_BackGround_Info={0,0,0x000};
const struct RLE2_HEADER SystemTimeMenu_BackGround_Header = {0x88, 0x87, 0x0320, 0x01E0, 0x0000A1C0, 0x01, 0xFF}; 

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
const SLIDEIMAGE img_SystemDataTimeErr	= {SystemTimeDataItemFlashAddress+0x00E4A0, 0x003AC0  };
const struct RLE2_HEADER SystemDataTimeErr_Header = { 0x88, 0x00, 0x0118, 0x0032, 0x000036B0, 0x01, 0xFF  };  

/////////////////////////////////////////////////////
/**********************系统帮助***************************/
#define		HelpInfomationMenuBackGroundFlashAddress	MeterPictureStartFlahAddress_11//0x4A0000
const SLIDEIMAGE img_HelpInfomationMenuBackGround	= {HelpInfomationMenuBackGroundFlashAddress+0x000000, 0x018ACC };
const struct RLE2_HEADER HelpInfomationMenuBackGround_Header = { 0x88, 0x85, 0x0320, 0x01E0, 0x000186BC, 0x01, 0xFF };  
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
const SLIDEIMAGE img_VersionQueryMenuBackGround	= {VersionQueryMenuBackGroundFlashAddress+0x000000, 0x016694 };
const struct RLE2_HEADER VersionQueryMenuBackGround_Header = {0x88, 0x85, 0x0320, 0x01E0, 0x00016284, 0x01, 0xFF};  
const struct menu_item_info VersionQueryMenuBackGround_Info= {0,0,0x000};

//胎压查询背景图
#define		TyrePressMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_13//0x518000	
const SLIDEIMAGE img_TyrePressMenuBackGround	= {TyrePressMenuBackGroundFlashAddress+0x000000, 0x021F4B };
const struct RLE2_HEADER TyrePressMenuBackGround_Header = { 0x88, 0x84, 0x0320, 0x01E0, 0x00021B3B, 0x01, 0xFF};  
const struct menu_item_info TyrePressMenuBackGround_Info= {0,0,0x000};
//胎压监测背景
#define		TyrePressMonitorMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_14//0x53A000	
const SLIDEIMAGE img_TyrePressMonitorMenuBackGround	= {TyrePressMonitorMenuBackGroundFlashAddress+0x000000, 0x010C49};
const struct RLE2_HEADER TyrePressMonitorMenuBackGround_Header = {0x88, 0x87, 0x0320, 0x01E0, 0x00010839, 0x01, 0xFF};  
const struct menu_item_info TyrePressMonitorMenuBackGround_Info= {0,0,0x000};

//胎压学习密码
#define		TyrePressPassWordMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_15//0x550000	
const SLIDEIMAGE img_TyrePressPassWordMenuBackGround	= {TyrePressPassWordMenuBackGroundFlashAddress+0x000000, 0x00CAFB  };
const struct RLE2_HEADER TyrePressPassWordMenuBackGround_Header = { 0x88, 0x87, 0x0320, 0x01E0, 0x0000C6EB, 0x01, 0xFF };  
const struct menu_item_info TyrePressPassWordMenuBackGround_Info= {0,0,0x000};
//密码输入错误
#define		TyrePressPassWordErrFlashAddress		MeterPictureStartFlahAddress_16//0x560000	
const SLIDEIMAGE img_TyrePressPassWordErr	= {TyrePressPassWordErrFlashAddress+0x000000, 0x003AC0  };
const struct RLE2_HEADER TyrePressPassWordErr_Header = { 0x88, 0x00, 0x0118, 0x0032, 0x000036B0, 0x01, 0xFF  };  
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
const SLIDEIMAGE img_DistanceFualConsumeMenu	= {DistanceFualConsumeMenuFlashAddress+0x000000, 0x00EA4C };
const struct RLE2_HEADER DistanceFualConsumeMenu_Header = { 0x88, 0x86, 0x0320, 0x01E0, 0x0000E63C, 0x01, 0xFF };  
const struct menu_item_info DistanceFualConsumeMenu_Info= {0,0,0x000};
//小计清零图标
#define		SmallDistanceClearMenuFlashAddress		MeterPictureStartFlahAddress_20//0x610000	
const SLIDEIMAGE img_SmallDistanceClearMenu	= {SmallDistanceClearMenuFlashAddress+0x000000, 0x0011D0   };
const SLIDEIMAGE img_SmallDistanceClearSelMenu	= {SmallDistanceClearMenuFlashAddress+0x0011D0, 0x0011D0   };
const struct RLE2_HEADER SmallDistanceClearMenu_Header = { 0x88, 0x00, 0x0058, 0x0028, 0x00000DC0, 0x01, 0xFF  };  
const struct menu_item_info SmallDistanceClearMenu_Info= {362,420,0x100};
//参数设置背景
#define		SystemParameterMenuBackGroundFlashAddress		MeterPictureStartFlahAddress_21//0x620000	
const SLIDEIMAGE img_SystemParameterMenuBackGround	= {SystemParameterMenuBackGroundFlashAddress+0x000000, 0x01474D };
const struct RLE2_HEADER SystemParameterMenuBackGround_Header = { 0x88, 0x86, 0x0320, 0x01E0, 0x0001433D, 0x01, 0xFF};  
const struct menu_item_info SystemParameterMenuBackGround_Info= {0,0,0x000};
//参数设置选项
#define		SystemParameterMenuItemFlashAddress		MeterPictureStartFlahAddress_22//0x640000	
const struct RLE2_HEADER SystemParameterMenuItem_Header = {0x88, 0x00, 0x0204, 0x0038, 0x000070E0, 0x01, 0xFF };  
const SLIDEIMAGE img_SystemParameterMenuItemRainWiper	= {SystemParameterMenuItemFlashAddress+0x000000, 0x0074F0 };
const SLIDEIMAGE img_SystemParameterMenuItemBackLight	= {SystemParameterMenuItemFlashAddress+0x0074F0, 0x0074F0};
const SLIDEIMAGE img_SystemParameterMenuItemBuzzer		= {SystemParameterMenuItemFlashAddress+0x00E9E0, 0x0074F0};

const struct menu_item_info SystemParameterMenuItemRainWiper_Info= {152,131,0x100};
const struct menu_item_info SystemParameterMenuItemBackLight_Info= {152,208,0x100};
const struct menu_item_info SystemParameterMenuItemBuzzer_Info= {152,284,0x100};

#define		NoVideoMenuItemFlashAddress	 MeterPictureStartFlahAddress_23
const SLIDEIMAGE img_NoVideoMenu[]=
{
	{ NoVideoMenuItemFlashAddress+0x000000, 0x00A6EE },    // 前门
  { NoVideoMenuItemFlashAddress+0x00A6EE, 0x00A71E },    // 中门
  { NoVideoMenuItemFlashAddress+0x014E0C, 0x00A7BD },    // 倒车
  { NoVideoMenuItemFlashAddress+0x01F5C9, 0x00A793 },    // 顶部 
};
const struct menu_item_info NoVideoMenu_Info={215,150,0x000};
const struct RLE2_HEADER NoVideoMenu_Header =	{0x88, 0x82, 0x0171, 0x0085, 0x0000A383, 0x01, 0xFF };

/************************************************************************************************************************************
1、新项目需要更改的界面
2、其所能用的空间为0X900000~0xFE0000,其后面被定义为字符库的空间
*************************************************************************************************************************************/
#define		ConfigStart_FlahAddress			0x900000
const SLIDEIMAGE MeterPictureAllConfigFlahAddress[] = { 
			{ ConfigStart_FlahAddress+0x000000, 0x006D99 },		// 地址1-仪表图标 
			{ ConfigStart_FlahAddress+0x006D99, 0x08B740 },		// 地址2-故障内容的图标 
			{ ConfigStart_FlahAddress+0x0924D9, 0x0EB640 },		// 地址3-仪表诊断内容 
			{ ConfigStart_FlahAddress+0x17DB19, 0x010CD2 }, 	// 地址4-申龙LOGO 

}; 
#define		ConfigStart_FlahAddress1		(ConfigStart_FlahAddress+0x000000)
#define		ConfigStart_FlahAddress2		(ConfigStart_FlahAddress+0x006D99)
#define		ConfigStart_FlahAddress3		(ConfigStart_FlahAddress+0x0924D9)
#define		ConfigStart_FlahAddress4		(ConfigStart_FlahAddress+0x17DB19)	





#define		NewProjectChange_FlahAddress			ConfigStart_FlahAddress1


const SLIDEIMAGE ing_led1 = {     
    0x000000, 0x0036D4     // J2_2 

      };
const SLIDEIMAGE ing_led2 = {     

     0x0036D4, 0x0036D4    // J3_1 

      };
const SLIDEIMAGE ing_led3 = {     

     0x006DA8, 0x0036D4     // J2_1  
      };
      
 const struct RLE2_HEADER MainMenu_Led1={ 0x88, 0x00, 0x0072, 0x0072, 0x000032C4, 0x01, 0xFF };    // J2_2 }
 const struct RLE2_HEADER MainMenu_Led2={0x88, 0x00, 0x0072, 0x0072, 0x000032C4, 0x01, 0xFF } ;    // J3_1 }
 const struct RLE2_HEADER MainMenu_Led3={0x88, 0x00, 0x0072, 0x0072, 0x000032C4, 0x01, 0xFF };     // J2_1 }
 
 const struct menu_item_info MainMenu_LedInfo1={0,0,0x100};
 const struct menu_item_info MainMenu_LedInfo2={100,100,0x100};
 const struct menu_item_info MainMenu_LedInfo3={200,200,0x100};
///一级菜单，LED指示灯
const SLIDEIMAGE img_LedIndicate	= {NewProjectChange_FlahAddress+0x000000, 0x006D99};
const struct menu_item_info MainMenu_LedIndicate_Info={0,0,0x000};
const struct RLE2_HEADER MainMenu_LedIndicate_Header = { 0x88, 0x84, 0x0320, 0x0070, 0x00006989, 0x01, 0xFF};    // 汽车报警符号_24 
///三级故障查询内容界面
#define	offset_MulfunctionContent_Address		ConfigStart_FlahAddress2
///中门未关报警
const SLIDEIMAGE img_ThirdMenu_GuZhan_Content[]	=
{ 
	{ offset_MulfunctionContent_Address+0x078DC0, 0x004A60 },		 // 祝您一路平安
	///仪表模块,共3+3=6个
	{ offset_MulfunctionContent_Address+0x000000, 0x004A60 },    // ACC电源总开关 
	{ offset_MulfunctionContent_Address+0x041140, 0x004A60 },    // 启动电源总开关 
	{ offset_MulfunctionContent_Address+0x06F900, 0x004A60 },		 // 门控电源
  { offset_MulfunctionContent_Address+0x07D820, 0x004A60 }, 	 // 视频电源1报警 
	{ offset_MulfunctionContent_Address+0x082280, 0x004A60 }, 	 // 视频电源2报警 
	{ offset_MulfunctionContent_Address+0x086CE0, 0x004A60 }, 	 // 车速电源报警 

	///前驱模块,共20个
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///1
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///2
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///3
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///4
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///5
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///6
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///7
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///8
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///9
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///10
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///11
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///12
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///13
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///14
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///15
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///16
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///17
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///18
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///19
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///20
	///后驱模块,共20个
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///1
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///2
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///3
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///4
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///5
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///6
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///7
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///8
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///9
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///10
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///11
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///12
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///13
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///14
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///15
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///16
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///17
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///18
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///19
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///20

	///顶驱模块,共20个
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///1
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///2
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///3
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///4
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///5
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///6
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///7
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///8
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///9
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///10
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///11
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///12
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///13
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///14
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///15
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///16
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///17
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///18
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///19
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///20

	///配电模块，共18个负载，实际多占用2个的位置
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///1
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///2
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///3
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///4
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///5
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///6
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///7
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///8
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///9
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///10
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///11
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///12
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///13
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///14
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///15
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///16
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///17
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///18
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///19
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///20

	///配电模块，共17个保险丝
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///1
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///2
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///3
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///4
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///5
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///6
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///7
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///8
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///9
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///10
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///11
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///12
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///13
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///14
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///15
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///16
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///17
//	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///18
//	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///19
//	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///20

	///二级故障
	{ offset_MulfunctionContent_Address+0x04A600, 0x004A60 },    // 前气压报警 
	{ offset_MulfunctionContent_Address+0x0208A0, 0x004A60 },    // 后气压报警 
	{ offset_MulfunctionContent_Address+0x01BE40, 0x004A60 },    // 发动机水温过高报警 
	{ offset_MulfunctionContent_Address+0x02E7C0, 0x004A60 },    // 机油压力报警 
	{ offset_MulfunctionContent_Address+0x037C80, 0x004A60 },    // 冷却水位过低报警 
	{ offset_MulfunctionContent_Address+0x00DF20, 0x004A60 },    // 电池电压报警 
	
	{ offset_MulfunctionContent_Address+0x04F060, 0x004A60 },    // 前气压传感器故障报警 
	{ offset_MulfunctionContent_Address+0x025300, 0x004A60 },    // 后气压传感器故障报警 
	{ offset_MulfunctionContent_Address+0x058520, 0x004A60 },    // 油量传感器故障报警 
	{ offset_MulfunctionContent_Address+0x06AEA0, 0x004A60 },		 // 机油压力传感器故障
	{ offset_MulfunctionContent_Address+0x012980, 0x004A60 },    // 发动机ECU通讯故障 
	{ offset_MulfunctionContent_Address+0x004A60, 0x004A60 },    // SCR后处理通讯故障 
	{ offset_MulfunctionContent_Address+0x074360, 0x004A60 },///13
	{ offset_MulfunctionContent_Address+0x074360, 0x004A60 },///14
	{ offset_MulfunctionContent_Address+0x074360, 0x004A60 },///15
	{ offset_MulfunctionContent_Address+0x074360, 0x004A60 },///16
	{ offset_MulfunctionContent_Address+0x074360, 0x004A60 },///17

	{ offset_MulfunctionContent_Address+0x066440, 0x004A60 },    // 左刹车片报警 
	{ offset_MulfunctionContent_Address+0x05CF80, 0x004A60 },    // 右刹车片报警 
	{ offset_MulfunctionContent_Address+0x03C6E0, 0x004A60 },    // 门泵低气压报警 
	
	{ offset_MulfunctionContent_Address+0x0094C0, 0x004A60 },    // 变速箱故障报警 
	{ offset_MulfunctionContent_Address+0x053AC0, 0x004A60 },    // 卫生间水位报警  
	{ offset_MulfunctionContent_Address+0x0173E0, 0x004A60 },    // 发动机仓温报警 
	{ offset_MulfunctionContent_Address+0x029D60, 0x004A60 },    // 缓速器高温报警 
	{ offset_MulfunctionContent_Address+0x033220, 0x004A60 },    // 空滤器报警 
//3类故障文字报警	
	{offset_MulfunctionContent_Address+0x045BA0, 0x004A60 },///前门未关报警		
	{offset_MulfunctionContent_Address+0x0619E0, 0x004A60 },///中门未关报警	
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///8
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///9
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///10
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///11
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///12
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///13
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///14
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///15
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///16
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///17
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///18
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///19
	{offset_MulfunctionContent_Address+0x074360, 0x004A60 },///20

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
#define	ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress		ConfigStart_FlahAddress3
const SLIDEIMAGE img_ThirdMenu_XiTongZhenDuanNeiRong_YiBiao[]	=
{
	{ ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x000000, 0x03AD90 },    // 仪表_1 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x03AD90, 0x03AD90 },    // 仪表_2 
  { ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x075B20, 0x03AD90 },    // 仪表_3 
  
 	{ ThirdMenu_XiTongZhenDuanNeiRong_FlashAddress+0x0B08B0, 0x03AD90 },    // 模块温度_1 
};
const struct menu_item_info ThirdMenu_XiTongZhenDuanNeiRong_Item_Info={0,80,0x100};
const struct RLE2_HEADER ThirdMenu_XiTongZhenDuanNeiRong_YiBiao_Header[] =
{
	{ 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 仪表_1 
  { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 仪表_2 
  { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 仪表_3 

  { 0x88, 0x00, 0x0320, 0x012C, 0x0003A980, 0x01, 0xFF },     // 模块温度_1 
};

///LOGO图标
#define LOGO_ShenLong			0			///申龙LOGO

#define		MainMenu_LOGO_FlashAddress		0x400000
const SLIDEIMAGE img_MainMenu_LOGO[]=
{
	{ MainMenu_LOGO_FlashAddress+0x000000, 0x010CD2 },    // 申龙 
  { MainMenu_LOGO_FlashAddress+0x000000, 0x010CD2 },    // 
  { MainMenu_LOGO_FlashAddress+0x000000, 0x010CD2 },    // 
  
};
const struct menu_item_info MainMenu_LOGO_Info={0,0,0};
const struct RLE2_HEADER MainMenu_LOGO_Header[] =
{
	{ 0x88, 0x87, 0x0320, 0x01E0, 0x000108C2, 0x01, 0xFF },     // 申龙
  { 0x88, 0x87, 0x0320, 0x01E0, 0x000108C2, 0x01, 0xFF },     //
  { 0x88, 0x87, 0x0320, 0x01E0, 0x000108C2, 0x01, 0xFF },     //
};

#endif


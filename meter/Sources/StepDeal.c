#define		STEPDEAL_C
#include	"config.h"
#pragma CODE_SEG  USER_ROM
/********************************

*********************************/
void	StepDeal(void)
{
	unsigned char	i;
	
  if(OnKeyBit==KeyOff)
	{	
		carspeed=0;
		engSpeed=0;
		if(resetback0==0x00)
		{
			for(i=0;i<StepMotorNum;i++)
				motor_move(i,0);
			
			//ON档是指针归0
			i=TIE;
			i&=0x3f;
			if(i==0)
				resetback0=2;			
		}
		return;
	}
	else
	{
		if(resetback0==2)
		{			
			back0_motor(0);
		}
	}
  if(resetback0==0x00)
	{
//		VoltageStep();
//		SOCStep();
		GasPres1(); 	
		GasPres2();	
		WatTemp();	
		OilPlant();
		///EngRotation();
		///CARSPEED();
  }
}
/****************************************************************************************
                 
*****************************************************************************************/
static	unsigned char check_pos(unsigned char channel,unsigned int position)
{
    unsigned char flag;
    
    if(position == MotorFinalStep[channel])
       flag = STEPNOT;
    else if(position > MotorMaxStep[channel])
    {
        if(MotorFinalStep[channel] == MotorMaxStep[channel])
        {
            flag = STEPNOT;
        }
        else
        {
            flag = STEPHIG;
        }
    }
    else if(position < MotorMinStep[channel])
    {
        if(MotorFinalStep[channel] == MotorMinStep[channel])
        {
            flag = STEPNOT;
        }
        else
        {
            flag = STEPLOW;
        }
    }
    else
    {
        flag = OK;
    }   
    return flag;
}
/****************************************************************************************
              
*****************************************************************************************/
static void	motor_move(unsigned char channel,unsigned int position)
{   
    unsigned int data=0;
    unsigned char flag;
		
    flag = check_pos(channel,position);
    if(flag == OK||flag == STEPHIG||flag == STEPLOW)
   	{
 			MotorFinalStep[channel] = position;

			if(channel == Oilplant)
				TIE |= 0x04;
			if(channel == TACHOMETER)
			 	TIE |= 0x08;  
			if(channel == TEMP)
				TIE |= 0x02;
			if(channel == Carspeed)
				TIE |= 0x10;
			if(channel == PRES)
				TIE |=0x01;
			if(channel == PRES2)
				TIE |=0x20;
    }
}
/*************************************************************
Arithmatics:TACHOMAXSTEP=5000/25*14
*************************************************************/
void EngRotation(void) 
{ 	
	uint	ChangeTemp=0;

	ChangeTemp=engSpeed;
	if(ChangeTemp>3500)
	ChangeTemp=3500;
	ChangeTemp =ChangeTemp *6;
	ChangeTemp =ChangeTemp /7;
	ChangeTemp = ChangeTemp *6; 				///change to corner(3500/245=14)
	ChangeTemp = ChangeTemp /5; 					///change to motor step 
	ChangeTemp = ChangeTemp *4;
	ChangeTemp = ChangeTemp /5; 					///change to motor step 	
	motor_move(TACHOMETER,ChangeTemp);										 
} 
/*
void EngRotation(void) 
{		
	uint	ChangeTemp=0;

	ChangeTemp=engSpeed;
  if(ChangeTemp>6000)//3500
	ChangeTemp=6000;

//	ChangeTemp=ChangeTemp/12;
//	ChangeTemp=ChangeTemp*7;
	
//	ChangeTemp =ChangeTemp *6;
//	ChangeTemp =ChangeTemp /7;
  ChangeTemp = ChangeTemp *3;    			///change to corner(3500/245=14)
  ChangeTemp = ChangeTemp /5;     			///change to motor step 
  ChangeTemp = ChangeTemp *4;
  ChangeTemp = ChangeTemp /5;     			///change to motor step 	
  motor_move(TACHOMETER,ChangeTemp);        						 
} 
*/
/*************************************************************
Arithmatics:TACHOMAXSTEP=(320-180)
*************************************************************/
static void VoltageStep(void) 
{		
	uint	ChangeTemp=0;

	ChangeTemp=SystemVolatage;
  if(ChangeTemp>320)
		ChangeTemp=320;
	if(ChangeTemp<180)
		ChangeTemp=180;
	ChangeTemp=ChangeTemp-180;						///0 point be set to 140 data,is 14v
	ChangeTemp = ChangeTemp * 7;   
  motor_move(TEMP,ChangeTemp);         						 
} 
/*************************************************************
Arithmatics:TACHOMAXSTEP=(320-180)
*************************************************************/
static void SOCStep(void) 
{		
	uint	ChangeTemp=0;

	ChangeTemp=BatteryPresentSOC;
  if(ChangeTemp>200)
		ChangeTemp=200;
	ChangeTemp = ChangeTemp * 11;   
	ChangeTemp = ChangeTemp /2;  
  motor_move(Oilplant,ChangeTemp);         						 
} 

/*************************************************************
Arithmatics:CARSPEEDMAXSTEP=140*84
*************************************************************/
void CARSPEED(void) 
{		
	uint	ChangeTemp=0;
	ChangeTemp = carspeed;
	ChangeTemp =ChangeTemp*100;  ////衰减98 %
	ChangeTemp =ChangeTemp/98;
	
	ChangeTemp = ChangeTemp*21;	    
	motor_move(Carspeed,ChangeTemp);             
} 
/****************************************************************************************
Arithmatics:OILMAXSTEP=45*96
*****************************************************************************************/
static void OilPlant(void) 
{  
	uint	ChangeTemp=0;
  ChangeTemp = OilNumberData;	                
  ChangeTemp = ChangeTemp * 24;   
  motor_move(Oilplant,ChangeTemp);  	  
}
/**********************************************************
Arithmatics:TEMPMAXSTEP=100*44
***********************************************************/
#define	WATERLOWNUM		40///水温最低为40摄氏度
#define	WATERHIGHNUM		120///水温最高为120摄氏度
static void WatTemp(void) 
{   
	uint	ChangeTemp=0;   
  if(watTemp> WATERLOWNUM) 
    ChangeTemp = watTemp - WATERLOWNUM;            
  else 
    ChangeTemp = 0;
  if(ChangeTemp>(WATERHIGHNUM-WATERLOWNUM))
   	ChangeTemp=WATERHIGHNUM-WATERLOWNUM;
	ChangeTemp = ChangeTemp * 14;	
  motor_move(TEMP,ChangeTemp);                  
}
/***************************************************************

***************************************************************/
static void GasPres1(void) 
{  
	uint	ChangeTemp=0;
	
  ChangeTemp = pres_value;	              
  if(ChangeTemp>250)
		ChangeTemp=250;
  ChangeTemp = ChangeTemp * 35;	  
	ChangeTemp = ChangeTemp/8;               	  
  motor_move(PRES,ChangeTemp);  	  
}
/***************************************************************

***************************************************************/
static void GasPres2(void) 
{  
	uint	ChangeTemp=0;
  ChangeTemp = gaspress2;	                
  if(ChangeTemp>250)
		ChangeTemp=250;
  ChangeTemp = ChangeTemp * 35;	  
	ChangeTemp = ChangeTemp/8;
  motor_move(PRES2,ChangeTemp);  	  
}
/**********************************************************

***********************************************************/
void back0_motor(uchar BigBack ) 
{
	delay1s=0x00;					///clear reset back time
	if(BigBack!=0)
	{
		resetback0 = 1;				///set reset back flag
		MotorFinalStep[PRES] = 1;
		MotorCurStep[PRES] = PRESMAXSTEP;
	  motor_move(PRES,0);

	  MotorFinalStep[TEMP] = 1;
		MotorCurStep[TEMP] = TEMPMAXSTEP;
	  motor_move(TEMP,0);
	      
	  MotorFinalStep[Oilplant] = 1;
		MotorCurStep[Oilplant] = OILMAXSTEP;
	  motor_move(Oilplant,0);    

	  MotorFinalStep[TACHOMETER] = 1;
		MotorCurStep[TACHOMETER] = TACHOMAXSTEP;
	  motor_move(TACHOMETER,0);  

		MotorFinalStep[Carspeed] = 1;        
		MotorCurStep[Carspeed] = CARSPEEDMAXSTEP;
	  motor_move(Carspeed,0);  

	  MotorFinalStep[PRES2] = 1;
		MotorCurStep[PRES2] = PRES2MAXSTEP;
	  motor_move(PRES2,0);
	}
	else
	{
		resetback0 = 3;				///set reset back flag
		MotorFinalStep[PRES] = 1;
		MotorCurStep[PRES] = 360;//PRESMAXSTEP>>3;
	  motor_move(PRES,0);

	  MotorFinalStep[TEMP] = 1;
		MotorCurStep[TEMP] = 360;//TEMPMAXSTEP>>3;
	  motor_move(TEMP,0);
	      
	  MotorFinalStep[Oilplant] = 1;
		MotorCurStep[Oilplant] = 360;//OILMAXSTEP>>3;
	  motor_move(Oilplant,0);    

	  MotorFinalStep[TACHOMETER] = 1;
		MotorCurStep[TACHOMETER] = 360;//TACHOMAXSTEP>>4;
	  motor_move(TACHOMETER,0);  

		MotorFinalStep[Carspeed] = 1;        
		MotorCurStep[Carspeed] = 360;//CARSPEEDMAXSTEP>>4;
	  motor_move(Carspeed,0);  

	  MotorFinalStep[PRES2] = 1;
		MotorCurStep[PRES2] = 360;//PRES2MAXSTEP>>3;
	  motor_move(PRES2,0);
	}
} 

#pragma CODE_SEG DEFAULT      
      

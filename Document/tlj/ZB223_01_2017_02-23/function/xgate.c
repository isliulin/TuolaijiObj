#include "xgate.h"
#define 	MOTOR_FRQ		30000
//uint32_t g_tachoMeterTimer,g_carSpeedTimer;	//指针定时计数
const char CosTbl[24]={85,91,100, 91,85,59,39, 19,0, 81, 61,41, 15, 9, 0, 9, 15, 41, 61,81, 100, 19, 39, 59};// 3
const char SinTbl[24]={85,59,39,  19,0, 81,61, 41,15,9,  0,  9, 15, 41,61,81,100,  19, 39,59, 85, 91,100,91};// 2

void motor_update(uchar channel, uchar dir)
{
    uint32_t *motor_channel1,*motor_channel2,*motor_channel3,*motor_channel4;
    int     ctemp=0,itemp=0;

    itemp = MotorFinalStep[channel] - MotorCurStep[channel];
    if (itemp!=0)
    {
        if (itemp<0)  MotorCurStep[channel]--;
        else if (itemp>0)  MotorCurStep[channel]++;       
        
        //pwm通道选择
		if(channel ==  Carspeed){
			motor_channel1 = &(FTM0->C6V);
			motor_channel4 = &(FTM0->C1V);

			motor_channel2 = &(FTM0->C7V);
			motor_channel3 = &(FTM0->C0V);
		}else if(channel == TACHOMETER){
			motor_channel1 = &(FTM0->C5V);
			motor_channel4 = &(FTM0->C2V);

			motor_channel2 = &(FTM0->C4V);
			motor_channel3 = &(FTM0->C3V);
		}else return;

        //motor_channel = (uchar*)(&MCDC0 + channel*2);

        ctemp = MotorCurStep[channel];
      	ctemp = ctemp%6;
      	if(channel ==TACHOMETER) ctemp = 5-ctemp;
        if(dir == CLOCKWISE)
        { 
           if(ctemp==5)//  (-0)
           {
             *motor_channel1 = FTM_PWM_Duty(MOTOR_FRQ,100);
             *motor_channel4 = FTM_PWM_Duty(MOTOR_FRQ,100);
             *motor_channel2 = 0;
             *motor_channel3 = 0;
           }
           else if(ctemp==4)  //  (0+)
           {
             *motor_channel1 = 0;
             *motor_channel4 = FTM_PWM_Duty(MOTOR_FRQ,100);
             *motor_channel2 = 0;
             *motor_channel3 = 0;
           }
           else if(ctemp==3)
           {  
             *motor_channel1 = 0;
             *motor_channel4 = FTM_PWM_Duty(MOTOR_FRQ,100);
             *motor_channel2 = FTM_PWM_Duty(MOTOR_FRQ,100);
             *motor_channel3 = FTM_PWM_Duty(MOTOR_FRQ,100);
			 /*
			 *motor_channel1 = FTM_PWM_Duty(3000,55);
             *motor_channel4 = FTM_PWM_Duty(3000,55);
             *motor_channel2 = FTM_PWM_Duty(3000,55);
             *motor_channel3 = FTM_PWM_Duty(3000,55);
			 
			 while(1);*/
           }
           else if(ctemp==2)   
           {
             *motor_channel1 = 0;
             *motor_channel4 = 0;
             *motor_channel2 = FTM_PWM_Duty(MOTOR_FRQ,100);
             *motor_channel3 = FTM_PWM_Duty(MOTOR_FRQ,100);
           }    
           else if(ctemp==1)
           {  
             *motor_channel1 = FTM_PWM_Duty(MOTOR_FRQ,100);
             *motor_channel4 = 0;
             *motor_channel2 = FTM_PWM_Duty(MOTOR_FRQ,100);
             *motor_channel3 = FTM_PWM_Duty(MOTOR_FRQ,100);  
           }
           else if(ctemp==0)   
           {
             *motor_channel1 = FTM_PWM_Duty(MOTOR_FRQ,100);
             *motor_channel4 = 0;
             *motor_channel2 = 0;
             *motor_channel3 = 0;
           }  	 	               				 
        }
    }
    else
    {
        if(channel == TACHOMETER) LPIT0_CLRTEN = LPIT_CLRTEN_CLR_T_EN_1_MASK;//TACHO = 0;
		else if(channel == Carspeed) LPIT0_CLRTEN = LPIT_CLRTEN_CLR_T_EN_2_MASK;//bCARSPEED = 0;
    }    
}

/****************************************************************************
*  motor_micro_update(char channel, char dir) -
*
* Description : Control motor drivers PWM value and polarity
*			  :  
* Example     : N/A  
* Input 	  : motor channel, direction
* Output	  : MotorCurStyep[?]
* Modify      : Motor driver PWM value and polarity
* Return      : N/A
****************************************************************************/
void motor_micro_update(uchar channel, uchar dir)
{
	uint32_t *motor_channel1,*motor_channel2,*motor_channel3,*motor_channel4;
	int		ctemp=0,itemp=0;

	if(dir == CLOCKWISE)
	{
	;
	}

	itemp = MotorFinalStep[channel] - MotorCurStep[channel];
	
	if (itemp!=0)
	{
		if (itemp<0)  
			MotorCurStep[channel]--;
		else if (itemp>0)  
			MotorCurStep[channel]++;

		//pwm通道选择
		if(channel ==  Carspeed){
			motor_channel1 = &(FTM0->C6V);
			motor_channel4 = &(FTM0->C1V);

			motor_channel2 = &(FTM0->C7V);
			motor_channel3 = &(FTM0->C0V);
		}else if(channel == TACHOMETER){
			motor_channel1 = &(FTM0->C5V);
			motor_channel4 = &(FTM0->C2V);

			motor_channel2 = &(FTM0->C4V);
			motor_channel3 = &(FTM0->C3V);
		}else return;
		//motor_channel= (uchar*)(&MCDC0 + channel*2);

		ctemp = MotorCurStep[channel];
		ctemp = ctemp%24;
		if(channel ==TACHOMETER) ctemp = 23-ctemp;
		if(ctemp<5)
		{
			*motor_channel1 = 0;
			*motor_channel4 = 0;
		}
		else if(ctemp<9)
		{
			*motor_channel1 = 0;//FTM_PWM_Duty(30000,100);
			*motor_channel4 = FTM_PWM_Duty(MOTOR_FRQ,100);
		}
		else if(ctemp<17)
		{
			*motor_channel1 = FTM_PWM_Duty(MOTOR_FRQ,100);
			*motor_channel4 = FTM_PWM_Duty(MOTOR_FRQ,100);
		}
		else if(ctemp<21)
		{
			*motor_channel1 = FTM_PWM_Duty(MOTOR_FRQ,100);
			*motor_channel4 = 0;//FTM_PWM_Duty(30000,100);
		}
		else if(ctemp<24)
		{
			*motor_channel1 = 0;
			*motor_channel4 = 0;
		}
		*motor_channel2 = FTM_PWM_Duty(MOTOR_FRQ,CosTbl[ctemp]);
		*motor_channel3 = FTM_PWM_Duty(MOTOR_FRQ,SinTbl[ctemp]);
	}	
	else
	{
		if(channel == TACHOMETER) LPIT0_CLRTEN = LPIT_CLRTEN_CLR_T_EN_1_MASK;//TACHO = 0;
		if(channel == Carspeed) LPIT0_CLRTEN = LPIT_CLRTEN_CLR_T_EN_2_MASK;//bCARSPEED = 0;
	} 


	/*
//pwm通道选择
		if(channel ==  TACHOMETER){
			motor_channel1 = &(FTM0->C6V);
			motor_channel4 = &(FTM0->C1V);

			motor_channel2 = &(FTM0->C7V);
			motor_channel3 = &(FTM0->C0V);
		}else if(channel == Carspeed ){
			motor_channel1 = &(FTM0->C5V);
			motor_channel4 = &(FTM0->C2V);

			motor_channel2 = &(FTM0->C4V);
			motor_channel3 = &(FTM0->C3V);
		}else return;
		//motor_channel= (uchar*)(&MCDC0 + channel*2);

		ctemp = MotorCurStep[channel];
		ctemp = ctemp%24;

		
		if(channel ==Carspeed) ctemp = 23-ctemp;
		if(ctemp<5)
		{
			*motor_channel1 = 0;
			*motor_channel4 = 0;
		}
		else if(ctemp<9)
		{
			*motor_channel1 = 0;//FTM_PWM_Duty(30000,100);
			*motor_channel4 = FTM_PWM_Duty(MOTOR_FRQ,100);
		}
		else if(ctemp<17)
		{
			*motor_channel1 = FTM_PWM_Duty(MOTOR_FRQ,100);
			*motor_channel4 = FTM_PWM_Duty(MOTOR_FRQ,100);
		}
		else if(ctemp<21)
		{
			*motor_channel1 = FTM_PWM_Duty(MOTOR_FRQ,100);
			*motor_channel4 = 0;//FTM_PWM_Duty(30000,100);
		}
		else if(ctemp<24)
		{
			*motor_channel1 = 0;
			*motor_channel4 = 0;
		}
		*motor_channel2 = FTM_PWM_Duty(MOTOR_FRQ,CosTbl[ctemp]);
		*motor_channel3 = FTM_PWM_Duty(MOTOR_FRQ,SinTbl[ctemp]);
	}	
	else
	{
		if(channel == Carspeed) LPIT0_CLRTEN = LPIT_CLRTEN_CLR_T_EN_1_MASK;//TACHO = 0;
		if(channel == TACHOMETER) LPIT0_CLRTEN = LPIT_CLRTEN_CLR_T_EN_2_MASK;//bCARSPEED = 0;
	}   


	*/
}
void init_motor_data(void)
{
    unsigned char i;
	//g_carSpeedTimer = 0;
	//g_carSpeedTimer = 0;
	for(i=0; i<6; i++)        //add 09-2-14
    {
        MotorFinalStep[i]=0;
        MotorCurStep[i]=0;
        MotorMinStep[i] = 0;                //motor head limit
    }
    MotorMaxStep[PRES] = PRESMAXSTEP;   //motor end limit
    MotorMaxStep[TEMP]= TEMPMAXSTEP;
    MotorMaxStep[Oilplant]= OILMAXSTEP;
    MotorMaxStep[TACHOMETER]= TACHOMAXSTEP;
    MotorMaxStep[Carspeed]= CARSPEEDMAXSTEP;  //add 09-2-14
    MotorMaxStep[PRES2]= PRES2MAXSTEP;

	
}
void init_motor_io(){ 
    FTM_PWM_init(ftm0, CH0, MOTOR_FRQ, 0);
    FTM_PWM_init(ftm0, CH1, MOTOR_FRQ, 0);
    FTM_PWM_init(ftm0, CH2, MOTOR_FRQ, 0);
    FTM_PWM_init(ftm0, CH3, MOTOR_FRQ, 0);
    FTM_PWM_init(ftm0, CH4, MOTOR_FRQ, 0);
    FTM_PWM_init(ftm0, CH5, MOTOR_FRQ, 0);
    FTM_PWM_init(ftm0, CH6, MOTOR_FRQ, 0);
    FTM_PWM_init(ftm0, CH7, MOTOR_FRQ, 0);
		
}
void init_motor(void){
	init_motor_data();
	init_motor_io();
	//back0_motor();
}

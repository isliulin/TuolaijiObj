#ifndef	_typedef_h
#define	_typedef_h
/************************************/
typedef	unsigned	char	UINT8;
typedef	unsigned	int	UINT16;
typedef	unsigned	long	UINT32;
/***************************/
typedef	union 
{
    struct
    {
    unsigned         ST0:1;             
    unsigned         ST1:1;             
    unsigned         ST2:1;                
    unsigned         ST3:1;             
    unsigned         ST4:1;             
    unsigned         ST5:1;             
    unsigned         ST6:1;             
    unsigned         ST7:1;                                  
    }b;
    unsigned     char    Data;
}StStruct;
/*******************************/
typedef	union
{
	unsigned	int	result;
	unsigned	char	adre[2];	
}ADStruct;
/*******************************/
typedef	union
{
  struct
  {
    unsigned         Key0:1;           
    unsigned         Key1:1;
    unsigned         Key2:1;           
    unsigned         Key3:1;
    unsigned         Key4:1;           
    unsigned         Key5:1;
    unsigned         Key6:1;           
    unsigned         Key7:1;
  }b;
  unsigned char      Data;
}SelfKeyStruct;
/**********************************/
typedef	union
{
	struct
	{
		unsigned     b0:1;
		unsigned     b1:1;
		unsigned     b2:1;		
	  unsigned     S:5;
	}b;
	unsigned  char Data;
}SelfIDStruct;
/*******************************/
typedef	union
{
    struct
    {
    unsigned         D0:1;             
    unsigned         D1:1;             
    unsigned         D2:1;             
    unsigned         D3:1;             
    unsigned         D4:1;             
    unsigned         D5:1;             
    unsigned         D6:1;            
    unsigned         D7:1; 
    }b;
    unsigned char   Data;
}ADErrorStruct;
/*********************************/
typedef	union 
{
    struct
    {
    unsigned         K0:1;             
    unsigned         K1:1;             
    unsigned         K2:1;             
    unsigned         K3:1;             
    unsigned         K4:1;             
    unsigned         K5:1;             
    unsigned         K6:1;             
    unsigned         K7:1;             
    }Key;
    
    unsigned char    Data;
}RecieveKeyStruct;
/******************************/
typedef	union
 {
    struct
    {
    unsigned         S0:2;             
    unsigned         S1:2;             
    unsigned         S2:2;             
    unsigned         S3:2;             
    }St;
       
    unsigned char    Data;
}LoadStruct;
/******************************/
typedef	union
{
	struct
	{
		unsigned		bit0:1;
		unsigned		bit1:1;
		unsigned		bit2:1;
		unsigned		bit3:1;
		unsigned		bit4:1;
		unsigned		bit5:1;
		unsigned		bit6:1;
		unsigned		bit7:1;	
	}bits;
	unsigned	char	Data;	
}SwLogicConfig;
/**************************************/
typedef	union
{
	struct
	{
		unsigned	Load0:1;
		unsigned	Load1:1;
		unsigned	Load2:1;
		unsigned	Load3:1;
		unsigned 	Load4:1;
		unsigned 	Load5:1;
		unsigned 	Load6:1;
		unsigned 	Load7:1;	
	}Loads;
	unsigned	char	Load;	
}PutLoadStruct;
/////////////////////////////////////////////
typedef struct
{
   unsigned long	identifier;
   unsigned char  data[8];
   unsigned char  byte_count;
}CAN_PACKET_T;

#endif

#ifndef __VARIABLE_H
#define __VARIABLE_H

#ifndef GLOBAL
#define GLOBAL
#include "global.h"
#endif

#define CALI_OK 4096*0
#define CALI_0 	4096*1
#define CALI_1 	4096*2

typedef struct 
{
	U8	year;	
	U8	month;	
	U8	day;
	U8	hour;
	U8	minute;
	U8	second;
}xTIME;

xTIME	xTime ;
//xTime.year= 15;xTime.month = 7;xTime.day = 20;xTime.hour = 20;xTime.miute = 7;xTime.second = 28;// {15,7,20,20,7,28};
//xTIME	current_Time = {15,7,20,20,7,28};
U32 SysTickConter;
U32 RxTimer;
float UaRms;
float UbRms;
float UcRms;
float IaRms;
float IbRms;
float IcRms;
U32 mcuID[3];
U8 	Uok[3];
U8 	Iok[3];
U32	u32Ugain0[3];
U32	u32Igain0[3];
U32	u32Ugain1[3];
U32	u32Igain1[3];

void cpuidGetId(void);
#endif

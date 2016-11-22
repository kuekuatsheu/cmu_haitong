#ifndef _WX_I2C_H
#define _WX_I2C_H
#include <stdio.h>
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"
#include "BSP_Oled.h"
//#include "Variable.h"
#if 0
#ifndef GLOBAL
#define GLOBAL
#include "general/global.h"
#endif
#ifndef GPIO
#define GPIO
#include "general/gpio.h"
#endif
#endif
#ifndef VARIABLE
#define VARIABLE
#include "general/Variable.h"
#endif

#define RTC_SCL TFT_B7
#define RTC_SDA I2C1_SDA
#define RTC_INTA MCASP0_AHCLKR	

void R8025t_Init(void);
U8 IIC_Write(U8 Addr,U8 *dat, U8 size);//IIC 往指定地址Address里写Data
U8 IIC_Read(U8 Addr, U8 *dat, U8 size);//IIC 读指定地址Address里的Data
void vRtcGetTime(void);
void vRtcSetTime(xTIME	time_temp);
void vRtcPrintfTime(U8 x,U8 y,xTIME	time_temp);

#endif

/*******************************************************************************************************
					endfile!!!
********************************************************************************************************/


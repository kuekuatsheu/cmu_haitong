#ifndef __OLED_H
#define __OLED_H	 	
#include<stdio.h>
//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"
#include <unistd.h>
#include <sys/syscall.h>
#if 1
#ifndef GPIO
#define GPIO
#include "general/gpio.h"
#endif

#ifndef GLOBAL
#define GLOBAL
#include "general/global.h"
#endif
#endif
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	
//---------------------------------------------
//							GND  ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PC1��SCL��
//              D1   ��PC0��SDA��
//              RES  ��PC3
//              DC   ��PC2
//              CS   ��PH2     
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_RST TFT_G7	//TFT_G7
#define OLED_DC	 TFT_G5	//TFT_G5
#define OLED_D0	 TFT_B5	//TFT_B5
#define OLED_D1	 TFT_B3	//TFT_B3
#define OLED_CS	 TFT_B1	//TFT_B1


#define OLED_CS_Clr()  wr2gpio(0,OLED_CS)//GPIO_ResetBits(GPIOH,GPIO_Pin_2)//CS
#define OLED_CS_Set()  wr2gpio(1,OLED_CS)//GPIO_SetBits(GPIOH,GPIO_Pin_2)

#define OLED_RST_Clr() wr2gpio(0,OLED_RST)//GPIO_ResetBits(GPIOC,GPIO_Pin_3)//RES
#define OLED_RST_Set() wr2gpio(1,OLED_RST)//GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define OLED_DC_Clr() wr2gpio(0,OLED_DC)//GPIO_ResetBits(GPIOC,GPIO_Pin_2)//DC
#define OLED_DC_Set() wr2gpio(1,OLED_DC)//GPIO_SetBits(GPIOC,GPIO_Pin_2)

#define OLED_SCLK_Clr() wr2gpio(0,OLED_D1)//GPIO_ResetBits(GPIOC,GPIO_Pin_1)//CLK
#define OLED_SCLK_Set() wr2gpio(1,OLED_D1)//GPIO_SetBits(GPIOC,GPIO_Pin_1)

#define OLED_SDIN_Clr() wr2gpio(0,OLED_D0)//GPIO_ResetBits(GPIOC,GPIO_Pin_0)//DIN
#define OLED_SDIN_Set() wr2gpio(1,OLED_D0) //GPIO_SetBits(GPIOC,GPIO_Pin_0)
 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//OLED�����ú���
void OLED_WR_Byte(U8 dat,U8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(U8 x,U8 y,U8 t);
void OLED_Fill(U8 x1,U8 y1,U8 x2,U8 y2,U8 dot);
void OLED_ShowChar(U8 x,U8 y,U8 chr);
void OLED_ShowNum(U8 x,U8 y,U32 num,U8 len,U8 size);
void OLED_ShowString(U8 x,U8 y, U8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(U8 x,U8 y,U8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 




/**********************************************************
 *  文件名称： OLED驱动文件
 *  功能简述： OLED驱动，主要参考中景园电子的例程修改而来
 *  创建时间： 20150601
 *  创建人员： 张春虎
 *	修		改： 
 ***********************************************************/	
#include "BSP_Oled.h"
#include "BSP_Oled_bmp.h"
#include "BSP_Oled_font.h"
#ifndef GLOBAL
#define GLOBAL
#include "general/global.h"
#endif
#ifndef VARIABLE
#define VARIABLE
#include "general/Variable.h"
#endif
extern xTIME xTime;
void OLED_WR_Byte(U8 dat,U8 cmd)
{	
	U8 i;			  
	if(cmd)	OLED_DC_Set();
	else 	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80) OLED_SDIN_Set();
		else  		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	U8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(U8 x,U8 y,U8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);				
			}
}
//m^n函数
U32 oled_pow(U8 m,U8 n)
{
	U32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(U8 x,U8 y,U32 num,U8 len,U8 size)
{         	
	U8 t,temp;
	U8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//显示一个字符号串
void OLED_ShowString(U8 x,U8 y,U8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(U8 x,U8 y,U8 no)
{      			    
	U8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//---------------------------------------------
//-----------------OLED端口定义----------------  	
void OLED_Init(void)
{ 		 
	U32 i;
	int request_number = 0;
	re_request:
	if(!gpio_request_o(OLED_RST)||!gpio_request_o(OLED_DC)||\
		!gpio_request_o(OLED_D1)||!gpio_request_o(OLED_D0)||\
		!gpio_request_o(OLED_CS)\
		)
	{
		printf("gpio request error!\r\n");
		request_number++;
		sleep(1);
		if(request_number < 5)
			goto re_request;
		
		
	}
printf("led request success!\r\n");
	gpio_direction_output_o(OLED_RST,0);	
	gpio_direction_output_o(OLED_DC,0);	
	gpio_direction_output_o(OLED_D1,0);	
	gpio_direction_output_o(OLED_D0,0);	
	gpio_direction_output_o(OLED_CS,0);	
 
	OLED_RST_Set();
	for(i=0;i<1000000;i++);
	OLED_RST_Clr();
	for(i=0;i<1000000;i++);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 
	//-------------------测试用
	OLED_ShowCHinese(13,0,1);
	OLED_ShowCHinese(31,0,2);
	OLED_ShowCHinese(49,0,3);
	OLED_ShowCHinese(67,0,4);
	OLED_ShowCHinese(85,0,5);
	OLED_ShowCHinese(102,0,6);
// 	OLED_ShowString(36,6,"LTU3000");
	sleep(5);
	//OLED_Clear();
	unsigned char number_1[] = {'h','a','r','d','w','a','r','e',':',' ','v','0','5','\0'};
	//unsigned char chars[] = {'a','B','.','c','D','v','A','\0'};
	unsigned char number_2[] = {'s','o','f','t','w','a','r','e',':',' ','v','1','.','0','1','\0'};
	OLED_ShowString(12,2,number_1);
	OLED_ShowString(12,4,number_2);
	//OLED_Show();
	//-------------------测试用
} 


void vPrintfUI	(void)
{
	U8 i;
	U32 u32UaRms,u32UbRms,u32UcRms;
	U32 u32IaRms,u32IbRms,u32IcRms;
	static U8 u8DisBuf[17];
	for(i=0;i<17;i++)u8DisBuf[i]='\0';
	u32UaRms=UaRms*10;
	u32UbRms=UbRms*10;
	u32UcRms=UcRms*10;
	u32IaRms=IaRms*10;
	u32IbRms=IbRms*10;
	u32IcRms=IcRms*10;
	u8DisBuf[0]='A';
	u8DisBuf[1]=':';
	u8DisBuf[2]=' ';
	u8DisBuf[3]=(u32UaRms/1000%10)+0x30;
	u8DisBuf[4]=(u32UaRms/100%10)+0x30;
	u8DisBuf[5]=(u32UaRms/10%10)+0x30;
	u8DisBuf[6]='.';
	u8DisBuf[7]=(u32UaRms%10)+0x30;
	u8DisBuf[8]='V';
	u8DisBuf[9]=' ';
	u8DisBuf[10]=(u32IaRms/1000%10)+0x30;
	u8DisBuf[11]=(u32IaRms/100%10)+0x30;
	u8DisBuf[12]=(u32IaRms/10%10)+0x30;
	u8DisBuf[13]='.';
	u8DisBuf[14]=(u32IaRms%10)+0x30;
	u8DisBuf[15]='A';
	OLED_ShowString(0,0,u8DisBuf);
	u8DisBuf[0]='B';
	u8DisBuf[1]=':';
	u8DisBuf[2]=' ';
	u8DisBuf[3]=(u32UbRms/1000%10)+0x30;
	u8DisBuf[4]=(u32UbRms/100%10)+0x30;
	u8DisBuf[5]=(u32UbRms/10%10)+0x30;
	u8DisBuf[6]='.';
	u8DisBuf[7]=(u32UbRms%10)+0x30;
	u8DisBuf[8]='V';
	u8DisBuf[9]=' ';
	u8DisBuf[10]=(u32IbRms/1000%10)+0x30;
	u8DisBuf[11]=(u32IbRms/100%10)+0x30;
	u8DisBuf[12]=(u32IbRms/10%10)+0x30;
	u8DisBuf[13]='.';
	u8DisBuf[14]=(u32IbRms%10)+0x30;
	u8DisBuf[15]='A';
	OLED_ShowString(0,3,u8DisBuf);
	u8DisBuf[0]='C';
	u8DisBuf[1]=':';
	u8DisBuf[2]=' ';
	u8DisBuf[3]=(u32UcRms/1000%10)+0x30;
	u8DisBuf[4]=(u32UcRms/100%10)+0x30;
	u8DisBuf[5]=(u32UcRms/10%10)+0x30;
	u8DisBuf[6]='.';
	u8DisBuf[7]=(u32UcRms%10)+0x30;
	u8DisBuf[8]='V';
	u8DisBuf[9]=' ';
	u8DisBuf[10]=(u32IcRms/1000%10)+0x30;
	u8DisBuf[11]=(u32IcRms/100%10)+0x30;
	u8DisBuf[12]=(u32IcRms/10%10)+0x30;
	u8DisBuf[13]='.';
	u8DisBuf[14]=(u32IcRms%10)+0x30;
	u8DisBuf[15]='A';
	OLED_ShowString(0,6,u8DisBuf);
}
//U8 buf_tty[128];	
#if 0
void main()
{
	unsigned char number[] = {'0','1','2','3','4','5','6','7','8','9','\0'};
	unsigned char chars[] = {'a','B','.','c','D','v','A','\0'};
	OLED_Init();
	//OLED_Clear();
	//OLED_Set_Pos(0,0); 
	OLED_ShowString(12,2,number);
	OLED_ShowString(12,4,chars);
	//OLED_Show();
}
#endif 

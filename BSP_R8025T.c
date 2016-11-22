/**********************************************************
 *  文件名称： R8025T驱动文件
 *  功能简述： R8025T驱动
 *  创建时间： 20150603
 *  创建人员： 张春虎
 *	修		改： 
 ***********************************************************/	
//INT			SCL			SDA
//PI1			PH15		PI0

#include "BSP_R8025T.h"

#define DELAY_IIC 10

#define IIC_Write_Address 0x64 //Write Address
#define IIC_Read_Address 0x65 //Read Address
#define DEBUG 0
//data wr
//#define IIC_SDA_SET_RD gpio_direction_input_o(RTC_SDA)
//#define IIC_SDA_SET_WR gpio_direction_output_o(RTC_SDA,1)

//#define IIC_SDA_DAT   rdfromgpio(RTC_SDA)
#if 0
#define IIC_SDA_DAT   rdfromgpio(RTC_SDA)
#define IIC_SDA_L   wr2gpio(0,RTC_SDA)//GPIO_ResetBits(GPIOI, GPIO_Pin_0)
#define IIC_SDA_H   wr2gpio(1,RTC_SDA)//GPIO_SetBits(GPIOI, GPIO_Pin_0)
#else
#define IIC_SDA_DAT   read_SDA()
#define IIC_SDA_L   write_SDA(0)//GPIO_ResetBits(GPIOI, GPIO_Pin_0)
#define IIC_SDA_H   write_SDA(1)//GPIO_SetBits(GPIOI, GPIO_Pin_0)
#endif 
//2clk
#define IIC_SCL_L   wr2gpio(0,RTC_SCL)//GPIO_ResetBits(GPIOH, GPIO_Pin_15)
#define IIC_SCL_H   wr2gpio(1,RTC_SCL)//GPIO_SetBits(GPIOH, GPIO_Pin_15)
void R8025t_Init(void)//IIC初始化程序
{
#if 0
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;											  
  GPIO_Init(GPIOH, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;											  
  GPIO_Init(GPIOI, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;											  
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#endif
	if(!gpio_request_o(RTC_SCL)||!gpio_request_o(RTC_SDA))
	{	
		printf("RTC request gpio error!\r\n");
	}
printf("RTC request gpio success!\r\n");
	gpio_direction_output_o(RTC_SCL,1);
//	gpio_direction_output_o(RTC_SDA,1);
//	gpio_direction_input_o(RTC_SDA);
//	gpio_direction_input_o(RTC_INTA);
	
	IIC_SDA_H;
	IIC_SCL_H;
	//-------------------测试用
//	xTime.year=15;
//	xTime.month=12;
//	xTime.day=25;
//	xTime.hour=17;
//	xTime.minute=5;
//	xTime.second=0;
//	vRtcSetTime(xTime);
	//-------------------测试用
}
void delay_5us(U16 num)//满足IIC时序的延时//10us--lyb
{
	U16 i,j;
	for(i=num;i>0;i--)
	{
		for(j=10;j>0;j--);//5
	}
}
int read_SDA(void)
{
	int ret = 0;
	gpio_direction_input_o(RTC_SDA);
	usleep(1);
	//usleep(DELAY_IIC);	
	ret = gpio_get_value_o(RTC_SDA);
	//gpio_direction_output_o(RTC_SDA,1);
//	printf("%d",ret);
//if(ret)
//printf("1");
	if(ret)	
		return 1;
	else
		return 0;
}
void write_SDA(int input)
{
	int ret = 0;
//	IIC_SCL_L;
	if(input)
		gpio_direction_output_o(RTC_SDA,1);
	else
		gpio_direction_output_o(RTC_SDA,0);
	usleep(DELAY_IIC);	
	//ret = gpio_set_value_o(RTC_SDA);
//	gpio_direction_input_o(RTC_SDA);
}	
void IIC_Start(void)//IIC开始信号
{
	IIC_SDA_H;
	IIC_SCL_H;
	usleep(DELAY_IIC);
//	delay_5us(1);
	IIC_SDA_L;
//	usleep(DELAY_IIC);
//	delay_5us(1);
	IIC_SCL_L;
}

void IIC_Stop(void)//IIC停止信号
{
	IIC_SCL_L;
	IIC_SDA_L;
//	usleep(DELAY_IIC);
	IIC_SCL_H;
	usleep(DELAY_IIC);
	IIC_SDA_H;
	usleep(DELAY_IIC);
//	delay_5us(1);
}

U8 IIC_Wait_ack(void)//IIC主器件等待应答  ?????
{
	U16 ErrTime=1000;
	U8 INT_IIC = 0;
	IIC_SDA_H;
	IIC_SCL_H;
	usleep(DELAY_IIC);
//	INT_IIC = rdfromgpio(RTC_INTA);
//	printf("INT_IIC:%d\r\n",INT_IIC);	
//	delay_5us(1);
//gpio_direction_input_o(RTC_SDA);//test-----lyb----------
	while(IIC_SDA_DAT)
	{
		if(ErrTime--==0)
		{
		IIC_Stop();
		printf("Wait return 0\r\n");
		return 0;
		}
		else usleep(DELAY_IIC);
	}
	IIC_SCL_L;
//	printf("Wait return 1");
	return 1;
}
void IIC_Send_ack(void)//IIC主器件发送应答
{
	IIC_SDA_L;
//	usleep(DELAY_IIC);
	IIC_SCL_H;
	usleep(DELAY_IIC);
	IIC_SCL_L;
}
void IIC_Send_noack(void)//IIC主器件发送非应答
{
	IIC_SDA_H;
	IIC_SCL_H;
	usleep(DELAY_IIC);
	IIC_SCL_L;
}
void IIC_Writebyte(U8 Data)//IIC写一个字节
{
	U8 i;
	for(i=8;i>0;i--)
	{
		 IIC_SCL_L;
		 if((Data&0x80)==0x80)
		 {
			IIC_SDA_H;
		 }
		 else
		 {
			IIC_SDA_L;
		 }
		 Data<<=1;
	usleep(DELAY_IIC);
		 IIC_SCL_H;
	usleep(DELAY_IIC);
	}
	IIC_SCL_L;
	IIC_SDA_H;
}
U8 IIC_Readbyte(void)//IIC读一个字节
{
	U8 i,Data=0;
	IIC_SDA_H;
	for(i=8;i>0;i--)
	{
		 Data<<=1;
		 IIC_SCL_L;
	usleep(DELAY_IIC);
		 IIC_SCL_H;
	usleep(DELAY_IIC);
		 Data|=IIC_SDA_DAT; //读管脚的状态
//	if(IIC_SDA_DAT) printf(".");
//	else	printf("/");
	}
//printf("::0x%02x",Data);
	IIC_SCL_L;
	usleep(DELAY_IIC);
//	printf("read:%x\r\n",Data);
	return(Data);
}
U8 IIC_Write(U8 Addr,U8 *dat, U8 size)//IIC 往指定地址Address里写Data
{
	IIC_Start();
	IIC_Writebyte(IIC_Write_Address);
	if (!IIC_Wait_ack())return 0;
	IIC_Writebyte(Addr);
	if (!IIC_Wait_ack())return 0;
	while (size--)
  	{
		IIC_Writebyte(*(dat++));;
		if (!IIC_Wait_ack())return 0;
	}
	IIC_Stop();
	return 1;
}

U8 IIC_Read(U8 Addr, U8 *dat, U8 size)//IIC 读指定地址Address里的Data
{
	IIC_Start();
	IIC_Writebyte(IIC_Write_Address);
	if (!IIC_Wait_ack())return 0;
	IIC_Writebyte(Addr);
	if (!IIC_Wait_ack())return 0;
	IIC_Start();
	IIC_Writebyte(IIC_Read_Address);
	if (!IIC_Wait_ack())return 0;
	size -= 1;
	while (size)
	{
		*(dat++) = IIC_Readbyte();
//printf(" 0x%02x",*(dat));
		if (size) IIC_Send_ack();
		size--;
	}
	*(dat++) = IIC_Readbyte();
//printf(" 0x%02x",*(dat));
	IIC_Send_noack();
	IIC_Stop();
	return 1;  
}
void vRtcSetTime(xTIME	time_temp)
{
	U8 u8Dat[7];
	u8Dat[6]=((time_temp.year/10)<<4)+(time_temp.year%10);
	u8Dat[5]=((time_temp.month/10)<<4)+(time_temp.month%10);
	u8Dat[4]=((time_temp.day/10)<<4)+(time_temp.day%10);
	u8Dat[2]=((time_temp.hour/10)<<4)+(time_temp.hour%10);
	u8Dat[1]=((time_temp.minute/10)<<4)+(time_temp.minute%10);
	u8Dat[0]=((time_temp.second/10)<<4)+(time_temp.second%10);
	IIC_Write(0,u8Dat,7);
}
void vRtcGetTime(void)
{	
	
	U8 u8Dat[7];
	//U8 *u8Dat_p = u8Dat;
//printf("into the method of vRtcGetTime!\r\n");
	IIC_Read(0,u8Dat,7);
	int i = 0;
//printf("the IIC_Read Byte = ");
#if DEBUG
for(i = 0;i<7;i++)
	printf("%d:0x%02x ",i,u8Dat[i]);
printf("\r\n");
#endif
	xTime.year 		= 10*(u8Dat[6]>>4)+(u8Dat[6]&0x0F);	
	xTime.month		= 10*(u8Dat[5]>>4)+(u8Dat[5]&0x0F);
	xTime.day			= 10*(u8Dat[4]>>4)+(u8Dat[4]&0x0F);
	xTime.hour		= 10*(u8Dat[2]>>4)+(u8Dat[2]&0x0F);
	xTime.minute	= 10*(u8Dat[1]>>4)+(u8Dat[1]&0x0F);
	xTime.second	= 10*(u8Dat[0]>>4)+(u8Dat[0]&0x0F);
//	return xTime;
}
void vRtcPrintfTime(U8 x,U8 y,xTIME	time_temp)//xianshi
{
	U8 i;
	static U8 u8SendBuf[11];
	OLED_Clear();
	for(i=0;i<11;i++)u8SendBuf[i]='\0';
	u8SendBuf[0]=(2000+xTime.year)/1000+0x30;
	u8SendBuf[1]=((2000+xTime.year)/100%10)+0x30;
	u8SendBuf[2]=((2000+xTime.year)/10%10)+0x30;
	u8SendBuf[3]=((2000+xTime.year)%10)+0x30;
	u8SendBuf[4]='-';
	u8SendBuf[5]=(xTime.month/10)+0x30;
	u8SendBuf[6]=(xTime.month%10)+0x30;
	u8SendBuf[7]='-';
	u8SendBuf[8]=(xTime.day/10)+0x30;
	u8SendBuf[9]=(xTime.day%10)+0x30;
	OLED_ShowString(x,y,u8SendBuf);
	for(i=0;i<11;i++)u8SendBuf[i]='\0';
	u8SendBuf[0]=(xTime.hour/10)+0x30;
	u8SendBuf[1]=(xTime.hour%10)+0x30;
	u8SendBuf[2]=':';
	u8SendBuf[3]=(xTime.minute/10)+0x30;
	u8SendBuf[4]=(xTime.minute%10)+0x30;
	u8SendBuf[5]=':';
	u8SendBuf[6]=(xTime.second/10)+0x30;
	u8SendBuf[7]=(xTime.second%10)+0x30;
	OLED_ShowString(x+8,y+2,u8SendBuf);
}
#if 0
void main()
{
	int i = 0;
	OLED_Init();
	R8025t_Init();
	
	for(i = 0;i<100;i++)
	{
#if 0
		int type;
		printf("\r\n>");
		scanf("%d",&type);
		if(type)// {IIC_SDA_H; IIC_SCL_H;}
			{//gpio_direction_output_o(RTC_SDA,1);gpio_direction_output_o(RTC_SCL,1);
			IIC_SDA_H;IIC_SCL_H;
			gpio_direction_output_o(OLED_1,1);gpio_direction_output_o(OLED_2,1);}
		else// {IIC_SCL_L; IIC_SDA_L;}
			{//gpio_direction_output_o(RTC_SDA,0);gpio_direction_output_o(RTC_SCL,0);
			IIC_SDA_L;IIC_SCL_L;
			gpio_direction_output_o(OLED_1,0);gpio_direction_output_o(OLED_2,0);}
#else
		vRtcGetTime();
		printf("the current time is:%d %d %d %d %d %d\r\n",xTime.year,xTime.month,xTime.day,xTime.hour,xTime.minute,xTime.second);
		vRtcPrintfTime(10,2,xTime);	
#endif
		sleep(2);
	}	
}
#endif
/*******************************************************************************************************
                                 end  file!!!
********************************************************************************************************/


//#define _GNU_SOURCE
#include<stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>

#include "general/gpio.h"
#ifndef GLOBAL
#define GLOBAL
#include "general/global.h"
#endif
#include "BSP_ATT7022EU.h"
#include "BSP_Oled.h"

//#include "general/Variable.h"
#if 0
#define __NR_gpio_request (__NR_SYSCALL_BASE+379)
#define __NR_gpio_direction_input (__NR_SYSCALL_BASE+380)
#define __NR_gpio_direction_output (__NR_SYSCALL_BASE+381)
#define __NR_gpio_set_value (__NR_SYSCALL_BASE+382)
#define __NR_gpio_get_value (__NR_SYSCALL_BASE+383)
#define __NR_gpio_free (__NR_SYSCALL_BASE+384)
#endif

#ifndef uchar
#define uchar unsigned char
#define uint unsigned int
#define xchar unsigned char
#endif
#ifndef Byte
#define Byte unsigned char
#endif

#define DELAYMS 3
#define N 1.4
#if 0
#define ATT_RST TFT_R4//0: for reset 1:work
#define SPI_CLK TFT_G0
#define SPI_DI TFT_R6
#define SPI_DO EMU1//PIN89
#define SPI_CS TFT_G2
#endif
#define KEY1 MCASP0_AXR0
#define KEY2 MCASP0_AXR1

#define RD_DELAY 5//us
#define W_DELAY 10//us
void delayms();
void delay(unsigned char uc_delyms);
//void wr2gpio(unsigned char value,int	gpio_NO);
//unsigned char spi_init();
//unsigned char spi_free();
//unsigned char rdfromgpio(unsigned int gpio_NO);
void WriteByte2Spi(Byte cmd,int value);
unsigned int  ReadBytesFromSpi(unsigned char cmd,int *value);
//_syscall1(int,gpio_request,int,arg1);
//int while_control = 1;
U32 DataUI;
float UIgain;
U8 u8DisplayID;
U8 u8AlreadyCali;

unsigned char spi_free()
{
	gpio_free_o(SPI_CS);
	gpio_free_o(SPI_CLK);
	gpio_free_o(SPI_DO);
	gpio_free_o(SPI_DI);
	gpio_free_o(ATT_RST);
}
void * thread_key_dect(void* argc)
{
	int ret_value_1 = 0;
	int ret_value_2 = 2;
	while(1)
	{
				
		printf("into while\r\n");
		ret_value_1 = (int)rdfromgpio(KEY1);
		ret_value_2 = (int)rdfromgpio(KEY2);
		printf("key1 value is :%d",ret_value_1);
		printf("key2 value is :%d",ret_value_2);
		usleep(500000);
	}
	return;

}
void * thread_write(void* argc)
{
	int i = 0;	
	printf("into the thread_wirte\r\n");
	sleep(2);
	ATT7022EU_Init();
  while (1)
  {
	{
		SysTickConter=0;
		//Ua
		ATT7022EU_ReadReg(0x0D,&DataUI);
			
		UaRms=(float)DataUI/8192;
		printf(" Ua: %f V ",UaRms);
			//Ia
		ATT7022EU_ReadReg(0x10,&DataUI);	
		IaRms=((float)DataUI/8192)/(float)N;
		printf("Ia: %f A\n",IaRms);
			//UB
		ATT7022EU_ReadReg(0x0E,&DataUI);	
		UbRms=(float)DataUI/8192;
		printf("Ub: %f V ",UbRms);
			//IB
		ATT7022EU_ReadReg(0x11,&DataUI);	
		IbRms=((float)DataUI/8192)/(float)N;
		printf("Ib: %f A \n",IbRms);
			//UC
		ATT7022EU_ReadReg(0x0F,&DataUI);	
		UcRms=(float)DataUI/8192;
		printf("Ua: %f V ",UcRms);
			//IC
		ATT7022EU_ReadReg(0x12,&DataUI);	
		IcRms=((float)DataUI/8192)/(float)N;
		printf("Ic: %f A \n",IcRms);
		//if(u8DisplayID==0) 
		//	;//vPrintfUI();
		//else if(u8DisplayID==1)	
		//	;//vPrintfMcuIDandUIgain();
		}
		//if(KEY1==0) u8DisplayID=1;
		OLED_Clear();
	//	OLED_ShowChar(12,2,'U');OLED_ShowNum(28,2,UaRms,7,1);
	//	OLED_ShowChar(12,4,'A');OLED_ShowNum(28,4,IaRms,8,1);
	vPrintfUI();
		sleep(1);
	}
	return;
}
void * thread_read(void* argc)
{
	OLED_Init();
	return;
}

int main()
{
	int i = 0;
	int j = 0;
	U8 cmd_1 = 0x00;
	U8 cmd_2 = 0x5D;
	int value = 0 ;
	int ret_value = 0;
	void *retval;
	pthread_t p_write,p_read,p_keydect,p_heart;

	//pthread_create(&p_keydect,NULL,thread_key_dect,0);	
	
//	pthread_join(p_keydect,&retval);
	
	pthread_create(&p_write,NULL,thread_write,0);	
	pthread_create(&p_read,NULL,thread_read,0);	
	
	pthread_join(p_write,&retval);
	pthread_join(p_write,&retval);
	
	//spi_free();
	return 0;
}




















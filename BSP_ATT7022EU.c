#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include "BSP_ATT7022EU.h"

#define delay_ATT 3
#define N 1.4 
//#include "general/gpio.h"
//#ifndef GLOBAL
//#define GLOBAL
//#include "general/global.h"
//#endif
/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
//SPI_CE	SPI_SCK		SPI_SO		SPI_SI			ATT_CS		ATT_REVP 
//PB12		PB13			PB14			PB15				PC4				PC5
unsigned char spi_init()
{
	if(!gpio_request_o(SPI_CS)&&!gpio_request_o(SPI_CLK)&&!gpio_request_o(SPI_DI)&&!gpio_request_o(SPI_DO)&&!gpio_request_o(ATT_RST))
	{
	printf("gpio_request error please check!\r\n");
	return -1;	
	}
	
	gpio_direction_output_o(SPI_CS,0);
	gpio_direction_output_o(SPI_CLK,0);
	gpio_direction_output_o(SPI_DI,0);
//	gpio_direction_output_o(SPI_DI,1);
	gpio_direction_output_o(ATT_RST,1);

	gpio_direction_input_o(SPI_DO);

//	gpio_direction_input_o(KEY1);
//	gpio_direction_input_o(KEY2);
	
	wr2gpio(0,ATT_RST);
	sleep(5);
	wr2gpio(1,ATT_RST);
	
	return 0;
}
#if 1 
void* p_update_VA_ATT(void * argc)
{
	U32 DataUI;
//	ATT7022EU_Init();

	while(1)
	{
		SysTickConter = 0;
		ATT7022EU_ReadReg(0x0D,&DataUI);
//	printf("read from 0x0D data=0x%02x",DataUI);		
		UaRms=(float)DataUI/8192;
//		printf(" Ua: %f V ",UaRms);
			//Ia
		ATT7022EU_ReadReg(0x10,&DataUI);	
		IaRms=((float)DataUI/8192)/(float)N;
//		printf("Ia: %f A\n",IaRms);
			//UB
		ATT7022EU_ReadReg(0x0E,&DataUI);	
		UbRms=(float)DataUI/8192;
//		printf("Ub: %f V ",UbRms);
			//IB
		ATT7022EU_ReadReg(0x11,&DataUI);	
		IbRms=((float)DataUI/8192)/(float)N;
//		printf("Ib: %f A \n",IbRms);
			//UC
		ATT7022EU_ReadReg(0x0F,&DataUI);	
		UcRms=(float)DataUI/8192;
//		printf("Ua: %f V ",UcRms);
			//IC
		ATT7022EU_ReadReg(0x12,&DataUI);	
		IcRms=((float)DataUI/8192)/(float)N;
//printf("IcRms = %f",IcRms);
//		printf("Ic: %f A \n",IcRms);
		//if(u8DisplayID==0) 
		//	;//vPrintfUI();
		//else if(u8DisplayID==1)	
		//	;//vPrintfMcuIDandUIgain();
		OLED_Clear();
		vPrintfUI();
		sleep(2);
	}
		
}
#endif
void ATT7022EU_ConfigWrite(void)
{
	ATT7022EU_WriteReg(0x81,0xB9FE);//模式选择
	ATT7022EU_WriteReg(0x83,0xF804);//EMU配置
	ATT7022EU_WriteReg(0x9E,0x016D);//高频脉冲输出数据：3200
	ATT7022EU_WriteReg(0xB1,0x3437);//模拟模块使能
	ATT7022EU_WriteReg(0x82,0x0155);//ADC增益选择
}
void ATT7022EU_CaliParaWrite(void)
{
	ATT7022EU_WriteReg(0xC9,0x00005A);//校表数据写使能
	ATT7022EU_WriteReg(0xA4,0x0037);//A相电压Offset校正
	ATT7022EU_WriteReg(0xA5,0x002C);//B相电压Offset校正
	ATT7022EU_WriteReg(0xA6,0x0040);//C相电压Offset校正
	ATT7022EU_WriteReg(0xA7,0x0007);//A相电流Offset校正
	ATT7022EU_WriteReg(0xA8,0x0007);//B相电流Offset校正
	ATT7022EU_WriteReg(0xA9,0x0007);//C相电流Offset校正
		
//	ATT7022EU_WriteReg(0x9A,u32Igain0[0]);//A相电流增益校正
//	ATT7022EU_WriteReg(0x9B,u32Igain0[1]);//B相电流增益校正
//	ATT7022EU_WriteReg(0x9C,u32Igain0[2]);//C相电流增益校正

//	ATT7022EU_WriteReg(0x97,u32Ugain0[0]);//A相电压增益校正
//	ATT7022EU_WriteReg(0x98,u32Ugain0[1]);//B相电压增益校正
//	ATT7022EU_WriteReg(0x99,u32Ugain0[2]);//C相电压增益校正
	
	ATT7022EU_WriteReg(0x97,0x4EB1);//A相电流增益校正
	ATT7022EU_WriteReg(0x98,0x4DCC);//B相电流增益校正
	ATT7022EU_WriteReg(0x99,0x4DE0);//C相电流增益校正

	ATT7022EU_WriteReg(0x9A,0xC37B);//A相电压增益校正
	ATT7022EU_WriteReg(0x9B,0xC3AE);//B相电压增益校正
	ATT7022EU_WriteReg(0x9C,0xC3EA);//C相电压增益校正

// 	ATT7022EU_WriteReg(0x84,0x8C47);//A相有功功率增益校正
// 	ATT7022EU_WriteReg(0x85,0x8C31);//B相有功功率增益校正
// 	ATT7022EU_WriteReg(0x86,0x8C5F);//C相有功功率增益校正

// 	ATT7022EU_WriteReg(0x87,0x8BF4);//A相无功功率增益校正
// 	ATT7022EU_WriteReg(0x88,0x8C03);//B相无功功率增益校正
// 	ATT7022EU_WriteReg(0x89,0x8C29);//C相无功功率增益校正
	ATT7022EU_WriteReg(0xC9,0x000000);//校表数据写禁止	
}
void ATT7022EU_Init(void)
{
  	U32 i;
	spi_init();
printf("+++++++++++++++++++into the ATT7022EU method \r\n");	  
	sATT7022EU_RST_LOW();
//	for(i=0;i<1000000;i++);
	sleep(1);
	sATT7022EU_RST_HIGH();
	sleep(1);
	ATT7022EU_WriteReg(0xC3,0x000000);//校表寄存器清零
	ATT7022EU_WriteReg(0xC9,0x00005A);//校表数据写使能
	ATT7022EU_ConfigWrite();
	ATT7022EU_CaliParaWrite();
#if 0	
	SSTF032B_RD(Uok,CALI_OK,3);
	SSTF032B_RD(Iok,CALI_OK+3,3);
	if(	(Uok[0]==0x5A)&&(Uok[1]==0x5A)&&(Uok[2]==0x5A)
		&&(Iok[0]==0x5A)&&(Iok[1]==0x5A)&&(Iok[2]==0x5A))
	{
		SSTF032B_RD((u8*)u32Ugain0,CALI_0,4*3);
		SSTF032B_RD((u8*)u32Igain0,CALI_0+12,4*3);
		SSTF032B_RD((u8*)u32Ugain1,CALI_1,4*3);
		SSTF032B_RD((u8*)u32Igain1,CALI_1+12,4*3);
		if(	(u32Ugain1[0]==u32Ugain0[0])&&(u32Ugain1[1]==u32Ugain0[1])&&(u32Ugain1[2]==u32Ugain0[2])
			&&(u32Igain1[0]==u32Igain0[0])&&(u32Igain1[1]==u32Igain0[1])&&(u32Igain1[2]==u32Igain0[2]))
		{
			ATT7022EU_CaliParaWrite();
		}
	}
#endif	
	ATT7022EU_WriteReg(0xC9,0x000000);//	
}
/*******************************************************************************
* Function Name  : ATT7022EU_ReadWriteByte
* Description    : 通过硬件SPI发送一个字节到ATT7022EU
* Input          : - data: 发送的数据
* Output         : None
* Return         : SST25VF032B 返回的数据
* Attention		 : None
*******************************************************************************/
#if 0
U8 ATT7022EU_ReadWriteByte(U8 data)		
{
  /*!< Loop while DR register in not empty */
  while (SPI_I2S_GetFlagStatus(sATT7022EU_SPI, SPI_I2S_FLAG_TXE) == RESET);
  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(sATT7022EU_SPI, data);
  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(sATT7022EU_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(sATT7022EU_SPI);                                            
}
#endif
/*******************************************************************************
* Function Name	: ATT7022EU_ReadReg
* Description	: Read ATT7022E	Register
* Input	  		: None 
* Output	 	: None
* Return	 	: None
*******************************************************************************/
void ATT7022EU_ReadReg(U8 cmd, U32 *value)
{
#if 0
	u32 i;
	sATT7022EU_CS_LOW();
	for(i=0;i<100;i++);
	ATT7022EU_ReadWriteByte(reg & 0x7F);        //写地址
	*data = ATT7022EU_ReadWriteByte(sATT7022EU_DUMMY_BYTE);         //读数据
	*data <<= 8;
	*data |= ATT7022EU_ReadWriteByte(sATT7022EU_DUMMY_BYTE);    
	*data <<= 8;
	*data |= ATT7022EU_ReadWriteByte(sATT7022EU_DUMMY_BYTE);
	for(i=0;i<100;i++);
	sATT7022EU_CS_HIGH();
	for(i=0;i<100;i++);
#endif
	int n = 0;
	int ret_read = 0;
	int spi_rd_value = 0;
	wr2gpio(1,SPI_CS);
	wr2gpio(0,SPI_CLK);
	wr2gpio(0,SPI_CS);
	usleep(delay_ATT);
//iopriintf("the cmd is:0x%x\r\n",cmd);	
	for(n=7;n>=0;n--)
	{
		wr2gpio(1,SPI_CLK);
		wr2gpio((cmd >> n)&0x01,SPI_DI);
		wr2gpio(0,SPI_CLK);
	}
	usleep(delay_ATT);
	
	for(n=23,*value  = 0;n>=0;n--)
	{
		spi_rd_value = 0;
		wr2gpio(1,SPI_CLK);
		spi_rd_value = rdfromgpio(SPI_DO);
//		printf(" %d",spi_rd_value);
		*value = ((*value) << 1)|spi_rd_value;
		wr2gpio(0,SPI_CLK);
	}
	usleep(delay_ATT);
	wr2gpio(1,SPI_CS);
	usleep(delay_ATT);
}

/*******************************************************************************
* Function Name	: ATT7022EU_WriteReg
* Description	: Write ATT7022E Register
* Input	  		: None 
* Output	 	: None
* Return	 	: None
*******************************************************************************/
void ATT7022EU_WriteReg(U8 cmd, U32 value)
{
#if 0
  U32 i;	
	sATT7022EU_CS_LOW();
	for(i=0;i<100;i++);	
	ATT7022EU_ReadWriteByte(reg | 0x80);     //写地址
	ATT7022EU_ReadWriteByte(dat >> 16);     //写数据  
	ATT7022EU_ReadWriteByte(dat >> 8);      
	ATT7022EU_ReadWriteByte(dat);   
	for(i=0;i<100;i++);
  sATT7022EU_CS_HIGH();
	for(i=0;i<100;i++);
#endif
	//printf("into WriteByte2Spi cmd is 0x%x,value is:%d-----",cmd,value);
	int n = 0;
	wr2gpio(1,SPI_CS);
	wr2gpio(0,SPI_CLK);
	wr2gpio(0,SPI_CS);
	usleep(delay_ATT);
	for(n=7;n>=0;n--)
	{
		wr2gpio(1,SPI_CLK);
		wr2gpio((cmd>>n)&0x01,SPI_DI);
		wr2gpio(0,SPI_CLK);
	}
	for(n=23;n>=0;n--)
	{
		wr2gpio(1,SPI_CLK);
		wr2gpio((value>>n)&0x01,SPI_DI);
		wr2gpio(0,SPI_CLK);
	}
	usleep(delay_ATT);
	wr2gpio(1,SPI_CS);
	usleep(delay_ATT);
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


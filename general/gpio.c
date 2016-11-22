#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <asm/termios.h>
#include <sys/syscall.h>
//#include <linux/delay.h>

//#include <pthread.h>
//#include "serial.h"

#include "global.h"


#define __NR_kmd (__NR_SYSCALL_BASE+378)
#define __NR_gpio_request (__NR_SYSCALL_BASE+379)
#define __NR_gpio_direction_input (__NR_SYSCALL_BASE+380)
#define __NR_gpio_direction_output (__NR_SYSCALL_BASE+381)
#define __NR_gpio_set_value (__NR_SYSCALL_BASE+382)
#define __NR_gpio_get_value (__NR_SYSCALL_BASE+383)
#define __NR_gpio_free (__NR_SYSCALL_BASE+384)

//#define GPRS_ONOFF 41//old
//#define GPRS_RST 5
#define GPRS_ONOFF 23//new
#define GPRS_RST 23

//U8 sign_read = 1;//for the thread_uart_read circle run sign 
//U8 exit_sign[] = {'1','1','0',0x0d,0x0a};
//U8 sign_write = 1,sign_read = 1;
//int data_sign = 0;
//static int n_com_port = ttyO4;//old
//static int n_com_port = ttyO2;//new
//int num_msg = 0;

#ifndef uchar
#define uchar unsigned char
#define uint unsigned int
#define xchar unsigned char
#endif
#ifndef Byte
#define Byte unsigned char
#endif

#define DELAYMS 40

//#define SPI_CLK 110
//#define SPI_DI 111
//#define SPI_DO 112
//#define SPI_CS 13

//int sim_900_release();
//_syscall1(int,gpio_request,int,arg1);
//int while_control = 1;


int gpio_request_o(unsigned int gpio_NO)
{
	int ret;
	ret = syscall(__NR_gpio_request,gpio_NO);
	return ret;
}

int gpio_direction_input_o(unsigned int gpio_NO)
{
	int ret;
	ret = syscall(__NR_gpio_direction_input,gpio_NO);
	return ret;
}
int gpio_direction_output_o(unsigned int gpio_NO,int value)
{
	int ret;
	ret = syscall(__NR_gpio_direction_output,gpio_NO,value);
	return ret;
}
void gpio_set_value_o(unsigned int gpio_NO,int value)
{
	syscall(__NR_gpio_set_value,gpio_NO,value);
	return ;
}
int gpio_get_value_o(unsigned int gpio_NO)
{
	int ret;
	//ret = syscall(ret,__NR_gpio_get_value,gpio_NO);
	ret = syscall(__NR_gpio_get_value,gpio_NO);
	return ret;
}
void gpio_free_o(unsigned int gpio_NO)
{
	syscall(__NR_gpio_free,gpio_NO);
	return;
}


void wr2gpio(unsigned char value,int	gpio_NO)
{
	if(0 == value)
		gpio_set_value_o(gpio_NO,0);
	else
		gpio_set_value_o(gpio_NO,1);
}

unsigned char rdfromgpio(unsigned int gpio_NO)
{
	if(gpio_get_value_o(gpio_NO))
		return 1;
	else
		return 0;
}
void led_init()
{
	gpio_direction_output_o(OLED_1,1);
	gpio_direction_output_o(OLED_2,1);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <asm/termios.h>
#include <sys/syscall.h>
//#include <linux/delay.hi

#include <pthread.h>

#include "adjust.h"
#define BEEP TFT_VSYNC
//#define GPRS_RST 77

U8 g_act_msg[][128] =
{
{0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x94,0x00,0xFF,0x16},//adjust right message
{0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0xD4,0x01,0x00,0xFF,0x16},//adjust wront act message
{}
}; 

static int adjust_port = ttyO1;//temp
int num_msg_adjust = 0;
INT32 fd_adjust;
//xTIME adjust_time;

#ifndef uchar
#define uchar unsigned char
#define uint unsigned int
#define xchar unsigned char
#endif
#ifndef Byte
#define Byte unsigned char
#endif

U8 getCS_adjust(U8* data,int len)
{
    U8 result = 0;
    int i = 0;
    for(i = 0; i < len; i++)
        result += *data++;
    return(result);
}

U8 adjust_time(U8* msg_byte)
{
	xTIME time_set;
	U8 *time_byte = msg_byte+10;
	U8 act_msg[28];
	U8 act_flag = 1;
	int i = 0;
	if(*(time_byte+7) != 0x16)
	{
		printf("message tail error!\r\n");
		return -1;
	}
	printf("time_byte:");
	for(i = 0;i<6;i++)
		(*(time_byte + i))-=0x33;
	printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\r\n",*time_byte,*(time_byte+1),*(time_byte+2),*(time_byte+3),*(time_byte+4),*(time_byte+5));
	time_set.second = (((*time_byte))/16) *10 + *(time_byte)%16;
	time_set.minute = ((*(time_byte+1))/16) *10 + *(time_byte+1)%16;
	time_set.hour = ((*(time_byte+2))/16) *10 + *(time_byte+2)%16;
	time_set.day = ((*(time_byte+3))/16) *10 + *(time_byte+3)%16;
	time_set.month = ((*(time_byte+4))/16) *10 + *(time_byte+4)%16;
	time_set.year = ((*(time_byte+5))/16) *10 + *(time_byte+5)%16;
	vRtcSetTime(time_set);
printf("adjust time successful\r\n!");
gpio_direction_output_o(BEEP,0);
sleep(1);
gpio_direction_output_o(BEEP,1);
	//send act message--------
	if(act_flag)
	{
		memcpy(act_msg,g_act_msg[0],12);
		memcpy(&act_msg[1],msg_byte+1,6);
		act_msg[10] = getCS_adjust(act_msg,10);
   //        	WriteComPort(act_msg,12,fd_adjust);
		printf("send message:");
		for( i= 0;i<12;i++) printf("0x%x ",act_msg[i]);
		printf("\r\n");
			
	}
	for(i = 0;i< 10;i++)
	{
		vRtcGetTime();
		vRtcPrintfTime(10,2,xTime);
		char time_sys[128];
		sprintf(time_sys,"date -s \"20%02d-%02d-%02d %d:%d:%d\"",xTime.year,xTime.month,xTime.day,xTime.hour,xTime.minute,xTime.second);
		system(time_sys);
		sleep(1);
	}
	return 0;
	
}
U8 msg_process(U8* msg_g,int len)
{
	U8 msg_type = 0x00;
	U8 C_byte = 0x00;
	U8 L_byte = 0x00;
	U8 *msg = msg_g;
	int i = 0;
	
	while(*msg == 0xFE)
	{
		msg++;
		msg_g++;
		i++;	
	}
	if(*msg == 0x68)
	{
		msg += 7;
		if(*msg == 0x68)
		{
			msg++;
			C_byte = *msg;msg++;
			L_byte = *msg;msg++;
			msg_type = C_byte;
			switch(C_byte)
			{
		//	case 0x14:
			case 0x08:
				msg += 10;
				adjust_time(msg_g);
				break;
			default:
			printf("the C_type unrecongnize C_type:0x%x\r\n",C_byte);
			}		
		}else{printf("message header_2 error!\r\n ");}
	}else{printf("message header error!\r\n");}

		
	return msg_type;
	
}

int sim_900_init()
{
        if(!gpio_request_o(GPRS_RST))
        {
        printf("gpio_request error please check!\r\n");
        return -1;
        }
    gpio_direction_output_o(GPRS_RST,0);
    sleep(5);
    gpio_direction_output_o(GPRS_RST,1);
    sleep(5);

//    system("pppd call gprs");

    return 0;
}


int adjust_init()
{
	int ret	;
	//sim_900_init();
    ret = OpenComPort(adjust_port, 1200, 8, "1", 'N',&fd_adjust);
    if (ret < 0) {
        fprintf(stderr, "Error: Opening Com Port %d\n", adjust_port);
        return -1;
    }else{
        printf("Open Com Port %d Success, Now going to read port\n", adjust_port);
    }

    return 0;
}
U8 process_MSG(U8* msg,int msg_len)
{
	
}
int adjust_release()
{
	
	CloseComPort(fd_adjust);
	return 0;
    //gpio_free_o(GPRS_RST);
    //system("sh /etc/ppp/ppp-off");
}
void * thread_adjust_read(void * argc)
{
    U8 len_tty = 0,i = 0;
    U8 buf_tty[256];
        while(1)
        {
           bzero(buf_tty,sizeof(buf_tty));
           len_tty = ReadComPort_byte(buf_tty,128,fd_adjust,2);

            if (len_tty < 0) 
            {
                printf("Error: Read Com Port\n");
                break;
            }

            if (len_tty == 0) 
            {
//                write(STDOUT_FILENO, ".", sizeof("."));
                continue;
            }
	num_msg_adjust++;
	//printf("receive the NO.%d message\n",num_msg);
        //printf("Recv: %d bytes, %s\r\n", len_tty, buf_tty);
	printf("receive MSG the message is:");
	for( i = 0;i<len_tty+12;i++)
		printf(" 0x%x",buf_tty[i]);
	printf("\n");
	
msg_process(buf_tty,len_tty);
            //sleep(1);
        }
}
void * thread_adjust_write(void *	argc)
{
        U8 len_tty = 0,len_cmd = 0;
        U8 buf_cmd[128];
	U8 data_tail[]={0x1a};
        U8 i;
        while(1)
        {
            len_cmd = 0;
            bzero(buf_cmd,sizeof(buf_cmd));
            
            printf("-->");
            scanf("%s",buf_cmd);
            
            len_cmd = strlen(buf_cmd);
            buf_cmd[len_cmd] =0x0d;
            buf_cmd[len_cmd+1] =0x0a;
            
            printf("buf_cmd is");
            for(i = 0;i < len_cmd+2;i++)
                printf("0x%x",buf_cmd[i]);
            printf("\r\n");
            
                len_cmd = WriteComPort(buf_cmd, (len_cmd + 2),fd_adjust);
                
                if (len_cmd < 0) 
                {
                    printf("Error: WriteComPort Error\n");
                }
                else{
                    printf("write %s to uart\n",buf_cmd);
                }
            sleep(1);
        }
}
void adjust_close()
{
	CloseComPort(fd_adjust);
}
#if 0
int main (int argc, char *argv[])
{
    int ret = -1;
    int i = 0;
    int len_tty = -1,len_cmd = -1;
    //int fd_rs485;
    pthread_t p_write,p_read;
    void *retval;

	R8025t_Init();
    	if(adjust_init())
	{
		printf("serial port init error!\r\n");
		return -1;
	}
	pthread_create(&p_write,NULL,thread_adjust_write,0);
   	pthread_create(&p_read,NULL,thread_adjust_read,0);
    
    	pthread_join(p_write,&retval);
    	pthread_join(p_read,&retval);

//    adjust_release();
    return(0);
}
#endif

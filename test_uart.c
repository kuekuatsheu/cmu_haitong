#include "plc.h"
#include "BSP_ATT7022EU.h"
#include <string.h>
#include <signal.h>

#define WAIT_SECOND 5
#define debug_flg 1
#define ltu_number 32
#define N 1.4
//extern int send_alarm(int ltu_num,int type);
#define VA_delay 20

U8 g_get_ltu_number_flag = 0;
U8 g_flg_03 = 0;
U32 DataUI;
float UIgain;
sem_t sem_plc;
//int ltu_number = 10;
extern xTIME xTime;
extern U8 socket_status;
extern  U8 LTU_number_socket[][6];
extern U8 g_cmu_NO[6];
extern void *p_update_VA_ATT(void* argc);
extern void ATT7022EU_Init(void);

int g_recv_msg_num[ltu_number] = {0};
U8 g_ltu_VA_current[ltu_number][13] = {0};
int plc_recv_number = 0;
char log_buffer[256];
int g_ltu_num = 0;
char g_ltu_array_char[100][5][28];//100-ltu_number 5-ltu's attribute 28-attribute's max length
char g_cmu_array_char[28];
int fd_ATT;

void watch_ltu_cmd(U8* addr,U8 ltu_NO);
void message_process_test_();
#if 0
#endif
void shutdown_route()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;
	len_cmd =(((int)CCO_data[2][2]) << 8) +(int)CCO_data[2][1]; 
        memcpy(buf_cmd,CCO_data[2],len_cmd); 
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
       	ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
//--	logfile(1,"stop route cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
//--	logfile(1,log_buffer);
	printf("stop route cmd len: %d buf_cmd is",len_cmd);
//        for(i = 0;i < len_cmd;i++)
//            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
           
		
}
void restart_route()
{
	int ret_len = 0;
	int i = 0;
	U8 buf_cmd[128];
	int len_cmd = 0;		
        len_cmd =(((int)CCO_data[5][2]) << 8) +(int)CCO_data[5][1]; 
        memcpy(buf_cmd,CCO_data[5],len_cmd); 
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
//--	logfile(1,"restart route cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
//--	logfile(1,log_buffer);
        printf("restart route cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
	
}
void start_route()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;		
        len_cmd =(((int)CCO_data[7][2]) << 8) +(int)CCO_data[7][1]; 
        memcpy(buf_cmd,CCO_data[7],len_cmd); 
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
//	logfile(1,"start route cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
//	logfile(1,log_buffer);
        printf("start route cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
	
}
void renew_route()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;		
        len_cmd =(((int)CCO_data[7][2]) << 8) +(int)CCO_data[5][1]; 
        memcpy(buf_cmd,CCO_data[5],len_cmd); 
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
	//logfile(1,"renew route cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
	//logfile(1,log_buffer);
        printf("restart route cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
	
}
void get_ltu_number()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;	
        len_cmd =(((int)CCO_data[3][2]) << 8) +(int)CCO_data[3][1]; 
        memcpy(buf_cmd,CCO_data[3],len_cmd); 
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
//	logfile(1,"get_ltu_number cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
//	logfile(1,log_buffer);
        printf("get ltu number cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
	
}
void get_ltu_info()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;
        len_cmd =(((int)CCO_data[4][2]) << 8) +(int)CCO_data[4][1]; 
        memcpy(buf_cmd,CCO_data[4],len_cmd); 
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
//--	logfile(1,"get_ltu_info cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
//--	logfile(1,log_buffer);
        printf("get ltu info cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
	
}
void set_cmu_number()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;
        len_cmd =(((int)CCO_data[12][2]) << 8) +(int)CCO_data[12][1]; 
        memcpy(buf_cmd,CCO_data[12],len_cmd); 
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
	//logfile(1,"set CMU NO. cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
	//logfile(1,log_buffer);
        printf("set cmu number cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
	
}
void add_ltu_byone(U8* addr)
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;	
	int ret_len = 0;
        len_cmd =(((int)CCO_data[13][2]) << 8) +(int)CCO_data[13][1]; 
        memcpy(buf_cmd,CCO_data[13],len_cmd); 
//        *(buf_cmd+13) = 2;
	memcpy(buf_cmd+14,addr,6);
	buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
	//logfile(1,"add_ltu_byone cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
	//logfile(1,log_buffer);
	
        printf("add lte byone cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
	printf("\r\n");
	
}
void get_cmu_number_cmd()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;
        len_cmd =(((int)CCO_data[1][2]) << 8) +(int)CCO_data[1][1]; 
        memcpy(buf_cmd,CCO_data[1],len_cmd); 
//	memcpy(buf_cmd+16,addr,6);
//	memcpy(buf_cmd+30,addr,6);
//	*(buf_cmd+43) = getCS((buf_cmd+23),14);
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
//--	logfile(1,"get_cmu_number cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
//--	logfile(1,log_buffer);
        printf("get_cmu_number_cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
	
}
/*

*/
U8 get_ltu_VA(U8* addr,U8 ltu_NO,U8* VA,int size_VA)
{
	U8 ret = 0,i = 0;
	int count = 0;
	int count_second = 0;
	U8 VA_temp[128];
	int msg_number = g_recv_msg_num[ltu_NO]; 
//	printf("into the method of get_ltu_VA before watch_ltu_cmd\r\n");
	char log_temp[256];
	sprintf(log_temp,"AAAAAAAAAAAAAAthe ltu address=%02x%02x%02x%02x%02x%02x\r\n",*(addr+5),*(addr+4),*(addr+3),*(addr+2),*(addr+1),*(addr));
	printf(log_temp,"AAAAAAAAAAAAAAthe ltu address=%02x%02x%02x%02x%02x%02x\r\n",*(addr+5),*(addr+4),*(addr+3),*(addr+2),*(addr+1),*(addr));
//	logfile(6,log_temp);
	watch_pos:
	count_second = 0;
	watch_ltu_cmd(addr,ltu_NO);
//	printf("into the method of get_ltu_VA after watch_ltu_cmd\r\n");
	while((msg_number == g_recv_msg_num[ltu_NO])&&count_second < 10)
	{
		sleep(1);
		count_second++;
	}
	if(msg_number == g_recv_msg_num[ltu_NO])
	{	
		printf("retry get ltu_VA count = %d\r\n",count);
		if(count < 2)
		{
		count++;
		printf("retry to get the VandA\r\n");	
		goto watch_pos;
		}else 
		{
			
			memset(log_buffer,0,256);
			sprintf(log_buffer,"%s","get_ltu_VA:get ltu information=");
			sprintf(log_buffer,"%s %s",log_buffer,"ltu:");
			for(i = 0;i < 6 ;i++)
			{
				sprintf(log_buffer,"%s%02x",log_buffer,LTU_number_1[ltu_NO][5-i]);
			}
			sprintf(log_buffer,"%s %s",log_buffer,"failed");
			logfile(6,log_buffer);
			return -1;
		}
		
	}else
	{
		memcpy(VA,g_ltu_VA_current[ltu_NO],size_VA);

		memset(log_buffer,0,256);
		sprintf(log_buffer,"%s","get_ltu_VA:get ltu information=");
		sprintf(log_buffer,"%s %s",log_buffer,"ltu:");
		for(i = 0;i < 6 ;i++)
		{
			sprintf(log_buffer,"%s%02x",log_buffer,LTU_number_1[ltu_NO][5-i]);
		}
		sprintf(log_buffer,"%s %s",log_buffer,"V_A array[]:");
		for(i = 0;i < 13;i++)
			sprintf(log_buffer,"%s %02x",log_buffer,g_ltu_VA_current[ltu_NO][i]);
		logfile(6,log_buffer);
	}
	
	return ret;
}
void watch_ltu_cmd(U8* addr,U8 ltu_NO)
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	static j = 0;
	int ret_len = 0;
	U8 data_DI[16][4] = {{0x00,0xff,0xff,0x00},\
				{0x00,0xff,0x01,0x02},\
				{0x00,0xff,0x02,0x02},\
				{0x00,0x01,0x03,0x02},\
				{0x00,0x01,0x04,0x02},\
			//	{0x00,0xff,0x05,0x02},
				{0x01,0x01,0x0a,0x02},\
				{0x02,0x01,0x0a,0x02},\
				{0x03,0x01,0x0a,0x02},\
				{0x04,0x01,0x0a,0x02},\
				{0x01,0x02,0x0a,0x02},\
				{0x02,0x02,0x0a,0x02},\
				{0x01,0x03,0x0a,0x02},\
				{0x02,0x03,0x0a,0x02},\
				{0x00,0xff,0xff,0x00},\
				/*{0x00,0xff,0x02,0x03}*/};
        len_cmd =(((int)CCO_data[16][2]) << 8) +(int)CCO_data[16][1]; 
        memcpy(buf_cmd,CCO_data[16],len_cmd); 
	buf_cmd[9] = msg_sequence[ltu_NO]++; 
	memcpy(buf_cmd+16,addr,6);
	memcpy(buf_cmd+30,addr,6);
//
#if 1 
	memcpy(buf_cmd+39,data_DI[0],4);
	j++;
	if(j>5 || j==5) j = 0;
	for(i = 0;i<4/*buf_cmd[38]*/;i++)
		buf_cmd[39+i]+=0x33;
#endif
//
	*(buf_cmd+43) = getCS((buf_cmd+29),14);
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
//	logfile(1,"pull_ltu_info cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%02x",log_buffer,buf_cmd[i]);
	}
//	logfile(1,log_buffer);
#if debug_flg 
       printf("watch ltu info cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%02x",buf_cmd[i]);
        printf("\r\n");
#endif
}
#if 0
void config_ltu_cmd(U8* addr)
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;
        len_cmd =(((int)CCO_data[19][2]) << 8) +(int)CCO_data[19][1]; 
        memcpy(buf_cmd,CCO_data[19],len_cmd); 
//	buf_cmd[9] = msg_sequence[ltu_NO]++; 
	memcpy(buf_cmd+16,addr,6);
	memcpy(buf_cmd+30,addr,6);
	*(buf_cmd+43) = getCS((buf_cmd+29),22);
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd);
        printf("watch ltu info cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
	
}
#endif
void allow_ltu_send()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;
        len_cmd =(((int)CCO_data[18][2]) << 8) +(int)CCO_data[18][1]; 
        memcpy(buf_cmd,CCO_data[18],len_cmd); 
	//memcpy(buf_cmd+16,addr,6);
//	memcpy(buf_cmd+30,addr,6);
//	*(buf_cmd+43) = getCS((buf_cmd+29),14);
        buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
	logfile(1,"allow ltu initiative cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
	logfile(1,log_buffer);
        printf("allow ltu send cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
}
void send_0x03_msg()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;	
	int ret_len = 0;
        len_cmd =(((int)CCO_data[0][2]) << 8) +(int)CCO_data[0][1]; 
        memcpy(buf_cmd,CCO_data[0],len_cmd); 
      //  	memcpy(buf_cmd+14,addr,6);
	buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
	memset(log_buffer,0,256);
	logfile(1,"send 0x03 message:");	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%x",log_buffer,buf_cmd[i]);
	}
	logfile(1,log_buffer);
        printf("get factory info cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
	printf("\r\n");
	
}
/*not used the send_0x03_msg()have the same function*/
void set_cco_cmu_addr()
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;	
	int ret_len = 0;	
        len_cmd =(((int)CCO_data[0][2]) << 8) +(int)CCO_data[0][1]; 
        memcpy(buf_cmd,CCO_data[0],len_cmd); 
      //  	memcpy(buf_cmd+14,addr,6);
	buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
        printf("set_cco_cmu_addr cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
	printf("\r\n");
}
U8 plc_ltu_add()
{
	int num_cmd = 0;
	int i = 0;
	U8 buf_tty[128];
	int len_tty = 0;
	int plc_recv_number_bef = 0;
	
	printf("into ltu_add_1\r\n");
//plc_on_sign:
	send_0x03_msg();//get infomation
	while(!plc_power_on && i < 10)
	{
	//	send_0x03_msg();
		sleep(1);
		i++;
	//add message process if no message recv
	//	goto plc_on_sign;// 
	}
	//shutdown the route
printf("in plc_ltu_add 1 \r\n");
	plc_recv_number_bef = plc_recv_number;
printf("in plc_ltu_add 2 \r\n");
	shutdown_route();
	while(plc_recv_number == plc_recv_number_bef)
		sleep(1);
#if 0	//get ltu number
	plc_recv_number_bef = plc_recv_number;
	get_ltu_number();
	while(plc_recv_number == plc_recv_number_bef)
		sleep(1);
#endif	
	plc_recv_number_bef = plc_recv_number;
	set_cmu_number();
	while(plc_recv_number == plc_recv_number_bef)
		sleep(1);
	
	sleep(2);
	get_cmu_number_cmd();//sure  about the set_cum_number() command
	//if(ltu_number != 10)
	sleep(2);
//add ltu one by one
printf("+++++++++++++++++++the g_ltu_num = %d",g_ltu_num);
	for(i = 0;i<g_ltu_num;i++)
	{	
		plc_recv_number_bef = plc_recv_number;
		add_ltu_byone(LTU_number_1[i]);			
		while(plc_recv_number == plc_recv_number_bef)
			sleep(1);
	}
	get_ltu_info();//make sure about the ltu information	
	//restart_route();	
	return ;
}
void set_sync_time_msg(const U8* recvbuf)
{
	U8 buf_cmd[128];
	int len_cmd = 0;
	int i = 0;
	int ret_len = 0;	
        len_cmd =(((int)CCO_data[6][2]) << 8) +(int)CCO_data[6][1]; 
        memcpy(buf_cmd,CCO_data[6],len_cmd); 
        buf_cmd[9] = *(recvbuf+9);
	buf_cmd[13] = xTime.second/10 * 16 + xTime.second%10;
        buf_cmd[14] = xTime.minute/10 * 16 + xTime.minute%10;
        buf_cmd[15] = xTime.hour/10 * 16 + xTime.hour%10;
        buf_cmd[16] = xTime.day/10 * 16 + xTime.day%10;
        buf_cmd[17] = xTime.month/10 * 16 + xTime.month%10;
        buf_cmd[18] = xTime.year/10 * 16 + xTime.year%10;
	buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
        ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
        printf("sync the time  cmd len: %d buf_cmd is",len_cmd);
        for(i = 0;i < len_cmd;i++)
            printf(" 0x%x",buf_cmd[i]);
        printf("\r\n");
	sleep(2);
	start_route();	
}
U8 get_14_msg_01(U8* buffer)
{
	int ret = 0;
	int i = 0;
	int len_cmd = 0;
	int msg_number = *(buffer+9);
	U8 msg_send[256];
	U8 msg_645[128];
	printf("into 14 message\r\n");
        len_cmd =(((int)msg_send[2]) << 8) +(int)msg_send[1]; 
	printf("the 0x14 message lenght :%d\r\n",len_cmd);
	memcpy(msg_send,CCO_data[11],35);
	*(msg_send+9) = msg_number;
	memcpy(msg_645,msg_send+16,16);//645 begin from the 16s bytes 	
	*(msg_645+14) = getCS(msg_645,14);
	memcpy(msg_send+16,msg_645,16);
        *(msg_send+len_cmd-2) = getCS(msg_send+3,len_cmd-5); 
        WriteComPort(msg_send, len_cmd,fd_ATT);
            printf("send 0x14 msg :");
            for(i = 0;i < len_cmd;i++)
                printf(" 0x%x",msg_send[i]);
            printf("\r\n");
	
	//*(msg_send+len_cmd -2) = getCS();
	
}
int get_10_msg(const U8* buffer)
{
	int number_ret = 0;
	if(*(buffer+11) == 0x01 && *(buffer+12) == 0x00) // 10 01
	{
		g_get_ltu_number_flag = 1;
		number_ret = (int)(*(buffer+13)) + (((int)*(buffer+14)) << 8);
	}else if(*(buffer+11) == 0x02 && *(buffer+12) == 0x00)//10 02
	{
	int num_ltu = 0;
	int num_act = 0;
	num_ltu = (int)(*(buffer+13)) + ((int)(*(buffer+14))<<8);
	num_act = *(buffer+15);		
		
	}
	return number_ret;
}
U8 get_03_msg(const U8* buffer)
{
	U8 ret = 0;
//	int DT_m = *(buffer+12) * 8 + *(buffer+11)/2;
	if(*(buffer+11) == 0x02 && *(buffer+12) == 0x01)
	{
				plc_power_on = 1;
	}
printf("0x03 message over\r\n");
	return ret;
}
U8 get_645(U8* buffer,int len)
{
	int ret = 0;
	int i = 0;
	U8 log_buffer[256];
	int len_data = 0;
	int pos_data = 10;
	U8 addr_ltu[6] = {0x00};
	U8 U_ltu[6];
	U8 A_ltu[6];
#if 0
printf("the 645 data len = %d is:",len);
for(i = 0;i<len;i++)
printf("0x%02x",buffer[i]);
printf("\r\n");
#endif
	memcpy(addr_ltu,buffer+1,6);
	len_data = *(buffer+9);
#if 0
for(i = 0;i < ltu_number;i++)
{
	if(0 == memcmp(addr_ltu,ltu_add_1[i],6))
	{	
		g_recv_msg_num[i]++;
		memcpy(g_ltu_VA_current[i]);
	}
/*
int g_recv_msg_num[ltu_number] = {0};
U8 g_ltu_VA_current[ltu_number][13] = {0};
*/
}
#endif

//printf("==============after tranmit:");
printf("645 frame data:");
	for(i=0;i<len_data;i++)
	{
		*(buffer+pos_data+i)-=0x33;
printf("%02x",*(buffer+pos_data+i));
//printf(" 0x%02x",*(buffer+pos_data+i));
	}
printf("\r\n");
if(*(buffer+9) == 0x11)
{
	memcpy(U_ltu,buffer+14,6);
	memcpy(A_ltu,buffer+20,6);
}else
{
memcpy(U_ltu,buffer+10,6);
memcpy(A_ltu,buffer+16,6);
}
#if 1 	
	sprintf(log_buffer,"%s","the U_ltu & A_ltu =");
	for(i = 0;i<6;i++)
	{
		sprintf(log_buffer,"%s 0x%02x",log_buffer,U_ltu[i]);
	}
	for(i = 0 ;i<6;i++)
	{
		sprintf(log_buffer,"%s 0x%02x",log_buffer,A_ltu[i]);
	}
	logfile(1,log_buffer);
#endif
	float Ua,Ub,Uc,Aa,Ab,Ac;
	Ua = (float)((((int)U_ltu[1]) << 8)+(int)U_ltu[0])*0.1;
	Ub = (float)((((int)U_ltu[3]) << 8)+(int)U_ltu[2])*0.1;
	Uc = (float)((((int)U_ltu[5]) << 8)+(int)U_ltu[4])*0.1;
	Aa = (float)((((int)A_ltu[1]) << 8)+(int)A_ltu[0])*0.1;
	Ab = (float)((((int)A_ltu[3]) << 8)+(int)A_ltu[2])*0.1;
	Ac = (float)((((int)A_ltu[5]) << 8)+(int)A_ltu[4])*0.1;
	int Ua_10 = Ua*10,Ub_10 = Ub*10,Uc_10 = Uc*10,Aa_10 =Aa*10 ,Ab_10 = Ab*10,Ac_10 = Ac*10;
printf("the Ua_10=%d Ub_10 =%d Uc_10 = %d Ua_10/1000<<4+(Ua_10%1000)/100 = %x\r\n",Ua_10,Ub_10,Uc_10,(Ua_10/1000)<<4 + (Ua_10%1000)/100);
printf("the Aa_10=%d Ab_10 =%d Ac_10 = %d Aa_10/1000<<4+(Aa_10%1000)/100 = %x\r\n",Aa_10,Ab_10,Ac_10,(Aa_10/1000)<<4 + (Aa_10%1000)/100);
	U_ltu[1] =((U8)((Ua_10/1000)<<4) + (Ua_10%1000)/100);
	U_ltu[0] =((U8)((Ua_10%100)/10)<<4) + Ua_10%10; 

	U_ltu[3] =((U8)((Ub_10/1000)<<4) + (Ub_10%1000)/100);
	U_ltu[2] =((U8)((Ub_10%100)/10)<<4) + Ub_10%10; 
	U_ltu[5] =((U8)((Uc_10/1000)<<4) + (Uc_10%1000)/100);
	U_ltu[4] =((U8)((Uc_10%100)/10)<<4) + Uc_10%10; 

	A_ltu[1] =((U8)((Aa_10/1000)<<4) + (Aa_10%1000)/100);
	A_ltu[0] =((U8)((Aa_10%100)/10)<<4) + Aa_10%10; 
	A_ltu[3] =(U8)((Ab_10/1000)<<4 + (Ab_10%1000)/100);
	A_ltu[2] =((U8)((Ab_10%100)/10)<<4) + Ab_10%10; 
	A_ltu[5] =((U8)((Ac_10/1000)<<4) + (Ac_10%1000)/100);
	A_ltu[4] =((U8)((Ac_10%100)/10)<<4) + Ac_10%10;
printf("the U_ltu = ");
	for(i = 0;i < 6; i++)
	{
		printf("0x%02x",U_ltu[i]);
	}
printf("\r\n");
printf("the A_ltu = ");
	for(i = 0;i < 6; i++)
	{
		printf("0x%02x",A_ltu[i]);
	}
printf("\r\n");
//update the g_recv_msg_num and the g_ltu_VA_current
for(i = 0;i < ltu_number;i++)
{
//printf("try match the ltu number......%d\r\n",i);
	if(0 == memcmp(addr_ltu,LTU_number_1[i],6))
	{
		printf("match %d the ltu number \r\n",i);	
//		memcpy(g_ltu_VA_current[i],buffer+10,13);
		memcpy(g_ltu_VA_current[i],U_ltu,6);//V 6 bytes	
		memcpy(&(g_ltu_VA_current[i][6]),A_ltu,6);
		g_recv_msg_num[i]++;
		break;
	}
}
printf("test_1\r\n");
//----------for log------------------
	memset(log_buffer,0,256);
	sprintf(log_buffer,"%s","get ltu information=");
	sprintf(log_buffer,"%s %s",log_buffer,"ltu:");
printf("test_2\r\n");
	for(i = 0;i < 6 ;i++)
	{
		sprintf(log_buffer,"%s%02x",log_buffer,addr_ltu[5-i]);
	}
	sprintf(log_buffer,"%s Ua:%f Ub:%f Uc:%f Aa:%f Ab:%f Ac:%f",log_buffer,Ua,Ub,Uc,Aa,Ab,Ac);
//	logfile(6,log_buffer);
printf("test_______\r\n");
}
U8 get_06_msg(const U8* buffer)
{
	U8 ret = 0;
	int i = 0;
	int alarm_type = 0;
	int alarm_id= 0;
	int len_645 = 0;
	int alarm_ABC = 0;
	U8 DT[2] = {};
	U8 log_buffer[256];
	U8 buffer_645[128] = {0};
	memcpy(DT,buffer+11,2);
	len_645 = *(buffer+15);
	U8 num_ltu[6];
	U8 V_A_alarm[12];
	memset(log_buffer,0,256);
	memcpy(buffer_645,buffer+16,len_645);
//for many  alarm 
	int alarm_number = 1;
	int arr_alarm_type[16] = {0};
//-------alarm type detected------------
	int pos_data = 10;// the data start postion of 645------
	int len_data = *(buffer_645+9);
	for(i = 0;i < len_data;i++)
		*(buffer_645+pos_data+i)-=0x33;

	if(*(buffer_645+14) == 1)//alarm kongka
		{
			alarm_type = 0;
//			alarm
			alarm_number++;
			alarm_ABC = 0;
		}
	else if(*(buffer_645+15) == 1)//alarm A  lost V
		{alarm_type = 1; alarm_ABC = 1;}
	else if(*(buffer_645+15) == 2)//alarm A too low V
		{alarm_type = 2; alarm_ABC = 1;}
	else if(*(buffer_645+15) == 3)//alarm A too high V
		{alarm_type = 3; alarm_ABC = 1;}
	else if(*(buffer_645+16) == 1)//alarm B lost V
		{alarm_type = 1; alarm_ABC = 2;}
	else if(*(buffer_645+16) == 2)//alarm B too low V
		{alarm_type = 2; alarm_ABC = 2;}
	else if(*(buffer_645+16) == 3)//alarm B too high V
		{alarm_type = 3; alarm_ABC = 2;}
	else if(*(buffer_645+17) == 1)//alarm C lost V
		{alarm_type = 1; alarm_ABC = 3;}
	else if(*(buffer_645+17) == 2)//alarm C too low V
		{alarm_type = 2; alarm_ABC = 3;}
	else if(*(buffer_645+17) == 3)//alarm C too high V
		{alarm_type = 3; alarm_ABC = 3;}
	else if(*(buffer_645+18) == 1)//alarm A too high A
		{alarm_type = 4; alarm_ABC = 1;}
//	else if(*(buffer_645+18) == 2)//alarm A too low V
//		alarm_type = 5;
	else if(*(buffer_645+19) == 1)//alarm A too high A
		{alarm_type = 4; alarm_ABC = 2;}
//	else if(*(buffer_645+19) == 2)//alarm A too low V
//		alarm_type = 5;
	else if(*(buffer_645+20) == 1)//alarm A too high A
		{alarm_type = 4; alarm_ABC = 3;}
//	else if(*(buffer_645+20) == 2)//alarm A too low V
//		alarm_type = 5;
#if 1
	memcpy(V_A_alarm,buffer_645+26,12);
	printf("the V_A_alarm = ");
	for(i = 0;i < 12;i++)
		printf(" 0x%02x",V_A_alarm[i]);
	printf("\r\n");
		
	float Ua,Ub,Uc,Aa,Ab,Ac;
	Ua = (float)((((int)V_A_alarm[1]) << 8)+(int)V_A_alarm[0])*0.1;
	Ub = (float)((((int)V_A_alarm[3]) << 8)+(int)V_A_alarm[2])*0.1;
	Uc = (float)((((int)V_A_alarm[5]) << 8)+(int)V_A_alarm[4])*0.1;
	Aa = (float)((((int)V_A_alarm[7]) << 8)+(int)V_A_alarm[6])*0.1;
	Ab = (float)((((int)V_A_alarm[8]) << 8)+(int)V_A_alarm[8])*0.1;
	Ac = (float)((((int)V_A_alarm[11]) << 8)+(int)V_A_alarm[10])*0.1;
	int Ua_10 = Ua*10,Ub_10 = Ub*10,Uc_10 = Uc*10,Aa_10 =Aa*10 ,Ab_10 = Ab*10,Ac_10 = Ac*10;
//printf("the Ua_10=%d Ub_10 =%d Uc_10 = %d Ua_10/1000<<4+(Ua_10%1000)/100 = %x\r\n",Ua_10,Ub_10,Uc_10,(Ua_10/1000)<<4 + (Ua_10%1000)/100);
//printf("the Aa_10=%d Ab_10 =%d Ac_10 = %d Aa_10/1000<<4+(Aa_10%1000)/100 = %x\r\n",Aa_10,Ab_10,Ac_10,(Aa_10/1000)<<4 + (Aa_10%1000)/100);
	V_A_alarm[1] =((U8)((Ua_10/1000)<<4) + (Ua_10%1000)/100);
	V_A_alarm[0] =((U8)((Ua_10%100)/10)<<4) + Ua_10%10; 

	V_A_alarm[3] =((U8)((Ub_10/1000)<<4) + (Ub_10%1000)/100);
	V_A_alarm[2] =((U8)((Ub_10%100)/10)<<4) + Ub_10%10; 
	V_A_alarm[5] =((U8)((Uc_10/1000)<<4) + (Uc_10%1000)/100);
	V_A_alarm[4] =((U8)((Uc_10%100)/10)<<4) + Uc_10%10; 

	V_A_alarm[7] =((U8)((Aa_10/1000)<<4) + (Aa_10%1000)/100);
	V_A_alarm[6] =((U8)((Aa_10%100)/10)<<4) + Aa_10%10; 
	V_A_alarm[9] =((U8)((Ab_10/1000)<<4) + (Ab_10%1000)/100);
	V_A_alarm[8] =((U8)((Ab_10%100)/10)<<4) + Ab_10%10; 
	V_A_alarm[11] =((U8)((Ac_10/1000)<<4) + (Ac_10%1000)/100);
	V_A_alarm[10] =((U8)((Ac_10%100)/10)<<4) + Ac_10%10;
#endif
	
	static struct timeval current_time,last_alarm_time;
	//struct timezone tz;
	gettimeofday(&current_time,0);
	printf("the current time = %d the last_alrm_time = %d",current_time.tv_sec,last_alarm_time.tv_sec);
	if(current_time.tv_sec - last_alarm_time.tv_sec < 10 )
	{
		printf("the current time = %d the last_alrm_time = %d",current_time.tv_sec,last_alarm_time.tv_sec);
		printf("receive duplict alarm!\r\n");
		return 0;
	}	
	last_alarm_time.tv_sec = current_time.tv_sec;
	if(DT[0] == 0x10 && DT[1] == 0x00)
	{
		memcpy(num_ltu,buffer+17,6);
		sprintf(log_buffer,"%s%02x%02x%02x%02x%02x%02x--%d","get alarm from ltu:",num_ltu[5],num_ltu[4],num_ltu[3],num_ltu[2],num_ltu[1],num_ltu[0],alarm_type);	
printf("++++++++++++++++++get alarm alarm type = %d!!!!!!!!!!!!!!!11\r\n",alarm_type);
		logfile(7,log_buffer);
		for(i = 0;i<10;i++)
		{
printf("++alarm ltu number memcmp = %d \r\n",i);
			if(!memcmp(num_ltu,LTU_number_1[i],6))
			{
printf("++++++++++++++++++alarm ltu number memcmp get alarm_id=%d socket status =  %d!!!!!!!!!!!!!!!11\r\n",socket_status,i);
				alarm_id = i;	
				break;
			}
		}
//
//			gpio_direction_output_o(OLED_1,0);
//			sleep(1);
//			gpio_direction_output_o(OLED_1,1);
			gpio_direction_output_o(OLED_2,0);
			sleep(5);
			gpio_direction_output_o(OLED_2,1);
			gpio_direction_output_o(TFT_VSYNC,0);
			sleep(1);
			gpio_direction_output_o(TFT_VSYNC,1);
//
//		send_alarm(alarm_id,alarm_type,V_A_alarm,alarm_ABC);
		if(socket_status == 2)//-------temp---------
		{
			send_alarm(alarm_id,alarm_type,V_A_alarm,alarm_ABC);
			gpio_direction_output_o(OLED_2,1);
			sleep(1);
			gpio_direction_output_o(OLED_2,0);
		}
	}
		
}
U8 get_13_msg(const U8* buffer)
{
	U8 ret = 0;
	int i = 0;
	int len_645 = 0;
	U8 frm_645[128] = {0x00};
	U8 DT[2] = {0x00};
	len_645 = *(buffer+28);
	if(len_645 > 0)
		memcpy(frm_645,buffer+29,len_645);
	memcpy(DT,(buffer+23),2);	
#if 0
printf("the frm_645 data is:");
for(i = 0;i<len_645;i++)
printf("0x%02x",frm_645[i]);
printf("\r\n");
#endif
	//int DT_m = *(buffer+12) * 8 + *(buffer+11)/2;
	if(len_645 > 0 && DT[0] == 0x01 && DT[1] == 0x00)
	{
//		printf("+++++++++++++++++get_645\r\n");
		get_645(frm_645,len_645);
	}
	return ret;
}
U8 get_message(const U8* buffer,int len_total)
{
	int msg_len = 0;
	U8 addr_flg = 0;
	int msg_number;
	U8 INFO[4] = {0x00};
	U8 ADDR_S[6] = {0x00};
	U8 ADDR_D[6] = {0x00};
	U8 AFN = 0x00;
	U8 DT[2] = {0x00};
        msg_len =(((int)buffer[2]) << 8) +(int)buffer[1]; 
	memcpy(INFO,buffer+4,4);
	if((INFO[0]&&0x04))
	{
		addr_flg = 1;	
		AFN = *(buffer+22);
		memcpy(ADDR_S,buffer+10,6);
		memcpy(ADDR_D,buffer+16,6);
	}else
	{
		AFN = *(buffer+10);
	}
	msg_number =*(buffer+9);
printf("!!!!!!!!!the control byte is:0x%x\r\n",AFN);
	//switch(*(buffer+10))
	switch(AFN)
	{
	case 0x00:
		plc_recv_number++;	
		break;
	case 0x06:
		plc_recv_number++;
		get_06_msg(buffer);	
		break;
	case 0x14:
		plc_recv_number++;
		if((*(buffer+11) == 0x01)&& (*(buffer+12) == 0x00))
//			get_14_msg_01(buffer);
				;
		else if(*(buffer+11) == 0x02 && (*(buffer+12) == 0x00))
//			set_sync_time_msg(buffer);
				;
		break;
	case 0x03:
		plc_recv_number++;
		get_03_msg(buffer);
		break;
	case 0x10:
		plc_recv_number++;
		get_10_msg(buffer);
		break;
	case 0x13:
		plc_recv_number++;
		printf("get_13_msg\r\n");
		get_13_msg(buffer);
		break;
	default:
		plc_recv_number++;
		if(*(buffer+22) == 0x13)
			printf("receive message!!!!!!!!!!!!!!!!111");
		printf("case none,C:0x%x",*(buffer+10));
	}
	if(msg_len < len_total)
	{
		printf("LLLLLLLLLLLLLLLLLLLLLLLLL\r\nLLLLLLLLLLLLLLLLLLLLLget double message!\r\n");
		get_message(buffer+msg_len,len_total-msg_len);	
	}
printf("total message deal over!\r\n");
	return msg_len;

}
U8 getCS(U8* data,int len)
{
    U8 result = 0;
    int i = 0;
    for(i = 0; i < len; i++)
        result += *data++;
    return(result);
}
int plc_init()
{
	if(!gpio_request_o(PLC_RST))
	{
		printf("gpio_request error please check!\r\n");
		return -1;	
	}else
	{
		printf("plc gpio request success!");
	}	
	gpio_direction_output_o(PLC_RST,0);
	sleep(5);
	gpio_direction_output_o(PLC_RST,1);
	sleep(5);
	sem_init(&sem_plc,0,1);//bkup	
	return 0;

}
void * thread_uart_read(void* arg)
{
    U8 len_tty = 0,i = 0;
    U8 buf_tty[256];
        while(sign_read)
        {
           bzero(buf_tty,sizeof(buf_tty));
           len_tty = ReadComPort_byte(buf_tty,256,fd_ATT,1);
            if (len_tty < 0) 
            {
                printf("Error: Read Com Port\n");
                break;
            }

            if (len_tty == 0) 
            {
                //write(STDOUT_FILENO, ".", sizeof("."));
                continue;
            }
	        num_msg++;

	//plc_ltu_add();	
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","recv:<-");
	for(i = 0;i<len_tty;i++)
	{
		sprintf(log_buffer,"%s 0x%2x",log_buffer,buf_tty[i]);
	}
//	logfile(1,log_buffer);
#if debug_flg 
        printf("plc get message len=%d is:",len_tty);
	for( i = 0;i<len_tty;i++)
		printf(" 0x%02x",buf_tty[i]);
	printf("\r\n");
#endif 
	get_message(buf_tty,len_tty);
       //     printf("-->");
           // sleep(1);
        }
}
void * thread_uart_write(void* arg)
{
        U8 len_tty = 0;
        int len_cmd = 0,num_cmd = 0;
        U8 buf_cmd[128];
    	U8 data_tail[]={0x1a};
        U8 i;
        int ret_len;
	int msg_num = 0;
        while(sign_write)
        {
            ret_len = 0;
            len_cmd = 0;
            num_cmd = 0;
            bzero(buf_cmd,sizeof(buf_cmd));
#if 1            
//	scanf_begin:
	    printf("-->");
            scanf("%d",&num_cmd);
		if(num_cmd == 129)
		{
			watch_ltu_cmd(LTU_number_1[0],0);
//			goto scanf_begin;
		}else if(num_cmd ==128)
		{
			allow_ltu_send();
		}
#if 0	
else if(num_cmd == 127)
		{
			config_ltu_cmd(LTU_number_1[9]);
		}
#endif
#else
		if(msg_num < 8)
			num_cmd = msg_num;
		else
			num_cmd = 8;
#endif            
            printf("CCO_data[%d][2] CCO_data[%d][1]is: %d %d\r\n",num_cmd,num_cmd,(int)(CCO_data[num_cmd][2]) << 8,(int)CCO_data[num_cmd][1]);
            len_cmd =(((int)CCO_data[num_cmd][2]) << 8) +(int)CCO_data[num_cmd][1]; 
            printf("the len_cmd is:%d\r\n",len_cmd);

            memcpy(buf_cmd,CCO_data[num_cmd],len_cmd); 
            buf_cmd[len_cmd-2] = getCS(&buf_cmd[3],len_cmd-5); 
            printf("WRITE buf_cmd is");
            for(i = 0;i < len_cmd;i++)
                printf(" 0x%x",buf_cmd[i]);
            printf("\r\n");
           
            ret_len = WriteComPort(buf_cmd, len_cmd,fd_ATT);
            printf("the ret_len is %d\r\n",ret_len); 
            if (len_cmd < 0) 
            {
                printf("Error: WriteComPort Error\n");
            }else{
                printf("write %s to uart\n",buf_cmd);
            }
		msg_num++;
            sleep(5);
        }
}
void * thread_test(void* arg)
{
	int i = 0,j = 0, ltu_current = 0,error_count = 1;
	U8 plc_recv_number_bef = 0;
	//sleep(2);
	plc_ltu_add();	
	int get_VA_array_bef[ltu_number];
	U8 V_and_A[13];
	sleep(2);
//	shutdown_route();
#if 1
//	for(i = 0;i <1000 ; i++ )
	while(1)
	{
//		printf("1111111111into to the for\r\n");
//		shutdown_route();
		plc_recv_number_bef = plc_recv_number;
//		shutdown_route();
	//	while(plc_recv_number == plc_recv_number_bef)
	//		sleep(1);
		for(j = 0;j<g_ltu_num;j++)
		{
			ltu_current = j;	
			plc_recv_number_bef = plc_recv_number;
#if 0
			watch_ltu_cmd(LTU_number_1[j],j);
//		while(plc_recv_number == plc_recv_number_bef)
			error_count = 0;
			while(get_VA_array_bef[ltu_current] == g_recv_msg_num[ltu_current] && error_count < 20)
			{
				sleep(1);
				error_count ++;
			}
			while(plc_recv_number_bef == plc_recv_number)//wait for the error react message
				sleep(1);
#if 1//--test--
		
		if(get_VA_array_bef[ltu_current] == g_recv_msg_num[ltu_current])
		{
			memset(log_buffer,0,256);
			sprintf(log_buffer,"%s","get ltu information=");
			sprintf(log_buffer,"%s %s",log_buffer,"ltu:");
			for(i = 0;i < 6 ;i++)
			{
				sprintf(log_buffer,"%s%02x",log_buffer,LTU_number_1[ltu_current][5-i]);
			}
			sprintf(log_buffer,"%s %s",log_buffer,"failed");
			logfile(7,log_buffer);
		}	
#endif
#else 
//		;
//		 get_ltu_VA(LTU_number_1[j],j,V_and_A,13);//lyb-------for read self--------------		
#endif
//			sleep(5);
		}
		vRtcGetTime();
		sleep(VA_delay);
	}	
#endif
//	restart_route();
}
int ltu_conf()
{
	int ret = 0;
	int i = 0;
	char ltu_conf_filename[] = "conf/ltu_conf.conf";
	FILE *fp_ltu;
	char str_line[1024];
	int line_num = 0;
	int ltu_num = 0;
	char str_line_temp[1024];
	FILE *fp;
	printf("into the method lt_conf\r\n");
	if((fp_ltu = fopen(ltu_conf_filename,"r")) == NULL)
	{
		printf("open conf/ltu_conf.conf error please check\r\n");
		return -1;
	}else{
		printf("open file success\r\n");
	}
	while(!feof(fp_ltu))
	{
		printf("into the while\r\n");
		fgets(str_line,1024,fp_ltu);
		printf("into the while after the fgets\r\n");
		memcpy(str_line_temp,str_line,1024);
		printf("into the while after the memcpy\r\n");
		printf("%s\r\n",str_line);
		if(str_line[0] != '#')
		{
			char *p_temp = NULL;
			if(i == 0)
			{
				p_temp = strtok(str_line_temp,"/");
				memcpy(g_cmu_array_char,p_temp,strlen(p_temp)+1);	
printf("cmu number=%s\r\n",p_temp);	
			}
			else if(i == 1)
			{
				char *p_ltu_num = NULL;
				p_ltu_num = strtok(str_line_temp,"/");
				if(p_ltu_num == NULL)
					p_ltu_num = strtok(str_line_temp,"\0");
				ltu_num = atoi(p_ltu_num);
				printf("the ltu_num = %d\r\n",ltu_num);	
			}else
			{
printf("begain strtok\r\n");
				p_temp = strtok(str_line_temp,"/");
printf("ltu number=%s\r\n",p_temp);	
				memcpy(g_ltu_array_char[i-2][0],p_temp,strlen(p_temp)+1);
//				printf("ltu number:%s",g_ltu_array_char[i-1][0]);
				p_temp = strtok(NULL,"/");
printf("VA number=%s\r\n",p_temp);	
				memcpy(g_ltu_array_char[i-2][1],p_temp,strlen(p_temp)+1);
//				printf(" :%s",g_ltu_array_char[i-1][1]);
				p_temp = strtok(NULL,"/");
printf("VA_1=%s\r\n",p_temp);	
				memcpy(g_ltu_array_char[i-2][2],p_temp,strlen(p_temp)+1);
//				printf(":%s",g_ltu_array_char[i-1][2]);
				p_temp = strtok(NULL,"/");
printf("VA_2=%s\r\n",p_temp);	
				memcpy(g_ltu_array_char[i-2][3],p_temp,strlen(p_temp)+1);
//				printf(" :%s",g_ltu_array_char[i-1][3]);
				p_temp = strtok(NULL,"/");
printf("VA_3=%s\r\n",p_temp);	
				memcpy(g_ltu_array_char[i-2][4],p_temp,strlen(p_temp)+1);
//				printf(" :%s",g_ltu_array_char[i-1][4]);
			}
			printf("\r\n");
			i++;
			line_num++;
		}
	}
	g_ltu_num = ltu_num;
	fclose(fp_ltu);
	return 0;
}
U8 updata_LTU_number()
{
	int i = 0,j = 0,k = 0;
	U8 ret = 0;
	char temp_number[3];
	int temp_number_int = 0;
	temp_number[2] = '\0';
	//updata the cmu number
	memset(g_cmu_NO,0x00,6);
	temp_number[0] = g_cmu_array_char[0];
	temp_number[1] = g_cmu_array_char[1];
	temp_number_int = atoi(temp_number);
	g_cmu_NO[5] = (temp_number_int/10) *16 + temp_number_int%10;
printf("temp_number=%02x",g_cmu_NO[5]);	
	temp_number[0] = g_cmu_array_char[2];
	temp_number[1] = g_cmu_array_char[3];
	temp_number_int = atoi(temp_number);
	g_cmu_NO[4] = (temp_number_int/10) *16 + temp_number_int%10;
printf("temp_number=%02x",g_cmu_NO[4]);	
	temp_number[0] = g_cmu_array_char[4];
	temp_number[1] = g_cmu_array_char[5];
	temp_number_int = atoi(temp_number);
	g_cmu_NO[3] = (temp_number_int/10) *16 + temp_number_int%10;
printf("temp_number=%02x",g_cmu_NO[3]);	
	temp_number[0] = g_cmu_array_char[6];
	temp_number[1] = g_cmu_array_char[7];
	temp_number_int = atoi(temp_number);
	g_cmu_NO[2] = (temp_number_int/10) *16 + temp_number_int%10;
printf("temp_number=%02x",g_cmu_NO[2]);	
	temp_number[0] = g_cmu_array_char[8];
	temp_number[1] = g_cmu_array_char[9];
	temp_number_int = atoi(temp_number);
	g_cmu_NO[1] = (temp_number_int/10) *16 + temp_number_int%10;
printf("temp_number=%02x",g_cmu_NO[1]);	
	temp_number[0] = g_cmu_array_char[10];
	temp_number[1] = g_cmu_array_char[11];
	temp_number_int = atoi(temp_number);
	g_cmu_NO[0] = (temp_number_int/10) *16 + temp_number_int%10;
printf("temp_number=%02x",g_cmu_NO[0]);
printf("\r\n");	
printf("the cmu NO.= %02x%02x%02x%02x%02x%02x\r\n",g_cmu_NO[5],g_cmu_NO[4],g_cmu_NO[3],g_cmu_NO[2],g_cmu_NO[1],g_cmu_NO[0]);
	memset(temp_number,0,2);
	temp_number_int = 0;
	//updata the ltu number
	for(i = 0;i < g_ltu_num;i++)
	{
		memset(LTU_number_1[i],0x00,6);

		temp_number[0] = g_ltu_array_char[i][0][0];
		temp_number[1] = g_ltu_array_char[i][0][1];
		temp_number_int = atoi(temp_number);
printf("result_1 is:%d",temp_number_int);
		LTU_number_1[i][5] = (temp_number_int/10) *16 + temp_number_int%10;
printf(" 0x%02x", LTU_number_1[i][5]);
		
		temp_number[0] = g_ltu_array_char[i][0][2];
		temp_number[1] = g_ltu_array_char[i][0][3];
		temp_number_int = atoi(temp_number);
printf(" %d", temp_number_int);
		LTU_number_1[i][4] = (temp_number_int/10) *16 +temp_number_int%10;
printf(" 0x%02x", LTU_number_1[i][4]);
	
		temp_number[0] = g_ltu_array_char[i][0][4];
		temp_number[1] = g_ltu_array_char[i][0][5];
		temp_number_int = atoi(temp_number);
printf(" %d",temp_number_int);
		LTU_number_1[i][3] = (temp_number_int/10) *16 +temp_number_int%10;
printf(" 0x%02x", LTU_number_1[i][3]);
		
		temp_number[0] = g_ltu_array_char[i][0][6];
		temp_number[1] = g_ltu_array_char[i][0][7];
		temp_number_int = atoi(temp_number);
printf(" %d",temp_number_int);
		LTU_number_1[i][2] = (temp_number_int/10) *16 +temp_number_int%10;
printf(" 0x%02x", LTU_number_1[i][2]);

		
		temp_number[0] = g_ltu_array_char[i][0][8];
		temp_number[1] = g_ltu_array_char[i][0][9];
		temp_number_int = atoi(temp_number);
printf(" %d",temp_number_int);
		LTU_number_1[i][1] = (temp_number_int/10) *16 +temp_number_int%10;
printf(" 0x%02x", LTU_number_1[i][1]);
		
		temp_number[0] = g_ltu_array_char[i][0][10];
		temp_number[1] = g_ltu_array_char[i][0][11];
		temp_number_int = atoi(temp_number);
printf(" %02d\r\n",temp_number_int);
		LTU_number_1[i][0] = (temp_number_int/10) *16 +temp_number_int%10;
printf(" 0x%02x", LTU_number_1[i][0]);
		memcpy(LTU_number_socket[i],LTU_number_1[i],6);
printf("the result is:");
for(j = 0;j < 6;j++)
{
	printf("%02x",LTU_number_1[i][j]);
}
printf("\r\n");
	}
}
#if 1
int main (int argc, char *argv[])
{
    int ret = -1;
    int i = 0;
    int len_tty = -1,len_cmd = -1;
    //int fd_rs485;
    pthread_t p_write,p_read,p_test,p_socket_read,p_socket_heart,p_socket_write,p_adjust_ATT,p_adjust_infrared;
	pthread_t p_update_VA;
    void *retval;

//  unsigned char *buf_cmd = "AT\r\n";

//message_process_test_();
	gpio_direction_output_o(OLED_2,0);
	log_init();
	plc_init();
	led_init();
	OLED_Init();
	R8025t_Init();
	socket_init();
//	plc_ltu_add();	
	ATT7022EU_Init();
	adjust_init();
char str_adjust_time[256];
vRtcGetTime();
sprintf(str_adjust_time,"date -s \"20%02d-%02d-%02d %02d:%02d:%02d\"",xTime.year,xTime.month,xTime.day,xTime.hour,xTime.minute,xTime.second);
printf("=====================the str_adjust_time=%s\r\n",str_adjust_time);
system(str_adjust_time);
				
ltu_conf();
updata_LTU_number();
    if (argc > 1) {
        n_com_port = strtol( argv[1], NULL, 10 );
    }
	
    ret = OpenComPort(plc_uart, 9600, 9, "1", 'E',&fd_ATT);
    if (ret < 0) {
        fprintf(stderr, "Error: Opening Com Port %d\n", n_com_port);
        return ret;
    }else{
        printf("Open Com Port %d Success, Now going to read port\n", n_com_port);
    }
    
	sleep(2);
//	pthread_create(&p_write,NULL,thread_uart_write,NULL);
	pthread_create(&p_read,NULL,thread_uart_read,NULL);
	pthread_create(&p_test,NULL,thread_test,NULL);//for test
	pthread_create(&p_socket_read,NULL,process_conn_client_read,NULL);
	pthread_create(&p_socket_heart,NULL,process_conn_client_heart,NULL);

//	pthread_create(&p_socket_write,NULL,process_conn_client_write,NULL);
	pthread_create(&p_update_VA,NULL,p_update_VA_ATT,NULL);
	pthread_create(&p_adjust_ATT,NULL,thread_adjust_read,NULL);
//	pthread_create(&p_adjust_infrared,NULL,thread_adjust_write,NULL);
    
//	pthread_join(p_write,&retval);
	pthread_join(p_read,&retval);
	pthread_join(p_test,&retval);
	pthread_join(p_socket_read,&retval);
	pthread_join(p_socket_heart,&retval);

//	pthread_join(p_socket_write,&retval);
	pthread_join(p_update_VA,&retval);
	pthread_join(p_adjust_ATT,&retval);
//	pthread_join(p_adjust_infrared,&retval);
	

	CloseComPort(fd_ATT);
	adjust_close();

    printf("Program Terminated\n");

    return(0);
}
#else
void * thread_read(void* argc)
{
	OLED_Init();
	return;
}
#if 0
U8 stop_cmu_thread()
{
	pthread_cancel();
}
void* thread_key_dect(void *argc)
{
	U8 value_key_1,value_key_2;
	U8 current_status_key_1,current_status_key_2;
	U8 prior_status_key_1;prior_status_key_2;
	int persist_tm_key_1 = 0;persist_tm_key_2 = 0;
	gpio_direction_input_o(KEY_1);	
	gpio_direction_input_o(KEY_2);	
printf("into thread_key_dect\r\n");
	while(1)
	{
printf("into while\r\n");
		//value_key_1 = 0;value_key_2;
		value_key_1 = gpio_get_value_o(KEY_1);
		value_key_2 = gpio_get_value_o(KEY_2);
		current_status_key_1 = value_key_1;
		current_status_key_2 = value_key_2;
			
		if(current_status_key_1 == 0)
		{
			if(prior_status_key_1 == current_status_key_1)
				persist_tm_key_1++;
			else
				prior_status_key_1 = current_status_key_1;
		
		}else if(prior_status_key_1 == 0)
		{
			//
			if(persist_tm_key_1 > 50)//update
			{
		//		pthread_cancel();
				
				plc_add_ltu();		
			}
			persist_tm_key_1 = 0;
			prior_status_key_1 = current_status_key_1;
		}
		//usleep(1000);
		printf("key1=%d;key2=%d",value_key_1,value_key_2);
		usleep(1000);
	}
	return;
}
#endif
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
	ltu_conf();
	updata_LTU_number();
	//pthread_create(&p_keydect,NULL,thread_key_dect,0);	
	
//	pthread_join(p_keydect,&retval);
	
//	pthread_create(&p_write,NULL,thread_write,0);	
//	pthread_create(&p_read,NULL,thread_read,0);	
//	pthread_create(&p_keydect,NULL,thread_key_dect,0);	
	
//	pthread_join(p_write,&retval);
//	pthread_join(p_write,&retval);
	pthread_join(p_keydect,&retval);
	
	//spi_free();
	return 0;
}
#endif
void message_process_test_()
{
	U8 test[] = {0x68,0x15,0x00,0x83,0x00,0x00,0x00,0x00,0x00,0x02,\
		0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x86,\
		0x16,0x68,0x38,0x00,0x83,0x04,0x00,0x00,0x00,0x00,\
		0x52,0x00,0x10,0x15,0x20,0x32,0x06,0x01,0x00,0x00,\
		0x00,0x00,0x01,0x13,0x01,0x00,0x01,0x00,0x02,0x19,\
		0x68,0x00,0x10,0x15,0x20,0x32,0x06,0x68,0x91,0x0d,\
		0xf3,0x3d,0xe4,0x3b,0xea,0x3b,0x33,0x33,0x33,0x33,\
		0x33,0x33,0x33,0xc2,0x16,0x22,0x16,};
	get_message(test,77);
}


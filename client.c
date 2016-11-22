#ifndef CLIENT
#define CLIENT
#include "client.h"
#endif
#define PORT 4020 //for  test with huang junwei|dong yang /tsintao
//#define PORT 7899 //shd provence service port
//#define PORT    7900  
//#define PORT    2345 //for wangtao test test  
#define Buflen  1024  
#define SOCKET_ERR_TIME 20 //seconds
#define FRAME_HEADER_LEN 2
#define APCI_LEN 6
#define ASDU_HEAD_LEN 9
#define LEN_PER_DEV 16
#define DEV_POINT_NUM 7
//extern LTU_number_1[][6];
extern get_ltu_VA(U8* addr,U8 ltu_NO,U8* VA,int size_VA);
extern int g_ltu_num;

char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
U8 socket_status = 0;
int g_socket_heart_recv_number = 0;
int g_socket_recv_number = 0;
//char server_ip[50] = ""; 
U8 g_cmu_register = 0;
int g_PORT = 4020;
//double g_socket_recv_number = 0;
//double g_socket_recved_number = 0;
struct sockaddr_in server_addr; 
int g_disconnect_sign = 0; 
U8 g_server_addr_exit = 0;
U16 g_Imsg_num_send = 0;
U16 g_Imsg_num_recv = 0;
U8 g_cmu_NO[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
U8 CCO_data_socket[][256]={
//--0x68---Len--C-----------------C-A12---------A22------A3-----AFN-SEQ--DA2---------DT2------CS1--0x16--
    {0x68, 0x0a,0x07,0x00,0x00,0x00,0x01,0x00,0x16,0x20,0x32,0x05},//0----- start message-----
    {0x68, 0x04,0x0b,0x00,0x00,0x00 },//1----- start confirm message
    {0x68, 0x04,0x43,0x00,0x00,0x00 },//2----- heart message
    //{0x68, 0x04,0x01,0x00,0x02,0x00 },//2----- data confirm message    
    {0x68, 0x0D,0x00,0x00,0x02,0x00,0x64,0x01, 0x07,0x00, 0x01,0x00,0x00, 0x00,0x00},//3----- all call confirm message
    {0x68, 0x9E,0x00,0x5E,0x00,0x68, 0xDA, 0x00,0x00, 0x00,0x01,0x02, 0x0E,0x71, 0x01,0x01, 0x02,0x00,\
    0x00,0x00,0x00,0x00,  0x32,0x0F, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x05,0x00,0x00,0x00,0x00,0x00, 0x00,0x16 },//4-gongkai
    {0x68, 0x9E,0x00,0x5E,0x00,0x68, 0xDA, 0x00,0x00, 0x00,0x01,0x02, 0x0E,0x71, 0x01,0x01, 0x02,0x00,\
    0x00,0x00,0x00,0x00,  0x33,0x0F, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x05,0x00,0x00,0x00,0x00,0x00, 0x00,0x16 },//5-V

{0x68, 0x9E,0x00,0x5E,0x00,0x68, 0xDA, 0x00,0x00, 0x00,0x01,0x02, 0x0E,0x71, 0x01,0x01, 0x02,0x00,\
    0x00,0x00,0x00,0x00,  0x34,0x0F, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x05,0x00,0x00,0x00,0x00,0x00, 0x00,0x16 },//6-A
    {0x68, 0x9E,0x00,0x5E,0x00,0x68, 0xDA, 0x00,0x00, 0x00,0x01,0x02, 0x0E,0x71, 0x01,0x01, 0x02,0x00,\
    0x00,0x00,0x00,0x00,  0x35,0x0F, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x00,0x00,0x00, 0x05,0x00,0x00,0x00,0x00,0x00, 0x00,0x16 },//7-lost V
//---|----------------------APCI---------------------|-------ASDU header--------------------------------------|
//---0x68-Len------------------send num---recv num---|msgtype--kbjgxdc-|-reason----|public addr|-first addr---|
    {0x68,0x3D/*���������ѹ*/, 0x04,0x00, 0x02, 0x00, 0x42,    0x83,   0x14, 0x00, 0x01, 0x00, 0x01,0x00,0x00, 
	 0x22, 0x22, 0x22,0x22, 0x22, 0x22,0x11, 0x11,  0x01, 0x11,0x11,0x01,0x11,0x11,0x01,0x01,
	 0x22, 0x22, 0x22,0x22, 0x22, 0x22, 0x11, 0x11, 0x01, 0x11,0x11,0x01,0x11,0x11,0x01,0x01,
	 0x22, 0x22, 0x22,0x22, 0x22, 0x22, 0x11, 0x11, 0x01, 0x11,0x11,0x01,0x11,0x11,0x01,0x01,
	 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,0x00,0x00,0x00 },//8
    {0x68, 0x0D,0x14,0x00,0x02,0x00,0x64,0x01, 0x0a,0x00, 0x01,0x00,0x00, 0x00,0x00,0x14},//9----- all call end message
    {0x68, 0x2A,		0x28,0x00,0x02,0x00,	0x01,0x01, 	0x03,0x00,  0x01,0x00, 0x02,0x00,0x00, 0x01,/**/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,/**/0x00,0x00,0x00,0x00,0x00,0x00,/**/0x00,0x00,0x11,0x11,0x22,0x22,0x00,0x11,0x11,0x00,0x22,0x22,0x00,0x33,0x33},//10----- alarm message

    {0x68, 0x1D, 0x14,0x00, 0x02,0x00, 0x6C,0x81, 0x14,0x00, 0x01,0x00, 0x00, 0x00,0x00, 0x22,0x22,0x22,0x22,0x22,0x11,0x11,0x01,0x11,0x11,0x01,0x11,0x11,0x01},//11----- CMU call response message
{0}
};

U8 LTU_number_socket[][6] = 
{
//{0x05,0x32,0x20,0x15,0x00,0x01},
//{0x01,0x00,0x15,0x20,0x32,0x05},
//{0x02,0x00,0x15,0x20,0x32,0x05},
{0x00,0x10,0x15,0x20,0x32,0x06},
{0x02,0x10,0x15,0x20,0x32,0x06},
//{0x22,0x00,0x15,0x20,0x32,0x05},
//{0x33,0x00,0x15,0x20,0x32,0x05},
{0x03,0x10,0x15,0x20,0x32,0x06},
{0x04,0x10,0x15,0x20,0x32,0x06},
{0x22,0x11,0x15,0x20,0x32,0x05},
//{0x03,0x02,0x15,0x20,0x32,0x05},
//{0x05,0x02,0x15,0x20,0x32,0x05},
{0x06,0x10,0x15,0x20,0x32,0x06},
{0x38,0x00,0x15,0x20,0x32,0x05},
//{0x07,0x00,0x15,0x20,0x32,0x05},
//{0x05,0x02,0x15,0x20,0x32,0x05},
//{0x04,0x02,0x15,0x20,0x32,0x05},
//{0x08,0x00,0x15,0x20,0x32,0x05},
{0x09,0x00,0x15,0x20,0x32,0x05},
{0x10,0x00,0x15,0x20,0x32,0x05},
{0x11,0x00,0x15,0x20,0x32,0x05},
{0x1,0x00,0x00,0x00,0x00,0x00},
{}
#if 0
{0x22,0x00,0x15,0x20,0x32,0x05},
{0x33,0x00,0x15,0x20,0x32,0x05},
{0x03,0x00,0x15,0x20,0x32,0x05},
{0x04,0x00,0x15,0x20,0x32,0x05},
{0x05,0x00,0x15,0x20,0x32,0x05},
{0x05,0x02,0x15,0x20,0x32,0x05},
{0x04,0x02,0x15,0x20,0x32,0x05},
{0x08,0x00,0x15,0x20,0x32,0x05},
{0x09,0x00,0x15,0x20,0x32,0x05},
{0x10,0x00,0x15,0x20,0x32,0x05},
{0x1,0x00,0x00,0x00,0x00,0x00},
{}
#endif
};
U8 socket_reconnect(struct sockaddr_in server_addr_);

void process_I_msg(U8* data,int len);

void process_S_msg(U8* data,int len);

void process_U_msg(U8* data,int len);
void send_msg_callall_begin();
void send_msg_callall_end();
void message_process_test();
//struct sockaddr_in server_addr;  
//int s_socket;   
struct tm *p;

int g_number_seq_send = 0;
int g_number_seq_recv = 0;

char log_buffer[256];
/***************************************************************************************
*description: the method of get the system time of the current
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
struct tm get_current_time()
{
    time_t timep;
    time(&timep);
    p=localtime(&timep);
    return *p;
}
/***************************************************************************************
*description: the method of calc the check code of the message
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
U8 getCS_socket(U8* data,int len)
{
    U8 result = 0;
    int i = 0;
    for(i = 0; i < len; i++)
        result += *data++;
    return(result);
}
/***************************************************************************************
*description: the interface of the socket_init method
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
int socket_init()
{
//    struct sockaddr_in server_addr;  
    pthread_t p_write,p_read;
    int err;  
    sighandler_t ret;  
    char server_ip[50] = ""; 
    void * retval;

    /********************socket()*********************/  
    //s= socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); 
//printf("s_socket = %d",s_socket); 
	if(!s_socket || g_disconnect_sign)
	{
//		s_socket = -1;
		s_socket= socket(AF_INET,SOCK_STREAM,0); 
		g_disconnect_sign = 0; 
	}
printf("s_socket = %d",s_socket);
    if(s_socket<0)  
    {  
        printf("client : create socket error\n");  
        logfile(4,"client : create socket error");
        return -1;  
    }  
#if 1
    ret = signal(SIGTSTP,sig_pipe);  
    if(SIG_ERR == ret)  
    {  
        printf("signal request error!\n");  
        logfile(4,"client : signal request error");  
        return -1;  
    }else
	{  
	printf("signal request success!\n") ;  
        logfile(4,"client : signal request success");  
	}
#endif
#if 1
//----------------read configure from the configure file----------
char server_conf_filename[]="conf/server.conf";
FILE *fp_server = NULL;
char str_line[1024];
char *ip_pos = str_line;
char str_head[28],ip_addr[56],ip_port[56];
char *p_temp = NULL;
if(!g_server_addr_exit)
{
fp_server = fopen(server_conf_filename,"r");
if(fp_server == NULL)
{
	printf("open conf/server.conf error please check\r\n");
	//logfile(7,"open conf/server.conf error please check");
}
//---------------------------------end---------------------------  
while(fgets(str_line,1024,fp_server))
{
	if(str_line[0] != '#')
	{
		ip_pos=strstr(str_line,"server ip");
		if(ip_pos != NULL)
		{
//printf("get server ip:%s\r\n",str_line);
			p_temp = strtok(str_line,":");
			p_temp = strtok(NULL,":");
printf("get server ip address :%s\r\n",p_temp);
			memcpy(server_ip,p_temp,strlen(p_temp)+1);	
			p_temp = strtok(NULL,"\0");
printf("get server ip PORT:%s\r\n",p_temp);
			g_PORT = atoi(p_temp);
		}
	}			
} 
#endif
/*******************connect()*********************/  
    memset(&server_addr,0,sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(g_PORT);  
//    server_addr.sin_port = htons(PORT);  
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
    //inet_pton(AF_INET,argv[1],&server_addr.sin_addr.s_addr);  
//    printf("please input server ip address : \n");  
//    read(0,server_ip,50);  
    //err = inet_pton(AF_INET,server_ip,&server_addr.sin_addr.s_addr);
//	memcpy(server_ip,"192.168.1.168",sizeof("192.168.1.168"));
//	memcpy(server_ip,"192.168.1.232",sizeof("192.168.1.232"));
//	memcpy(server_ip,"192.168.23.1",sizeof("192.168.23.1"));
//	memcpy(server_ip,"60.60.10.7",sizeof("60.60.10.7"));
    server_addr.sin_addr.s_addr = inet_addr(server_ip);  
	g_server_addr_exit = 1;
 }
 
    err = connect(s_socket,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
    if(err == 0)
    {
        printf("client : connect to server\n");
        logfile(4,"client : connect to server");  
	socket_status = 1;
    }else  
    {  
        printf("client : connect error\n");  
//        printf("try reconnect\n");  
        logfile(7,"client : connect error");  
//	socket_reconnect(server_addr);
        return -1;  
    }  
    //process_conn_client(s);  
	return 0;
	
}
/***************************************************************************************
*description: the interface of the alarm send method
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
int send_alarm_old(int ltu_num,int type,U8 *V_A,int alarm_ABC)
{
	int i = 0;
	U8  buffer[256];
	int cmd_len = 0;
	int cmd_type = 4;
	memset(buffer,0,256);
//	int DA_1,DA_2;
	U8 DA_1,DA_2;
//----------alarm type justice----------
	if(type == 0)
		cmd_type = 4;//alarm kongkai
	else if((type == 1))
		cmd_type = 7;//alarm lost V
	else if((type == 2)||(type == 3))
		cmd_type = 5;//alarm to low or to high V
	else if(type == 4)
		cmd_type = 6;//alarm to high A

	DA_1 = (U8)pow(2,((ltu_num-1)%8));
	DA_2 = (U8)ltu_num/8 + 1;
	printf("++++++++++++++the ltu_num = %d ltu_num%8 = %dthe DA = 0x%02x 0x%02x+++++++++++\r\n",ltu_num,(ltu_num%8),DA_1,DA_2);
	if(type == 0||type == 1 || type == 2 ||type == 3 ||type == 4)//--------type = 0 power off---------------
	{
//	cmd_len =(((int)CCO_data_socket[4][2]) << 6) +(((int)CCO_data_socket[4][1])>>2) + 8; 
//		memcpy(buffer,CCO_data_socket[4],47);
		vRtcGetTime();
		cmd_len = 37; 
		memcpy(buffer,CCO_data_socket[10],37);
		buffer[15] = 0x01;
		buffer[16] = (U8)xTime.second*1000;
		buffer[17] = (U8)((xTime.second*1000) >> 8);
		buffer[18] = (U8)xTime.minute;
		buffer[19] = (U8)xTime.hour;
		buffer[20] = (U8)xTime.day;
		buffer[21] = (U8)xTime.month;
		buffer[22] = (U8)xTime.year;
		memcpy(&buffer[23],LTU_number_socket[ltu_num],6);	
#if 1//----------update the V&&A-----------------
		
		memcpy(buffer+29,V_A,6);
		memcpy(buffer+35,&V_A[6],2);
		memcpy(buffer+38,&V_A[8],2);
		memcpy(buffer+41,&V_A[10],2);	
#endif
		if(1)// should make sure the socket is alive
		{
	//	printf("+++++++++++++send alarm successfully!\r\n");
            	write(s_socket,buffer,47);   //Ïò·þÎñÆ÷¶ËÐ´ 
		printf("++++++++++++++++++++++++++++++++send alarm:"); 
		for(i = 0;i<47;i++)
		{
			printf(" 0x%02x",buffer[i]);
		}
		printf("\r\n");	
		}//if(1) should make sure the socket is alive
	}else
	{
		printf("alarm type error!,the type is:%d",type);
	}
}
int send_alarm(int ltu_num,int type,U8 *V_A,int alarm_ABC)
{
	int i = 0;
	U8  data_send[256];
	int cmd_len = 0;
	int cmd_type = 4;
	memset(data_send,0,256);
//	int DA_1,DA_2;
//----------alarm type justice----------
	if(type == 0)
		cmd_type = 1;//alarm kongkai
	else if((type == 1))
		cmd_type = 3;//alarm lost V
	else if((type == 2)||(type == 3))
		cmd_type = 2;//alarm to low or to high V
	else if(type == 4)
		cmd_type = 4;//alarm to high A
printf("=+++++++++++=the alarm type = %d\r\n",type);
//	DA_1 = (U8)pow((ltu_num%8),2);
//	DA_2 = (U8)ltu_num/8 + 1;
//	DA_1 = (U8)pow(2,((ltu_num-1)%8));
//	DA_2 = (U8)ltu_num/8 + 1;
//	printf("++++++++++++++the ltu_num = %d ltu_num%8 = %d+++++++++++\r\n",ltu_num,(ltu_num%8));
	if(type == 0||type == 1 || type == 2 ||type == 3 ||type == 4)//--------type = 0 power off---------------
	{
//	cmd_len =(((int)CCO_data_socket[4][2]) << 6) +(((int)CCO_data_socket[4][1])>>2) + 8; 
//		memcpy(buffer,CCO_data_socket[4],47);
//	cmd_len =(((int)CCO_data_socket[cmd_type][2]) << 6) +(((int)CCO_data_socket[cmd_type][1])>>2) + 8; 
		memcpy(data_send,CCO_data_socket[10],44);
#if 1//----------update the V&&A-----------------
			
			U8 u8_temp = 0x00;	
			u8_temp = (U8)(((g_Imsg_num_send & 0x007f)) << 1) & 0xfe;
			data_send[FRAME_HEADER_LEN+0] = u8_temp;
			u8_temp = (U8)((g_Imsg_num_send & 0x7f80)>>7);
			data_send[FRAME_HEADER_LEN+1] = u8_temp;
			u8_temp = (U8)(((g_Imsg_num_recv & 0x007f)) << 1) & 0xfe;
			data_send[FRAME_HEADER_LEN+2] = u8_temp;
			u8_temp = (U8)((g_Imsg_num_recv & 0x7f80)>>7);
			data_send[FRAME_HEADER_LEN+3] = u8_temp;
			//ASDU
			data_send[APCI_LEN+0] = 0x42;
			data_send[APCI_LEN+1] = 0x00;// if split the message for server the g_ltu_num is not ok
			data_send[APCI_LEN+2] = 0x03;//tranfer reason s
			data_send[APCI_LEN+3] = 0x00;//tranfer reason m
			data_send[APCI_LEN+4] = 0x01;	
			data_send[APCI_LEN+5] = 0x00;
		int alarm_dev_first_point_num = (ltu_num+1)*DEV_POINT_NUM + 1;	
			data_send[APCI_LEN+6] = (U8)(alarm_dev_first_point_num & 0x000000ff);	
			data_send[APCI_LEN+7] = (U8)(alarm_dev_first_point_num & 0x0000ff00);	
			data_send[APCI_LEN+8] = (U8)(alarm_dev_first_point_num & 0x00ff0000);	
		data_send[APCI_LEN+ASDU_HEAD_LEN+0] = (U8)cmd_type|(U8)(alarm_ABC<<6);
		data_send[APCI_LEN+ASDU_HEAD_LEN+1] = (U8)xTime.second*1000;
		data_send[APCI_LEN+ASDU_HEAD_LEN+2] = (U8)((xTime.second*1000) >> 8);
		data_send[APCI_LEN+ASDU_HEAD_LEN+3] = (U8)xTime.minute;
		data_send[APCI_LEN+ASDU_HEAD_LEN+4] = (U8)xTime.hour;
		data_send[APCI_LEN+ASDU_HEAD_LEN+5] = (U8)xTime.day;
		data_send[APCI_LEN+ASDU_HEAD_LEN+6] = (U8)xTime.month;
		data_send[APCI_LEN+ASDU_HEAD_LEN+7] = (U8)xTime.year;
		memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+8],&LTU_number_socket[ltu_num],6);//ltu_number
		memcpy(&data_send[29],V_A,6);
		memcpy(&data_send[36],&V_A[6],2);
		memcpy(&data_send[39],&V_A[8],2);
		memcpy(&data_send[42],&V_A[10],2);	
#endif
		if(socket_status )// should make sure the socket is alive
		{
		printf("+++++++++++++send alarm successfully!\r\n");
		write(s_socket,data_send,44);   //Ïò·þÎñÆ÷¶ËÐ´
		g_Imsg_num_recv++; 
		printf("++++++++++++++++++++++++++++++++send alarm:"); 
		for(i = 0;i<47;i++)
		{
			printf(" 0x%02x",data_send[i]);
		}
		printf("\r\n");	
		}//if(1) should make sure the socket is alive
	}else
	{
		printf("alarm type error!,the type is:%d",type);
	}
}
#if 0
/***************************************************************************************
*-------------for test
*****************************************************************************************/
int main(int argc,char *argv[])  
{  
  
    struct sockaddr_in server_addr;  
    pthread_t p_write,p_read,p_heart;
    int err;  
    sighandler_t ret;  
    char server_ip[50] = ""; 
    void * retval;
#if 0
    /********************socket()*********************/  
    //s= socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    s= socket(AF_INET,SOCK_STREAM,0);  
    if(s<0)  
    {  
        printf("client : create socket error\n");  
        return -1;  
    }  
    
    ret = signal(SIGTSTP,sig_pipe);  
    if(SIG_ERR == ret)  
    {  
        printf("signal re/quest error!\n");  
        return -1;  
    }  
    else  
        printf("signal request success!\n") ;  
  
  
    /*******************connect()*********************/  
    memset(&server_addr,0,sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(PORT);  
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
    //inet_pton(AF_INET,argv[1],&server_addr.sin_addr.s_addr);  
    printf("please input server ip address : \n");  
    read(0,server_ip,50);  
    //err = inet_pton(AF_INET,server_ip,&server_addr.sin_addr.s_addr);  
    server_addr.sin_addr.s_addr = inet_addr(server_ip);  
  
    err = connect(s,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));  
    if(err == 0)  
    {  
        printf("client : connect to server\n");  
    }  
    else  
    {  
        printf("client : connect error\n");  
        return -1;  
    }
#endif
#if 1
	if(socket_init())
	{
		printf("socket init error!\r\n");
		return -1;
	}

#endif
	sleep(2);
    //process_conn_client(s);  
    pthread_create(&p_read,NULL,process_conn_client_read,NULL);
//    pthread_create(&p_write,NULL,process_conn_client_write,NULL);
    pthread_create(&p_heart,NULL,process_conn_client_heart,NULL);
    
    pthread_join(p_read,&retval);
//	pthread_join(p_write,&retval);
	pthread_join(p_heart,&retval);
    close(s_socket);  
  
}  
#endif
/***************************************************************************************
*description: the thread process method of the heart beat 
*called by: main method
*author:lyb
*
*
*
*****************************************************************************************/
void * process_conn_client_heart(void* arg)  
{  
  
 //   int s ;
    ssize_t size = 0;  
    U8 buffer[Buflen]; 
    struct tm tm_loc;
   // s =(int)s_temp; 
    int len_cmd = 0;
    int num_cmd = 0;
    int i = 0,j = 0;
	U8 socket_response = 0;
sleep(2);
printf("+++++++++++++++ socket status=0x%x",socket_status);
	if(socket_status == 0)
		socket_reconnect(server_addr);

//    for(;;)
	while(1)  
	{
		int socket_recv_number_temp = 0;
		if(socket_status == 0)
		{
			sleep(10);
printf("+++++++heart empty\r\n");
			continue;
		}else sleep(1);
//	if(j < 2)
	if(!g_cmu_register)
	{
		num_cmd = 0;//command of register cmu message
		sleep(2);
	}
	else
        	num_cmd = 2;//command of heartbeat message

        memset(buffer,'\0',Buflen);  
        len_cmd = CCO_data_socket[num_cmd][1]+2; 
        printf("the len_cmd is:%d\r\n",len_cmd);     

        memcpy(buffer,CCO_data_socket[num_cmd],len_cmd); 
        if(num_cmd == 10000)//-------for just nouse test 
        {//set the time
            tm_loc = get_current_time();
   // printf("%d%d%d",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday) ;
   // printf("%s%d:%d:%d",wday[p->tm_wday],p->tm_hour,p->tm_min,p->tm_sec) ;
            buffer[18] = (U8)((p->tm_sec)/10) << 4 + (U8)((p->tm_sec)%10);
            buffer[19] = (U8)((p->tm_min)/10) << 4 + (U8)((p->tm_min)%10);
            buffer[20] = (U8)((p->tm_hour)/10) << 4 + (U8)((p->tm_hour)%10);
            buffer[21] = (U8)((p->tm_mday)/10) << 4 + (U8)((p->tm_mday)%10);
            buffer[22] = (U8)(p->tm_wday) << 5 + (U8)(p->tm_mon);
            buffer[23] = (U8)((1900+p->tm_year)/10) << 4 + (U8)((1900+p->tm_year)%10);
            
        }
	if(num_cmd == 0)//register message
	{
		memcpy(&(buffer[6]),g_cmu_NO,6);
	}
//        buffer[len_cmd-2] = getCS_socket(&buffer[6],len_cmd-8); 
        printf("+++++++++socket send >:");
        for(i = 0;i < len_cmd;i++)
                printf(" 0x%x",buffer[i]);
        printf("\r\n");
	socket_recv_number_temp = g_socket_recv_number;
	if(socket_status)
        write(s_socket,buffer,len_cmd);   //Ïò·þÎñÆ÷¶ËÐ´ 
	logfile(4,"heart cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%02x",log_buffer,buffer[i]);
	}
	logfile(4,log_buffer);
	j++; 
 int cont_recv = SOCKET_ERR_TIME;
 while((socket_recv_number_temp == g_socket_recv_number) && cont_recv)
 {
 	cont_recv --;
 	sleep(1);
 }
 if(socket_recv_number_temp == g_socket_recv_number)
 {
 	//no act message in the 20 seconds
 	socket_response = 0;
 }else
 {
 	socket_response = 1;
 	//register successed or heart act successed
 	if(!g_cmu_register)
 		g_cmu_register = 1;
 	
 }
 if(socket_response)//if no recv response message no delay needed
 {
 	sleep(20);
 }
	}  
}  
/***************************************************************************************
*description: the thread function of the test
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
void * process_conn_client_write(void* arg)  
{  
  
 //   int s ;
    ssize_t size = 0;  
    U8 buffer[Buflen]; 
    struct tm tm_loc;
   // s =(int)s_temp; 
    int len_cmd = 0;
    int num_cmd = 0;
    int i = 0;
    for(;;)  
    {  
        num_cmd = 0;
        memset(buffer,'\0',Buflen);  
//        size = read(0,buffer,Buflen);   // 0£¬±»Ä¬ÈÏµÄ·ÖÅäµ½±ê×¼ÊäÈë  1£¬±ê×¼Êä³ö  2£¬error  
        printf("-->");
        scanf("%d",&num_cmd);
        len_cmd =(((int)CCO_data_socket[num_cmd][2]) << 6) +(((int)CCO_data_socket[num_cmd][1])>>2) + 8; 
        printf("the len_cmd is:%d\r\n",len_cmd);     

        memcpy(buffer,CCO_data_socket[num_cmd],len_cmd); 
        if(num_cmd == 1)
        {//set the time
            tm_loc = get_current_time();
   // printf("%d%d%d",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday) ;
   // printf("%s%d:%d:%d",wday[p->tm_wday],p->tm_hour,p->tm_min,p->tm_sec) ;
            buffer[18] = (U8)((p->tm_sec)/10) << 4 + (U8)((p->tm_sec)%10);
            buffer[19] = (U8)((p->tm_min)/10) << 4 + (U8)((p->tm_min)%10);
            buffer[20] = (U8)((p->tm_hour)/10) << 4 + (U8)((p->tm_hour)%10);
            buffer[21] = (U8)((p->tm_mday)/10) << 4 + (U8)((p->tm_mday)%10);
            buffer[22] = (U8)(p->tm_wday) << 5 + (U8)(p->tm_mon);
            buffer[23] = (U8)((1900+p->tm_year)/10) << 4 + (U8)((1900+p->tm_year)%10);
            
        }
        buffer[len_cmd-2] = getCS_socket(&buffer[6],len_cmd-8); 
        printf("buf_cmd is");
        for(i = 0;i < len_cmd;i++)
                printf(" 0x%x",buffer[i]);
        printf("\r\n");
           
     //   if(size >  0)  
        if(len_cmd >  0)  
        {  
            write(s_socket,buffer,len_cmd);   //Ïò·þÎñÆ÷¶ËÐ´  
  
        //    for(size = 0 ; size == 0 ; size = read(s,buffer,Buflen) );
          //      write(1,buffer,strlen(buffer)+1);   //Ïò±ê×¼Êä³öÐ´  
        }  
    }  
}  
/***************************************************************************************
*description: the method of socket reconnet
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
U8 socket_reconnect(struct sockaddr_in server_addr_)
//struct sockaddr_in server_addr;  
{
	int error;
	int i = 0;
	U8 buffer[Buflen];
	int len_cmd = 0;	
	int num_cmd = 0;
//    	error = connect(s_socket,(struct sockaddr *)&server_addr_,sizeof(struct sockaddr));
//	while(connect(s_socket,(struct sockaddr *)&server_addr_,sizeof(struct sockaddr)))
	while(socket_init() != 0)
	{
printf("reconnect\r\n");
	sleep(2);	
	}
#if 0
	sleep(20);	
        memset(buffer,'\0',Buflen);  
        len_cmd =(((int)CCO_data_socket[num_cmd][2]) << 6) +(((int)CCO_data_socket[num_cmd][1])>>2) + 8; 
        printf("the len_cmd is:%d\r\n",len_cmd);     

        memcpy(buffer,CCO_data_socket[num_cmd],len_cmd); 
        buffer[len_cmd-2] = getCS_socket(&buffer[6],len_cmd-8); 
        printf("+++++reconnect socket buf_cmd is");
        for(i = 0;i < len_cmd;i++)
                printf(" 0x%x",buffer[i]);
        printf("\r\n");
        write(s_socket,buffer,len_cmd);   //Ïò·þÎñÆ÷¶ËÐ´ 

	logfile(4,"reconnect resign cmd:");
	memset(log_buffer,0,256);	
	sprintf(log_buffer,"%s","send->: ");
	for(i = 0;i<len_cmd;i++)
	{
		sprintf(log_buffer,"%s 0x%02x",log_buffer,buffer[i]);
	}
	logfile(4,log_buffer);
	sleep(20);
#endif
	socket_status = 1;
	return 0;

} 
/***************************************************************************************
*description: the thread function of the receive message of the socket
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
void *process_conn_client_read(void * arg )  
{ 
printf("into thread_socket_read_1\r\n"); 
    int size_socket = 0; 
    U8 buffer[256];  //
	U8 *p_buffer = buffer; 
//printf("into thread_socket_read_2\r\n"); 
    int i = 0;
    int len_cmd = 0;
	int rd_flag_socket;
	struct timeval tv_timeout_socket;
//printf("into the socket read method\r\n");	
	fd_set fs_read_socket; 
    for(;;)  
    {  
//printf("into the socket read for(;;)\r\n");
	if(socket_status == 0)
	{
		sleep(10);
//test--------lyb---------------
//printf("testtesttesttesttesttest\r\n");
//message_process_test();
		continue;
	}
#if 1
	rd_flag_socket = 0;
        size_socket = 0;
	FD_ZERO(&fs_read_socket);
	FD_SET(s_socket,&fs_read_socket);
	tv_timeout_socket.tv_sec = 0;
	tv_timeout_socket.tv_usec = 100000;

//printf("into the socket read before select\r\n");

	rd_flag_socket = select(s_socket+1,&fs_read_socket,NULL,NULL,&tv_timeout_socket);
	if(rd_flag_socket <= 0)
	{
	    continue;
	}
//	printf("the rd_flag_socket= %d\r\n",rd_flag_socket);
	if(FD_ISSET(s_socket,&fs_read_socket))
	{
//		printf("the rd_flag is %d\r\n",rd_flag_socket);
//-------test----------------
#if 0
		read(s_socket,p_buffer,1);
		read(s_socket,p_buffer+1,1);
		int len = *(buffer+1);
printf("CCCCCCCCCCCcthe len = %d\r\n",len);
#endif
//-------test----------------	
		size_socket = read(s_socket,buffer,256);
	    	if(size_socket == 0 || size_socket == -1)
	    	{
			printf("the receive buffer is zero!!!!!!\r\n");
		//--------------try------20150908---------
	//	shutdown(s_socket,0);
			close(s_socket);
			s_socket = -1;
			g_disconnect_sign = 1;

			socket_status = 0;
			g_cmu_register = 0;
			socket_reconnect(server_addr);
		//----------------------------------------
			continue;
	    	}else
		{
		//	printf("CCCCCCCCCCCCCCCCCCthe length of the socket is:%d\r\n",size_socket);
			;
		}
//	size = read(s,buffer,Buflen);
	    	if(strcmp(buffer,"quit") == 0)  
	    	{  
			close(s_socket);   //  
			return ; //must modify this code  
	   	}
            	printf("+++++++++socket receive lenght=%d <-:",size_socket);
	    	len_cmd = buffer[1]+2; 
	    	for(i = 0;i < len_cmd;i++)
	        	printf("0x%x ",buffer[i]);
	    	printf("\r\n");
		memset(log_buffer,0,256);	
		sprintf(log_buffer,"%s","recv <-: ");
		for(i = 0;i<len_cmd;i++)
		{
			sprintf(log_buffer,"%s 0x%02x",log_buffer,buffer[i]);
		}
		logfile(4,log_buffer);
	    	get_msg(buffer,size_socket);
	}
      usleep(100000);
#endif
    } 
}  
/***************************************************************************************
*description: the noused message
*called by:
*author:the prototype
*
*
*
******************************************************************************************/
void sig_pipe(int signo)    //´«ÈëÌ×½Ó×ÖÃèÊö·û  
{  
    printf("Catch a signal\n");  
    if(signo == SIGTSTP)  
    {  
  
        printf("receive the sigtstp signal\n");  
        int ret = close(s_socket);  
        if(ret == 0)  
            printf("close the socket success\n");  
        else if(ret ==-1 )  
            printf("wrong!close the socket failed\n");  
        exit(1);  
    }  
} 
/***************************************************************************************
*description: the method of deal with message received
*called by:
*author:lyb
*buff: the message content msg_len: the lenght of the message
*
*
*****************************************************************************************/
U8 get_msg(U8* buff,int msg_len)
{
    U8 ret;
    int len_cmd = 0;
    U8 cs_msg = 0x00;
    len_cmd =*(buff+1);
    U8 sign_1 = *(buff+2)&0x01;
    U8 sign_2 = *(buff+2)&0x02;
    if(!sign_1)// I message
    {
	printf("get I message\r\n");
	ret = 1;
	process_I_msg(buff+2,len_cmd);
    }else if(!sign_1)// S message
    {
	printf("get S message!\r\n");
	ret = 2;
	process_S_msg(buff+2,len_cmd);
    }else  // U message
    {
	printf("get U message!\r\n");
	ret = 3;
	process_U_msg(buff+2,len_cmd);
    }
    if(msg_len > len_cmd+2)
    {
	get_msg(buff+(len_cmd+2),msg_len-(len_cmd+2));
	printf("get double socket message!\r\n");		              
    }
    return ret;
}
/***************************************************************************************
*author:lyb
*
*
*
*****************************************************************************************/
void process_I_msg(U8* data,int len)
{
	int i = 0;
	int message_type = *(data+4);
	U8 data_send[256]={0x00};
	U8 u8_temp;
	U8 V_and_A[50][13]={0x00};//max ltu number 50
	U8 ltu_num[6];
	g_Imsg_num_recv++;
	//int len_msg_send = CCO_data_socket[8][1];
	int len_msg_send = 13+16*(1+g_ltu_num);
//printf("the len_msg_send = 0x%02x\r\n",len_msg_send);
	if(message_type == 100)//call of all cmu and ltu
	{
		send_msg_callall_begin();	
//printf("--------------------the ltu number=%d\r\n",g_ltu_num);
		for(i = 0;i<g_ltu_num;i++)
		{
			memcpy(ltu_num,LTU_number_socket[i],6);
			get_ltu_VA(ltu_num,i,V_and_A[i],13);	
printf("in the process_I_msg V_and_A[] = ");int j = 0;
	for(j = 0;j<13;j++)
		printf(" %02x",V_and_A[i][j]);
printf("\r\n");		
		}
		memcpy(data_send,CCO_data_socket[8],len_msg_send+2);
//		len_msg_send = APCI_LEN+ASDU_HEAD_LEN+LEN_PER_DEV*(g_ltu_num+1);
		if(len_msg_send + 2 > 256)//for multiple message 
		{
			
		}else//for alone message
		{
			//APCI
			data_send[1] = len_msg_send;
			if(g_Imsg_num_send >= 32767)g_Imsg_num_send = 0;
			
			u8_temp = (U8)(((g_Imsg_num_send & 0x007f)) << 1) & 0xfe;
			data_send[FRAME_HEADER_LEN+0] = u8_temp;
			u8_temp = (U8)((g_Imsg_num_send & 0x7f80)>>7);
			data_send[FRAME_HEADER_LEN+1] = u8_temp;
			u8_temp = (U8)(((g_Imsg_num_recv & 0x007f)) << 1) & 0xfe;
			data_send[FRAME_HEADER_LEN+2] = u8_temp;
			u8_temp = (U8)((g_Imsg_num_recv & 0x7f80)>>7);
			data_send[FRAME_HEADER_LEN+3] = u8_temp;
			//ASDU
			data_send[APCI_LEN+0] = 0x64;
			data_send[APCI_LEN+1] = (0x80|(g_ltu_num+1));// if split the message for server the g_ltu_num is not ok
			data_send[APCI_LEN+2] = 0x14;//tranfer reason s
			data_send[APCI_LEN+3] = 0x00;//tranfer reason m
			data_send[APCI_LEN+4] = 0x01;	
			data_send[APCI_LEN+5] = 0x00;	
			data_send[APCI_LEN+6] = 0x01;	
			data_send[APCI_LEN+7] = 0x00;	
			data_send[APCI_LEN+8] = 0x00;	
			
			U8 VA_tmp[2],VB_tmp[2],VC_tmp[2];
			U8 IA_tmp[3],IB_tmp[3],IC_tmp[3];
			int VA_v,VB_v,VC_v,IA_v,IB_v,IC_v;
			VA_v = (int)(UaRms*10);VB_v = (int)(UbRms*10);VC_v = (int)(UcRms*10);
			IA_v = (int)(IaRms*10);IB_v = (int)(IbRms*10);IC_v = (int)(IcRms*10);
//printf("==============IcRms = %f IC_v = %d\r\n",IcRms,IC_v);
			VA_tmp[1] = (VA_v/1000)*16+((VA_v%1000)/100);
			VA_tmp[0] = ((VA_v%100)/10)*16+(VA_v%10);
			VB_tmp[1] = (VB_v/1000)*16+((VB_v%1000)/100);
			VB_tmp[0] = ((VB_v%100)/10)*16+(VB_v%10);
			VC_tmp[1] = (VC_v/1000)*16+((VC_v%1000)/100);
			VC_tmp[0] = ((VC_v%100)/10)*16+(VC_v%10);
			
			IA_tmp[2] = (IA_v/1000)*16+((IA_v%1000)/100);
			IA_tmp[1] = ((IA_v%100)/10)*16+((IA_v%10)/1);
			//IA_tmp[0] = (IA_v%100/10)*16+(IB_v%10);
			IA_tmp[0] = 0x00;
			IB_tmp[2] = (IB_v/1000)*16+((IB_v%1000)/100);
			IB_tmp[1] = ((IB_v%100)/10)*16+((IB_v%10)/1);
			//IB_tmp[0] = (IB_v%100/10)*16+(IC_v%10);
			IB_tmp[0] = 0x00;
		//	IC_tmp[2] =(U8)(((IC_v/1000)<<4)&0x0f) + (U8)(((IC_v%1000)/100)&0xf0);
		//	IC_tmp[1] =(U8)(((IC_v%100)/10)<<4)+(U8)((IC_v%10)/1);
			IC_tmp[2] = 0x00;//temp
			IC_tmp[1] = 0x00;//temp
			//IC_tmp[0] = (IC_v%100/10)*16+(IC_v%10);
			IC_tmp[0] = 0x00;
printf("TTTTTTTTTTTTTTTTTTTTTTthe IABC=0x%02x %02x 0x%02x%02x 0x%02x%02x",IA_tmp[2],IA_tmp[1],IB_tmp[2],IB_tmp[1],IC_tmp[2],IC_tmp[1]);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+0],VA_tmp,2);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+2],VB_tmp,2);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+4],VC_tmp,2);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+6],IA_tmp,3);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+9],IB_tmp,3);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+12],IC_tmp,3);			
			data_send[APCI_LEN+ASDU_HEAD_LEN+15] = 0x01;//cmu no use the byte
printf("the g_ltu_num=%d\r\n",g_ltu_num);
			if(g_ltu_num)//	
			for(i = 0;i<g_ltu_num;i++)
			{
			U8 VA_ltu[2],VB_ltu[2],VC_ltu[2];
			U8 IA_ltu[3],IB_ltu[3],IC_ltu[3];
			int VA_vl,VB_vl,VC_vl,IA_vl,IB_vl,IC_vl;
			VA_vl = V_and_A[i][1]<<8+V_and_A[i][0];
			VB_vl = V_and_A[i][3]*256+V_and_A[i][2];
			VC_vl = V_and_A[i][5]*256+V_and_A[i][4];
printf("the VA VB VC = %d %d %d\r\n",VA_vl,VB_vl,VC_vl);
			IA_vl = V_and_A[i][7]*256+V_and_A[i][6];
			IB_vl = V_and_A[i][9]*256+V_and_A[i][8];
			IC_vl = V_and_A[i][11]*256+V_and_A[i][10];
			
		/*
			VA_ltu[1] = (VA_vl/1000)*16+((VA_vl%1000)/100);
			VA_ltu[0] = ((VA_vl%100)/10)*16+(VA_vl%10);
			VB_ltu[1] = (VB_vl/1000)*16+((VB_vl%1000)/100);
			VB_ltu[0] = ((VB_vl%100)/10)*16+(VB_vl%10);
			VC_ltu[1] = (VC_vl/1000)*16+((VC_vl%1000)/100);
			VC_ltu[0] = ((VC_vl%100)/10)*16+(VC_vl%10);
		*/	
			VA_ltu[1] = V_and_A[i][1];
			VA_ltu[0] =  V_and_A[i][0];
			VB_ltu[1] =  V_and_A[i][3];
			VB_ltu[0] =  V_and_A[i][2];
			VC_ltu[1] =  V_and_A[i][5];
			VC_ltu[0] =  V_and_A[i][4];
			
			IA_ltu[2] = (IA_vl/1000)*16+((IA_vl%1000)/100);
			IA_ltu[1] = ((IA_vl%100)/10)*16+((IA_vl%10)/1);
			IA_ltu[0] = 0x00;
			IB_ltu[2] = (IB_vl/1000)*16+((IB_vl%1000)/100);
			IB_ltu[1] = ((IB_vl%100)/10)*16+((IB_vl%10)/1);
			IB_ltu[0] = 0x00;
			IC_ltu[2] = ((IC_vl/1000)<<4)+((IC_vl%1000)/100);
			IC_ltu[1] = (((IC_vl%100)/10)<<4)+((IC_vl%10)/1);
			IC_ltu[0] = 0x00;
			
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+LEN_PER_DEV*(i+1)+0],VA_ltu,2);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+LEN_PER_DEV*(i+1)+2],VB_ltu,2);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+LEN_PER_DEV*(i+1)+4],VC_ltu,2);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+LEN_PER_DEV*(i+1)+6],IA_ltu,3);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+LEN_PER_DEV*(i+1)+9],IB_ltu,3);
			memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+LEN_PER_DEV*(i+1)+12],IC_ltu,3);
			data_send[APCI_LEN+ASDU_HEAD_LEN+LEN_PER_DEV*(i+1)+15] = (V_and_A[i][12]) ? 2:1;
			}
		}
		printf("=========socket response is");        
		for(i = 0;i < len_msg_send+2;i++)                
			printf(" 0x%02x",data_send[i]);       
		printf("\r\n");        
		write(s_socket,data_send,len_msg_send+2);
		g_Imsg_num_send++;
		sleep(2);
		send_msg_callall_end(); 
	}else if(message_type == 0x6C)
	{
//		send_msg_callall_begin();
		send_msg_cmu_call();		
//		send_msg_callall_end(); 
		
	}

    return; 
}
/***************************************************************************************
*description: the methd of process the message of S make sure of the I message
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
void process_S_msg(U8 * data,int len)
{
    printf("into the process_0x0E message\r\n");
	logfile(4,"process 0x0E message");
    return;
}
/***************************************************************************************
*description: the methd of process the message of U control message
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
void process_U_msg(U8 * data,int len)
{
	int message_type;
	if((*data)&0x04 == 0x04)//startdt_begin
	{
		printf("recive startdt_begin message");
		g_cmu_register = 1;
	}else if((*data)& 0x08 == 0x08)//startdt_sure
	{
		printf("recive startdt_sure message");
		g_cmu_register = 1;
	}else if((*data)& 0x10 == 0x10)//stopdt_begin
	{
		printf("recive stopdt_begin message");
		
	}else if((*data)&0x20 == 0x20)//stopdt_sure
	{
		printf("recive stopdt_sure message");
		
	}else if((*data)&0x40 == 0x40)//testfr_begin
	{
		printf("recive testfr_begin message");
		
	}else if(*(data)&0x80 == 0x80)//testfr_sure
	{
		printf("recive testfr_sure message");
	}
    return;
}

/***************************************************************************************
*description: the methd of process the message of U control message
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
void send_msg_callall_begin()
{
	int i = 0;
	U8 data_send[256] = {0x00};
	U8 u8_temp=0x00;
	int len_msg_send = CCO_data_socket[3][1]+2;
	memcpy(data_send,CCO_data_socket[3],len_msg_send+2);
			u8_temp = (U8)(((g_Imsg_num_send & 0x007f)) << 1) & 0xfe;
			data_send[FRAME_HEADER_LEN+0] = u8_temp;
			u8_temp = (U8)((g_Imsg_num_send & 0x7f80)>>7);
			data_send[FRAME_HEADER_LEN+1] = u8_temp;
			u8_temp = (U8)(((g_Imsg_num_recv & 0x007f)) << 1) & 0xfe;
			data_send[FRAME_HEADER_LEN+2] = u8_temp;
			u8_temp = (U8)((g_Imsg_num_recv & 0x7f80)>>7);
			data_send[FRAME_HEADER_LEN+3] = u8_temp;
	write(s_socket,data_send,len_msg_send+2);
	g_Imsg_num_send++; 
	printf(" CCCCCCCCCCCCCCCCCCCCccallall  begin socket send all call confirm data->:");
	for(i = 0;i<len_msg_send;i++)
		printf("0x%02x ",data_send[i]);
	printf("\r\n");
	return;
}

/***************************************************************************************
*description: the methd of process the message of U control message
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
void send_msg_callall_end()
{
	int i = 0;
	U8 data_send[256] = {0x00};
	U8 u8_temp = 0x00;
	int len_msg_send = CCO_data_socket[9][1]+2;
	memcpy(data_send,CCO_data_socket[9],len_msg_send+2);
	printf("=============the g_Imsg_num_send=%d\r\n",g_Imsg_num_send);
			u8_temp = (U8)(((g_Imsg_num_send & 0x007f)) << 1) & 0xfe;
			data_send[FRAME_HEADER_LEN+0] = u8_temp;
			u8_temp = (U8)((g_Imsg_num_send & 0x7f80)>>7);
			data_send[FRAME_HEADER_LEN+1] = u8_temp;
			u8_temp = (U8)(((g_Imsg_num_recv & 0x007f)) << 1) & 0xfe;
			data_send[FRAME_HEADER_LEN+2] = u8_temp;
			u8_temp = (U8)((g_Imsg_num_recv & 0x7f80)>>7);
			data_send[FRAME_HEADER_LEN+3] = u8_temp;
	write(s_socket,data_send,len_msg_send+2);
	g_Imsg_num_send++; 
	printf("socket send all call confirm data->:");
	for(i = 0;i<len_msg_send;i++)
		printf("0x%02x ",data_send[i]);
	printf("\r\n");
	
	return;
}
void message_process_test()
{
	U8 test_data[256] = {0x68,0x00,0x00,0x00,0x64,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	process_I_msg(test_data,10);
}

/***************************************************************************************
*description: the methd of process the message of U control message
*called by:
*author:lyb
*
*
*
*****************************************************************************************/
void send_msg_cmu_call()
{
	int i = 0;
	U8 data_send[256] = {0x00};
	U8 u8_temp = 0x00;
	int len_msg_send = CCO_data_socket[11][1]+2;
	memcpy(data_send,CCO_data_socket[11],len_msg_send+2);
	printf("============= send callcmuresponsemsg the g_Imsg_num_send=%d\r\n",g_Imsg_num_send);
	u8_temp = (U8)(((g_Imsg_num_send & 0x007f)) << 1) & 0xfe;
	data_send[FRAME_HEADER_LEN+0] = u8_temp;
	u8_temp = (U8)((g_Imsg_num_send & 0x7f80)>>7);
	data_send[FRAME_HEADER_LEN+1] = u8_temp;
	u8_temp = (U8)(((g_Imsg_num_recv & 0x007f)) << 1) & 0xfe;
	data_send[FRAME_HEADER_LEN+2] = u8_temp;
	u8_temp = (U8)((g_Imsg_num_recv & 0x7f80)>>7);
	data_send[FRAME_HEADER_LEN+3] = u8_temp;
			
//	data_send[APCI_LEN+0] = 0x6C;
//	data_send[APCI_LEN+1] = (0x80|0x01);// if split the message for server the g_ltu_num is not ok
//	data_send[APCI_LEN+2] = 0x0e;
//	data_send[APCI_LEN+3] = 0x00;
			data_send[APCI_LEN+0] = 0x6C;
			data_send[APCI_LEN+1] = (0x80|0x01);// if split the message for server the g_ltu_num is not ok
			data_send[APCI_LEN+2] = 0x14;//tranfer reason s
			data_send[APCI_LEN+3] = 0x00;//tranfer reason m
			data_send[APCI_LEN+4] = 0x01;	
			data_send[APCI_LEN+5] = 0x00;	
			data_send[APCI_LEN+6] = 0x01;	
			data_send[APCI_LEN+7] = 0x00;	
			data_send[APCI_LEN+8] = 0x00;	
	
	U8 VA_tmp[2],VB_tmp[2],VC_tmp[2];
	U8 IA_tmp[3],IB_tmp[3],IC_tmp[3];
	int VA_v,VB_v,VC_v,IA_v,IB_v,IC_v;
	VA_v = (int)(UaRms*10);VB_v = (int)(UbRms*10);VC_v = (int)(UcRms*10);
	IA_v = (int)(IaRms*10);IB_v = (int)(IbRms*10);IC_v = (int)(IcRms*10);
//printf("==============IcRms = %f IC_v = %d\r\n",IcRms,IC_v);
	VA_tmp[1] = (VA_v/1000)*16+((VA_v%1000)/100);
	VA_tmp[0] = ((VA_v%100)/10)*16+(VA_v%10);
	VB_tmp[1] = (VB_v/1000)*16+((VB_v%1000)/100);
	VB_tmp[0] = ((VB_v%100)/10)*16+(VB_v%10);
	VC_tmp[1] = (VC_v/1000)*16+((VC_v%1000)/100);
	VC_tmp[0] = ((VC_v%100)/10)*16+(VC_v%10);
		
	IA_tmp[2] = (IA_v/1000)*16+((IA_v%1000)/100);
	IA_tmp[1] = ((IA_v%100)/10)*16+((IA_v%10)/1);
	//IA_tmp[0] = (IA_v%100/10)*16+(IB_v%10);
	IA_tmp[0] = 0x00;
	IB_tmp[2] = (IB_v/1000)*16+((IB_v%1000)/100);
	IB_tmp[1] = ((IB_v%100)/10)*16+((IB_v%10)/1);
	//IB_tmp[0] = (IB_v%100/10)*16+(IC_v%10);
	IB_tmp[0] = 0x00;
	//	IC_tmp[2] =(U8)(((IC_v/1000)<<4)&0x0f) + (U8)(((IC_v%1000)/100)&0xf0);
	//	IC_tmp[1] =(U8)(((IC_v%100)/10)<<4)+(U8)((IC_v%10)/1);
	IC_tmp[2] = 0x00;//temp
	IC_tmp[1] = 0x00;//temp
	//IC_tmp[0] = (IC_v%100/10)*16+(IC_v%10);
	IC_tmp[0] = 0x00;
printf("TTTTTTTTTTTTTTTTTTTTTTthe IABC=0x%02x %02x 0x%02x%02x 0x%02x%02x",IA_tmp[2],IA_tmp[1],IB_tmp[2],IB_tmp[1],IC_tmp[2],IC_tmp[1]);
	memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+0],VA_tmp,2);
	memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+2],VB_tmp,2);
	memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+4],VC_tmp,2);
	memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+6],IA_tmp,3);
	memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+9],IB_tmp,3);
	memcpy(&data_send[APCI_LEN+ASDU_HEAD_LEN+12],IC_tmp,3);			
	data_send[APCI_LEN+ASDU_HEAD_LEN+15] = 0x01;//cmu no use the byte
			
	write(s_socket,data_send,len_msg_send+2);
	g_Imsg_num_send++; 
	printf("socket send call cmu message data->:");
	for(i = 0;i<len_msg_send;i++)
		printf("0x%02x ",data_send[i]);
	printf("\r\n");
	
	return;
}



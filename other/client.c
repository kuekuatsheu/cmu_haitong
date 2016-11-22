#include <stdio.h>  
#include <strings.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
//#include <linux/in.h>  
#include <stdlib.h>  
#include <memory.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  
#include <semaphore.h>  
#include <signal.h> //添加信号处理  防止向已断开的连接通信  
#include<pthread.h> 
#include<time.h>

#include "client.h"

 
#define PORT    7899 
#define Buflen  1024  

char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
U8 CCO_data[][256]={
//--0x68----L1             L2--0x68---C----A12---------A22------A3-----AFN-SEQ--DA2---------DT2------CS1--0x16--
    {0x68, 0x32,0x00,0x32,0x00,0x68, 0xDA, 0x00,0x00, 0x00,0x01,0x02, 0x02,0x71, 0x00,0x00, 0x01,0x00,0x00,0x16 },//0----- 0x03 F10 req route  module version
    {0x68, 0x4A,0x00,0x4A,0x00,0x68, 0xDA, 0x00,0x00, 0x00,0x01,0x02, 0x02,0x71, 0x00,0x00, 0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x16 },//1----- 0x03 F10 req route  module version
    {0x68, 0xAA,0x00,0xAA,0x00,0x68, 0xDA, 0x00,0x00, 0x00,0x01,0x02, 0x0E,0x71, 0x01,0x00, 0x02,0x00,\
    0x0A,0x1C,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x87,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,                                                0x00,0x16 },//2
    //0----- 0x03 F10 req route  module version
    {0x68, 0x5E,0x00,0x5E,0x00,0x68, 0xDA, 0x00,0x00, 0x00,0x01,0x02, 0x0E,0x71, 0x01,0x01, 0x02,0x00,\
    0x00,0x00,0x00,0x00,  0x32,0x05,0x00,0x00,0x00,0x00,0x00,                                         0x00,0x16 },//3
   {0x68, 0x3E,0x01,0x4A,0x00,0x68, 0xDA, 0x00,0x00, 0x00,0x01,0x02, 0x0C,0x71, 0x01,0x01, 0x01,0x03,\
    0x00,0x00,0x00,0x00,0x00, 0x11,0x11,0x11, 0x11,0x11,0x11, 0x11,0x11,0x11, 0x11,0x11,0x11,\
                              0x22,0x22,0x22, 0x22,0x22,0x22, 0x22,0x22,0x22, 0x22,0x22,0x22,\
                              0x33,0x33, 0x33,0x33, 0x33,0x33, 0x33,0x33, 0x44,0x44, 0x44,0x44, 0x44,0x44,\
                              0x55,0x55,0x55,  0x55,0x55,0x55, 0x55,0x55,0x55,\
    0x66,0x66,0x66,  0x77,0x77,0x77,  0x88,0x88,0x88, 0x88,0x88,0x88, 0x88,0x88,0x88,                 0x00,0x16 },//4
{0}
};

int s = 0;   
struct tm *p = NULL;
sem_t sem_socket_rw;
  
struct tm get_current_time()
{
    time_t timep;
    time(&timep);
    p=localtime(&timep);
    return *p;
}

U8 getCS(U8* data,int len)
{
    U8 result = 0;
    int i = 0;
    for(i = 0; i < len; i++)
        result += *data++;
    return(result);
}
int main(int argc,char *argv[])  
{  
  
    struct sockaddr_in server_addr;  
    pthread_t p_write,p_read;
    int err;  
    sighandler_t ret;  
    char server_ip[50] = ""; 
    void * retval;

    /********************socket()*********************/ 

    /********************socket()*********************/  
    /********************socket()*********************/  
    s= socket(AF_INET,SOCK_STREAM,0);  
    if(s<0)  
    {  
        printf("client : create socket error\n");  
        return 1;  
    }  
  //-------------------------------
  sem_init(&sem_socket_rw,0,1);
  //-------------------------------
    //信号处理函数  SIGINT 是当用户按一个 Ctrl-C 建时发送的信号  
    ret = signal(SIGTSTP,sig_pipe);  
    if(SIG_ERR == ret)  
    {  
        printf("signal request error!\n");  
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
    //process_conn_client(s);  
    pthread_create(&p_write,NULL,process_conn_client_write,NULL);
    pthread_create(&p_read,NULL,process_conn_client_read,NULL);
    
    pthread_join(p_write,&retval);
    pthread_join(p_read,&retval);
    close(s);  
  
}  
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
    long msg_number = 0;
    for(;;)  
    {  
        //num_cmd = 0;
        memset(buffer,0x00,Buflen);  
#if 1
//	size = read(0,buffer,Buflen);
        printf("-->");
        scanf("%d",&num_cmd);
#endif
#if 0
//        if(msg_number == 0) 
	if(msg_number <= 3)
            num_cmd = 0;
        else
            num_cmd = 1;
	printf("msg_number = %d\r\n",msg_number);
	msg_number++;
#endif
        len_cmd =(((int)CCO_data[num_cmd][2]) << 6) +(((int)CCO_data[num_cmd][1])>>2) + 8; 
        printf("the len_cmd is:%d\r\n",len_cmd);     

        memcpy(buffer,CCO_data[num_cmd],len_cmd); 
        if(num_cmd == 1)
        {//set the time
            tm_loc = get_current_time();
            buffer[18] = (U8)((p->tm_sec)/10) << 4 + (U8)((p->tm_sec)%10);
            buffer[19] = (U8)((p->tm_min)/10) << 4 + (U8)((p->tm_min)%10);
            buffer[20] = (U8)((p->tm_hour)/10) << 4 + (U8)((p->tm_hour)%10);
            buffer[21] = (U8)((p->tm_mday)/10) << 4 + (U8)((p->tm_mday)%10);
            buffer[22] = (U8)(p->tm_wday) << 5 + (U8)(p->tm_mon);
            buffer[23] = (U8)((1900+p->tm_year)/10) << 4 + (U8)((1900+p->tm_year)%10);
            
        }
        buffer[len_cmd-2] = getCS(&buffer[6],len_cmd-8); 
	    sem_wait(&sem_socket_rw);
            write(s,buffer,len_cmd);   //向服务器端写  
	    sem_post(&sem_socket_rw);
        printf("buf_cmd is");
        for(i = 0;i < len_cmd;i++)
                printf(" 0x%x",buffer[i]);
        printf("\r\n");
           
	sleep(5); 
    } 
}  
  
void *process_conn_client_read(void * arg )  
{  
    int size = 0; 
 //   U8 buffer[Buflen];  // 
	U8 buffer[256];
    int i = 0;
    int len_cmd = 0; 
    
    static struct timeval tv_timeout;
    int rd_flag = 0;
    fd_set fs_read;
    printf("into the read message\r\n");
    for(;;)  
    {  
	rd_flag = 0;
        size = 0;
	tv_timeout.tv_sec = 0;
	tv_timeout.tv_usec = 100000;
	FD_ZERO(&fs_read);
	FD_SET(s,&fs_read);
//	printf("receive message\r\n");
//        for(size = 0;size == 0 ;size = read(s,buffer,Buflen));  
	rd_flag = select(s+1,&fs_read,NULL,NULL,&tv_timeout);
//	printf("the rd_flag = %d\r\n",rd_flag);
	if(rd_flag <= 0)
	{
	    continue;
	}
	if(FD_ISSET(s,&fs_read))
	{
	    printf("the rd_flag is %d\r\n",rd_flag);

	    sem_wait(&sem_socket_rw);
//	    size = read(s,buffer,Buflen);
//	    size = recv(s,buffer,Buflen,0);
		size = recv(s,buffer,256,0);
	    sem_post(&sem_socket_rw);
	    if(size == 0)
	    {
		printf("the receive buffer is zero!!!!!!\r\n");
		continue;
	    }
//	size = read(s,buffer,Buflen);
	    if(strcmp(buffer,"quit") == 0)  
	    {  
		close(s);   //  
		return ;  
	    }
            printf("receive message:");
	    len_cmd =(((int)buffer[2]) << 6) +(((int)buffer[1])>>2) + 8; 
	    for(i = 0;i < len_cmd;i++)
	        printf("0x%x ",buffer[i]);
	    printf("\r\n");
	    get_msg(buffer);
	}
        //sprintf(buffer,"%d bytes altogether\n",size);  
      //  write(s,buffer,strlen(buffer)+1); 
      usleep(10000);
    }  
}  
void sig_pipe(int signo)    //传入套接字描述符  
{  
    printf("Catch a signal\n");  
    if(signo == SIGTSTP)  
    {  
  
        printf("receive the sigtstp signal\n");  
        int ret = close(s);  
        if(ret == 0)  
            printf("close the socket success\n");  
        else if(ret ==-1 )  
            printf("wrong!close the socket failed\n");  
        exit(1);  
    }  
} 

U8 get_msg(U8* buff)
{
    U8 ret;
    int len_cmd = 0;
    U8 cs_msg = 0x00;
    U8 AFN_msg = buff[12];
    len_cmd =(((int)buff[2]) << 6) +(((int)buff[1])>>2) + 8;
    cs_msg = getCS(&buff[6],len_cmd-8);    
    if(cs_msg != buff[len_cmd-2])
       // return -1;
        printf("check CS error!");
    switch(AFN_msg)
    {
        case 0x00:// data confirm or deny
        break;
        case 0x01://reset
        break;
        case 0x02://link interface detection
        break;
        case 0x03://relay station commond
        break;
        case 0x04://setting parameters
        break;
        case 0x05://control commond
        break;
        case 0x06://identity authentication an key negotiation
        break;
        case 0x08://request terminal active report
        break;
        case 0x09://request terminal configuration
        break;
        case 0x0A://query parameters
        break;
        case 0x0B://request task data
        break;
        case 0x0C://request data of kind 1 
        process_0X0C(buff+6);
        break;
        case 0x0D://request data of kind 2
        break;
        case 0x0E://request data of kind 3
        process_0X0E(buff+6);
        break;
        case 0x0F://file transfer
        break;
        case 0x10://data retransimission
        break;
    }
    ret = AFN_msg;//return the AFN                     
}
void process_0X0C(U8* data)
{
    int i = 0;
    U8 buffer[128];
    int DA_msg = data[11]*8+data[10];
    int len_cmd = 0;
    printf("into the process_0x0C message\r\n");
    printf("the AFN is:%d",data[6]);
    if(DA_msg == 25)
    { 
        len_cmd =(((int)CCO_data[4][2]) << 6) +(((int)CCO_data[4][1])>>2) + 8; 
        printf("the len_cmd is:%d\r\n",len_cmd);     

        memcpy(buffer,CCO_data[4],len_cmd); 
        buffer[len_cmd-2] = getCS(&buffer[6],len_cmd-8); 
        printf("buf_cmd is");
        for(i = 0;i < len_cmd;i++)
                printf(" 0x%x",buffer[i]);
        printf("\r\n");
        write(s,buffer,len_cmd);  
    }
    return; 
}
void process_0X0E(U8 * data)
{
    printf("into the process_0x0E message\r\n");
    return;
}


void * process_socket(void* arg) 
{
	return;	
} 




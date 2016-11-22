/*client.h*/
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
  
#include <signal.h> //Ìí¼ÓÐÅºÅ´¦Àí  ·ÀÖ¹ÏòÒÑ¶Ï¿ªµÄÁ¬½ÓÍ¨ÐÅ  
#include <pthread.h> 
#include <time.h>
#include <math.h>
#include "general/Variable.h"
#include "log.h"

#ifndef GLOBAL
#define GLOBAL
#include "general/global.h"
#endif
#include "BSP_R8025T.h"
//#include "plc.h"

int s_socket;
typedef void (*sighandler_t)(int);
int socket_init();
int send_alarm(int ltu_num,int type,U8 *V_A,int alarm_ABC);
void* process_conn_client_write(void*);
void* process_conn_client_read(void*);  
void* process_conn_client_heart(void*);  
void sig_pipe(int signo);    //ÓÃ»§×¢²áµÄÐÅºÅº¯Êý,½ÓÊÕµÄÊÇÐÅºÅÖµ  
U8 getCS(U8* data,int len);
U8 message_process(U8*);
//struct tm* localtime(const time_t* timep);
struct tm get_current_time();
U8 get_msg(U8* buff,int len);
void process_0X0C(U8*);
void process_0X0E(U8*);

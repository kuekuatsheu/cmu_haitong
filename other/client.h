#include "global.h"


typedef void (*sighandler_t)(int);
void* process_conn_client_write(void*);
void* process_conn_client_read(void*);  
void sig_pipe(int signo);    //ÓÃ»§×¢²áµÄÐÅºÅº¯Êý,½ÓÊÕµÄÊÇÐÅºÅÖµ  
U8 getCS(U8* data,int len);
U8 message_process(U8*);
//struct tm* localtime(const time_t* timep);
struct tm get_current_time();
U8 get_msg(U8* buff);
void process_0X0C(U8*);
void process_0X0E(U8*);

void * process_socket(void* arg);  

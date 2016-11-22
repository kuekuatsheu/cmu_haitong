#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdarg.h>
#include<semaphore.h>
#include<signal.h>

sem_t sem_1;
sem_t sem_2;
sem_t sem_3;
sem_t sem_4;
sem_t sem_5;
sem_t sem_6;
sem_t sem_7;
int log_init();
void logfile(int type,const char* fmt,...);

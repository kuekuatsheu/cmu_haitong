#include "log.h"
#include "general/Variable.h"
#if 0
int main()
{
	int i = 0;
	for(i = 0;i < 100;i++)
	{
		logfile(3,"%s","log system");
		usleep(100000);
	}
	return;
}
#endif
sem_t sem_1;
sem_t sem_2;
sem_t sem_3;
sem_t sem_4;
sem_t sem_5;
sem_t sem_6;
sem_t sem_7;
xTIME xTime_pre;

int log_init()
{
	int ret_sem = -1;
	ret_sem = sem_init(&sem_1,0,1);
	if(ret_sem)
		printf("+++++++++sem_1 request error!\r\n");
	ret_sem = sem_init(&sem_2,0,1);
	if(ret_sem)
		printf("+++++++++sem_2 request error!\r\n");
	ret_sem = sem_init(&sem_3,0,1);
	if(ret_sem)
		printf("+++++++++sem_3 request error!\r\n");
	ret_sem = sem_init(&sem_4,0,1);
	if(ret_sem)
		printf("+++++++++sem_4 request error!\r\n");
	ret_sem = sem_init(&sem_5,0,1);
	if(ret_sem)
		printf("+++++++++sem_5 request error!\r\n");
	ret_sem = sem_init(&sem_6,0,1);
	if(ret_sem)
		printf("+++++++++sem_6 request error!\r\n");
	ret_sem = sem_init(&sem_7,0,1);
	if(ret_sem)
		printf("+++++++++sem_7 request error!\r\n");
	xTime_pre = xTime;
}
int get_log_name(char* name)
{
//	if((xTime.year == xTime_pre.year)&&(xTime.month == xTime_pre.month)&&(xTime.day == xTime_pre.day))
//	{
//		return 0;//no change
//	}else
//	{
		char name_temp[256];
		sprintf(name_temp,"//lyb/log/get_VA_20%02d%02d%02d.log\0",xTime.year,xTime.month,xTime.day);
		memcpy(name,name_temp,sizeof(name_temp));
		return 0;//change
//	}
}
void logfile(int type,const char *fmt,...)
{
	FILE *fp;
	char buffer[512];
	char *p;
	time_t t;
	va_list ap;
	char name[128];
	int ret_sem = -1;
	if(type == 1)
	{
//		memcpy(name,"log/general.log",sizeof("log/general.log"));
//		ret_sem = sem_init(&sem_1,0,1);
		return;
	}
	else if(type == 2)
	{
//		memcpy(name,"log/current.log",sizeof("log/current.log"));
//		ret_sem = sem_init(&sem_2,0,1);
		return;
	}
	else if(type == 3)
	{
//		memcpy(name,"log/voltage.log",sizeof("log/voltage.log"));
//		ret_sem = sem_init(&sem_3,0,1);
		return;
	}
	else if(type == 4)	
	{
//		memcpy(name,"log/376_1.log",sizeof("log/376_1.log"));
//		ret_sem = sem_init(&sem_4,0,1);
		return;
	}
	else if(type == 5)
	{
//		memcpy(name,"log/temp.log",sizeof("log/temp.log"));
//		ret_sem = sem_init(&sem_5,0,1);
		return;
	}
	else if(type == 6)
	{
		int val;
		char log_name[256];
//		sprintf(log_name,"//lyb/log/get_VA.log");
		sem_getvalue(&sem_6,&val);
		printf("the sem value is:%d",val);
		sem_wait(&sem_6);
		get_log_name(log_name);
		memcpy(name,log_name,sizeof(log_name));
//		memcpy(name,"//media/mmcblk0p1/log/get_VA.log",sizeof("//media/mmcblk0p1/log/get_VA.log"));
	}
	else if(type == 7)
	{
		return;
		sem_wait(&sem_7);
//		memcpy(name,"//media/mmcblk0p1/log/sys_error.log",sizeof("//media/mmcblk0p1/log/sys_error.log"));		
		memcpy(name,"//lyb/log/sys_error.log",sizeof("//lyb/log/sys_error.log"));		
//		return;
	}
	va_start(ap,fmt);
	vsprintf(buffer,fmt,ap);
	va_end(ap);
//	if((fp=fopen("log/general.log","a+")) == NULL)
	if((fp=fopen(name,"a+")) == NULL)
	{
		printf("open logfile failed name = %s\r\n",name);
	if(type == 7)
		sem_post(&sem_7);
	else if(type == 6)
		sem_post(&sem_6);
		
#if 0
		usleep(2);
		if((fp = fopen(name,"a+")) == NULL)
		{
			printf("retry open logfile failed\r\n");
			return;
		}
#endif
		return;
	}
	time(&t);//use this for the open server logs test
	p=ctime(&t);
	p[strlen(p) - 1] = '\0';
	fprintf(fp,"[%s]%s\n",p,buffer);
	fclose(fp);
	if(type == 7)
		sem_post(&sem_7);
	else if(type == 6)
		sem_post(&sem_6);

}

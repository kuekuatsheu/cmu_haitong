#include "cmu.h"
//#include "log.h"

void main(void)
{
//	pthread_t p_write,p_read,p_test;
	void *retval;
	int i = 0;
//	plc_init();
//	OLED_Init();
//	R8025_Init();
	log_init();
printf("for test!\r\n");	
	for(i = 0;i<100000;i++)
	{
		char str_printf[28];
		sprintf(str_printf,"test i = %d\r\n",i);
		logfile(6,str_printf);
		sleep(10);
	}
}


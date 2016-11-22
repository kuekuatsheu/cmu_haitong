/*cmu main programme file 
	author:lyb	*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <asm/termios.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <string.h>
#ifndef SERIAL
#define SERIAL
#include "general/serial.h"
#endif
#include "BSP_R8025T.h"
//#include "BSP_Oled.h"
#ifndef LOGFILE
#define LOGFILE
#include "log.h"
#endif
#include "BSP_ATT7022EU.h"
#include "plc.h"
//#define PLC_RST 45

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#ifndef GLOBAL
#define GLOBAL
#include "general/global.h"
#endif

#ifndef g_GPIO
#define g_GPIO
#include "general/gpio.h"
#endif
#include "general/serial.h"
#include "general/Variable.h"
#include "BSP_Oled.h"
#include "BSP_R8025T.h"
#ifndef _SERIAL_H
#define _SERIAL_H 1
#endif

int adjust_init();

void *thread_adjust_read();
void *thread_adjust_write(); 
void adjust_close();

#ifndef GLOBAL
#define GLOBAL
#include "global.h"
#endif

#ifndef _SERIAL_H
#define _SERIAL_H 1



/* serial.c */
INT32 OpenComPort (INT32 ComPort, INT32 baudrate, INT32 databit,const char *stopbit, char parity,INT32 *fd);
void CloseComPort (INT32 fd);

INT32 ReadComPort (void *data, INT32 datalength,INT32 fd);
INT32 ReadComPort_byte (U8 *data, INT32 datalength, INT32 fd,U8 mode);
INT32 WriteComPort (UINT8 * data, INT32 datalength, INT32 fd);

/**
 * export serial fd to other program to perform
 * directly read, write to serial.
 * 
 * @return serial's file description 
 */
int getPortFd();

#endif /* serial.c */


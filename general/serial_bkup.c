/*
 * Program:     serial.c
 * Author:      Paul Dean
 * Version:     0.0.3
 * Date:        2002-02-19
 * Description: To provide underlying serial port function,
 *              for high level applications.
 *
*/

#include <termios.h>            /* tcgetattr, tcsetattr */
#include <stdio.h>              /* perror, printf, puts, fprintf, fputs */
#include <unistd.h>             /* read, write, close */
#include <fcntl.h>              /* open */
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>             /* bzero, memcpy */
#include <limits.h>             /* CHAR_MAX */

#include "serial.h"
//#include "global.h"
/* 
 * Decription for TIMEOUT_SEC(buflen,baud);
 * baud bits per second, buflen bytes to send.
 * buflen*20 (20 means sending an octect-bit data by use of the maxim bits 20)
 * eg. 9600bps baudrate, buflen=1024B, then TIMEOUT_SEC = 1024*20/9600+1 = 3 
 * don't change the two lines below unless you do know what you are doing.
*/

#define TIMEOUT_SEC(buflen,baud) (buflen*20/baud+2)
#define TIMEOUT_USEC 0

#define CH_TO_WAIT 5
#define CH_BITS 11

#define BUFFER_LEN  1024    /* sendfile() */

//typedef unsigned char U8;
static INT32    fd;             //File descriptor for the port
static struct termios termios_old, termios_new;
static fd_set   fs_read, fs_write;
struct timeval tv_timeout;

static void     set_baudrate (INT32);
static INT32    get_baudrate ();
static void     set_data_bit (INT32 databit);
static INT32    baudrate2Bxx (INT32 baudrate);
static INT32    Bxx2baudrate (INT32 _baudrate);
static INT32    set_port_attr (
                             INT32 baudrate, 		//
                             INT32 databit,
                             const char *stopbit, 
                             char parity);
static INT32    set_port_attr_temp (
                             INT32 baudrate, 		//
                             INT32 databit,
                             const char *stopbit, 
                             char parity);
static void     set_stopbit (const char *stopbit);
static void     set_parity (char parity);

/* Open serial port ComPort at baudrate baud rate. */
INT32 OpenComPort (INT32 ComPort, INT32 baudrate, INT32 databit,
                   const char *stopbit, char parity)
{
    char           *pComPort;
    INT32           retval;

    switch (ComPort) {
    case 0:
        pComPort = "/dev/ttyO0";
        break;
    case 1:
        pComPort = "/dev/ttyO1";
        break;
    case 2:
        pComPort = "/dev/ttyO2";
        break;
    case 3:
        pComPort = "/dev/ttyO3";
        break;
    case 4:
        pComPort = "/dev/ttyO4";
        break;
    case 5:
        pComPort = "/dev/ttyO5";
        break;
    default:
        pComPort = "/dev/ttyO0";
        break;
    }

    fd = open (pComPort, O_RDWR | O_NOCTTY | O_NDELAY);
    if (-1 == fd) {
        fprintf (stderr, "cannot open port %s\n", pComPort);
        return (-1);
    }

    printf("comport fd = %d\n", fd);

    tcgetattr (fd, &termios_old);       /* save old termios value */
    /* 0 on success, -1 on failure */
    retval = set_port_attr (baudrate, databit, stopbit, parity);
    if (-1 == retval) {
        fprintf (stderr, "\nport %s cannot set baudrate at %d\n", pComPort,
                 baudrate);
    }
    return (retval);
}

/* close serial port by use of file descriptor fd */
void CloseComPort ()
{
    /* flush output data before close and restore old attribute */
    tcsetattr (fd, TCSADRAIN, &termios_old);
    close (fd);
}

int getPortFd(){
    return fd;
}

INT32 ReadComPort (void *data, INT32 datalength)
{
    INT32           retval = 0;
    
    FD_ZERO (&fs_read);
    FD_SET (fd, &fs_read);
    tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate ());
    tv_timeout.tv_usec = TIMEOUT_USEC;
    retval = select (fd + 1, &fs_read, NULL, NULL, &tv_timeout);
//	printf("the retval is [%d]\n",retval);
    if (retval > 0)
    {
        retval = read (fd, data, 128);
//	printf("the retval2 is %d\n",retval);
//	printf("the dataall is :%c %c %c %c %c %c %c %c %c %c\n",*(U8*)data,*(U8*)(data+1),*(U8*)(data+2),*(U8*)(data+3),*(U8*)(data+4),*(U8*)(data+5),*(U8*)(data+6),*(U8*)(data+7),*(U8*)(data+8),*(U8*)(data+9));
//  	printf("the data is %c\n",*(U8*)data);
      return (retval);
    }
    else
    {
        if (0 == retval ) 
        {
            return (0);
        }else{
            return (-1);
        }
    }


}

INT32 ReadComPort_byte (U8 *data, INT32 datalength)
{
    	INT32 retval = 0,retread = 0;
	int i = 0;	
	U8 data_over = 0;    
	U8 data_temp = 0x00;
	U8* data_p = data;
	INT32 data_len = 0;	
//	printf("into the read method!\n");
    FD_ZERO (&fs_read);
    FD_SET (fd, &fs_read);
//    tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate ());
//    tv_timeout.tv_usec = TIMEOUT_USEC;
    tv_timeout.tv_sec = 0;
    tv_timeout.tv_usec = 500*1000;
	retval = select (fd + 1, &fs_read, NULL, NULL, &tv_timeout);
//	printf("after the select part!\n");
    struct timeval tv_timeout_in;
    tv_timeout_in.tv_sec = 0;
    tv_timeout_in.tv_usec = 500*1000;
	i = 0;
	if(retval > 0)
	{
		data_len = 10000;
//		retread = read(fd,&data_temp,1);
//		printf("the retread is:%d\n",retread);
		//while(read(fd,&data_temp,1) > 0)
		while(select(fd+1,&fs_read,NULL,NULL,&tv_timeout_in) > 0 && i<data_len)
		{
//			printf("while--");
			read(fd,&data_temp,1);
			*data_p = data_temp;
//			printf("get byte:0x%02x ",data_temp);
			i++;
			if(i == 3)
			{
				data_len = (((INT32)*(data_p))<<8) + (INT32)*(data_p-1);
//				printf("the data[0] = 0x%02x data[1] = 0x%02x data[2] = 0x%02x data lenght = %d\r\n",*(data_p-2),*(data_p-1),*(data_p),data_len);
			}
			data_p++;
//			data_len++;
		//	usleep(1000);//
		}
		return data_len;
		printf("receive msg's length is:%d",data_len);
	}else
	{
		return 0;
	}
//	printf("the retval is [%d]\n",retval);


}
INT32 ReadComPortA (void *data, INT32 datalength)
{
    INT32           retval = 0;
    int bytes_read;
    int readlen;
    /**
     * caculate the time of 5 characters and get the maxim
     * with 3ms and 5 ch's time
    */
    tv_timeout.tv_sec = 0;
    tv_timeout.tv_usec = ( (CH_TO_WAIT * CH_BITS) * (1000000/get_baudrate()));
    //printf("port read timeout:%dus\n",tv_timeout.tv_usec);

    bytes_read = 0;
    while(bytes_read<datalength){
        tv_timeout.tv_sec = 0;
        tv_timeout.tv_usec = ( (CH_TO_WAIT * CH_BITS) * (1000000/get_baudrate()));
        FD_ZERO (&fs_read);
        FD_SET (fd, &fs_read);
        retval = select (fd + 1, &fs_read, NULL, NULL, &tv_timeout);
        if ( retval >0 ) {
            readlen = read (fd, (data+bytes_read), datalength);
            bytes_read += readlen;
		printf("the readlen is %d",readlen);
        } else
            return (bytes_read==0?-1:bytes_read);
    }

    return -1;

}
/* 
 * Write datalength bytes in buffer given by UINT8 *data,
 * return value: bytes written
 * Nonblock mode
*/
INT32 WriteComPort (UINT8 * data, INT32 datalength)
{
    INT32           retval, len = 0, total_len = 0;

    FD_ZERO (&fs_write);
    FD_SET (fd, &fs_write);
    tv_timeout.tv_sec = TIMEOUT_SEC (datalength, get_baudrate ());
    tv_timeout.tv_usec = TIMEOUT_USEC;

    for (total_len = 0, len = 0; total_len < datalength;) {
        retval = select (fd + 1, NULL, &fs_write, NULL, &tv_timeout);
        if (retval) {
            len = write (fd, &data[total_len], datalength - total_len);
            if (len > 0) {
                total_len += len;
            }
        }
        else {
            tcflush (fd, TCOFLUSH);     /* flush all output data */
            break;
        }
    }

    return (total_len);
}

/* get serial port baudrate */
static INT32 get_baudrate ()
{
    return (Bxx2baudrate (cfgetospeed (&termios_new)));
}



/* set serial port baudrate by use of file descriptor fd */
static void set_baudrate (INT32 baudrate)
{
    termios_new.c_cflag = baudrate2Bxx (baudrate);  /* set baudrate */
}

static void set_data_bit (INT32 databit)
{
    termios_new.c_cflag &= ~CSIZE;
    switch (databit) {
    case 8:
        termios_new.c_cflag |= CS8;
        break;
    case 7:
        termios_new.c_cflag |= CS7;
        break;
    case 6:
        termios_new.c_cflag |= CS6;
        break;
    case 5:
        termios_new.c_cflag |= CS5;
        break;
    default:
        termios_new.c_cflag |= CS8;
        break;
    }
}

static void set_stopbit (const char *stopbit)
{
    if (0 == strcmp (stopbit, "1")) {
        termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */
    }
    else if (0 == strcmp (stopbit, "1.5")) {
        termios_new.c_cflag &= ~CSTOPB; /* 1.5 stop bits */
    }
    else if (0 == strcmp (stopbit, "2")) {
        termios_new.c_cflag |= CSTOPB;  /* 2 stop bits */
    }
    else {
        termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */
    }
}

static void set_parity (char parity)
{
    switch (parity) {
    case 'N':                  /* no parity check */
        termios_new.c_cflag &= ~PARENB;
        break;
    case 'E':                  /* even */
        termios_new.c_cflag |= PARENB;
        termios_new.c_cflag &= ~PARODD;
        break;
    case 'O':                  /* odd */
        termios_new.c_cflag |= PARENB;
        termios_new.c_cflag |= ~PARODD;
        break;
    default:                   /* no parity check */
        termios_new.c_cflag &= ~PARENB;
        break;
    }
}



static INT32 set_port_attr (
                          INT32 baudrate,        // 1200 2400 4800 9600 .. 115200
                          INT32 databit,           // 5, 6, 7, 8
                          const char *stopbit,  //  "1", "1.5", "2"
                          char parity)              // N(o), O(dd), E(ven)
{
    bzero(&termios_new, sizeof (termios_new));
	cfmakeraw (&termios_new);

    set_baudrate (baudrate);
    termios_new.c_cflag |= CLOCAL | CREAD;      /* | CRTSCTS */
    set_data_bit (databit);
    set_parity (parity);
    set_stopbit (stopbit);
//  termios_new.c_oflag 			= 0;
//  termios_new.c_lflag 			|= 0;
    termios_new.c_lflag 			&= ~(ICANON | ECHO |ECHOE |ISIG);
    termios_new.c_oflag 			&= ~OPOST;
//  termios_new.c_cc[VTIME] 	= 1;        /* unit: 1/10 second. */
//  termios_new.c_cc[VMIN] 		= 255; /* minimal characters for reading */
    termios_new.c_cc[VTIME] 	= 150;        /* unit: 1/10 second. */
    termios_new.c_cc[VMIN] 		= 1; /* minimal characters for reading */
    tcflush (fd, TCIFLUSH);

	return (tcsetattr (fd, TCSANOW, &termios_new));
}

static INT32 set_port_attr_temp (
                          INT32 baudrate,        // 1200 2400 4800 9600 .. 115200
                          INT32 databit,           // 5, 6, 7, 8
                          const char *stopbit,  //  "1", "1.5", "2"
                          char parity)              // N(o), O(dd), E(ven)
{
    bzero(&termios_new, sizeof (termios_new));
	cfmakeraw (&termios_new);

   // set_baudrate (baudrate);
	cfsetispeed(&termios_new,B115200);
	cfsetospeed(&termios_new,B115200);
    	tcsetattr(fd,TCSANOW,&termios_new);
	tcflush(fd,TCIOFLUSH);
	
	termios_new.c_cflag &= ~CSIZE;
	termios_new.c_cflag |= CS8;
	termios_new.c_cflag &= ~PARENB;
	termios_new.c_cflag &= ~CSTOP;
    	termios_new.c_lflag &= ~(ICANON | ECHO |ECHOE |ISIG);
    	termios_new.c_oflag = ~OPOST;
//  termios_new.c_cc[VTIME] 	= 1;        /* unit: 1/10 second. */
//  termios_new.c_cc[VMIN] 		= 255; /* minimal characters for reading */
    termios_new.c_cc[VTIME] 	= 150;        /* unit: 1/10 second. */
    termios_new.c_cc[VMIN] 		= 0; /* minimal characters for reading */
	if(tcsetattr(fd,TCSANOW,&termios_new) < 0)
	{
		printf("configure error ,please check");
		close(fd);
		return -1;
	}

    	tcflush (fd, TCIFLUSH);
	
	return 0;
}






/**
 * baudrate xxx to Bxxx
 * 
 * @@param baudrate xxx
 * 
 * @@return 
 */
static INT32 baudrate2Bxx (INT32 baudrate)
{
    switch (baudrate) {
    case 0:
        return (B0);
    case 50:
        return (B50);
    case 75:
        return (B75);
    case 110:
        return (B110);
    case 134:
        return (B134);
    case 150:
        return (B150);
    case 200:
        return (B200);
    case 300:
        return (B300);
    case 600:
        return (B600);
    case 1200:
        return (B1200);
    case 2400:
        return (B2400);
    case 9600:
        return (B9600);
    case 19200:
        return (B19200);
    case 38400:
        return (B38400);
    case 57600:
        return (B57600);
    case 115200:
        return (B115200);
    default:
        return (B9600);
    }
}

/**
 * get boundrate from Bxxx
 * 
 * @@param baudrate Bxxx refers to bound rate
 * 
 * @@return 
 */
static INT32 Bxx2baudrate (INT32 _baudrate)
{
/* reverse baudrate */
    switch (_baudrate) {
    case B0:
        return (0);
    case B50:
        return (50);
    case B75:
        return (75);
    case B110:
        return (110);
    case B134:
        return (134);
    case B150:
        return (150);
    case B200:
        return (200);
    case B300:
        return (300);
    case B600:
        return (600);
    case B1200:
        return (1200);
    case B2400:
        return (2400);
    case B9600:
        return (9600);
    case B19200:
        return (19200);
    case B38400:
        return (38400);
    case B57600:
        return (57600);
    case B115200:
        return (115200);
    default:
        return (9600);
    }
}



#ifndef __BSP_ATT7022EU_H
#define __BSP_ATT7022EU_H
#include "general/gpio.h"
#ifndef VARIABLE
#define VARIABLE
#include "general/Variable.h"
#endif
#ifndef GLOBAL
#define GLOBAL
#include "general/global.h"
#endif

#define sATT7022EU_DUMMY_BYTE  0xA5

#define ATT_RST TFT_R4//0: for reset 1:work
#define SPI_CLK TFT_G0
#define SPI_DI TFT_R6
#define SPI_DO EMU1//PIN89
#define SPI_CS TFT_G2

#define sATT7022EU_CS_LOW()       gpio_set_value_o(0,SPI_CS)	
//GPIO_ResetBits(sATT7022EU_CS_GPIO_PORT, sATT7022EU_CS_PIN)

/* Deselect sATT7022EU: Chip Select pin high */
#define sATT7022EU_CS_HIGH()      gpio_set_value_o(1,SPI_CS)
//GPIO_SetBits(sATT7022EU_CS_GPIO_PORT, sATT7022EU_CS_PIN) 

/* Select sATT7022EU: RST pin low */
#define sATT7022EU_RST_LOW()      gpio_set_value_o(0,ATT_RST)
//GPIO_ResetBits(sATT7022EU_RST_GPIO_PORT, sATT7022EU_RST_PIN)
/* Deselect sATT7022EU: RST pin high */
#define sATT7022EU_RST_HIGH()     gpio_set_value_o(1,ATT_RST)
//GPIO_SetBits(sATT7022EU_RST_GPIO_PORT, sATT7022EU_RST_PIN) 
void ATT7022EU_Init(void);
void ATT7022EU_ReadReg(U8 reg, U32 *data);
void ATT7022EU_WriteReg(U8 reg, U32 dat);
void ATT7022EU_CaliParaWrite(void);
//void* p_update_VA_ATT(void* argc);

#endif

//#include "global.h"
//define the gpio number of the function-------------
#define GPRS_ONOFF 23
#define GPRS_RST 23
//---------------------------------------------------
//#define OLED_V_1 TFT_G1
//#define OLED_V_2 TFT_G3




int gpio_request_o(unsigned int gpio_NO);

int gpio_direction_input_o(unsigned int gpio_NO);
int gpio_direction_output_o(unsigned int gpio_NO,int value);
void gpio_set_value_o(unsigned int gpio_NO,int value);
int gpio_get_value_o(unsigned int gpio_NO);
void gpio_free_o(unsigned int gpio_NO);


void wr2gpio(unsigned char value,int	gpio_NO);
unsigned char rdfromgpio(unsigned int gpio_NO);

void led_init();



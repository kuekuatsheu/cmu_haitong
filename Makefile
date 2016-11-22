objects =  test_uart.o serial.o PLC 
CC = arm-none-linux-gnueabi-gcc
DIR_G = general/
CFLAG = -lpthread -lm
#test_uart_route:test_uart.o  ; $(CC) test_uart.o -o plc $(CFLAG)
PLC_zzh:test_uart.c BSP_Oled.c BSP_R8025T.c BSP_ATT7022EU.h BSP_ATT7022EU.c log.c client.c plc.h client.h adjust.h adjust.c $(DIR_G)serial.c $(DIR_G)gpio.c $(DIR_G)Variable.h 
	$(CC)  test_uart.c BSP_Oled.c BSP_R8025T.c BSP_ATT7022EU.h BSP_ATT7022EU.c log.c BSP_R8025T.h plc.h client.c client.h adjust.h adjust.c $(DIR_G)serial.c $(DIR_G)gpio.c $(DIR_G)Variable.h -o PLC_zzh $(CFLAG)
#PLC:test_uart.c $(DIR_G)serial.c $(DIR_G)gpio.c ;$(CC)  test_uart.c $(DIR_G)serial.c $(DIR_G)gpio.c -o PLC $(CFLAG)
#serial.o :serial.c serial.h;$(CC) -c serial.c -o serial.o
#gpio.o :gpio.c gpio.h;$(CC) -c gpio.c -o gpio.o
SPI:spi_gpio.c BSP_Oled.c BSP_ATT7022EU.h BSP_ATT7022EU.c log.c  plc.h client.h  $(DIR_G)gpio.c $(DIR_G)Variable.h 
	$(CC) spi_gpio.c BSP_Oled.c BSP_ATT7022EU.c $(DIR_G)gpio.c $(DIR_G)Variable.h -o spi_gpio_test $(CFLAG) 

client:client.c log.c
	$(CC) client.c log.c -o client $(CFLAG)
test: BSP_Oled.c BSP_R8025T.c $(DIR_G)gpio.c $(DIR_G)serial.c 
	$(CC) BSP_Oled.c BSP_R8025T.c $(DIR_G)gpio.c $(DIR_G)serial.c -o adjust $(CFLAG)
clean: ; rm PLC_zzh

move:
	cp PLC_zzh /mnt/hgfs/ubuntu_share/cmu/.
move_test:
	cp adjust /mnt/hgfs/ubuntu_share/cmu/.

#ifndef _DEFINE_
#define _DEFINE_

#include <SoftwareSerial.h>
#include <SPIMemory.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

//Arduino peri define
#define BAUD_RATE 9600 //for serial.print
#define FLASH_CS 4 //define cs pin of flash
#define TFT_DC 9 //TFT DC pin
#define TFT_CS 10 //TFT CS pin

//flash addr part
#define fig_width 320
#define fig_high 240
//#define fig_byte_number fig_high*fig_width*2 //r5g6b5, each pixel occupy 2 byte, 153600 bytes for 240*320*2
#define fig_byte_number 153600
#define fig_number 3 //total fig numbers in flash

#define fig_size 0x30000
#define fig0_start_addr 0x30000*0 //0x0 ~ 0x‭25800‬ for 240*320*2
#define fig1_start_addr 0x30000*1
#define fig2_start_addr 0x30000*2
#define fig3_start_addr 0x30000*3


//SoftwareSerial softSerial(3,2);

#endif
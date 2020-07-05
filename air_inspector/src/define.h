#ifndef _DEFINE_
#define _DEFINE_

#include <SoftwareSerial.h>
//#include "TW07ST.h"
#include <FlexiTimer2.h>
#include "SENSOR_SERIAL.h"
#include "display_flash.h"
#include "Adafruit_SHT31.h"
#include "sensor_group.h"
#include "MHZ19_SW_UART.h"
#include <PMS7003.h>
#include <ZE08_CH2O.h>

//Arduino peri define
#define BAUD_RATE 9600 //for serial.print

//fig fresh rate
#define time_each_fig_ms 10000 //30*60*1000ms

//timer1 TFT_LED PWM config
#define MCU_xtal_freq 16000000
#define LED_PWA_freq 10000 //10k PWM
uint8_t LED_birghtness[6] = {33, 35, 37, 39, 41, 43};

#endif
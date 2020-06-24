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

//Arduino peri define
#define BAUD_RATE 9600 //for serial.print

//fig fresh rate
#define time_each_fig_ms 10000 //30*60*1000ms

#endif
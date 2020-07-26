#ifndef _DEFINE_
#define _DEFINE_

#include <SoftwareSerial.h>
//#include "TW07ST.h"
//#include <FlexiTimer2.h>
#include "SENSOR_SERIAL.h"
#include "display_flash.h"
#include "Adafruit_SHT31.h"
#include "sensor_group.h"
#include "MHZ19_SW_UART.h"
#include <PMS7003.h>
#include <ZE08_CH2O.h>

//Arduino peri define
#define BAUD_RATE 9600 //for serial.print
#define AIR_INSPECTOR_UART_DEBUG
/* Setup debug printing macros. */
#define AIR_INSPECTOR_UART_DEBUG_PRINTER Serial
#ifdef AIR_INSPECTOR_UART_DEBUG
  #define AIR_INSPECTOR_UART_DEBUG_PRINT(...) { AIR_INSPECTOR_UART_DEBUG_PRINTER.print(__VA_ARGS__); }
  #define AIR_INSPECTOR_UART_DEBUG_PRINTLN(...) { AIR_INSPECTOR_UART_DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define AIR_INSPECTOR_UART_DEBUG_PRINT(...) {} /**< Debug Print Placeholder if Debug is disabled */
  #define AIR_INSPECTOR_UART_DEBUG_PRINTLN(...) {} /**< Debug Print Line Placeholder if Debug is disabled */
#endif

//fig fresh rate
#define time_each_fig_ms 180000 //30*60*1000ms

//timer1 TFT_LED PWM config
#define MCU_xtal_freq 16000000
#define LED_PWA_freq 10000 //10k PWM
uint8_t LED_birghtness[7] = {10, 20, 30, 40, 50, 70, 99}; //100 level
//uint8_t LED_birghtness[7] = {40, 80, 120, 160, 200, 230, 255}; //255 level
int LED_light_in_threshold[6] = {200, 230, 300, 400, 500, 600};

#endif
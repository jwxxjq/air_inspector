#ifndef _DEFINE_
#define _DEFINE_

#include <SoftwareSerial.h>
//#include "TW07ST.h"
//#include <FlexiTimer2.h>
#include <SENSOR_SERIAL.h>
#include <display_flash.h>
#include <Adafruit_SHT31.h>
#include <sensor_group.h>
#include <MHZ19_SW_UART.h>
#include <PMS7003.h>
#include <ZE08_CH2O.h>

//Arduino peri define
#define BAUD_RATE 9600 //for serial.print

//#define disable_LED_PWM //uncomment here to disable LED_TFT PWM auto brightness adjust feature
#define use_HW_LED_PWM //HW PWM 500Hz

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

//sensor value update rate
#define sensor_read_delay_ms 10000 //10*1000ms

//timer1 TFT_LED PWM config
#define MCU_xtal_freq 16000000
#define LED_PWA_freq 1000 //1k PWM
#ifndef use_HW_LED_PWM
  const uint8_t LED_birghtness[7] = {10, 20, 30, 40, 50, 70, 99}; //100 level
#else
  const uint8_t LED_birghtness[7] = {40, 80, 130, 160, 190, 230, 255}; //255 level for HW PWM
#endif
const uint8_t LED_light_in_threshold[6] = {40, 60, 75, 100, 125, 150}; //*4 to convert to 0~1023

#endif
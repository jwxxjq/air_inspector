#ifndef _PMS7003_H_
#define _PMS7003_H_

#include <PMS.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#define PMS7003_UART_RATE 9600
#define PMS7003_UART_default_TX_pin 15 // A1
#define PMS7003_UART_default_RX_pin 14 //A0
#define PMS7003_UART_init_time_mills 30000 //30s heating

//#define PMS7003_UART_DEBUG
/* Setup debug printing macros. */
#define PMS7003_UART_DEBUG_PRINTER Serial
#ifdef PMS7003_UART_DEBUG
  #define PMS7003_UART_DEBUG_PRINT(...) { PMS7003_UART_DEBUG_PRINTER.print(__VA_ARGS__); }
  #define PMS7003_UART_DEBUG_PRINTLN(...) { PMS7003_UART_DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define PMS7003_UART_DEBUG_PRINT(...) {} /**< Debug Print Placeholder if Debug is disabled */
  #define PMS7003_UART_DEBUG_PRINTLN(...) {} /**< Debug Print Line Placeholder if Debug is disabled */
#endif

class PMS7003_UART
{
public:
  PMS7003_UART();
  PMS7003_UART(int8_t RX_pin_in, int8_t TX_pin_in);
  void init();
  bool update_dust_value();
  void sleep();
  void wakeup();
  uint16_t PM25_value;
  uint16_t PM10_value;

private:
  uint8_t TX_pin;
  uint8_t RX_pin;
  bool heating;
  unsigned long lastInitTimestamp;

};

#endif
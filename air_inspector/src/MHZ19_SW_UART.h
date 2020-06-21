#ifndef MHZ19_SW_UART_H_
#define MHZ19_SW_UART_H_

#include <Arduino.h>
#include <MHZ19.h>
#include <SoftwareSerial.h>

#define MHZ19_SW_UART_RATE 9600
#define MHZ19_SW_UART_default_TX_pin 7
#define MHZ19_SW_UART_default_RX_pin 8
#define MHZ19_init_time_mills 180000 //180s

#define MHZ19_SW_UART_DEBUG
/* Setup debug printing macros. */
#define DEBUG_PRINTER Serial
#ifdef MHZ19_SW_UART_DEBUG
  #define MHZ19_SW_UART_DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define MHZ19_SW_UART_DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define MHZ19_SW_UART_DEBUG_PRINT(...) {} /**< Debug Print Placeholder if Debug is disabled */
  #define MHZ19_SW_UART_DEBUG_PRINTLN(...) {} /**< Debug Print Line Placeholder if Debug is disabled */
#endif

class MHZ19_SW_UART
{
public:
  MHZ19_SW_UART();
  MHZ19_SW_UART(int8_t RX_pin_in, int8_t TX_pin_in);
  void init();
  uint32_t read_CO2_value();

private:
  int8_t RX_pin;
  int8_t TX_pin;
  bool heating;
  unsigned long lastInitTimestamp;
};

#endif
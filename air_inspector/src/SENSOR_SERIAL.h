#ifndef SENSOR_SERIAL_H
#define SENSOR_SERIAL_H

#include "Arduino.h"
#include "SoftwareSerial.h"

#define SENSOR_SERIAL_band_rate 9600 //must be 9600

#define SENSOR_SERIAL_DEBUG_PRINTER Serial
#define SENSOR_SERIAL_default_TX_pin 3
#define SENSOR_SERIAL_default_RX_pin 4

#define SENSOR_SERIAL_SIG1 0xa5
#define SENSOR_SERIAL_SIG2 0x5a

//#define SENSOR_SERIAL_DEBUG
/* Setup debug printing macros. */
#ifdef SENSOR_SERIAL_DEBUG
  #define SENSOR_SERIAL_DEBUG_PRINT(...) { SENSOR_SERIAL_DEBUG_PRINTER.print(__VA_ARGS__); }
  #define SENSOR_SERIAL_DEBUG_PRINTLN(...) { SENSOR_SERIAL_DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define SENSOR_SERIAL_DEBUG_PRINT(...) {} /**< Debug Print Placeholder if Debug is disabled */
  #define SENSOR_SERIAL_DEBUG_PRINTLN(...) {} /**< Debug Print Line Placeholder if Debug is disabled */
#endif

class SERSOR_SERIAL {
  public:
    SERSOR_SERIAL();
    SERSOR_SERIAL(int8_t RX_pin_in, int8_t TX_pin_in);
    //void init(int8_t TX_pin, int8_t RX_pin);
    void print_value(uint16_t PM2_5_value, uint16_t PM10_value, uint16_t CO2_value, uint16_t HCHO_value, uint16_t Temperature_value, uint16_t Humidity_value);
  
  private:
    int8_t TX_pin;
    int8_t RX_pin;
    //void SoftwareSerial_open();
    //void SoftwareSerial_close();
};

#endif
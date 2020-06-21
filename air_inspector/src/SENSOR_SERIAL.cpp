#include "SENSOR_SERIAL.h"

//public
SERSOR_SERIAL::SERSOR_SERIAL(){
  TX_pin = SENSOR_SERIAL_default_TX_pin;
  RX_pin = SENSOR_SERIAL_default_RX_pin;
  SENSOR_SERIAL_DEBUG_PRINTLN("using default TX/RX pin for Sensor print");
}

SERSOR_SERIAL::SERSOR_SERIAL(int8_t RX_pin_in, int8_t TX_pin_in){
  TX_pin = TX_pin_in;
  RX_pin = RX_pin_in;
  SENSOR_SERIAL_DEBUG_PRINTLN("using TX_pin = ");
  SENSOR_SERIAL_DEBUG_PRINTLN(TX_pin);
  SENSOR_SERIAL_DEBUG_PRINTLN("using RX_pin = ");
  SENSOR_SERIAL_DEBUG_PRINTLN(RX_pin);
}

void SERSOR_SERIAL::print_value(uint16_t PM2_5_value, uint16_t PM10_value, uint16_t CO2_value, uint16_t HCHO_value, uint16_t Temperature_value, uint16_t Humidity_value){
  SoftwareSerial SENSOR_SERIAL_serial(RX_pin, TX_pin);
  //Serial.begin(9600);
  uint8_t Temperature_value_high_byte = Temperature_value/10;
  uint8_t Temperature_value_low_byte = Temperature_value%10*10;
  uint8_t Humidity_value_high_byte = Humidity_value/10;
  uint8_t Humidity_value_low_byte = Humidity_value%10*10;

  uint16_t sum = SENSOR_SERIAL_SIG1 + SENSOR_SERIAL_SIG2;
  sum += PM2_5_value/256 + PM2_5_value%256;
  sum += PM10_value/256 + PM10_value%256;
  sum += CO2_value/256 + CO2_value%256;
  sum += HCHO_value/256 + HCHO_value%256;
  sum += Temperature_value_high_byte + Temperature_value_low_byte;
  sum += Humidity_value_high_byte + Humidity_value_low_byte;

  SENSOR_SERIAL_serial.begin(SENSOR_SERIAL_band_rate);
  SENSOR_SERIAL_serial.write(SENSOR_SERIAL_SIG1);
  SENSOR_SERIAL_serial.write(SENSOR_SERIAL_SIG2);
  SENSOR_SERIAL_serial.write(PM2_5_value>>8);
  SENSOR_SERIAL_serial.write(PM2_5_value);
  SENSOR_SERIAL_serial.write(PM10_value>>8);
  SENSOR_SERIAL_serial.write(PM10_value);
  SENSOR_SERIAL_serial.write(CO2_value>>8);
  SENSOR_SERIAL_serial.write(CO2_value);
  SENSOR_SERIAL_serial.write(HCHO_value>>8);
  SENSOR_SERIAL_serial.write(HCHO_value);
  SENSOR_SERIAL_serial.write(Temperature_value_high_byte);
  SENSOR_SERIAL_serial.write(Temperature_value_low_byte);
  SENSOR_SERIAL_serial.write(Humidity_value_high_byte);
  SENSOR_SERIAL_serial.write(Humidity_value_low_byte);
  SENSOR_SERIAL_serial.write(sum>>8);
  SENSOR_SERIAL_serial.write(sum%256);
  
  SENSOR_SERIAL_serial.end();
}
#include "MHZ19_SW_UART.h"

//public
MHZ19_SW_UART::MHZ19_SW_UART(){
  TX_pin = MHZ19_SW_UART_default_TX_pin;
  RX_pin = MHZ19_SW_UART_default_RX_pin;
  MHZ19_SW_UART_DEBUG_PRINTLN("using default TX/RX pin for mhz19");
  lastInitTimestamp = 0;
  heating = true;
}

//calling with rx/tx pin config
MHZ19_SW_UART::MHZ19_SW_UART(int8_t RX_pin_in, int8_t TX_pin_in){
  TX_pin = TX_pin_in;
  RX_pin = RX_pin_in;
  MHZ19_SW_UART_DEBUG_PRINTLN("using TX_pin = ");
  MHZ19_SW_UART_DEBUG_PRINTLN(TX_pin);
  MHZ19_SW_UART_DEBUG_PRINTLN("using RX_pin = ");
  MHZ19_SW_UART_DEBUG_PRINTLN(RX_pin);
  lastInitTimestamp = 0;
  heating = true;
}

//init mhz19 & ss
void MHZ19_SW_UART::init(){
  SoftwareSerial ss(RX_pin, TX_pin);
  ss.begin(MHZ19_SW_UART_RATE);
  MHZ19 mhz19(&ss);
  mhz19.setRange(MHZ19_RANGE_2000);
  //mhz19.setAutoCalibration(true);
  MHZ19_SW_UART_DEBUG_PRINTLN("set MHZ19 range to 2000 and auto calibration, init done");
  ss.end();
  lastInitTimestamp = millis();
}

//read CO2 value
uint32_t MHZ19_SW_UART::read_CO2_value(){
  SoftwareSerial ss(RX_pin, TX_pin);
  ss.begin(MHZ19_SW_UART_RATE);
  MHZ19 mhz19(&ss);
  if (heating){
    heating = (millis() - lastInitTimestamp) < MHZ19_init_time_mills; //heat done after init time
    MHZ19_SW_UART_DEBUG_PRINTLN("MHZ19 heating...")
    return 0x0;
  }

  MHZ19_RESULT response = mhz19.retrieveData();

  if (response == MHZ19_RESULT_OK){
    uint32_t CO2_value = (uint32_t) mhz19.getCO2();
    MHZ19_SW_UART_DEBUG_PRINT("read CO2 value = ");
    MHZ19_SW_UART_DEBUG_PRINTLN(CO2_value);
    MHZ19_SW_UART_DEBUG_PRINT(F("Min CO2: "));
    MHZ19_SW_UART_DEBUG_PRINTLN(mhz19.getMinCO2());
    MHZ19_SW_UART_DEBUG_PRINT(F("Temperature: "));
    MHZ19_SW_UART_DEBUG_PRINTLN(mhz19.getTemperature());
    MHZ19_SW_UART_DEBUG_PRINT(F("Accuracy: "));
    MHZ19_SW_UART_DEBUG_PRINTLN(mhz19.getAccuracy());
    return CO2_value;
  }
  else {
    MHZ19_SW_UART_DEBUG_PRINT("Error, code: ");
    MHZ19_SW_UART_DEBUG_PRINTLN(response);
    return 0xffffffff;
  }
}
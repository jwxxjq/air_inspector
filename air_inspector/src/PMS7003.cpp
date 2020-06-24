#include <PMS7003.h>

//public
PMS7003_UART::PMS7003_UART(){
  TX_pin = PMS7003_UART_default_TX_pin;
  RX_pin = PMS7003_UART_default_RX_pin;
  PMS7003_UART_DEBUG_PRINTLN("using default TX/RX pin for PMS7003");
  lastInitTimestamp = 0;
  heating = true;
}

PMS7003_UART::PMS7003_UART(int8_t RX_pin_in, int8_t TX_pin_in){
  TX_pin = TX_pin_in;
  RX_pin = RX_pin_in;
  PMS7003_UART_DEBUG_PRINTLN("using TX_pin = ");
  PMS7003_UART_DEBUG_PRINTLN(TX_pin);
  PMS7003_UART_DEBUG_PRINTLN("using RX_pin = ");
  PMS7003_UART_DEBUG_PRINTLN(RX_pin);
  lastInitTimestamp = 0; //default is wakeup state
  heating = true;
}

//init task, record timestamp
void PMS7003_UART::init(){
  lastInitTimestamp = millis();
  PM25_value = 0;
  PM10_value = 0;
  PMS7003_UART_DEBUG_PRINTLN("PMS7003 init done");
}

//config PMS7003 enter sleep mode
void PMS7003_UART::sleep(){
  SoftwareSerial ss(RX_pin, TX_pin);
  ss.begin(PMS7003_UART_RATE);
  PMS pms(ss);
  
  pms.sleep();
  ss.end();
}

//config PMS7003 wakeup
void PMS7003_UART::wakeup(){
  SoftwareSerial ss(RX_pin, TX_pin);
  ss.begin(PMS7003_UART_RATE);
  PMS pms(ss);

  pms.wakeUp();
  ss.end();
  lastInitTimestamp = millis();
}

//update dust value
bool PMS7003_UART::update_dust_value(){
  bool result = false;

  //check power-on time first
  if (heating){
    heating = (millis() - lastInitTimestamp) < PMS7003_UART_init_time_mills; //heat done after init time
    PMS7003_UART_DEBUG_PRINTLN("PMS7003 heating...");
    PM25_value = 0;
    PM10_value = 0;
    return false;
  }

  SoftwareSerial ss(RX_pin, TX_pin);
  PMS pms(ss);
  PMS::DATA data;

  ss.begin(PMS7003_UART_RATE);
  pms.requestRead();

  if (pms.readUntil(data, 2000U)){ //read time out set to 2s
    PM25_value = data.PM_AE_UG_2_5;
    PM10_value = data.PM_AE_UG_10_0;
    PMS7003_UART_DEBUG_PRINT("PM 2.5: ");
    PMS7003_UART_DEBUG_PRINTLN(PM25_value);
    PMS7003_UART_DEBUG_PRINT("PM 10: ");
    PMS7003_UART_DEBUG_PRINTLN(PM10_value);
    result = true;
  }
  else{
    PMS7003_UART_DEBUG_PRINTLN("PMS read fail.");
    result = false;
  }

  return result;
}
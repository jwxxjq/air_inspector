#include <ZE08_CH2O.h>

//public
ZE08_CH2O::ZE08_CH2O(){
  TX_pin = ZE08_CH2O_SW_UART_default_TX_pin;
  RX_pin = ZE08_CH2O_SW_UART_default_RX_pin;
  ZE08_CH2O_SW_UART_DEBUG_PRINTLN("using default TX/RX pin for mhz19");
  lastInitTimestamp = 0;
  heating = true;
}

ZE08_CH2O::ZE08_CH2O(int8_t RX_pin_in, int8_t TX_pin_in){
  TX_pin = TX_pin_in;
  RX_pin = RX_pin_in;
  ZE08_CH2O_SW_UART_DEBUG_PRINTLN("using TX_pin = ");
  ZE08_CH2O_SW_UART_DEBUG_PRINTLN(TX_pin);
  ZE08_CH2O_SW_UART_DEBUG_PRINTLN("using RX_pin = ");
  ZE08_CH2O_SW_UART_DEBUG_PRINTLN(RX_pin);
  lastInitTimestamp = 0;
  heating = true;
}

//init ze08
void ZE08_CH2O::init(){
  SoftwareSerial ss(RX_pin, TX_pin);
  ss.begin(ZE08_CH2O_SW_UART_RATE);
  uint8_t init_cmd[9] = {0xff, 0x01, 0x78, 0x41, 0x0, 0x0, 0x0, 0x0, 0x46}; //set mode
  uart_write(&ss, init_cmd, 9);
  ss.end();
  lastInitTimestamp = millis();
//  ZE08_CH2O_SW_UART_DEBUG_PRINTLN(lastInitTimestamp);
}

uint32_t ZE08_CH2O::read_CH2O_value(){
  uint8_t init_cmd[9] = {0xff, 0x01, 0x86, 0x0, 0x0, 0x0, 0x0, 0x0, 0x79}; //read value
  uint8_t receive_cmd[9];
  ZE08_CH2O_RESULT sensor_flag = ZE08_CH2O_RESULT_OK;
  uint16_t CH2O_value = 0;

  if (is_heating()){
    return 0x0;
  }

  SoftwareSerial ss(RX_pin, TX_pin);
  ss.begin(ZE08_CH2O_SW_UART_RATE);

  uart_write(&ss, init_cmd, 9);

  unsigned long time = millis();
  while (ss.available() <= 0)
  {
    if (millis() - time >= ZE08_CH2O_SERIAL_TIMEOUT) //time out check
    {  
      sensor_flag = ZE08_CH2O_RESULT_ERR_TIMEOUT;
      break;
    }
  }

  memset(receive_cmd, 0, 9);
  ss.readBytes(receive_cmd, 9);
  ss.end();

  byte crc = calcCRC(receive_cmd); //crc
  sensor_flag = ZE08_CH2O_RESULT_OK;
  if (receive_cmd[0] != 0xFF)
  	sensor_flag = ZE08_CH2O_RESULT_ERR_FB;
  if (receive_cmd[8] != crc)
  	sensor_flag = ZE08_CH2O_RESULT_ERR_CRC;

  if (sensor_flag == ZE08_CH2O_RESULT_OK){ //calculate CH2O_value
    CH2O_value = receive_cmd[2]*256 + receive_cmd[3];
    ZE08_CH2O_SW_UART_DEBUG_PRINT("read CH2O value = ");
    ZE08_CH2O_SW_UART_DEBUG_PRINTLN(CH2O_value);
  }
  else if (sensor_flag == ZE08_CH2O_SERIAL_TIMEOUT) {
    ZE08_CH2O_SW_UART_DEBUG_PRINTLN("CH2O sensor timeout");
  }
  else if (sensor_flag == ZE08_CH2O_RESULT_ERR_CRC) {
    ZE08_CH2O_SW_UART_DEBUG_PRINTLN("CH2O sensor read CRC error");
  }
  else if (sensor_flag == ZE08_CH2O_RESULT_ERR_FB) {
    ZE08_CH2O_SW_UART_DEBUG_PRINTLN("CH2O read first byte error");
  }

  return CH2O_value;
}

//private
void ZE08_CH2O::uart_write(Stream* _serial, byte data[], byte len)
{
	while (_serial->available() > 0) { _serial->read(); }
	_serial->write(data, len);
	_serial->flush();
}

byte ZE08_CH2O::calcCRC(byte data[])
{
	byte i;
	byte crc = 0;
	for (i = 1; i < 8; i++)
	{
		crc += data[i];
	}
	crc = 255 - crc;
	crc++;

	return crc;
}

bool ZE08_CH2O::is_heating(){
  unsigned long current_time = millis();
  ZE08_CH2O_SW_UART_DEBUG_PRINT("ZE08 init time = ");
  ZE08_CH2O_SW_UART_DEBUG_PRINTLN(lastInitTimestamp);
  ZE08_CH2O_SW_UART_DEBUG_PRINT("current time = ");
  ZE08_CH2O_SW_UART_DEBUG_PRINTLN(current_time);

  //heating check
  if (heating){
    heating = (current_time - lastInitTimestamp) < ZE08_CH2O_init_time_mills; //heat done after init time
    ZE08_CH2O_SW_UART_DEBUG_PRINTLN("ZE08 heating...")
  }
  ZE08_CH2O_SW_UART_DEBUG_PRINTLN(heating);
//  if (heating == false){
//    while(1);
//  }
  return heating;
}

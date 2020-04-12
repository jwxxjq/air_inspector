#include "TW07ST.h"

//public
TW07ST::TW07ST(){
}

void TW07ST::init(){
//  if (Serial()){
//    Serial.println("warning: should not open Serial before TW07ST init");
//    Serial.println("reopening Serial");
//    
//    Serial.end();
//  }

  Serial.begin(TW07ST_BAUD_RATE);
  
  memset(CO2_value_list, 0, TW07ST_buffer_size*2);
  memset(TVOC_value_list, 0, TW07ST_buffer_size*2);
  memset(CH2O_value_list, 0, TW07ST_buffer_size*2);
  memset(PM25_list, 0, TW07ST_buffer_size*2);
  memset(PM10_list, 0, TW07ST_buffer_size*2);
  memset(TEMP_list, 0, TW07ST_buffer_size*2);
  memset(RH_list, 0, TW07ST_buffer_size*2);
  
  CO2_value = 0;
  TVOC_value = 0;
  CH2O_value = 0;
  PM25_value = 0;
  PM10_value = 0;
  TEMP_value = 0;
  RH_value = 0;
}

/*!
 *  @brief  try to receive data from Serial
 *  @param  no
 *
 *	@return false: update fail, no data update; true: data update successfully
 */
bool TW07ST::update_value(){
  int data_length = Serial.available();
  bool return_value = false;
  if (data_length > 33){
    DEBUG_PRINT("rx buffer number:");
    DEBUG_PRINTLN(data_length);
    
    Serial.readStringUntil((char)0x3c);
    if (Serial.read() == 0x02){
      DEBUG_PRINTLN("get header");
      //update data here
      char data[15];
      Serial.readBytes(data, 15); //read 15 bytes data from buffer
      DEBUG_PRINTLN("read 15 bytes from buffer");
      return_value = data_check(data);
    }
    else {
      return return_value; //header get wrong, update fail
    }
  }
  
  return return_value; //data length no enough, update fail
}


//private function
/*!
 *  @brief  check received data format & error byte
 *  @param  input data pointer
 *
 *	@return true:  check pass & update average value done; 
 *          false: check fail, average data not updated
 */
bool TW07ST::data_check(char* data){
  uint16_t this_CO2_value = ((byte)*(data))   * 256 + (byte)*(data+1);//CO2
  uint16_t this_CH2O_value = ((byte)*(data+2)) * 256 + (byte)*(data+3);//CH2O
  uint16_t this_TVOC_value = ((byte)*(data+4)) * 256 + (byte)*(data+5);//TVOC
  if (this_CO2_value == 0 && this_CH2O_value == 0 && this_TVOC_value == 0){
    DEBUG_PRINTLN("sensor heating...");
    return false;
  }
  uint16_t this_PM25_value = ((byte)*(data+6)) * 256 + (byte)*(data+7);//PM2.5
  uint16_t this_PM10_value = ((byte)*(data+8)) * 256 + (byte)*(data+9);//PM10
  uint16_t this_TEMP_value = ((byte)*(data+10))* 10  + (byte)*(data+11);//TEMP*10
  uint16_t this_RH_value = ((byte)*(data+12))* 10  + (byte)*(data+13);//Humidity
  
  uint8_t error_check_value = 62; //0x3c + 0x02
  for (uint8_t byte_count = 0; byte_count < 14; byte_count++){
    error_check_value += (uint8_t)*(data+byte_count);
  }
  if (error_check_value != (uint8_t)*(data+14)) {
    DEBUG_PRINTLN("error check fail:");
    for (uint8_t byte_count = 0; byte_count < 14; byte_count++){
      DEBUG_PRINT((uint8_t)*(data+byte_count));
      DEBUG_PRINT(" ");
    }
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN(error_check_value);
    DEBUG_PRINTLN((uint8_t)*(data+14));
  }
  else {
    DEBUG_PRINTLN("value_check pass");
    for (uint8_t data_count = TW07ST_buffer_size-1; data_count > 0; data_count--){ //shift the data list
      CO2_value_list[data_count] = CO2_value_list[data_count-1];
      TVOC_value_list[data_count] = TVOC_value_list[data_count-1];
      CH2O_value_list[data_count] = CH2O_value_list[data_count-1];
      PM25_list[data_count] = PM25_list[data_count-1];
      PM10_list[data_count] = PM10_list[data_count-1];
      TEMP_list[data_count] = TEMP_list[data_count-1];
      RH_list[data_count] = RH_list[data_count-1];
    }
    CO2_value_list[0] = this_CO2_value;
    TVOC_value_list[0] = this_TVOC_value;
    CH2O_value_list[0] = this_CH2O_value;
    PM25_list[0] = this_PM25_value;
    PM10_list[0] = this_PM10_value;
    TEMP_list[0] = this_TEMP_value;
    RH_list[0] = this_RH_value;
    
    calculate_average_value();
    return true;
  }
  
  return false;
}

void TW07ST::calculate_average_value(){
    if (CO2_value_list[TW07ST_buffer_size-1] == 0 && TVOC_value_list[TW07ST_buffer_size-1] == 0 && CH2O_value_list[TW07ST_buffer_size-1] == 0){
      DEBUG_PRINTLN("buffer not full... filling with new data");
    }
    else{
      CO2_value = get_average_data(CO2_value_list);
      TVOC_value = get_average_data(TVOC_value_list);
      CH2O_value = get_average_data(CH2O_value_list);
      PM25_value = get_average_data(PM25_list);
      PM10_value = get_average_data(PM10_list);
      TEMP_value = get_average_data(TEMP_list);
      RH_value = get_average_data(RH_list);
      
      DEBUG_PRINT("CO2: ");
      DEBUG_PRINTLN(CO2_value);
      DEBUG_PRINT("TVOC: ");
      DEBUG_PRINTLN(TVOC_value);
      DEBUG_PRINT("CH2O: ");
      DEBUG_PRINTLN(CH2O_value);
      DEBUG_PRINT("PM25: ");
      DEBUG_PRINTLN(PM25_value);
      DEBUG_PRINT("PM10: ");
      DEBUG_PRINTLN(PM10_value);
      DEBUG_PRINT("TEMP: ");
      DEBUG_PRINTLN(TEMP_value);
      DEBUG_PRINT("RH: ");
      DEBUG_PRINTLN(RH_value);
    }
}


/*!
 *  @brief  caculate average data by input 7 data
 *  @param  inputdata input_data list, length is 7
 *
 *	@return average value
 */
uint16_t TW07ST::get_average_data(uint16_t* input_data){
  DEBUG_PRINTLN("in average function:");
	uint16_t result = 0;
  uint16_t input_max = (uint16_t)*input_data;
  uint16_t input_min = (uint16_t)*input_data;
  for(uint8_t data_count = 0; data_count < TW07ST_buffer_size; data_count++) { //for each 16-bit data in buffer
    result += (uint16_t)*(input_data+data_count);
    if (input_max < (uint16_t)*(input_data+data_count)) {input_max = (uint16_t)*(input_data+data_count);}
    if (input_min > (uint16_t)*(input_data+data_count)) {input_min = (uint16_t)*(input_data+data_count);}
    DEBUG_PRINT((uint16_t)*(input_data+data_count));
    DEBUG_PRINT("  ");
  }
  result = result - input_max - input_min;
  result = result/5;
  DEBUG_PRINTLN("average function end");
  return result;
}

/*!
 *  @brief  change Serial input to uint16_t result
 *  @param  inputdata input_data list, length is 7
 *
 *	@return average value
 */
//void data_change(char* data, int* result){
//  result[0] = ((byte)*(data))   * 256 + (byte)*(data+1);//CO2
//  result[1] = ((byte)*(data+2)) * 256 + (byte)*(data+3);//CH2O
//  result[2] = ((byte)*(data+4)) * 256 + (byte)*(data+5);//TVOC
//  result[3] = ((byte)*(data+6)) * 256 + (byte)*(data+7);//PM2.5
//  result[4] = ((byte)*(data+8)) * 256 + (byte)*(data+9);//PM10
//  result[5] = ((byte)*(data+10))* 10  + (byte)*(data+11);//TEMP*10
//  result[6] = ((byte)*(data+12))* 10  + (byte)*(data+13);//Humidity
////  Serial.println((unsigned int)(*data));
////  Serial.println((unsigned int)(*(data+1)));
////  Serial.println(result[0]);
////  Serial.println(result[1]); 
////  Serial.println(result[2]);
////  Serial.println(result[3]); 
////  while(1);
//}
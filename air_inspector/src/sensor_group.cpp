#include "sensor_group.h"

//public
sensor_group::sensor_group(){
}

void sensor_group::init(){
  //init value
  memset(CO2_value_list, 0, sensor_buffer_length*2);
  memset(CH2O_value_list, 0, sensor_buffer_length*2);
  memset(PM25_value_list, 0, sensor_buffer_length*2);
  memset(PM10_value_list, 0, sensor_buffer_length*2);
  memset(TEMP_value_list, 0, sensor_buffer_length*2);
  memset(RH_value_list, 0, sensor_buffer_length*2);
  
  CO2_value = 0;
  CH2O_value = 0;
  PM25_value = 0;
  PM10_value = 0;
  TEMP_value = 0;
  RH_value = 0;
}

/*!
 *  @brief  put new temp & rh value into list
 *  @param  new_temp_value: new temp value to put into list
 *  @param  new_rh_value: new hum value to put into list
 *
 *	@return null
 */
void sensor_group::put_temp_rh_value(uint16_t new_temp_value, uint16_t new_rh_value){
  for(uint8_t i = sensor_buffer_length - 1; i > 0; i--){
    TEMP_value_list[i] = TEMP_value_list[i-1];
    RH_value_list[i] = RH_value_list[i-1];
  }
  TEMP_value_list[0] = new_temp_value;
  RH_value_list[0] = new_rh_value;
}

void sensor_group::put_CO2_value(uint16_t new_CO2_value){
  for(uint8_t i = sensor_buffer_length - 1; i > 0; i--){
    CO2_value_list[i] = CO2_value_list[i-1];
  }
  CO2_value_list[0] = new_CO2_value;
}

void sensor_group::put_CH2O_value(uint16_t new_CH2O_value){}
void sensor_group::put_dust_value(uint16_t new_PM25_value, uint16_t new_PM10_value){}

/*!
 *  @brief  recalculate average values of all sensors'
 *  @param  null
 *
 *	@return null
 */
void sensor_group::calculate_average_value(){
  CO2_value = get_average_data(CO2_value_list);
  CH2O_value = get_average_data(CH2O_value_list);
  PM25_value = get_average_data(PM25_value_list);
  PM10_value = get_average_data(PM10_value_list);
  TEMP_value = get_average_data(TEMP_value_list);
  RH_value = get_average_data(RH_value_list);
  
  SENSOR_GROUP_DEBUG_PRINT("CO2: ");
  SENSOR_GROUP_DEBUG_PRINTLN(CO2_value);
  SENSOR_GROUP_DEBUG_PRINT("CH2O: ");
  SENSOR_GROUP_DEBUG_PRINTLN(CH2O_value);
  SENSOR_GROUP_DEBUG_PRINT("PM25: ");
  SENSOR_GROUP_DEBUG_PRINTLN(PM25_value);
  SENSOR_GROUP_DEBUG_PRINT("PM10: ");
  SENSOR_GROUP_DEBUG_PRINTLN(PM10_value);
  SENSOR_GROUP_DEBUG_PRINT("TEMP: ");
  SENSOR_GROUP_DEBUG_PRINTLN(TEMP_value);
  SENSOR_GROUP_DEBUG_PRINT("RH: ");
  SENSOR_GROUP_DEBUG_PRINTLN(RH_value);
}


//private
/*!
 *  @brief  caculate average data by input 7 data
 *  @param  inputdata input_data list, length is 7
 *
 *	@return average value
 */
uint16_t sensor_group::get_average_data(uint16_t* input_data){
  SENSOR_GROUP_DEBUG_PRINTLN("in average function:");
  uint16_t result = 0;
  if (input_data[sensor_buffer_length-1] == 0){
    SENSOR_GROUP_DEBUG_PRINTLN("buffer not full, return 0");
    return result;
  }
  uint16_t input_max = (uint16_t)*input_data;
  uint16_t input_min = (uint16_t)*input_data;
  for(uint8_t data_count = 0; data_count < sensor_buffer_length; data_count++) { //for each 16-bit data in buffer
    result += (uint16_t)*(input_data+data_count);
    if (input_max < (uint16_t)*(input_data+data_count)) {input_max = (uint16_t)*(input_data+data_count);}
    if (input_min > (uint16_t)*(input_data+data_count)) {input_min = (uint16_t)*(input_data+data_count);}
    SENSOR_GROUP_DEBUG_PRINT((uint16_t)*(input_data+data_count));
    SENSOR_GROUP_DEBUG_PRINT("  ");
  }
  result = result - input_max - input_min;
  result = result/5;
  SENSOR_GROUP_DEBUG_PRINTLN("average function end");
  return result;
}
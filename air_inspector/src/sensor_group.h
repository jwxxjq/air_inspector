#ifndef SENSOR_GROUP_H_
#define SENSOR_GROUP_H_
#include "Arduino.h"

#ifndef max_7
  #define max_7(A,B,C,D,E,F,G) max(A, max(B, max(C, max(D, max(E, max(F, G))))))
#endif
#ifndef min_7
  #define min_7(A,B,C,D,E,F,G) min(A, min(B, min(C, min(D, min(E, min(F, G))))))
#endif

#define sensor_buffer_length 7

#define SENSOR_GROUP_DEBUG_PRINTER Serial

//#define SENSOR_GROUP_DEBUG
/* Setup debug printing macros. */
#ifdef SENSOR_GROUP_DEBUG
  #define SENSOR_GROUP_DEBUG_PRINT(...) { SENSOR_GROUP_DEBUG_PRINTER.print(__VA_ARGS__); }
  #define SENSOR_GROUP_DEBUG_PRINTLN(...) { SENSOR_GROUP_DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define SENSOR_GROUP_DEBUG_PRINT(...) {} /**< Debug Print Placeholder if Debug is disabled */
  #define SENSOR_GROUP_DEBUG_PRINTLN(...) {} /**< Debug Print Line Placeholder if Debug is disabled */
#endif

class sensor_group {
  public:
    sensor_group();
    void init();
    void put_temp_rh_value(uint16_t new_temp_value, uint16_t new_rh_value);
    void put_CO2_value(uint16_t new_CO2_value);
    void put_CH2O_value(uint16_t new_CH2O_value);
    void put_dust_value(uint16_t new_PM25_value, uint16_t new_PM10_value);
    uint16_t CO2_value;
    uint16_t CH2O_value;
    uint16_t PM25_value;
    uint16_t PM10_value;
    uint16_t TEMP_value;
    uint16_t RH_value;
    void calculate_average_value();
  
  private:
    uint16_t get_average_data(uint16_t* input_data);
    uint16_t CO2_value_list[sensor_buffer_length];
    uint16_t CH2O_value_list[sensor_buffer_length];
    uint16_t PM25_value_list[sensor_buffer_length];
    uint16_t PM10_value_list[sensor_buffer_length];
    uint16_t TEMP_value_list[sensor_buffer_length];
    uint16_t RH_value_list[sensor_buffer_length];
};

#endif
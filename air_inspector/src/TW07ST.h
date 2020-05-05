#ifndef TW07ST_H
#define TW07ST_H

#include "Arduino.h"
//#include "SoftwareSerial.h" //current version don't support soft uart
#define TW07ST_BAUD_RATE 9600 //must be 9600
#define TW07ST_item_number 7
#define TW07ST_buffer_size 7

#define max_7(A,B,C,D,E,F,G) max(A, max(B, max(C, max(D, max(E, max(F, G))))))
#define min_7(A,B,C,D,E,F,G) min(A, min(B, min(C, min(D, min(E, min(F, G))))))
#define DEBUG_PRINTER Serial

//#define TW07ST_DEBUG
/* Setup debug printing macros. */
#ifdef TW07ST_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {} /**< Debug Print Placeholder if Debug is disabled */
  #define DEBUG_PRINTLN(...) {} /**< Debug Print Line Placeholder if Debug is disabled */
#endif

class TW07ST {
  public:
    TW07ST();
    void init();
    uint16_t CO2_value;
    uint16_t TVOC_value;
    uint16_t CH2O_value;
    uint16_t PM25_value;
    uint16_t PM10_value;
    uint16_t TEMP_value;
    uint16_t RH_value;
    bool update_value();
  
  private:
    uint16_t get_average_data(uint16_t* input_data);
    uint16_t CO2_value_list[TW07ST_buffer_size];
    uint16_t TVOC_value_list[TW07ST_buffer_size];
    uint16_t CH2O_value_list[TW07ST_buffer_size];
    uint16_t PM25_list[TW07ST_buffer_size];
    uint16_t PM10_list[TW07ST_buffer_size];
    uint16_t TEMP_list[TW07ST_buffer_size];
    uint16_t RH_list[TW07ST_buffer_size];
    bool data_check(char* data);
    void calculate_average_value();
};

#endif
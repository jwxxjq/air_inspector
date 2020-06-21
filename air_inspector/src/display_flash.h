#ifndef DISPLAY_FLASH_H_
#define DISPLAY_FLASH_H_

#include "stdlib.h"
#include "Arduino.h"
#include <SPIMemory.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
//#include <FlexiTimer2.h>

//pin define
#define FLASH_CS_PIN 4 //define cs pin of flash
#define TFT_DC 9 //TFT DC pin
#define TFT_CS 10 //TFT CS pin

//flash addr part
#define fig_width 320
#define fig_high 240
//#define fig_byte_number fig_high*fig_width*2 //r5g6b5, each pixel occupy 2 byte, 153600 bytes for 240*320*2
#define fig_byte_number 153664
#define fig_pic_byte_number 153600
#define max_fig_number 10 //fig ocuupy 0x20000 ~ 0x200000
#define flash_max_size 0x200000 //2MB(16Mbit)
#define flash_size_each_fig 0x30000 //192K > 153600

#define char_lib_width 96
#define char_lib_high 200
#define char_lib_start_addr 0x1FB000 //0x1f_b000~0x1f_fb00

#define data_size_max_byte 1024 //max size for data size in flash

//test item number define
#define item_number_total 7
#define item_number_CO2 0
#define item_number_TVOC 1
#define item_number_CH2O 2
#define item_number_PM25 3
#define item_number_PM10 4
#define item_number_TEMP 5
#define item_number_RH 6

//item limit
#define item_stage_CO2_1 1200   // <1200 is green
#define item_stage_CO2_2 2000  // 1200-2000 is yellow; >= 2000 is red

#define item_stage_TVOC_1 300  // <300 ug/m^3 is green
#define item_stage_TVOC_2 500  // 300-500 is yellow; > 500 is red

#define item_stage_CH2O_1 50 // <50 ug/m^3 is green
#define item_stage_CH2O_2 80 // 50-80 is yellow; > 80 is red

#define item_stage_PM25_1 35 // <35 ug/m^3 is green
#define item_stage_PM25_2 75 // 35-75 is yellow; > 75 is red

#define item_stage_PM10_1 40 // <40 ug/m^3 is green
#define item_stage_PM10_2 80 // 40-80 is yellow; > 80 is red

#define item_stage_TEMP_1 1500 // <15 is blue
#define item_stage_TEMP_2 2800 // 15-28 is green
#define item_stage_TEMP_3 3500 // 28-35 is yellow; > 35 is red

#define item_stage_RH_1 4000 // <40 is yellow
#define item_stage_RH_2 8000 // 40-80 is green; > 80 is blue

//char lib positive
//#define char_lib_each_high 20
//#define char_lib_each_width 50
#define char_lib_number_width 10
#define char_lib_number_high 20
#define char_lib_number_radix_size 2
#define char_lib_number_radix_shift_dize 4

//#define char_lib_CO2_start_width  0
//#define char_lib_CO2_start_high  (char_lib_each_high*item_number_CO2)
//
//#define char_lib_TVOC_start_width 0
//#define char_lib_TVOC_start_high (char_lib_each_high*item_number_TVOC)
//
//#define char_lib_CH2O_start_width 0
//#define char_lib_CH2O_start_high (char_lib_each_high*item_number_CH2O)
//
//#define char_lib_PM25_start_width 0
//#define char_lib_PM25_start_high (char_lib_each_high*item_number_PM25)
//
//#define char_lib_PM10_start_width 0
//#define char_lib_PM10_start_high (char_lib_each_high*item_number_PM10)
//
//#define char_lib_unit_ppm_start_width 0
//#define char_lib_unit_ppm_start_high (char_lib_each_high*6)
//
//#define char_lib_unit_temp_start_width 0
//#define char_lib_unit_temp_start_high (char_lib_each_high*7)
//
//#define char_lib_unit_RH_start_width 0
//#define char_lib_unit_RH_start_high (char_lib_each_high*8)

#define char_lib_number_normal_start_width 50
#define char_lib_number_bold_start_width 62

//match char_lib.png
//#define char_lib_item_high 20 //height of each item and data
//#define char_lib_item_width_CO2 31
//#define char_lib_item_width_TVOC 45
//#define char_lib_item_width_CH2O 46
//#define char_lib_item_width_PM25 49
//#define char_lib_item_width_PM10 45

//#define char_lib_item_width_CO2  50
//#define char_lib_item_width_TVOC 50
//#define char_lib_item_width_CH2O 50
//#define char_lib_item_width_PM25 50
//#define char_lib_item_width_PM10 50
//
//#define char_lib_unit_ug_m_width 50
//#define char_lib_unit_ppm_width 32
////#define char_lib_unit_C_width 18
////#define char_lib_unit_percent_width 17
//
//#define char_lib_unit_C_width       20
//#define char_lib_unit_percent_width 20
//
//#define char_lib_num_width 10
//
//#define char_lib_display_max_width 90

#define char_lib_color_green_R 0
#define char_lib_color_green_G 61
#define char_lib_color_green_B 0

#define char_lib_color_yellow_R 31
#define char_lib_color_yellow_G 63
#define char_lib_color_yellow_B 0

#define char_lib_color_red_R 31
#define char_lib_color_red_G 0
#define char_lib_color_red_B 0

#define char_lib_color_blue_R 0
#define char_lib_color_blue_G 0
#define char_lib_color_blue_B 31

//flash buffer addr
//display each item need 8k(90*20*2*2=7200) (or 4k for TEMP/RH = 50*20*2) bytes buffer, total 8k*5 + 4k*2 = 48k
//#define flash_buffer_start_addr 0x10000
//#define flash_buffer_large_item_size 0x2000
//#define flash_buffer_small_item_size 0x1000
//#define flash_buffer_total_item_size 0xC000

//flash memory map
/*
0x00_0000     config reg
...
0x00_ffff     config reg end

0x02_0000     fig9 start
...
0x04_57ff     fig9 end
0x04_5800     fig9 value position start
0x04_5864     fig9 value position end

0x07_0000     fig8 start
...           fig8~2
0x1a_0000     fig1 start
...
0x1c_57ff     fig1 end
0x1c_5800     fig1 value position start
0x1c_5864     fig1 value position end

...           empty

0x1d_0000     fig0 start
...
0x1f_57ff     fig0 end
0x1f_fb00     fig0 value position start
0x1f_fb64     fig0 value position end

0x1f_b000     char_lib fig start
...
0x1f_faff     char_lib fig end


0x20_0000
*/

//#define DISPLAY_FLASH_DEBUG
/* Setup debug printing macros. */
#define DEBUG_PRINTER Serial
#ifdef DISPLAY_FLASH_DEBUG
  #define DISPLAY_FLASH_DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DISPLAY_FLASH_DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DISPLAY_FLASH_DEBUG_PRINT(...) {} /**< Debug Print Placeholder if Debug is disabled */
  #define DISPLAY_FLASH_DEBUG_PRINTLN(...) {} /**< Debug Print Line Placeholder if Debug is disabled */
#endif

class display_flash {
  public:
    display_flash();
    void init();
    void update_one_fig(uint8_t fig_num);
    void update_item_value(uint8_t fig_num, uint8_t item_count, uint16_t value);
    void read_bytes_from_flash(uint32_t start_addr, uint8_t* byte_array, uint32_t length);
    uint8_t read_one_byte_from_flash(uint32_t start_addr);
    bool write_bytes_to_flash(uint32_t start_addr, uint8_t* byte_array, uint32_t length);
  private:
    SPIFlash flash = SPIFlash(FLASH_CS_PIN);
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    bool erase_flash(uint32_t start_addr, uint32_t length);
    uint16_t mix_pixel_color(uint16_t org_color, uint8_t lib_alpha, uint8_t display_color_R, uint8_t display_color_G, uint8_t display_color_B);
    void update_one_number(uint8_t fig_num, uint8_t value,
                            uint16_t position_x, uint16_t position_y,
                            uint8_t display_color_R, uint8_t display_color_G, uint8_t display_color_B, bool is_blod);
    void draw_decimal_point(uint8_t fig_num, uint16_t position_x, uint16_t position_y, uint8_t display_color_R, uint8_t display_color_G, uint8_t display_color_B);
    void clear_one_number(uint8_t fig_num, uint16_t position_x, uint16_t position_y);
};


#endif

#include "display_flash.h"

//public
display_flash::display_flash(){
}

/**************************************************************************/
/*!
    @brief  init api, to init display & flash spi
    @param  null
*/
/**************************************************************************/
void display_flash::init(){
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  //flash init
  flash.begin();
}

/**************************************************************************/
/*!
    @brief  update one fig with the data in flash
    @param    tft       Adafruit_ILI9341 instance
    @param    fig_num   the fig number in flash
*/
/**************************************************************************/
void display_flash::update_one_fig(uint8_t fig_num){
  uint32_t start_addr = flash_max_size - flash_size_each_fig*(fig_num+1);
  uint32_t offset = 0;

//  DISPLAY_FLASH_DEBUG_PRINT ("fig ");
//  DISPLAY_FLASH_DEBUG_PRINT (fig_num);
//  DISPLAY_FLASH_DEBUG_PRINT (" start_addr = ");
//  DISPLAY_FLASH_DEBUG_PRINTLN (start_addr);

  for(uint32_t line_count = 0; line_count < fig_high; line_count++){
    uint16_t data[fig_width];
    flash.readByteArray(start_addr+offset, (uint8_t*)(data), fig_width*2);
    tft.drawRGBBitmap(
		  0,
		  line_count,
		  data,
		  fig_width,
		  1
    );
    offset += fig_width*2;
  }
//  DISPLAY_FLASH_DEBUG_PRINTLN("end");
}

/**************************************************************************/
/*!
    @brief  update the item value
    @param  fig_num     current fig number
    @param  item_count  the item_num that need to update with
    @param  value       the item value
    
    @return 0: success; 1: value over flow
*/
/**************************************************************************/
void display_flash::update_item_value(uint8_t fig_num, uint8_t item_count, uint16_t value){
  uint16_t position_x = 0;
  uint16_t position_y = 0;
  
  bool is_blod = false;

  //for color
  bool is_red = false;
  bool is_green = false;
  bool is_blue = false;
  bool is_yellow = false;

  //default color is green
  uint8_t display_color_R = char_lib_color_green_R;
  uint8_t display_color_G = char_lib_color_green_G;
  uint8_t display_color_B = char_lib_color_green_B;

//for blod size number & color
  switch (item_count){
    case item_number_CO2:
      if (value < item_stage_CO2_1){
        is_green = true;
        is_blod = true;
      }
      else if (value < item_stage_CO2_2){
        is_yellow = true;
        is_blod = false;
      }
      else{
        is_red = true;
        is_blod = true;
      }
      break;
    case item_number_TVOC:
      if (value < item_stage_TVOC_1){
        is_green = true;
        is_blod = false;
      }
      else if (value < item_stage_TVOC_2){
        is_yellow = true;
        is_blod = false;
      }
      else{
        is_red = true;
        is_blod = true;
      }
      break;
    case item_number_CH2O:
      if (value < item_stage_CH2O_1){
        is_green = true;
        is_blod = false;
      }
      else if (value < item_stage_CH2O_2){
        is_yellow = true;
        is_blod = false;
      }
      else {
        is_red = true;
        is_blod = true;
      }
      break;
    case item_number_PM25:
      if (value < item_stage_PM25_1){
        is_green = true;
        is_blod = false;
      }
      else if (value < item_stage_PM25_2){
        is_yellow = true;
        is_blod = false;
      }
      else {
        is_red = true;
        is_blod = true;
      }
      break;
    case item_number_PM10:
      if (value < item_stage_PM10_1){
        is_green = true;
        is_blod = false;
      }
      else if (value < item_stage_PM10_2){
        is_yellow = true;
        is_blod = false;
      }
      else {
        is_red = true;
        is_blod = true;
      }
      break;
    case item_number_TEMP:
      if (value < item_stage_TEMP_1){
        is_blue = true;
        is_blod = true;
      }
      else if (value < item_stage_TEMP_2){
        is_green = true;
        is_blod = false;
      }
      else if (value < item_stage_TEMP_3){
        is_yellow = true;
        is_blod = false;
      }
      else {
        is_red = true;
        is_blod = true;
      }
      break;
    case item_number_RH:
      if (value < item_stage_RH_1){
        is_yellow = true;
        is_blod = false;
      }
      else if (value < item_stage_RH_2){
        is_green = true;
        is_blod = false;
      }
      else {
        is_blue = true;
        is_blod = false;
      }
      break;
    default:
      is_green = true;
      is_blod = false;
      break;
  }
  if(is_green){
    display_color_R = char_lib_color_green_R;
    display_color_G = char_lib_color_green_G;
    display_color_B = char_lib_color_green_B;
  }
  else if (is_yellow){
    display_color_R = char_lib_color_yellow_R;
    display_color_G = char_lib_color_yellow_G;
    display_color_B = char_lib_color_yellow_B;
  }
  else if (is_red){
    display_color_R = char_lib_color_red_R;
    display_color_G = char_lib_color_red_G;
    display_color_B = char_lib_color_red_B;
  }
  else if (is_blue){
    display_color_R = char_lib_color_blue_R;
    display_color_G = char_lib_color_blue_G;
    display_color_B = char_lib_color_blue_B;
  }

  //calculate display position
  uint32_t flash_addr = 0;
  flash_addr = flash_max_size - flash_size_each_fig * (fig_num+1) + fig_pic_byte_number + 4 * item_count;
  //DISPLAY_FLASH_DEBUG_PRINT("flash addr = ");
  //DISPLAY_FLASH_DEBUG_PRINTLN(flash_addr);
  
  flash.readByteArray(flash_addr,     (uint8_t*)(&position_x), 2);
  flash.readByteArray(flash_addr + 2, (uint8_t*)(&position_y), 2);
//  DISPLAY_FLASH_DEBUG_PRINTLN(position_x);
//  DISPLAY_FLASH_DEBUG_PRINTLN(position_y);

  //calculate each number
  uint8_t display_number[4];
  bool find_first_non_zero_bit = false;
  display_number[3] = (value / 1000) % 10;
  display_number[2] = (value / 100) % 10;
  display_number[1] = (value / 10) % 10;
  display_number[0] = (value) % 10;
  
  uint16_t position_x_this_bit = position_x;
  
  if (item_count == item_number_TEMP || item_count == item_number_RH){ // 2 bits int + 1 radix
    if (display_number[3] != 0){
      update_one_number(fig_num, display_number[3], position_x_this_bit, position_y, display_color_R, display_color_G, display_color_B, is_blod);
      find_first_non_zero_bit = true;
    }
    else{
      clear_one_number(fig_num, position_x_this_bit, position_y);
    }
    position_x_this_bit += char_lib_number_width;
    
    update_one_number(fig_num, display_number[2], position_x_this_bit, position_y, display_color_R, display_color_G, display_color_B, is_blod);
    position_x_this_bit += char_lib_number_width;
    
    draw_decimal_point(fig_num, position_x_this_bit + 1, position_y + char_lib_number_high - 8, display_color_R, display_color_G, display_color_B);

    position_x_this_bit += char_lib_number_radix_shift_dize;
    
    update_one_number(fig_num, display_number[1], position_x_this_bit, position_y, display_color_R, display_color_G, display_color_B, is_blod);
    
  }
  else { // 4 bits int value
    if (display_number[3] != 0){
      update_one_number(fig_num, display_number[3], position_x_this_bit, position_y, display_color_R, display_color_G, display_color_B, is_blod);
      find_first_non_zero_bit = true;
    }
    else{
      clear_one_number(fig_num, position_x_this_bit, position_y);
    }
    position_x_this_bit += char_lib_number_width;
    
    if (display_number[2] != 0 || find_first_non_zero_bit == true){
      update_one_number(fig_num, display_number[2], position_x_this_bit, position_y, display_color_R, display_color_G, display_color_B, is_blod);
      find_first_non_zero_bit = true;
    }
    else {
      clear_one_number(fig_num, position_x_this_bit, position_y);
    }
    position_x_this_bit += char_lib_number_width;
    
    if (display_number[1] != 0 || find_first_non_zero_bit == true){
      update_one_number(fig_num, display_number[1], position_x_this_bit, position_y, display_color_R, display_color_G, display_color_B, is_blod);
      find_first_non_zero_bit = true;
    }
    else {
      clear_one_number(fig_num, position_x_this_bit, position_y);
    }
    position_x_this_bit += char_lib_number_width;

    update_one_number(fig_num, display_number[0], position_x_this_bit, position_y, display_color_R, display_color_G, display_color_B, is_blod);
  }

//  update_one_number(fig_num, 0, position_x, position_y, display_color_R, display_color_G, display_color_B, is_blod);
}

/**************************************************************************/
/*!
    @brief  clear number display area with orginal fig
    @param  fig_num     current fig number
    @param  position_x  display area position x
    @param  position_y  display area position y
*/
/**************************************************************************/
void display_flash::clear_one_number(uint8_t fig_num, uint16_t position_x, uint16_t position_y){
  uint32_t org_flash_start_addr = flash_max_size - (uint32_t)flash_size_each_fig * (fig_num+1);
  uint32_t org_flash_x_start_addr = org_flash_start_addr + ((uint32_t)fig_width * position_y + position_x) * 2; //offset caculate
  uint16_t org_color[char_lib_number_width]; //orginal fig is 16-bit each pixel
  
  for (uint8_t y_count = 0; y_count < char_lib_number_high; y_count ++){ //foreach line
    flash.readByteArray(org_flash_x_start_addr + y_count*fig_width*2, (uint8_t*)(&org_color), char_lib_number_width*2); //read orginal data into org_color[]
    tft.drawRGBBitmap(
		  position_x,
		  position_y + y_count,
		  org_color,
		  char_lib_number_width,
		  1
    );
  }
}

/**************************************************************************/
/*!
    @brief  dram a radix point at position
    @param  fig_num     current fig number
    @param  position_x  display area position x
    @param  position_y  display area position y
    @param  display_color_R  color R channel (0~31)
    @param  display_color_G  color G channel (0~63)
    @param  display_color_B  color B channel (0~31)
*/
/**************************************************************************/
void display_flash::draw_decimal_point(uint8_t fig_num, uint16_t position_x, uint16_t position_y, uint8_t display_color_R, uint8_t display_color_G, uint8_t display_color_B){
  //uint32_t org_flash_start_addr = flash_max_size - (uint32_t)flash_size_each_fig * (fig_num+1);
  //uint32_t org_flash_x_start_addr = org_flash_start_addr + ((uint32_t)fig_width * position_y + position_x) * 2; //offset caculate

  for (uint8_t y_count = 0; y_count < char_lib_number_radix_size; y_count ++){ //foreach line
    uint16_t mix_color[2];
    
    for (uint8_t x_count = 0; x_count < char_lib_number_radix_size; x_count ++){ //foreach pixel
      mix_color[x_count] = mix_pixel_color(0x0000, 255, display_color_R, display_color_G, display_color_B); //caculate each pixel
    }
    
    tft.drawRGBBitmap(
		  position_x,
		  position_y + y_count,
		  mix_color,
		  char_lib_number_radix_size,
		  1
    );
  }
}

/**************************************************************************/
/*!
    @brief  update one number
    @param  fig_num     current fig number
    @param  value       one number to display (0~9)
    @param  position_x  display area position x
    @param  position_y  display area position y
    @param  display_color_R  color R channel (0~31)
    @param  display_color_G  color G channel (0~63)
    @param  display_color_B  color B channel (0~31)
    @param  is_blod     0: nromal; 1: blod
*/
/**************************************************************************/
void display_flash::update_one_number(uint8_t fig_num, uint8_t value, uint16_t position_x, uint16_t position_y, uint8_t display_color_R, uint8_t display_color_G, uint8_t display_color_B, bool is_blod){
  uint32_t org_flash_start_addr = flash_max_size - (uint32_t)flash_size_each_fig * (fig_num+1);
  uint32_t org_flash_x_start_addr = org_flash_start_addr + ((uint32_t)fig_width * position_y + position_x) * 2; //offset caculate

//  uint32_t number_lib_flash_start_addr = char_lib_start_addr;
  uint32_t number_lib_flash_x_start_addr = char_lib_start_addr;
  if (is_blod){
    number_lib_flash_x_start_addr += char_lib_number_bold_start_width;
  }
  else{
    number_lib_flash_x_start_addr += char_lib_number_normal_start_width;
  }
  number_lib_flash_x_start_addr += char_lib_number_high*value*char_lib_width;
  
  for (uint8_t y_count = 0; y_count < char_lib_number_high; y_count ++){ //foreach line
    uint16_t org_color[char_lib_number_width]; //orginal fig is 16-bit each pixel
    uint8_t number_color[char_lib_number_width]; //number lib is 8-bit each pixel
    uint16_t mix_color[char_lib_number_width];
    
    flash.readByteArray(org_flash_x_start_addr + y_count*fig_width*2, (uint8_t*)(&org_color), char_lib_number_width*2); //read orginal data into org_color[]
    flash.readByteArray(number_lib_flash_x_start_addr + ((uint32_t)y_count)*char_lib_width, (uint8_t*)(&number_color), char_lib_number_width); //read number lib to number_color
    //DISPLAY_FLASH_DEBUG_PRINTLN(number_lib_flash_x_start_addr);
    //while(1);
    for (uint8_t x_count = 0; x_count < char_lib_number_width; x_count ++){ //foreach pixel
      mix_color[x_count] = mix_pixel_color(org_color[x_count], number_color[x_count], display_color_R, display_color_G, display_color_B); //caculate each pixel
//      DISPLAY_FLASH_DEBUG_PRINTLN("org:");
//      DISPLAY_FLASH_DEBUG_PRINTLN(org_color[x_count]);
//      DISPLAY_FLASH_DEBUG_PRINTLN("mix:");
//      DISPLAY_FLASH_DEBUG_PRINTLN(mix_color[x_count]);
    }
    
    tft.drawRGBBitmap(
		  position_x,
		  position_y + y_count,
		  mix_color,
		  char_lib_number_width,
		  1
    );
  }
}

/**************************************************************************/
/*!
    @brief  mix pixel color
    @param  org_color   orginal fig uint16_t color
    @param  lib_alpha   lib color alpha (0-31)
    @param  display_color_R  display color R channel
    @param  display_color_G  display color G channel
    @param  display_color_B  display color B channel
*/
/**************************************************************************/
uint16_t display_flash::mix_pixel_color(uint16_t org_color, uint8_t lib_alpha, uint8_t display_color_R, uint8_t display_color_G, uint8_t display_color_B){
  uint16_t result = org_color;
  
//  DISPLAY_FLASH_DEBUG_PRINTLN("orginal color = ");
//  DISPLAY_FLASH_DEBUG_PRINTLN(org_color);
//  
//  DISPLAY_FLASH_DEBUG_PRINTLN("lib_alpha = ");
//  DISPLAY_FLASH_DEBUG_PRINTLN(lib_alpha);
//  
//  while(1);  
  
  if (lib_alpha == 0){
    return result; //if lib alpha is 0, then return orginal color
  }
  if (lib_alpha == 31){
    result = (display_color_R << 11) + (display_color_G << 5) + (display_color_B);
    return result; //if lib alpha is 31, then return full display color
  }
  
  //else caculate
  uint8_t R_color = (org_color >> 11) & 31;
  uint8_t G_color = (org_color >> 5) & 63;
  uint8_t B_color = (org_color) & 31;
  
  uint32_t temp;
  temp = (R_color * (31 - lib_alpha) + display_color_R * lib_alpha) / 31;
  R_color = uint8_t(temp);
 
  temp = (G_color * (63 - lib_alpha) + display_color_G * lib_alpha) / 63;
  G_color = uint8_t(temp);

  temp = (B_color * (31 - lib_alpha) + display_color_B * lib_alpha) / 31;
  B_color = uint8_t(temp);

  result = (R_color << 11) + (G_color << 5) + (B_color);
  
  return result;
}

///**************************************************************************/
///*!
//    @brief  store the orginal fig data display area into flash
//    @param  fig_number
//*/
///**************************************************************************/
//void buffer_display_item(uint8_t fig_number){
//  uint32_t flash_offset = flash_buffer_start_addr;
//  uint32_t flash_buffer_size = flash_buffer_total_item_size;
//  
//  //need to read the start point of each display item from flash
//  uint32_t display_item_start_addr_x[item_number_total];
//  uint32_t display_item_start_addr_y[item_number_total];
//  
//  for (uint8_t item_count = 0; item_count < item_number_total; item_count ++){
//    display_item_start_addr_x[item_count] = 0;
//    display_item_start_addr_y[item_count] = item_count == 0 ? 0 : 
//      ( (item_count == item_number_RH) ? (display_item_start_addr_y[item_count-1] + char_lib_item_high) : (display_item_start_addr_y[item_count-1] + char_lib_item_high * 2) );
//  }  
//  //store orginal fig into flash
//  erase_flash(flash_offset, flash_buffer_size);
//
//  for (uint8_t item_count = 0; item_count < item_number_total; item_count ++){
//    buffer_display_item_section(display_item_start_addr_x[item_count], display_item_start_addr_y[item_count], item_count, fig_number);
//  }
//  while(1);
//}
//
///**************************************************************************/
///*!
//    @brief  store the orginal fig data of specific range, to used in display item data
//    @param  x        Top left corner horizontal coordinate.
//    @param  y        Top left corner vertical coordinate.
//    @param  item_num the item_number of this item
//*/
///**************************************************************************/
//void buffer_display_item_section(uint32_t x, uint32_t y, uint8_t item_number, uint8_t fig_number){
//  uint8_t width = 50;
//  uint8_t high = char_lib_item_high*2;
//  uint32_t flash_offset = flash_buffer_start_addr + item_number * uint32_t(flash_buffer_large_item_size);
//  uint32_t flash_length = flash_buffer_large_item_size;
//  switch (item_number){
//    case item_number_CO2:
//      width = char_lib_item_width_CO2;
//      break;
//    case item_number_TVOC:
//      width = char_lib_item_width_TVOC;
//      break;
//    case item_number_CH2O:
//      width = char_lib_item_width_CH2O;
//      break;
//    case item_number_PM25:
//      width = char_lib_item_width_PM25;
//      break;
//    case item_number_PM10:
//      width = char_lib_item_width_PM10;
//      break;
//    case item_number_TEMP:
//      high = char_lib_item_high;
//      flash_length = flash_buffer_small_item_size;
//      break;
//    case item_number_RH:
//      high = char_lib_item_high;
//      flash_length = flash_buffer_small_item_size;
//      flash_offset = flash_buffer_start_addr + flash_buffer_total_item_size - flash_buffer_small_item_size;
//      break;
//    default:
//      break;
//  }
////  DISPLAY_FLASH_DEBUG_PRINT("item_number: "); DISPLAY_FLASH_DEBUG_PRINTLN(item_number);
////  DISPLAY_FLASH_DEBUG_PRINT("item start_addr: "); DISPLAY_FLASH_DEBUG_PRINTLN(flash_offset);
////  DISPLAY_FLASH_DEBUG_PRINT("item length: "); DISPLAY_FLASH_DEBUG_PRINTLN(flash_length);
////  DISPLAY_FLASH_DEBUG_PRINT("item high: "); DISPLAY_FLASH_DEBUG_PRINTLN(high);
//  
//  if ((item_number != item_number_TEMP) && (item_number != item_number_RH)){
//    //this display item has two lines
////    DISPLAY_FLASH_DEBUG_PRINT("item_number: "); DISPLAY_FLASH_DEBUG_PRINTLN(item_number);
////    DISPLAY_FLASH_DEBUG_PRINT("offset: "); DISPLAY_FLASH_DEBUG_PRINTLN(flash_offset);
//    buffer_display_item_two_lines(x, y, width, high, flash_offset, fig_number);
//  }
//}
//
///**************************************************************************/
///*!
//    @brief  buffer display item area in orginal fig
//    @param  x        Top left corner horizontal coordinate.
//    @param  y        Top left corner vertical coordinate.
//    @param  width    the char length of this item in char_lib
//    @param  high     total high of this display item
//    @param  flash_offset  flash start addr to store fig data
//    @param  fig_number    the orginal fig number
//*/
///**************************************************************************/
//void buffer_display_item_two_lines(uint8_t x, uint8_t y, uint8_t width, uint8_t high, uint32_t flash_offset, uint8_t fig_number){
//  //uint8_t line_count = 0;
//  uint32_t flash_addr_this_line_start = flash_offset;
//  uint32_t flash_addr_org_fig_this_line_start = flash_max_size - flash_size_each_fig*(fig_number + 1) + (x << 1) + (uint32_t(y) * fig_width << 1);
//  //uint16_t pixel_count = 0;
//  uint32_t byte_count = 0;
//  
////  DISPLAY_FLASH_DEBUG_PRINT("fig_number: "); DISPLAY_FLASH_DEBUG_PRINTLN(fig_number);
////  DISPLAY_FLASH_DEBUG_PRINT("y: "); DISPLAY_FLASH_DEBUG_PRINTLN(y);
////  DISPLAY_FLASH_DEBUG_PRINT("fig addr: "); DISPLAY_FLASH_DEBUG_PRINTLN(flash_addr_org_fig_this_line_start);
////  DISPLAY_FLASH_DEBUG_PRINT("write addr: "); DISPLAY_FLASH_DEBUG_PRINTLN(flash_addr_this_line_start);
//  
//  //first two lines for round corner
////  flash.writeByte(flash_addr_this_line_start,flash.readByte(flash_addr_org_fig_this_line_start));
////  flash.writeByte(flash_addr_this_line_start + 1,flash.readByte(flash_addr_org_fig_this_line_start + 1));
////  flash.writeByte(flash_addr_this_line_start + 2,flash.readByte(flash_addr_org_fig_this_line_start + 2));
////  flash.writeByte(flash_addr_this_line_start + 3,flash.readByte(flash_addr_org_fig_this_line_start + 3));
//  for(uint8_t line_count = 0; line_count < high; line_count ++){
//    uint32_t flash_addr_org_line = flash_addr_org_fig_this_line_start + (uint32_t(line_count) * fig_width << 1);
//    uint32_t flash_addr_rev_line = flash_offset + (uint32_t(line_count) * fig_width << 1);
//    
//    for(uint16_t pixel_count = 0; pixel_count < char_lib_display_max_width; pixel_count ++){
//      uint32_t flash_addr_org_piexl = flash_addr_org_line + (uint32_t(pixel_count) << 1);
//      uint32_t flash_addr_rev_pixel = flash_addr_rev_line + (uint32_t(pixel_count) << 1);
//      
//      if (line_count < 2){ //if first two lines or last two lines, for round corner
//        if (pixel_count + line_count < 2){ //just copy three pixel data
//          flash.writeByte(flash_addr_rev_pixel, flash.readByte(flash_addr_org_piexl));
//          flash.writeByte(flash_addr_rev_pixel + 1, flash.readByte(flash_addr_org_piexl + 1));
//          //DISPLAY_FLASH_DEBUG_PRINT("copy pixel data @ "); DISPLAY_FLASH_DEBUG_PRINTLN(pixel_count);
//          //DISPLAY_FLASH_DEBUG_PRINT("read_addr: "); DISPLAY_FLASH_DEBUG_PRINTLN(flash_addr_org_piexl);
//          //DISPLAY_FLASH_DEBUG_PRINT("write_addr: "); DISPLAY_FLASH_DEBUG_PRINTLN(flash_addr_rev_pixel);
//        }
//        else if (pixel_count > (line_count + width - 3)){ //
//          flash.writeByte(flash_addr_rev_pixel, flash.readByte(flash_addr_org_piexl));
//          flash.writeByte(flash_addr_rev_pixel + 1, flash.readByte(flash_addr_org_piexl + 1));
//          //DISPLAY_FLASH_DEBUG_PRINT("copy pixel data @ "); DISPLAY_FLASH_DEBUG_PRINTLN(pixel_count);
//          //DISPLAY_FLASH_DEBUG_PRINT("read_addr: "); DISPLAY_FLASH_DEBUG_PRINTLN(flash_addr_org_piexl);
//          //DISPLAY_FLASH_DEBUG_PRINT("write_addr: "); DISPLAY_FLASH_DEBUG_PRINTLN(flash_addr_rev_pixel);
//        }
//        else{
//          DISPLAY_FLASH_DEBUG_PRINT("need to calculate @ "); DISPLAY_FLASH_DEBUG_PRINTLN(pixel_count);
//          DISPLAY_FLASH_DEBUG_PRINTLN(flash.readByte(flash_addr_org_piexl + 1)); DISPLAY_FLASH_DEBUG_PRINTLN(flash.readByte(flash_addr_org_piexl));
//          while(1);
//        }
//      }
//      else if (high - line_count < 2){
//      }
//      else{
//      }
//    }
//    while(1);
//  }
//
//}

/**************************************************************************/
/*!
    @brief  read flash
    @param  start_addr     start address
    @param  byte_array     the data array to fill into
    @param  length         total length in byte
*/
/**************************************************************************/
void display_flash::read_bytes_from_flash(uint32_t start_addr, uint8_t* byte_array, uint32_t length){
  flash.readByteArray(start_addr, byte_array, length);
}

/**************************************************************************/
/*!
    @brief  read flash
    @param  start_addr     start address
    @return byte value
*/
/**************************************************************************/
uint8_t display_flash::read_one_byte_from_flash(uint32_t start_addr){
  return flash.readByte(start_addr);
}

/**************************************************************************/
/*!
    @brief  write flash, total available range is 0x0 -> 0x400, 1k bytes
    @param  start_addr     start address
    @param  byte_array     the data array to write
    @param  length         total length in byte
    @return 0: write success; non-0: write fail
*/
/**************************************************************************/
#ifndef DISPLAY_FLASH_DISABLE_WRITE
bool display_flash::write_bytes_to_flash(uint32_t start_addr, uint8_t* byte_array, uint32_t length){
  if (start_addr + length > data_size_max_byte){ //if last addr > 1k, then return error
    return 1;
  }
  DISPLAY_FLASH_DEBUG_PRINT("start_addr = ");
  DISPLAY_FLASH_DEBUG_PRINTLN(start_addr);
  DISPLAY_FLASH_DEBUG_PRINT("length = ")
  DISPLAY_FLASH_DEBUG_PRINTLN(length);
  uint8_t data_buffer[data_size_max_byte];
  read_bytes_from_flash(0, data_buffer, data_size_max_byte);
  DISPLAY_FLASH_DEBUG_PRINTLN("data stored in flash:");
  for(uint32_t counter = 0; counter < data_size_max_byte; counter++){
    if (counter%50 == 0){DISPLAY_FLASH_DEBUG_PRINTLN("")};
    DISPLAY_FLASH_DEBUG_PRINT("  ");
    DISPLAY_FLASH_DEBUG_PRINT(data_buffer[counter]);
  }
  DISPLAY_FLASH_DEBUG_PRINTLN("");

  flash.eraseSector(0); //erase first 4KB flash
  for(uint32_t addr = 0; addr < length; addr++){
    data_buffer[start_addr+addr]=byte_array[addr];
  }

  DISPLAY_FLASH_DEBUG_PRINTLN("data to write to flash:");
  for(uint32_t counter = 0; counter < data_size_max_byte; counter++){
    if (counter%50 == 0){DISPLAY_FLASH_DEBUG_PRINTLN("")};
    DISPLAY_FLASH_DEBUG_PRINT("  ");
    DISPLAY_FLASH_DEBUG_PRINT(data_buffer[counter]);
  }
  DISPLAY_FLASH_DEBUG_PRINTLN("");

  bool result = flash.writeByteArray(0, data_buffer, data_size_max_byte);
  DISPLAY_FLASH_DEBUG_PRINT("write result = ");
  DISPLAY_FLASH_DEBUG_PRINTLN(result);
  return 0;
}
#endif

/**************************************************************************/
/*!
    @brief  earse flash from addr with length
    @param  start_addr     start address
    @param  length         total length in byte
*/
/**************************************************************************/
bool display_flash::erase_flash(uint32_t start_addr, uint32_t length){
  uint32_t current_start_addr = start_addr;
  bool result = true;
  uint32_t erase_length = 0;
//  DISPLAY_FLASH_DEBUG_PRINT("erase start addr: ");
//  DISPLAY_FLASH_DEBUG_PRINTLN(start_addr);
//  DISPLAY_FLASH_DEBUG_PRINT("size: ");
//  DISPLAY_FLASH_DEBUG_PRINTLN(length);
//  while(1);
  while(length > 0){
    if(length >= 65536){
      //length > 64KB
      erase_length = 65536;
      if (flash.eraseBlock64K(current_start_addr) != true){
        result = false;
      }
    }
    else if (length >= 32768){
      erase_length = 32768;
      if (flash.eraseBlock32K(current_start_addr) != true){
        result = false;
      }
    }
    else if (length >= 4096){
      erase_length = 4096;
      if (flash.eraseSector(current_start_addr) != true){
        result = false;
      }
    }
    else{
      erase_length = length;
      if (flash.eraseSection(current_start_addr, length) != true){
        result = false;
      }
    }
    current_start_addr += erase_length;
    length -= erase_length;
//    DISPLAY_FLASH_DEBUG_PRINT("erase length remaining: ");
//    DISPLAY_FLASH_DEBUG_PRINTLN(length);
  }
  return result;
}


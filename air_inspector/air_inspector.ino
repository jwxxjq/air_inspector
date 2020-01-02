#include "define.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
SPIFlash flash(FLASH_CS);

void setup() {
  // put your setup code here, to run once:
  tft.begin();
  flash.begin();
  Serial.begin(9600);
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  while(1){
    for (uint8_t i = 0; i < max_fig_number; i++){
      update_one_fig(i);
      buffer_display_item(1);
      delay(3000);
      while(1);
      tft.fillScreen(ILI9341_BLACK);
    }
  }
}

/**************************************************************************/
/*!
    @brief  update one fig with the data in flash
    @param    tft       Adafruit_ILI9341 instance
    @param    fig_num   the fig number in flash
*/
/**************************************************************************/
void update_one_fig(uint8_t fig_num){
  uint32_t start_addr = flash_max_size - flash_size_each_fig*(fig_num+1);
  uint32_t offset = 0;

//  Serial.print ("fig ");
//  Serial.print (fig_num);
//  Serial.print (" start_addr = ");
//  Serial.println (start_addr);

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
//  Serial.println("end");
}

/**************************************************************************/
/*!
    @brief  Draw a 16-bit image (565 RGB) at the specified (x,y) position.
            For 16-bit display devices; no color reduction performed.
            Adapted from https://github.com/PaulStoffregen/ILI9341_t3
            by Marc MERLIN. See examples/pictureEmbed to use this.
            5/6/2017: function name and arguments have changed for
            compatibility with current GFX library and to avoid naming
            problems in prior implementation.  Formerly drawBitmap() with
            arguments in different order. Handles its own transaction and
            edge clipping/rejection.
    @param  x        Top left corner horizontal coordinate.
    @param  y        Top left corner vertical coordinate.
    @param  pcolors  Pointer to 16-bit array of pixel values.
    @param  w        Width of bitmap in pixels.
    @param  h        Height of bitmap in pixels.
*/
/**************************************************************************/
//void display_section(uint32_t x, uint32_t y, uint16_t *pcolors, int16_t w, int16_t h){
//}

/**************************************************************************/
/*!
    @brief  store the orginal fig data display area into flash
    @param  fig_number
*/
/**************************************************************************/
void buffer_display_item(uint8_t fig_number){
  uint32_t flash_offset = flash_buffer_start_addr;
  uint32_t flash_buffer_size = flash_buffer_total_item_size;
  
  //need to read the start point of each display item from flash
  uint32_t display_item_start_addr_x[item_number_total];
  uint32_t display_item_start_addr_y[item_number_total];
  
  for (uint8_t item_count = 0; item_count < item_number_total; item_count ++){
    display_item_start_addr_x[item_count] = 0;
    display_item_start_addr_y[item_count] = item_count == 0 ? 0 : 
      ( (item_count == item_number_RH) ? (display_item_start_addr_y[item_count-1] + char_lib_item_high) : (display_item_start_addr_y[item_count-1] + char_lib_item_high * 2) );
  }  
  //store orginal fig into flash
  erase_flash(flash_offset, flash_buffer_size);

  for (uint8_t item_count = 0; item_count < item_number_total; item_count ++){
    buffer_display_item_section(display_item_start_addr_x[item_count], display_item_start_addr_y[item_count], item_count, fig_number);
  }
  while(1);
}

/**************************************************************************/
/*!
    @brief  store the orginal fig data of specific range, to used in display item data
    @param  x        Top left corner horizontal coordinate.
    @param  y        Top left corner vertical coordinate.
    @param  item_num the item_number of this item
*/
/**************************************************************************/
void buffer_display_item_section(uint32_t x, uint32_t y, uint8_t item_number, uint8_t fig_number){
  uint8_t width = 50;
  uint8_t high = char_lib_item_high*2;
  uint32_t flash_offset = flash_buffer_start_addr + item_number * uint32_t(flash_buffer_large_item_size);
  uint32_t flash_length = flash_buffer_large_item_size;
  switch (item_number){
    case item_number_CO2:
      width = char_lib_item_width_CO2;
      break;
    case item_number_TVOC:
      width = char_lib_item_width_TVOC;
      break;
    case item_number_CH2O:
      width = char_lib_item_width_CH2O;
      break;
    case item_number_PM25:
      width = char_lib_item_width_PM25;
      break;
    case item_number_PM10:
      width = char_lib_item_width_PM10;
      break;
    case item_number_TEMP:
      high = char_lib_item_high;
      flash_length = flash_buffer_small_item_size;
      break;
    case item_number_RH:
      high = char_lib_item_high;
      flash_length = flash_buffer_small_item_size;
      flash_offset = flash_buffer_start_addr + flash_buffer_total_item_size - flash_buffer_small_item_size;
      break;
    default:
      break;
  }
//  Serial.print("item_number: "); Serial.println(item_number);
//  Serial.print("item start_addr: "); Serial.println(flash_offset);
//  Serial.print("item length: "); Serial.println(flash_length);
//  Serial.print("item high: "); Serial.println(high);
  
  if ((item_number != item_number_TEMP) && (item_number != item_number_RH)){
    //this display item has two lines
//    Serial.print("item_number: "); Serial.println(item_number);
//    Serial.print("offset: "); Serial.println(flash_offset);
    buffer_display_item_two_lines(x, y, width, high, flash_offset, fig_number);
  }
}

/**************************************************************************/
/*!
    @brief  buffer display item area in orginal fig
    @param  x        Top left corner horizontal coordinate.
    @param  y        Top left corner vertical coordinate.
    @param  width    the char length of this item in char_lib
    @param  high     total high of this display item
    @param  flash_offset  flash start addr to store fig data
    @param  fig_number    the orginal fig number
*/
/**************************************************************************/
void buffer_display_item_two_lines(uint8_t x, uint8_t y, uint8_t width, uint8_t high, uint32_t flash_offset, uint8_t fig_number){
  //uint8_t line_count = 0;
  uint32_t flash_addr_this_line_start = flash_offset;
  uint32_t flash_addr_org_fig_this_line_start = flash_max_size - flash_size_each_fig*(fig_number + 1) + (x << 1) + (uint32_t(y) * fig_width << 1);
  //uint16_t pixel_count = 0;
  uint32_t byte_count = 0;
  
//  Serial.print("fig_number: "); Serial.println(fig_number);
//  Serial.print("y: "); Serial.println(y);
//  Serial.print("fig addr: "); Serial.println(flash_addr_org_fig_this_line_start);
//  Serial.print("write addr: "); Serial.println(flash_addr_this_line_start);
  
  //first two lines for round corner
//  flash.writeByte(flash_addr_this_line_start,flash.readByte(flash_addr_org_fig_this_line_start));
//  flash.writeByte(flash_addr_this_line_start + 1,flash.readByte(flash_addr_org_fig_this_line_start + 1));
//  flash.writeByte(flash_addr_this_line_start + 2,flash.readByte(flash_addr_org_fig_this_line_start + 2));
//  flash.writeByte(flash_addr_this_line_start + 3,flash.readByte(flash_addr_org_fig_this_line_start + 3));
  for(uint8_t line_count = 0; line_count < high; line_count ++){
    uint32_t flash_addr_org_line = flash_addr_org_fig_this_line_start + (uint32_t(line_count) * fig_width << 1);
    uint32_t flash_addr_rev_line = flash_offset + (uint32_t(line_count) * fig_width << 1);
    
    for(uint16_t pixel_count = 0; pixel_count < char_lib_display_max_width; pixel_count ++){
      uint32_t flash_addr_org_piexl = flash_addr_org_line + (uint32_t(pixel_count) << 1);
      uint32_t flash_addr_rev_pixel = flash_addr_rev_line + (uint32_t(pixel_count) << 1);
      
      if (line_count < 2){ //if first two lines or last two lines, for round corner
        if (pixel_count + line_count < 2){ //just copy three pixel data
          flash.writeByte(flash_addr_rev_pixel, flash.readByte(flash_addr_org_piexl));
          flash.writeByte(flash_addr_rev_pixel + 1, flash.readByte(flash_addr_org_piexl + 1));
          //Serial.print("copy pixel data @ "); Serial.println(pixel_count);
          //Serial.print("read_addr: "); Serial.println(flash_addr_org_piexl);
          //Serial.print("write_addr: "); Serial.println(flash_addr_rev_pixel);
        }
        else if (pixel_count > (line_count + width - 3)){ //
          flash.writeByte(flash_addr_rev_pixel, flash.readByte(flash_addr_org_piexl));
          flash.writeByte(flash_addr_rev_pixel + 1, flash.readByte(flash_addr_org_piexl + 1));
          //Serial.print("copy pixel data @ "); Serial.println(pixel_count);
          //Serial.print("read_addr: "); Serial.println(flash_addr_org_piexl);
          //Serial.print("write_addr: "); Serial.println(flash_addr_rev_pixel);
        }
        else{
          Serial.print("need to calcute @ "); Serial.println(pixel_count);
          Serial.println(flash.readByte(flash_addr_org_piexl + 1)); Serial.println(flash.readByte(flash_addr_org_piexl));
          while(1);
        }
      }
      else if (high - line_count < 2){
      }
      else{
      }
    }
    while(1);
  }

}


/**************************************************************************/
/*!
    @brief  earse flash from addr with length
    @param  start_addr     start address
    @param  length         total length in byte
*/
/**************************************************************************/
bool erase_flash(uint32_t start_addr, uint32_t length){
  uint32_t current_start_addr = start_addr;
  bool result = true;
  uint32_t erase_length = 0;
//  Serial.print("erase start addr: ");
//  Serial.println(start_addr);
//  Serial.print("size: ");
//  Serial.println(length);
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
//    Serial.print("erase length remaining: ");
//    Serial.println(length);
  }
  return result;
}

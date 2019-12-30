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
    update_one_fig(0);
    delay(3000);
    update_one_fig(1);
    delay(3000);
    update_one_fig(2);
    delay(3000);
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
  uint32_t start_addr = fig_size * fig_num;
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
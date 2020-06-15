#include "define.h"
#include "stdlib.h"

TW07ST u_TW07ST;
SERSOR_SERIAL u_SERSOR_SERIAL(3,2);
bool flag_to_update_fig;
display_flash u_display_flash;

void setup() {
  // put your setup code here, to run once:
  //display related
/*  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  //flash init
  flash.begin();
*/
  u_display_flash.init();
  //sensor init
  u_TW07ST.init();
//  Serial.begin(9600); //TW07ST will init serial

  //set timer to update backgroud fig
  flag_to_update_fig = false;
  FlexiTimer2::set(time_each_fig_ms, 1.0/1000, set_update_fig_flag);
  FlexiTimer2::start();
}

void loop() {
  while(1){
    for (uint8_t i = 0; i < max_fig_number; i++){
      Serial.print("fig");
      Serial.println(i);

      u_display_flash.update_one_fig(i);
      //set default to all-zeros
      u_display_flash.update_item_value(i, item_number_CO2,  0);
      u_display_flash.update_item_value(i, item_number_TVOC, 0);
      u_display_flash.update_item_value(i, item_number_CH2O, 0);
      u_display_flash.update_item_value(i, item_number_PM25, 0);
      u_display_flash.update_item_value(i, item_number_PM10, 0);
      u_display_flash.update_item_value(i, item_number_TEMP, 0);
      u_display_flash.update_item_value(i, item_number_RH,   0);
      
      //update real value display
      while(!flag_to_update_fig){
        //Serial.println("waiting for sensor update");
        if (u_TW07ST.update_value() && u_TW07ST.CO2_value != 0){
          u_display_flash.update_item_value(i, item_number_CO2,  u_TW07ST.CO2_value);
          u_display_flash.update_item_value(i, item_number_TVOC, u_TW07ST.TVOC_value);
          u_display_flash.update_item_value(i, item_number_CH2O, u_TW07ST.CH2O_value);
          u_display_flash.update_item_value(i, item_number_PM25, u_TW07ST.PM25_value);
          u_display_flash.update_item_value(i, item_number_PM10, u_TW07ST.PM10_value);
          u_display_flash.update_item_value(i, item_number_TEMP, u_TW07ST.TEMP_value*10);
          u_display_flash.update_item_value(i, item_number_RH,   u_TW07ST.RH_value*10);
          //Serial.println("update sensor value done");
          //Serial.println(u_TW07ST.TEMP_value);
          u_SERSOR_SERIAL.print_value(u_TW07ST.PM25_value, u_TW07ST.PM10_value, u_TW07ST.CO2_value, u_TW07ST.CH2O_value, u_TW07ST.TEMP_value, u_TW07ST.RH_value);
        }
      }
      flag_to_update_fig = false;
      //tft.fillScreen(ILI9341_BLACK);
    }
  }
}

void set_update_fig_flag(){
  //Serial.print("print message: Called at: ");
  //Serial.println(millis());
  //Serial.println("update one fig");
  flag_to_update_fig = true;
}
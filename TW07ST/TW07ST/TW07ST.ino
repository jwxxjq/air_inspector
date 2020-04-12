#include "TW07ST.h"

TW07ST u_TW07ST;

void setup() {
  // put your setup code here, to run once:
  u_TW07ST.init();

}

void loop() {
  delay(100);
  if (u_TW07ST.update_value()){
    if (u_TW07ST.CO2_value != 0 && u_TW07ST.TVOC_value != 0 && u_TW07ST.CH2O_value != 0){
      Serial.print("CO2: ");
      Serial.println(u_TW07ST.CO2_value);
//    Serial.print("TVOC: ");
//    Serial.println(u_TW07ST.TVOC_value);
//    Serial.print("CH2O: ");
//    Serial.println(u_TW07ST.CH2O_value);
//    Serial.print("PM25: ");
//    Serial.println(u_TW07ST.PM25_value);
//    Serial.print("PM10: ");
//    Serial.println(u_TW07ST.PM10_value);
//    Serial.print("TEMP: ");
//    Serial.println(u_TW07ST.TEMP_value);
//    Serial.print("RH: ");
//    Serial.println(u_TW07ST.RH_value);
    }
  }
}
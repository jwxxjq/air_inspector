#include "define.h"
#include "stdlib.h"

//TW07ST u_TW07ST;
SERSOR_SERIAL u_SERSOR_SERIAL(3,2);
MHZ19_SW_UART mhz19_sw_uart(8,7);
bool flag_to_update_fig;
display_flash u_display_flash;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
sensor_group u_sensor_value_group;
PMS7003_UART pms(14, 15);
 
void setup() {
  // put your setup code here, to run once:
  //display related
  Serial.begin(BAUD_RATE); // for debug print
  u_display_flash.init();

  //sensor init
  sht31.begin(0x44); //SHT31 init
  Serial.println("sht31 init done");
  mhz19_sw_uart.init();
  Serial.println("mhz19 init done");
  pms.init();
  Serial.println("pms init done");
  u_sensor_value_group.init();

  //set timer to update backgroud fig
  flag_to_update_fig = false;
  FlexiTimer2::set(time_each_fig_ms, 1.0/1000, set_update_fig_flag);
  FlexiTimer2::start();
 
  LED_PWM_timer1_init();
//  pinMode(,INPUT);
}

void loop() {
  LED_PWM_set(LED_birghtness[5]);
  Serial.println("entre loop");
  while(1){
    for (uint8_t i = 0; i < max_fig_number; i++){
      Serial.print("fig");
      Serial.println(i);
//      uint8_t data[2] = {1,1};
//      u_display_flash.write_bytes_to_flash(1022, data, 2);
//      Serial.println(u_display_flash.read_one_byte_from_flash(1023));

//      uint8_t data = u_display_flash.read_one_byte_from_flash(1023);
//      Serial.print("current data = ");
//      Serial.println(data);
//      data += 1;
//      u_display_flash.write_bytes_to_flash(1023, &data, 1);
//      Serial.print("write data = ");
//      Serial.println(u_display_flash.read_one_byte_from_flash(1023));
//      while(1);

      u_display_flash.update_one_fig(i);
      //set default to all-zeros
      u_display_flash.update_item_value(i, item_number_CO2,  0);
      u_display_flash.update_item_value(i, item_number_TVOC, 0);
      u_display_flash.update_item_value(i, item_number_CH2O, 0);
      u_display_flash.update_item_value(i, item_number_PM25, 0);
      u_display_flash.update_item_value(i, item_number_PM10, 0);
      u_display_flash.update_item_value(i, item_number_TEMP, 0);
      u_display_flash.update_item_value(i, item_number_RH,   0);

//      while(1){
//        for (uint16_t j = 6; j > 0; j = j-1){
//          u_display_flash.update_item_value(i, item_number_PM10, j-1);
//          LED_PWM_set(LED_birghtness[j-1]);
//          delay(1000);
//        }
//        //delay_ms(500);
//      }

      //update real value display
      while(!flag_to_update_fig){
        //Serial.println("waiting for sensor update");
        if (u_sensor_value_group.TEMP_value != 0 && u_sensor_value_group.RH_value != 0){
          u_display_flash.update_item_value(i, item_number_TEMP, u_sensor_value_group.TEMP_value);
          u_display_flash.update_item_value(i, item_number_RH, u_sensor_value_group.RH_value);
          u_display_flash.update_item_value(i, item_number_CO2, u_sensor_value_group.CO2_value);
          u_display_flash.update_item_value(i, item_number_PM25, u_sensor_value_group.PM25_value);
          u_display_flash.update_item_value(i, item_number_PM10, u_sensor_value_group.PM10_value);
        }
        //int temp = analogRead(7);
        //u_display_flash.update_item_value(i, item_number_CH2O, (uint16_t)temp);
        sensor_power_on();
        delay(1500);
        sensor_power_off();
        delay(1500);
        update_sensor_values();
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

void update_sensor_values(){
  //temperature & humidity
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  uint16_t temp = uint16_t (t*100);
  uint16_t rh = uint16_t (h*100);
  u_sensor_value_group.put_temp_rh_value(temp, rh);
  
  //CO2
  uint32_t CO2_value = mhz19_sw_uart.read_CO2_value();
  Serial.print("CO2_value = ");
  Serial.println(CO2_value);
  if (CO2_value != 15000 && CO2_value != 0 && CO2_value != 0xffffffff){  //not 0 & 15000 & time_out
    u_sensor_value_group.put_CO2_value(CO2_value);
  }

  //dust: PM2.5 & PM10
  if (pms.update_dust_value()){
    u_sensor_value_group.put_dust_value(pms.PM25_value,pms.PM10_value);
    Serial.print("PM2.5 value = ");
    Serial.println(pms.PM25_value);
    Serial.print("PM10 value = ");
    Serial.println(pms.PM10_value);
  }

  u_sensor_value_group.calculate_average_value();
}

//timer init
void LED_PWM_timer1_init(){
  pinMode(9, OUTPUT);
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS10) | (1 << WGM12); // CS10 -- prescaler 1, WGM12 -- CTC mode
  TIMSK1 = 0;
  TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B); // OCIE1A -- OCR1A, OCIE1B -- OCR1B
  OCR1A = MCU_xtal_freq / LED_PWA_freq - 1; // set OCR1A value, which return 0 in next cycle, change PB9 to high
  OCR1B = MCU_xtal_freq / LED_PWA_freq / 100 * 50 - 1; // set OCR1B, this will change PB9 to low, default set to 50%
}

//change PWM cycle
void LED_PWM_set(uint8_t PWM_precentage){
  if (PWM_precentage >= 100){
    PWM_precentage = 100; //max 100%
  }
  OCR1B = MCU_xtal_freq / LED_PWA_freq / 100 * PWM_precentage - 1; // set to PWA_precentage %
}

ISR(TIMER1_COMPA_vect) {
  PORTB |= B00000010; // pin 9设置为HIGH
}
 
ISR(TIMER1_COMPB_vect) {
  PORTB &= B11111101; // pin 9设置为LOW
}

void sensor_power_on(){
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
}

void sensor_power_off(){
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
}
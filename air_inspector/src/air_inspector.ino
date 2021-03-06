#include "define.h"
#include "stdlib.h"

//TW07ST u_TW07ST;
SERSOR_SERIAL u_SERSOR_SERIAL(3,2);
MHZ19_SW_UART mhz19_sw_uart(8,7);
display_flash u_display_flash;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
sensor_group u_sensor_value_group;
PMS7003_UART pms(14, 15);
ZE08_CH2O u_ZE08(5,6); //5: RXM, 6: TXM

unsigned long last_update_fig_time;
bool flag_to_update_fig; //set ture when need to update fig; set to false when one fig is updated
bool sensor_enable;

void setup() {
  // put your setup code here, to run once:
  //display related
  Serial.begin(BAUD_RATE); // for debug print
  u_display_flash.init();
  
  sensor_power_on();
  //sensor init
  sht31.begin(0x44); //SHT31 init
  Serial.println("sht31 init done");

  mhz19_sw_uart.init();
  Serial.println("mhz19 init done");
  pms.init();
  Serial.println("pms init done");
  u_ZE08.init();
  Serial.println("ZE08_CH2O init done");
  u_sensor_value_group.init();

  //set timer to update backgroud fig
  flag_to_update_fig = false;
  last_update_fig_time = millis();
  Serial.println(last_update_fig_time);

  //set sensor enable
  sensor_enable = true;

  //init timer1 for LED_PWM control
  LED_PWM_timer1_init();
}

void loop() {
//  update_LED_PWM_level();
  AIR_INSPECTOR_UART_DEBUG_PRINTLN("enter loop");
  while(1){
    for (uint8_t i = 0; i < max_fig_number; i++){
      AIR_INSPECTOR_UART_DEBUG_PRINT("fig");
      AIR_INSPECTOR_UART_DEBUG_PRINTLN(i);
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
      u_display_flash.update_item_value(i, item_number_CH2O, 0);
      u_display_flash.update_item_value(i, item_number_PM25, 0);
      u_display_flash.update_item_value(i, item_number_PM10, 0);
      u_display_flash.update_item_value(i, item_number_TEMP, 0);
      u_display_flash.update_item_value(i, item_number_RH,   0);

      //update real value display
      while(!flag_to_update_fig){
        //Serial.println("waiting for sensor update");
        if (u_sensor_value_group.TEMP_value != 0 && u_sensor_value_group.RH_value != 0){
          u_display_flash.update_item_value(i, item_number_TEMP, u_sensor_value_group.TEMP_value);
          u_display_flash.update_item_value(i, item_number_RH, u_sensor_value_group.RH_value);
          u_display_flash.update_item_value(i, item_number_CO2, u_sensor_value_group.CO2_value);
          u_display_flash.update_item_value(i, item_number_PM25, u_sensor_value_group.PM25_value);
          u_display_flash.update_item_value(i, item_number_PM10, u_sensor_value_group.PM10_value);
          u_display_flash.update_item_value(i, item_number_CH2O, u_sensor_value_group.CH2O_value);
        }
        check_if_update_fig();
        update_sensor_values();

        wait_next_sensor_update(); //delay time here
        update_sensor_enable();
      }
      flag_to_update_fig = false;
      //tft.fillScreen(ILI9341_BLACK);
    }
  }
}

//read enable pin to init / disable sensor
void update_sensor_enable(){
  int temp = analogRead(7);

  if (temp > 1000){ //enable pin pull high
    if (sensor_enable){
      //enable -> enable, do nothing
      sensor_enable = true;
    }
    else{
      //disable -> enable, turn on sensor
      sensor_enable = true;
      sensor_power_on();
      delay(10);

      pms.wakeup();
      mhz19_sw_uart.init();
      u_ZE08.init();
      //AIR_INSPECTOR_UART_DEBUG_PRINTLN("sensor wakeup done");
    }
  }
  else{
    if (sensor_enable){
      //enable -> disable, enter low power
      sensor_enable = false;
      //turn off sensor power
      sensor_power_off();
      pms.sleep(); //set pms into sleep
      //delay(10);
      //AIR_INSPECTOR_UART_DEBUG_PRINTLN("enter low power mode done");
    }
    else{
      //disable -> disable, do nothing
      sensor_enable = false;
    }
  }
}
//sensor enable set
void sensor_power_on(){
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
}

void sensor_power_off(){
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
}

//delay time for sensor update
//should adjust LED_PWM inside
void wait_next_sensor_update(){
  unsigned long enter_delay_time = millis();
  while(1){
    update_LED_PWM_level();
    delay(500);
    unsigned long this_time = millis();
    if (enter_delay_time + sensor_read_delay_ms < this_time){
      //AIR_INSPECTOR_UART_DEBUG_PRINTLN(enter_delay_time + sensor_read_delay_ms);
      break;
    }
    else if (enter_delay_time > this_time){
      break; //directly break when millis() return to 0x0
    }
  }
}

//set update flag to ture
void set_update_fig_flag(){
  flag_to_update_fig = true;
}

//check time duration for update fig
void check_if_update_fig(){
//  Serial.println("in check function");
  unsigned long this_time = millis();
  //AIR_INSPECTOR_UART_DEBUG_PRINTLN(this_time);
  //AIR_INSPECTOR_UART_DEBUG_PRINTLN(last_update_fig_time);
  if (this_time - last_update_fig_time > time_each_fig_ms){
    set_update_fig_flag();
    last_update_fig_time = this_time;
  }
  else if ((this_time < time_each_fig_ms) && (last_update_fig_time > time_each_fig_ms)){ // back to 0 about 50 days
    set_update_fig_flag();
    last_update_fig_time = this_time;
  }
}

void update_sensor_values(){
  //temperature & humidity
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  uint16_t temp = uint16_t (t*100);
  uint16_t rh = uint16_t (h*100);
  u_sensor_value_group.put_temp_rh_value(temp, rh);
  
  if (sensor_enable) {
    //delay(500);
    //CO2
    //AIR_INSPECTOR_UART_DEBUG_PRINTLN("start CO2");
    uint32_t CO2_value = mhz19_sw_uart.read_CO2_value();
    AIR_INSPECTOR_UART_DEBUG_PRINT("CO2_value = ");
    AIR_INSPECTOR_UART_DEBUG_PRINTLN(CO2_value);
    if (CO2_value != 15000 && CO2_value != 0 && CO2_value != 0xffffffff){  //not 0 & 15000 & time_out
      u_sensor_value_group.put_CO2_value(CO2_value);
    }
  
    delay(500);
    //dust: PM2.5 & PM10
    //AIR_INSPECTOR_UART_DEBUG_PRINTLN("start dust");
    if (pms.update_dust_value()){
      u_sensor_value_group.put_dust_value(pms.PM25_value,pms.PM10_value);
      AIR_INSPECTOR_UART_DEBUG_PRINT("PM2.5 value = ");
      AIR_INSPECTOR_UART_DEBUG_PRINTLN(pms.PM25_value);
      AIR_INSPECTOR_UART_DEBUG_PRINT("PM10 value = ");
      AIR_INSPECTOR_UART_DEBUG_PRINTLN(pms.PM10_value);
    }
  
    delay(500);
    //CH2O
    //AIR_INSPECTOR_UART_DEBUG_PRINTLN("start CH2O");
    uint16_t CH2O_value = u_ZE08.read_CH2O_value();
    AIR_INSPECTOR_UART_DEBUG_PRINT("CH2O value = ");
    AIR_INSPECTOR_UART_DEBUG_PRINTLN(CH2O_value);
    if (CH2O_value != 0){  //heating return 0x0
      u_sensor_value_group.put_CH2O_value(CH2O_value);
    }
  }
  else{
    AIR_INSPECTOR_UART_DEBUG_PRINTLN("sensor disabled");
  }

  u_sensor_value_group.calculate_average_value();

  u_SERSOR_SERIAL.print_value(u_sensor_value_group.PM25_value, u_sensor_value_group.PM10_value,
                              u_sensor_value_group.CO2_value, u_sensor_value_group.CH2O_value,
                              u_sensor_value_group.TEMP_value, u_sensor_value_group.RH_value);

}

//timer init for LED_PWM
void LED_PWM_timer1_init(){
  pinMode(9, OUTPUT);

  #ifdef disable_LED_PWM
    digitalWrite(9, HIGH);
    AIR_INSPECTOR_UART_DEBUG_PRINTLN("LED_PWM disabled");
  #else
    #ifdef use_HW_LED_PWM
      analogWrite(9, 255);
      AIR_INSPECTOR_UART_DEBUG_PRINTLN("USE HW PWM");
    #else
      TCCR1A = 0;
      TCCR1B = 0;
      TCCR1B |= (1 << CS10) | (1 << WGM12); // CS10 -- prescaler 1, WGM12 -- CTC mode
      TIMSK1 = 0;
      TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B); // OCIE1A -- OCR1A, OCIE1B -- OCR1B
      OCR1A = MCU_xtal_freq / LED_PWA_freq - 1; // set OCR1A value, which return 0 in next cycle, change PB9 to high
      OCR1B = MCU_xtal_freq / LED_PWA_freq / 100 * 50 - 1; // set OCR1B, this will change PB9 to low, default set to 50%
    #endif
  #endif
}

//change LED PWM cycle
void LED_PWM_set(uint8_t PWM_precentage){
  if (PWM_precentage >= 100){
    PWM_precentage = 100; //max 100%
  }
  OCR1B = MCU_xtal_freq / LED_PWA_freq / 100 * PWM_precentage - 1; // set to PWA_precentage %
}

ISR(TIMER1_COMPA_vect) {
  PORTB |= B00000010; // pin 9 set HIGH
}
 
ISR(TIMER1_COMPB_vect) {
  PORTB &= B11111101; // pin 9 set LOW
}

void update_LED_PWM_level(){
  #ifdef disable_LED_PWM
    digitalWrite(9, HIGH);
  #else
    int temp = analogRead(6);
    //AIR_INSPECTOR_UART_DEBUG_PRINT("light: ");
    //AIR_INSPECTOR_UART_DEBUG_PRINTLN(temp);

    uint8_t index = 0;
    uint8_t index_max = sizeof(LED_light_in_threshold)/sizeof(LED_light_in_threshold[0]);
    //AIR_INSPECTOR_UART_DEBUG_PRINTLN(index_max);
    for(index = 0; index < index_max; index++){
      int threhold_value = LED_light_in_threshold[index]<<2;
      if (temp > threhold_value){
        continue;
      }
      else{
        //AIR_INSPECTOR_UART_DEBUG_PRINTLN(index);
        break;
      }
    }
    //AIR_INSPECTOR_UART_DEBUG_PRINT("set LED_PWM to level");
    //AIR_INSPECTOR_UART_DEBUG_PRINTLN(index);

    #ifdef use_HW_LED_PWM
      analogWrite(9, LED_birghtness[index]);
    #else
      LED_PWM_set(LED_birghtness[index]);
    #endif
  #endif
}
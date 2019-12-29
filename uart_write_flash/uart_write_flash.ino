#include<SoftwareSerial.h>
#include<SPIMemory.h>
#define fig_width 320
#define fig_high 240
#define BAUD_RATE 9600 //for serial.print
#define TRANS_BAUD_RATE 74880 //for softSerial
#define FLASH_CS 4 //define cs pin of flash
//#define fig_byte_number fig_high*fig_width*2 //r5g6b5, each pixel occupy 2 byte, 153600 bytes for 240*320*2
#define fig_byte_number 153600

#define fig0_start_addr 0x30000*0 //0x0 ~ 0x‭25800‬ for 240*320*2
#define fig1_start_addr 0x30000*1
#define fig2_start_addr 0x30000*2
#define fig3_start_addr 0x30000*3

#define flash_start_addr fig2_start_addr //fig start addr



SPIFlash flash(FLASH_CS);
SoftwareSerial softSerial(3,2);
uint32_t offset = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  softSerial.begin(TRANS_BAUD_RATE);
  softSerial.listen();
  flash.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println(flash.readByte(0x0));
//  Serial.println(flash.readByte(0x30000));
//  Serial.println(flash.readByte(0x557ff));
//  Serial.println(fig_byte_number);
//  Serial.println(fig_high);
//  Serial.println(fig_width);
//  uint32_t test = fig_high * fig_width;
//  Serial.println(test);
  while(1){

    if (softSerial.available() > 5){
      //erase flash here instead of in setup(), in case of mistach reset putton
//      if (flash.eraseSection(flash_start_addr, 153600) == false){
//        Serial.println("flash erase error");
//        while(1);
//      }
//      else {
//        Serial.print("erase Section success, start addr: ");
//        Serial.print(flash_start_addr);
//        Serial.print("; length: ");
//        Serial.println(fig_byte_number);
//      }
      if (erase_flash(flash_start_addr, fig_byte_number) != true){
        Serial.println("flash erase error");
        while(1);
      }
//      flash.eraseSection(flash_start_addr, 65536);
//      flash.eraseSection(flash_start_addr+65536, 65536);
      //flash.eraseChip();
      break;
    }
  }
  while(1){
    if(softSerial.available() > 31){
      uint8_t data[32];
      bool result;
      softSerial.readBytes(data, 32);
      result = flash.writeByteArray(flash_start_addr+offset, data, 32); // write 32 byte to flash
      if (result != true) {
        Serial.print("write flash error @ ");
        Serial.println(offset);
      }
      uint8_t read_data[32];
      result = flash.readByteArray(flash_start_addr+offset, read_data, 32);
      if (result != true) {
        Serial.println("read flash error @");
        Serial.println(offset);
      }
      softSerial.write(summary(read_data));
      offset += 32;
    }
  }
}

uint8_t summary(uint8_t* data){
  uint8_t result = 0;
  for (uint8_t i = 0; i<32; i++){
    result += data[i];
  }
  return result;
}

bool erase_flash(uint32_t start_addr, uint32_t length){
  uint32_t current_start_addr = start_addr;
  bool result = true;
  uint32_t erase_length = 0;
  while(length > 0){
    if(length > 65536){
      //length > 64KB
      erase_length = 65536;
      if (flash.eraseBlock64K(current_start_addr) != true){
        result = false;
      }
    }
    else if (length > 32768){
      erase_length = 32768;
      if (flash.eraseBlock32K(current_start_addr) != true){
        result = false;
      }
    }
    else if (length > 4096){
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
    Serial.print("erase length remaining: ");
    Serial.println(length);
  }
  return result;
}
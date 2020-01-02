#include<SoftwareSerial.h>
#include<SPIMemory.h>
#define fig_width 320
#define fig_high 240
#define BAUD_RATE 9600 //for serial.print
#define TRANS_BAUD_RATE 57600 //for softSerial
#define FLASH_CS 4 //define cs pin of flash
//#define fig_byte_number fig_high*fig_width*2 //r5g6b5, each pixel occupy 2 byte, 153600 bytes for 240*320*2
#define fig_byte_number 153600
#define max_fig_number 1
#define flash_max_size 0x200000 //2MB(16Mbit)
#define flash_size_each_fig 0x30000 //192K > 153600

#define char_lib_width 96
#define char_lib_high 200
#define char_lib_start_addr 0x1FB000 //0x1f_b000~0x1f_fb00
#define char_lib_byte_number 19200

//#define fig1_start_addr flash_max_size - 0x30000*1 //0x0 ~ 0x‭25800‬ for 240*320*2
SPIFlash flash(FLASH_CS);
SoftwareSerial softSerial(3,2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  softSerial.begin(TRANS_BAUD_RATE);
  softSerial.listen();
  flash.begin();
//  uint8_t test;
//  uint32_t addr = char_lib_start_addr+0x5;
//  Serial.println(addr);
//  Serial.println(flash.readByte(addr));
//  
//  addr = char_lib_start_addr+0x4843;
//  Serial.println(addr);
//  Serial.println(flash.readByte(addr));
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("waiting for start");
  for (uint8_t fig_count = 0; fig_count < max_fig_number; fig_count ++){
    uint32_t byte_count = 0;
    uint32_t flash_start_addr = flash_max_size - (flash_size_each_fig * fig_count) - flash_size_each_fig;
    uint32_t offset = 0;
    while(1){
      if (softSerial.available() > 5){
        //erase flash here instead of in setup(), in case of mistach reset putton
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
    Serial.println("flash erase done");
    while(byte_count < fig_byte_number){

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
        byte_count += 32;
      }
    }
    Serial.println("one fig done");
  }
  
  Serial.println("write char_lib start");
  uint32_t byte_count = 0;
  uint32_t flash_start_addr = char_lib_start_addr;
  uint32_t offset = 0;
  while(1){
    if (softSerial.available() > 5){
      //erase flash here instead of in setup(), in case of mistach reset putton
      if (erase_flash(flash_start_addr, char_lib_byte_number) != true){
        Serial.println("flash erase error");
        while(1);
      }
      break;
    }
  }
  Serial.println("flash erase done");
  while(byte_count < char_lib_byte_number){
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
      byte_count += 32;
    }
  }
  Serial.print("write char lib done");
  while(1);
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
  Serial.print("erase start addr: ");
  Serial.println(start_addr);
  Serial.print("size: ");
  Serial.println(length);
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
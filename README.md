# air_inspector
indoor air quility inspector for PM2.5/PM10/CO2/CH2O/TEMP/RH

comptable with self-build ESPEasy version: https://github.com/jwxxjq/ESPEasy

hardware:

    Temperature Sensor: SHT31
    
    Dust Sensor: PMS7003
    
    CH2O Sensor: ZE08
    
    CO2 Sensor: MH-Z19B

    mcu: ATmega328P (running Arduino)

    displayer: 320*240 ILI9341

    flash: W25Q16JVSSIG

    wifi connector: ESP8266


lib necessary:

    python:

        serial
  
        matplotlib
  
    Arduino:

        SPIMemory
  
        Adafruit_ILI9341
        
        Adafruit_GFX_Library
        
        Adafruit_SHT31
        
        MHZ19
        
        PMS
        
PCB projcet:

    https://oshwhub.com/jwxxjq/air_inspector

STEPS:

1. write fig data into flash

    A. prepare fig0~9.png (must be 240*320)
  
    B. excute host_pc_script/trans_png_to_bin.py, output figx.bin
    
    C. edit host_pc_script/uart_write.py
    
        a. COM port @ line 25
    
        b. baud rate @ line 27 (should match step D)
    
        c. do not excute right now
  
    D. edit uart_write_flash/uart_write_flash.ino
  
        a. BAUD_TATE @ line 6 (should match step C above)
    
        b. load into MCU
    
        c. then excute host_pc_script/uart_write.py, wait all lines finish
    
    E. if checksum error happens, may need to slow down the baud rate
  
2. load main program to MCU

    A. load air_inspector/air_inspector.ino to MCU
    
3. flash ESP8266 with this version: https://github.com/jwxxjq/ESPEasy

    A. select Environment - custom_sensor in Devices page, RX pin: 3, TX pin: 1
    
    B. other settings same with orginal ESPEasy(https://github.com/letscontrolit/ESPEasy) (like MQTT or others)


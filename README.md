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

    A. prepare figx.png (must be 240*320), x start with 0
  
    B. edit host_pc_script/trans_png_to_bin.py
  
        a. update file name @ line 8
        
        b. edit default value for display area @ line 62 if needed
    
        b. excute this script, then get figx.bin
    
    C. edit host_pc_script/uart_write.py
  
        a. fig name @ line 7
    
        b. COM port @ lien 25
    
        c. baud rate @ line 27
    
        d. do not excute right now
  
    D. edit uart_write_flash/uart_write_flash.ino
  
        a. BAUD_TATE @ line 6
    
        b. FLASH cs pin @ line 7
    
        c. update fig_addr @ lien 16
    
        d. load into MCU
    
        e. then excute host_pc_script/uart_write.py, wait all lines finish
    
    E. if checksum error happens, may need to slow down the baud rate
  
 
2. test display fig

    A. load air_inspector/air_inspector.ino to MCU
    
    B. should display figs in turns with the values of every test items update and refrush randomly.
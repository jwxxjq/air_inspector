# air_inspector
indoor air quility inspector for PM2.5/PM10/CO2/TVOC/HCOH/TEMP/RH


hardware:

main Sensor: TW70ST

mcu: ATmega328P (running Arduino)

displayer: 320*240 ILI9341

flash: W25Q16JVSSIG

Sensor 2: DHT11

wifi connector: ESP8266


lib necessary:

python:

  serial
  
  matplotlib
  
Arduino:

  SPIMemory
  
  Adafruit_ILI9341


STEPS:

1. write fig data into flash

  1.1. prepare figx.png (must be 240*320)
  
  1.2. edit host_pc_script/trans_png_bt_bin.py
  
    a. update file name @ line 6
    
    b. excute this script, then get figx.bin
    
  1.3. edit host_pc_script/uart_write.py
  
    a. fig name @ line 9
    
    b. COM port @ lien 25
    
    c. baud rate @ line 27
    
    d. do not excute now
  
  1.4. update uart_write_flash/uart_write_flash.ino
  
    a. BAUD_TATE @ line 6
    
    b. FLASH cs pin @ line 7
    
    c. update fig_addr @ lien 16
    
    d. load into MCU
    
    e. then excute host_pc_script/uart_write.py, wait all lines finish
    
  1.5. if checksum error happens, may need to slow down the baud rate
  
  1.6. change to next fig, repeat 1.1 ~ 1.5
  
2. test display fig

  on-going
  
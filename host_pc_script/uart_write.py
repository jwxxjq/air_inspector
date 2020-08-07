import serial
import binascii
import time
import struct
import re

total_fig_number = 10 # toatal fig number
#total_line_number = 240 # high of each fig
#write_byte_each_line = 640 # 320*2, each pixel occupy 2 bytes

total_char_lib_line_number = 200
write_char_lib_byte_each_line = 96 #80
fig_byte_number = 153664

def caculate_sum(data):
  sum = 0
  data_array = re.findall(r'.{2}', data)
#  print (data_array)
  for one_byte in data_array:
    sum += int(one_byte, 16)
  sum_byte = sum%256
  return sum_byte

try:
  portx="COM7"
#  bps=9600
  bps=76800
  timex=5
  ser=serial.Serial(portx,bps,timeout=timex)
  print ("open serial ok")

  fig_count = 0
  while fig_count < total_fig_number:
    byte_count = 0
    line_count = 0
    fig_name_count = fig_count
    file_name = "fig"+str(fig_name_count)+".bin"
    file = open(file_name, 'rb')
    print ("fig start", fig_name_count)
    byte_count = 0
    while byte_count < fig_byte_number:
      str_string = file.read(32);
      hex_string = str(binascii.b2a_hex(str_string))[2:-1]
      #print (hex_string);
      str_sum = caculate_sum(hex_string);
      #print ("sum should be", str_sum)
      if not str_string:
        break
      ser.write(bytes().fromhex(hex_string))
      while (1):
        if ser.in_waiting > 0:
          break
      read_string = ord(ser.read(ser.in_waiting))
      ser.reset_input_buffer();
      #print (read_string)
      if int(read_string) != str_sum:
        print ("error @ line", line_count)
        print ("sum should be", str_sum)
        print ("get number is", int(read_string))
        ser.close()
        exit
      if not str_string:
        break
      byte_count += 32
      if ((byte_count % 10240) == 0): #every 1k bytes
        print ("byte finish", byte_count)
    file.close()
    print ("total byte finish", byte_count)
    fig_count += 1

  byte_count = 0
  line_count = 0
  file_name = "char_lib.bin"
  file = open(file_name, 'rb')
  print ("char lib start");
  while line_count < total_char_lib_line_number:
    byte_count = 0
    while byte_count < write_char_lib_byte_each_line:
      str_string = file.read(32);
      hex_string = str(binascii.b2a_hex(str_string))[2:-1]
      #print (hex_string);
      str_sum = caculate_sum(hex_string);
      #print ("sum should be", str_sum)
      if not str_string:
        break
      ser.write(bytes().fromhex(hex_string))
      while (1):
        if ser.in_waiting > 0:
          break
      read_string = ord(ser.read(ser.in_waiting))
      ser.reset_input_buffer();
      #print (read_string)
      if int(read_string) != str_sum:
        print ("error @ line", line_count)
        print ("sum should be", str_sum)
        print ("get number is", int(read_string))
        ser.close()
        exit
      if not str_string:
        break
      byte_count += 32
    line_count += 1
    if ((line_count %10) == 0):
      print ("finish line", line_count)
  file.close()
  
  ser.close()
  print ("end here")

except Exception as e:
  print ("error:", e)
  ser.close()
  

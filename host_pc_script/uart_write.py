import serial
import binascii
import time
import struct
import re

total_line_number = 240
write_byte_each_line = 640 # 320*2, each pixel occupy 2 bytes
file_name = "fig3.bin"

def caculate_sum(data):
    sum = 0
    data_array = re.findall(r'.{2}', data)
#    print (data_array)
    for one_byte in data_array:
      sum += int(one_byte, 16)
    sum_byte = sum%256
    return sum_byte

try:
    file = open(file_name, 'rb')
    byte_count = 0
    line_count = 0
    
    portx="COM7"
#    bps=9600
    bps=74880
    timex=5
    ser=serial.Serial(portx,bps,timeout=timex)
    print ("open serial ok")

    while line_count < total_line_number:
        byte_count = 0
        while byte_count < write_byte_each_line:
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
                print ("error")
                print ("sum should be", str_sum)
                print ("get number is", int(read_string))
            if not str_string:
                break
            byte_count += 32
        line_count += 1
        print ("finish line", line_count)
    ser.close()
    print ("end here")

except Exception as e:
    print ("error:", e)
    ser.close()
    

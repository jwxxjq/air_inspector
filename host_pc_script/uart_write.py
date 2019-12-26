import serial
import binascii
import time
import struct
try:
    file = open('fig1.bin', 'rb')
    byte_count = 0
    
    portx="COM7"
    bps=9600
    timex=5
    ser=serial.Serial(portx,bps,timeout=timex)
    print ("open serial ok")

    while 1:
        str_string = file.read(32);
        hex_string = str(binascii.b2a_hex(str_string))[2:-1]
        print (hex_string);
        if not str_string:
            break
        ser.write(bytes().fromhex(hex_string))
        while (1):
            if ser.in_waiting > 0:
                break
        read_string = ord(ser.read(ser.in_waiting))
        ser.reset_input_buffer();
        print (read_string)
        if int(read_string) != 240:
            print ("error")
        if not str_string:
            break
        
    ser.close()

except Exception as e:
    print ("error:", e)
    

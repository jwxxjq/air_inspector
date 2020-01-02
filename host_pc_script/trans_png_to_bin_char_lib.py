import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import struct

char_lib_high = 200
char_lib_width = 96

file_name = "char_lib"

fig1 = mpimg.imread(file_name+".png")

file = open(file_name+".bin", 'wb')
for x in range(0,char_lib_high,1):
  for y in range(0,char_lib_width,1):
    char_lib_value = int(fig1[x,y,0]*31); # range 0~31 to reduce the computational complexity of arduino
    byte = struct.pack('B', int(char_lib_value))
    file.write(byte)

file.close()

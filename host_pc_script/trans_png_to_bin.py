import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import struct

#include "..\air_inspector\define.h"

file_name = "fig1"

fig1 = mpimg.imread(file_name+".png")

#plt.imshow(fig1) # 显示图片
#plt.axis('off') # 不显示坐标轴
#plt.show()

#print (fig1.shape)
#print (fig1[0,0,0]*31) #R
#print (fig1[0,0,1]*63) #G
#print (fig1[0,0,2]*31) #B

#print (fig1[0,319,0])
#print (fig1[0,319,1])
#print (fig1[0,319,2])

#def r8b8g8_2_r5g6b5(rgb):
#  full_range_value = rgb*255;
#  int_full_range_value = 
  #return np.dot(rgb[...,:3], [31*64*32, 63*32, 31])

def rgb_hex565(red, green, blue):
    # take in the red, green and blue values (0-255) as 8 bit values and then combine
    # and shift them to make them a 16 bit hex value in 565 format. 
    return ( (int(red / 255 * 31) << 11) | (int(green / 255 * 63) << 5) | (int(blue / 255 * 31)) )

#fig1_r5g6b5 = r8b8g8_2_r5g6b5(fig1)

#print (fig1_r5g6b5.shape)
#print (fig1_r5g6b5[0,0])
#print (fig1_r5g6b5[0,319])

file = open(file_name+".bin", 'wb')
for x in range(0,240,1):
  for y in range(0,320,1):
    red_value = int(fig1[x,y,0]*255);
    green_value = int(fig1[x,y,1]*255);
    blue_value = int(fig1[x,y,2]*255);
    pixel_r5g6b5 = rgb_hex565(red_value, green_value, blue_value)
#    if (x == 0 and y == 0):
#      print (pixel_r5g6b5)
#      print (red_value)
#      print (green_value)
#      print (blue_value)
    byte1 = struct.pack('B', int(pixel_r5g6b5%256))
    file.write(byte1)
    byte2 = struct.pack('B', int(pixel_r5g6b5/256))
    file.write(byte2)

    

file.close()

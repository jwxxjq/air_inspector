import matplotlib.pyplot as plt # plt 用于显示图片
import matplotlib.image as mpimg # mpimg 用于读取图片
import numpy as np
import struct

file_name = "fig1"

fig1 = mpimg.imread(file_name+".png")

#plt.imshow(fig1) # 显示图片
#plt.axis('off') # 不显示坐标轴
#plt.show()

#print (fig1.shape)
#print (fig1[0,1,0]) #R
#print (fig1[0,1,1]) #G
#print (fig1[0,1,2]) #B

#print (fig1[0,319,0])
#print (fig1[0,319,1])
#print (fig1[0,319,2])

def r8b8g8_2_r5g6b5(rgb):
  return np.dot(rgb[...,:3], [31*64*32, 63*32, 31])
  
fig1_r5g6b5 = r8b8g8_2_r5g6b5(fig1)

print (fig1_r5g6b5.shape)
print (fig1_r5g6b5[0,0])
print (fig1_r5g6b5[0,319])

file = open(file_name+".bin", 'wb')
for x in range(0,240,1):
  for y in range(0,320,1):
#    print (fig1_r5g6b5[x,y])
#    int_value = int(fig1_r5g6b5[x,y])
#    print (int_value)
    byte1 = struct.pack('B', int(fig1_r5g6b5[x,y]/256))
    file.write(byte1)
    byte2 = struct.pack('B', int(fig1_r5g6b5[x,y]%256))
    file.write(byte2)

file.close()

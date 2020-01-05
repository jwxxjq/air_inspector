import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import struct
import cv2

#fig defines
max_fig_number = 1
fig_high = 240
fig_width = 320
display_item_num = 7
display_line_high = 18
display_item_alpha = 0.4
display_item_width_1 = 50 #for items with two lines, the width of first line
display_item_width_2 = 60 #for items only one line (temp & rh)
display_item_width_num = 90 #for item number line width

char_lib_name = "char_lib.png"
char_lib_each_item_high = 20
char_lib_color_green = (0, 255, 0)
char_lib_color_red   = (255, 0, 0)
char_lib_color_blue  = (0, 0, 255)
char_lib_color_yellow= (255, 255, 0)
char_lib_color_white = (255, 255, 255)
char_lib_color_black = (0, 0, 0)
char_lib_offset_x = 1
char_lib_offset_y = 2

char_lib_number_start_x = 50
char_lib_number_start_y = 0
char_lib_number_blod_start_x = 62
char_lib_number_blod_start_y = 0
char_lib_number_point_y = 11
char_lib_number_offset_x = 5
char_lib_number_offset_y = 2
char_lib_number_width = 10
char_lib_number_high = char_lib_each_item_high


#display position for different figs
position_x = np.zeros((max_fig_number, display_item_num), np.int16)
position_y = np.zeros((max_fig_number, display_item_num), np.int16)

#display position default is on the left
for fig_number in range(0, max_fig_number, 1):
  for display_item_count in range(0, display_item_num, 1):
    position_x[fig_number, display_item_count] = 0
    if (display_item_count == 0):
      position_y[fig_number, display_item_count] = 0
    elif (display_item_count != 6):
      position_y[fig_number, display_item_count] = (display_item_count)*2*display_line_high
    else:
      position_y[fig_number, display_item_count] = position_y[fig_number, display_item_count - 1] + display_line_high
#    print ("x = ", position_x[fig_number, display_item_count])
#    print ("y = ", position_y[fig_number, display_item_count])


def draw_white(fig, start_x, start_y, width, high):
  result = fig
  for x in range(start_x, start_x+width, 1):
    for y in range(start_y, start_y+high, 1):
      if ((int(x+y) > int(start_x+start_y+1) )        and
          (int(y-x) > int(start_y-start_x+2-width))   and
          (int(x-y) > int(start_x-start_y+2-high))    and
          (int(x+y) < int(start_x+start_y+high+width-3)) ):
        if (result[y, x, 0] != 0):
          print ("overlapped @ ", y, x)
        result[y, x] = (255, 255, 255)
#        result[y, x, 1] = 255
#        result[y, x, 2] = 255
  return result
  
def get_rgb(fig):
  result = np.zeros((fig.shape[0], fig.shape[1], 3), np.uint8)
  for y in range(0, fig.shape[0], 1):
    for x in range(0, fig.shape[1], 1):
      result[y,x,0] = np.uint8(fig[y,x,0] * 255);
      result[y,x,1] = np.uint8(fig[y,x,1] * 255);
      result[y,x,2] = np.uint8(fig[y,x,2] * 255);
  return result

def fill_remain_area_with_orginal(fig_display_area, fig):
  result = fig_display_area
  for y in range(0, fig_high, 1):
    for x in range(0, fig_width, 1):
      if (fig_display_area[y, x, 0] == 0):
        result[y, x] = fig[y, x]
  return result

def mix_color_for_pixel(orginal_pixel, char_lib_pixel, char_lib_color):
  result = orginal_pixel
  if (char_lib_pixel[0] == 255): #if is total white, then no need for mix orginal pixel color
    result = char_lib_color
  else:
    char_lib_alpha = char_lib_pixel[0] / 255 #RBG is equal, any channel is ok
    for channel in range (0, orginal_pixel.size, 1):
      result[channel] = np.uint8(result[channel] * (1 - char_lib_alpha) + char_lib_color[channel] * char_lib_alpha)
  return result

def fill_in_display_item_name(fig, char_lib, char_lib_color):
  result = fig
  for display_item_count in range(0, display_item_num - 2, 1): # temp & rh no need
    for y in range (0, display_line_high - char_lib_offset_y, 1):
      for x in range (0, display_item_width_1 - char_lib_offset_x, 1):
        char_lib_y = char_lib_each_item_high * display_item_count + y
        char_lib_x = x
        orginal_fig_y = display_line_high*display_item_count * 2 + y + char_lib_offset_y
        orginal_fig_x = x + char_lib_offset_x
        result[orginal_fig_y, orginal_fig_x] = mix_color_for_pixel(result[orginal_fig_y, orginal_fig_x], char_lib[char_lib_y, char_lib_x], char_lib_color)
  return result

#def fill_in_display_item_uint(fig, char_lib, char_lib_color):
#  result = fig
#  for display_item_count in range(0, display_item_num, 1):
#    if (display_item_count == 0) # CO2 use "ppm"
#      uint_
        

def fill_in_test_number(fig, char_lib):
  number = 1
  result = fig
  display_color = char_lib_color_green
  is_blod = 0
  for display_item_count in range(0, display_item_num - 2, 1): #not for temp & rh
    if (display_item_count % 2 == 0):
      is_blod = 0
    else:
      is_blod = 1

    if (display_item_count % 3 == 0):
      display_color = char_lib_color_green
    elif (display_item_count % 3 == 1):
      display_color = char_lib_color_red    
    else:
      display_color = char_lib_color_yellow
    for display_number_count in range(0, 4, 1):
      for y in range (0, display_line_high - char_lib_number_offset_y, 1):
        for x in range (0, char_lib_number_width, 1):
          char_lib_y = char_lib_number_start_y + number * char_lib_number_high + y
          if (is_blod):
            char_lib_x = char_lib_number_blod_start_x + x
          else:
            char_lib_x = char_lib_number_start_x + x
          orginal_fig_y = display_line_high * (display_item_count * 2 + 1) + y + char_lib_number_offset_y
          orginal_fig_x = x + char_lib_number_offset_x + char_lib_number_width * display_number_count
          result[orginal_fig_y, orginal_fig_x] = mix_color_for_pixel(result[orginal_fig_y, orginal_fig_x], char_lib[char_lib_y, char_lib_x], display_color)
      number = (number + 1) % 10

  #for temp
  display_color = char_lib_color_yellow
  is_blod = 1
  display_item_count = 5
  for display_number_count in range(0, 3, 1):
    for y in range (0, display_line_high - char_lib_number_offset_y, 1):
      for x in range (0, char_lib_number_width, 1):
        char_lib_y = char_lib_number_start_y + number * char_lib_number_high + y
        if (is_blod):
          char_lib_x = char_lib_number_blod_start_x + x
        else:
          char_lib_x = char_lib_number_start_x + x
        orginal_fig_y = display_line_high * (display_item_count * 2) + y + char_lib_number_offset_y
        orginal_fig_x = x + char_lib_number_offset_x + char_lib_number_width * display_number_count
        if (display_number_count == 2):
          orginal_fig_x += 3
        result[orginal_fig_y, orginal_fig_x] = mix_color_for_pixel(result[orginal_fig_y, orginal_fig_x], char_lib[char_lib_y, char_lib_x], display_color)
    number = (number + 1) % 10
  #fill the point
  point_x = char_lib_number_offset_x + char_lib_number_width * 2
  point_y = display_line_high * (5 * 2) + char_lib_number_offset_y + char_lib_number_point_y
  result[point_y, point_x] = display_color
  result[point_y+1, point_x] = display_color
  result[point_y, point_x+1] = display_color
  result[point_y+1, point_x+1] = display_color

  #for rh
  display_color = char_lib_color_green
  is_blod = 0
  display_item_count = 5
  for display_number_count in range(0, 3, 1):
    for y in range (0, display_line_high - char_lib_number_offset_y, 1):
      for x in range (0, char_lib_number_width, 1):
        char_lib_y = char_lib_number_start_y + number * char_lib_number_high + y
        if (is_blod):
          char_lib_x = char_lib_number_blod_start_x + x
        else:
          char_lib_x = char_lib_number_start_x + x
        orginal_fig_y = display_line_high * (display_item_count * 2 + 1) + y + char_lib_number_offset_y
        orginal_fig_x = x + char_lib_number_offset_x + char_lib_number_width * display_number_count
        if (display_number_count == 2):
          orginal_fig_x += 3
        result[orginal_fig_y, orginal_fig_x] = mix_color_for_pixel(result[orginal_fig_y, orginal_fig_x], char_lib[char_lib_y, char_lib_x], display_color)
    number = (number + 1) % 10
  #fill the point
  point_x = char_lib_number_offset_x + char_lib_number_width * 2
  point_y = display_line_high * (display_item_count * 2 + 1) + char_lib_number_offset_y + char_lib_number_point_y
  result[point_y, point_x] = display_color
  result[point_y+1, point_x] = display_color
  result[point_y, point_x+1] = display_color
  result[point_y+1, point_x+1] = display_color
  return result


#read char_lib
print ("reading char_lib")
char_lib = mpimg.imread(char_lib_name)
char_lib = get_rgb(char_lib)

for fig_number in range(0, max_fig_number, 1):
  print ("start on fig ", fig_number)
  
  #read .png into np
  fig = mpimg.imread("fig"+str(fig_number)+".png")
  fig = get_rgb(fig)

  fig_display_area = np.zeros((fig_high, fig_width, 3), np.uint8)
#  fig_display_area = draw_white(fig_display_area, 0, 34, 50, 17)

  for display_item_count in range(0, display_item_num, 1):
    if (display_item_count < 5): #first 5 items need two lines, second is 90 width
      fig_display_area = draw_white(fig_display_area, position_x[fig_number, display_item_count], position_y[fig_number, display_item_count], display_item_width_1, display_line_high)
      fig_display_area = draw_white(fig_display_area, position_x[fig_number, display_item_count], (position_y[fig_number, display_item_count] + display_line_high), display_item_width_num, display_line_high)
    else:
      fig_display_area = draw_white(fig_display_area, position_x[fig_number, display_item_count], position_y[fig_number, display_item_count], display_item_width_2, display_line_high)

  fig_display_area = fill_remain_area_with_orginal(fig_display_area, fig)

  dst = cv2.addWeighted(fig, 1-display_item_alpha, fig_display_area, display_item_alpha, 0)

  dst_with_item_name = fill_in_display_item_name(dst, char_lib, char_lib_color_black)

  #dst_with_item = fill_in_display_item_unit(dst_with_item_name, char_lib, char_lib_color_blue)
  dst_with_item = dst_with_item_name
  dst_with_test_number = fill_in_test_number(dst_with_item, char_lib)

  plt.imshow(dst_with_test_number)
  plt.axis('off')
  plt.show()

import numpy as np
import cv2 as cv

from HSI import *

#Set up filenames
raw_filename = "/media/ryan/HDD/Perception/TO8_SVN/TO8-docs/HS_Images/8_12_2015/Water008_VNIR.raw"
hdr_filename = "/media/ryan/HDD/Perception/TO8_SVN/TO8-docs/HS_Images/8_12_2015/Water008_VNIR.hdr"

######################### hsimage testing ###################################3####3

#test loading
test_img_1 = hsimage.hsimage(hdr_filename,raw_filename)
test_img_2 = hsimage.hsimage()

test_img_2.load(hdr_filename,raw_filename)

#test static has-spectrometer-file function
test_img_1.hasSpecFiles(hdr_filename)
test_img_2.hasSpecFiles(hdr_filename)

#test get pixel vector
val_1 = test_img_1.getPixelSpectra(20,20)
test_img_2.getPixelSpectra(20,20)

print(val_1)
arr_1 = np.array(val_1)
print(arr_1)

test_img_1.getPixelTransferFunction(20,20)
test_img_2.getPixelTransferFunction(20,20)

#test get single band image
img550_1 = test_img_1.getBand(550)
img550_2 = test_img_2.getBand(550)

print(np.array_equal(img550_1,img550_2))

#cv.imshow("img550_1",img550_1)
#cv.imshow("img550_2",img550_2)
#cv.waitKey(1000)
#cv.destroyAllWindows()

#test get band range images
range_1 = test_img_1.getRange(630,640)
range_2 = test_img_2.getRange(630,640)

for img_1, img_2 in list(zip(range_1,range_2)):
 #   cv.imshow("img_1",img_1)
  #  cv.imshow("img_2",img_2)
    print(np.array_equal(img_1,img_2))
   # cv.waitKey(1000)

#cv.destroyAllWindows()

#test get band set(non-continuous)
test_range = [500,450,600,700,800,900]

#range_vec = double_vector.extend(arg for arg in test_range)

range_1 = test_img_1.getSet(test_range)
range_2 = test_img_2.getSet(test_range)

for img_1, img_2 in list(zip(range_1,range_2)):
 #   cv.imshow("img_1",img_1)
 #   cv.imshow("img_2",img_2)
    print(np.array_equal(img_1,img_2))
 #   cv.waitKey(1000)
##########################################################################################

############################### classified_hsimage testing ###############################
class_filename = "/media/ryan/HDD/Perception/TO8_SVN/TO8-docs/HS_Images/8_12_2015/Water008_VNIR_CLASS.png"
class_hdr_filename = "/media/ryan/HDD/Perception/TO8_SVN/TO8-docs/HS_Images/8_12_2015/Water008_VNIR_CLASS.hdr"

#testing loaders
cimg = classified_hsimage(raw_filename,hdr_filename,class_filename,class_hdr_filename)

cimg2 = cimg

#testing getClassSpectra
waterVec = cimg.getClassSpectra("Water",100)
print(waterVec)

waterArray = np.array(waterVec)

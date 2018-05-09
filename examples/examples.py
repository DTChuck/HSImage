import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import HSI

#The chosen files are the test files in the GitHub repository. This script can be run within the folder containing the images without editing.

directory = "../test/HSI_test_images/"
filename = "HokieStoneWindowSny_exp20000_VNIR"
raw_ext = ".raw"
hdr_ext = ".hdr"
lif_ext = ".lif"

VIS_spec_file = "VIS_HokieStoneWindowSny.txt"
NIR_spec_file = "NIR_HokieStoneWindowSny.txt"

#load hsimage object
img = HSI.hsimage(directory + filename + hdr_ext,directory + filename + raw_ext,[directory + VIS_spec_file,directory + NIR_spec_file])

#load labelfile object
lif = HSI.labelfile(directory + filename+lif_ext)

#load classified_hsimage object
cl_img = HSI.classified_hsimage(directory + filename+raw_ext,directory + filename+hdr_ext,directory + filename+lif_ext)

#Acquire the RGB image from the labelfile
rgb_img = lif.getRGBImage()

#Choose a pixel to view
pixel = (200,300)

#Get the chosen spectra
pix = img.getPixelSpectra(pixel[0],pixel[1])

#Plot the chosen point on the RGB image
cv.circle(rgb_img,pixel,2,(0,0,255),2)

#Display the RGB image
#The images are saved by the camera sideways, so a rotation is applied for ease of viewing
rgb_img = cv.transpose(rgb_img)
rgb_img = cv.flip(rgb_img,0)
cv.imshow("RGB",rgb_img)

#Create matplotlib subplots for spectral data
fig, axs = plt.subplots(3,1,sharex=True)
fig.subplots_adjust(hspace=0.3,left=0.15)
labely = -0.13

#Display the spectrum of the measured point
wavelengths = img.getWavelengths()
axs[0].plot(wavelengths,pix)
axs[0].set_title("Spectrum of point {0}".format(pixel))
axs[0].set_ylabel("Intensity")
axs[0].yaxis.set_label_coords(labely, 0.5)

#Display the measured ambient light
ambient = img.getAmbientIntensities()
axs[1].plot(wavelengths,ambient)
axs[1].set_title("Ambient Light Spectra")
axs[1].set_ylabel("Intensity")
axs[1].yaxis.set_label_coords(labely, 0.5)

#Display the spectral reflectance (pix/ambient) 
refl = np.array(pix) / np.array(ambient)
axs[2].plot(wavelengths,refl)
axs[2].set_title("Reflectance at pixel {0}".format(pixel))
axs[2].set_xlabel("Wavelength (nm)")
axs[2].set_ylabel("Percent Reflected (%)")
axs[2].yaxis.set_label_coords(labely, 0.5)

cv.imwrite("Marked RGB.png",rgb_img)
plt.savefig("Pixel Spectral Data")

#Show all plots
cv.waitKey(2000)
plt.show()

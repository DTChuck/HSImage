# HSImage #

Interface Library for ENVI-BIL Hyperspectral Images
(C++/Python)

This library is designed to allow open-source experimentation with ENVI-BIL hyperspectral images. The goal of this software is to promote the study of hyperspectral images in the academic computer vision research world. This software provides both a C++ and Python interface for ease of use. An associated project has resulted in a publicly available library of images available at [https://osf.io/pd49t/](). These images are all compatible and intended to be used with this software interface library.

## Installation ##
This package requires the boost crossplatfrom C++ libraries and OpenCV 3.x for the C++ library. To use the package in Python 3.x, OpenCV 3.x is required as a Python module.
After the prerequisites are installed, simply install with pip:

`pip install HSI`

This will install the package to the site-packages folder in your current Python environment.

If you want to install from this repository, simply clone and install using setuptools:

`python setup.py install`


## Testing ##
Cloning the repository will include the test suite which can be run using setuptools:

`python setup.py pytest`

## Documentation ## 
Full documentation is available at [https://dtchuck.github.io/HSImage/]()

## Example Usage ##
Loading a image, converting to a pixel vector array and normalizing by the ambient intensity to create spectral reflectance data.

```python
img = HSI.hsimage(header_filename,raw_filename)

band = img.getBand(550)
pix_array = img.getPixelArray()
amb = img.getAmbientIntensities()
shape = img.getShape()

image_vector = np.reshape(np.array(pix_array), (shape[0] * shape[1], -1))
image_vector = image_vector / np.array(amb)
```

Loading the labelfile and retrieving the class overlay image for viewing.
```python
lif = HSI.labelfile(lif_filename)

overlay_image = lif.getOverlayImage()
cv2.imshow("Overlay",overlay_image)
cv2.waitKey(0)
```

## Support ##
This software is under active development. If you wish to contribute to this software, contact Ryan Brown at **brownrc@vt.edu**. Issues can be reported here using the GitHub issue tracker.

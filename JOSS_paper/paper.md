---
title: 'HSImage: A Python and C++ library to allow interaction with ENVI-BIL hyperspectral images'
tags:
- Hyperspectral
- Camera
- ENVI
- Classified
- Labeled
- Image
authors:
- name: Ryan C Brown
  orcid: 0000-0002-3832-775X
  affiliation: 1
- name: Joshua Moser
  orcid: 
  affiliation: 1
affiliations:
- name: Virginia Tech
  index: 1
date: 06 March 2018
bibliography: paper.bib
---

# Summary
Hyperspectral images have been used in agricultural and geoscience studies since the 1980s, mostly from a satellite or aerial platform.[@Rinker1990] The benefit of hyperspectral images is also their main drawback in that they are very high dimensional data, usually on the order of 100 times greater spectral resolution than a traditional RGB camera. A forthcoming study has attempted to leverage hyperspectral imaging to improve classification accuracy in ground level sensor applications.

This library is designed to allow open-source experimentation with ENVI-BIL type hyperspectral images. The goal of this software is to promote the study of hyperspectral images in computer vision research by providing an open source, easy to use interface to these hyperspectral images. This software provides both a C++ and Python interface for ease of use. An associated project has resulted in a publicly available library of images available at [https://osf.io/pd49t/](). These images are all compatible and intended to be used with this software interface library. Similar libraries are HyperSpy, a Python multidiemsional data analysis library, and hsdar, an R packaged for managing and analyzing hyperspectral data.[@francisco_de_la_pena_2017_583693,@hsdar]. There are several non-free software packages that can also analyze hyperspectral data.

The differences between HSImage and these other packages are in the focus on classification, as each hyperspectral image can be paired with a CSAIL-LabelMe style label file to provide inherent classification accuracy, and the ability to par hyperspectral images with external spectrometer readings. Also, this software is built to provide functionality in both Python, for ease of use, and C++ for speed in processing. 

HSImage can be found at [https://github.com/DTChuck/HSImage]() and documentation can be found at [https://dtchuck.github.io/HSImage/](). A sister project to this is a ground-level classified hyperspectral image dataset, the Labeled Hyperspectral Image Database (LHID)[@Brown_Moser_2018]. This dataset is currently being used, along with the HSImage software to enable several research projects into the possibility of leveraging hyperspectral data for ground level sensing, particularly in the field of autonomous vehicle navigation.

An abbreviated list of the capabilities of HSImage is:

* Open and Load hyperspectral images
* Collect and gather specific slices of the image array
* Display data in OpenCV compatible images
* Filter hyperspectral pixels by labeled class
* Transform raw emittance data using spectrometers into spectral reflectance
* Allow native processing of hyperspectral data using Numpy or OpenCV

A short example of the software can be found on the GitHub repository. The software was written using C++11 and the Python module was created using Pybind11, and OpenCV was included for image viewing and matrix operations.[@itseez2015opencv,;@pybind11]

# References 

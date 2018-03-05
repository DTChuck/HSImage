import numpy as np
import cv2 as cv

import HSI

import os
import pytest

FIXTURE_DIR = os.path.join(os.path.dirname(os.path.realpath(__file__)),'HSI_test_images',)

raw_file = os.path.join(FIXTURE_DIR,'HokieStoneWindowSny_exp20000_VNIR.raw')
hdr_file = os.path.join(FIXTURE_DIR,'HokieStoneWindowSny_exp20000_VNIR.hdr')
lif_file = os.path.join(FIXTURE_DIR,'HokieStoneWindowSny_exp20000_VNIR.lif')
vis_spec_file = os.path.join(FIXTURE_DIR,'VIS_HokieStoneWindowSny.txt')
nir_spec_file = os.path.join(FIXTURE_DIR,'NIR_HokieStoneWindowSny.txt')

ALL_FILES = pytest.mark.datafiles(
  raw_file,
  hdr_file,
  lif_file,
  vis_spec_file,
  nir_spec_file,
  )

@pytest.mark.incremental
@ALL_FILES
class TestHSI(object):
  image = HSI.hsimage(hdr_file,raw_file)
  def test_init(self):
    img = HSI.hsimage()
    pass

  def test_no_spec_loading(self):
    img = HSI.hsimage(hdr_file,raw_file,[vis_spec_file,nir_spec_file])
    pass

  def test_hsi_loading(self):
    img = HSI.hsimage(hdr_file,raw_file)
    self.image = img
    pass

  def test_hsi_has_spec_files(self):
    assert self.image.hasSpecFiles(hdr_file) == True

  def test_hsi_getPixelSpectra(self):
    data = self.image.getPixelSpectra(20,20)
    assert np.mean(data) == 2674.9738095238095 and np.sum(data) == 1123489
  
  def test_hsi_getWavelengths(self):
    assert self.image.getWavelengths()[-1] == 1005.8599853515625

  def test_hsi_getAmbientIntensities(self):
    assert self.image.getAmbientIntensities()[-1] == 1920.47802734375

  def test_hsi_getRange(self):
    data = self.image.getRange(550,560)
    assert np.sum(data) == 5796849308 and np.mean(data) == 2581.425591378696

  def test_hsi_getSet(self):
    data = self.image.getSet([550,560])
    assert np.sum(data) == 1645185148 and np.mean(data) == 2564.1913154613467

  def test_hsi_getBand(self):
    data = self.image.getBand(550)
    assert np.sum(data) == 829177542 and np.mean(data) == 2584.718023690773

  def test_hsi_getPixelArray(self):
    data = self.image.getPixelArray()
    assert len(data) == 134736000

  def test_hsi_getShape(self):
    assert self.image.getShape() == (401,800,420)

@pytest.mark.incremental
@ALL_FILES
class TestClassifiedHSI(object):
  cl_image = HSI.classified_hsimage(raw_file,hdr_file,lif_file)

  def test_cl_hsi_init(self):
    img = HSI.classified_hsimage()
    pass

  def test_cl_hsi_loading(self):
    img = HSI.classified_hsimage(raw_file,hdr_file,lif_file)
    pass
  
  def test_cl_hsi_hsimage_access(self):
    assert self.cl_image.hsimage.getShape() == (401,800,420)
    
  def test_cl_hsi_classes_access(self):
    assert 'HokieStone' in self.cl_image.classes.keys()

  def test_cl_hsi_getClassSpectra(self):
    assert len(self.cl_image.getClassSpectra('HokieStone',1)[0])

  def test_cl_hsi_getClassTF(self):
    assert len(self.cl_image.getClassSpectra('HokieStone',1)[0])

@pytest.mark.incremental
@ALL_FILES
class TestLabelfile(object):
  labelfile = HSI.labelfile(lif_file)

  def test_label_init(self):
    lif = HSI.labelfile()
    pass

  def test_label_loading(self):
    lif = HSI.labelfile(lif_file)
    pass
  
  def test_label_getRGBImage(self):
    assert np.shape(self.labelfile.getRGBImage()) == (401,800,3)

  def test_label_getLabelImage(self):
    assert np.shape(self.labelfile.getLabelImage()) == (401,800,3)

  def test_label_getOverlayImage(self):
    assert np.shape(self.labelfile.getOverlayImage()) == (401,800,3)

  def test_label_getClassInfo(self):
    assert 'HokieStone' in self.labelfile.getClassInfo()[0]

  def test_label_getColorInfo(self):
    assert 'HokieStone' in self.labelfile.getColorInfo()[0]

  

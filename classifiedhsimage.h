#ifndef CLASSIFIEDHSIMAGE_H
#define CLASSIFIEDHSIMAGE_H

#include <map>
#include <utility>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "hsimage.h"
#include "hsimage_global.h"
#include "colormap.h"

typedef std::pair<std::string,cv::Vec3b> classColor;

class HSIMAGESHARED_EXPORT ClassifiedHSImage
{
public:
    ClassifiedHSImage();
    ClassifiedHSImage(HSImage hsimage, cv::Mat labels, std::vector<classColor> c_names);
    ClassifiedHSImage(std::string raw_file, std::string hdr_file, std::string label_file, std::string class_hdr_file);

    HSImage image;
    cv::Mat label;
    std::map<std::string, cv::Vec3b> class_names;

    void load(HSImage hsimage, cv::Mat labels, std::vector<classColor> c_names);
    std::vector<std::vector<u_int16_t> > getClassSpectra(std::string class_label, unsigned int num_spectra = 100);

    std::vector<double> getAvgClassTF(std::string class_label);
    std::vector<std::vector<double> > getClassTF(std::string class_label, unsigned int num_spectra = 100);

    void setSpectraClass(int row, int col, std::string class_label);
    void setSpectraClass(std::vector<std::pair< int, int > > pixel_locs, std::string class_label);
    void setImageClass(cv::Mat class_labels, std::vector<classColor> class_list);
};

#endif // CLASSIFIEDHSIMAGE_H

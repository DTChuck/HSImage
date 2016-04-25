#ifndef HSIMAGE_H
#define HSIMAGE_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "hsimage_global.h"

class HSIMAGESHARED_EXPORT HSImage
{

public:
    HSImage();
    HSImage(std::string header_location, std::string image_location);
    HSImage(std::string header_location, std::string image_location, std::vector<std::string> spec_location);
    HSImage(const HSImage& other);
    HSImage& operator=( const HSImage& other);

    void load(std::string header_location, std::string image_location);
    void load(std::string header_location, std::string image_location, std::vector<std::string> spec_locations);

    void loadHeader(std::string header_location);
    void loadRawImage(std::string image_location);
    void loadSpectrometerData(std::vector<std::string> filenames);

    static bool hasSpecFiles(std::string header_location);
    void addSpecDataToHeader(std::vector<std::string> filenames);

    std::vector<u_int16_t> getPixelSpectra(int row, int col);
    std::vector<u_int16_t> getNormalizedPixelSpectra(int row, int col);
    std::vector<double> getPixelTransferFunction(int row, int col);

    std::vector<cv::Mat> getRange(const float lower_wavelength, const float upper_wavelength);
    std::vector<cv::Mat> getSet(const std::vector<float> wavelength_set);
    cv::Mat operator[] (const float wavelength);

    //image metadata
    std::string acquisition_date;
    int tint;
    int samples;
    int lines;
    int bands;
    int fps;
    int binning[2];
    std::vector<float> wavelengths;
    std::vector<float> fwhm;
    std::vector<float> ambient_intensities;
    bool has_spec_data;

    //image file location
    std::string img_file;
    std::string hdr_file;
    std::string nir_spec_file;
    std::string vis_spec_file;

private:
    std::unordered_map<float,uchar*> image_map;
    std::unique_ptr<u_int16_t[]> image_data;
    std::unique_ptr<u_int16_t[]> pixel_data;

    std::string createRelativeSpecFilepath(std::string abs_spec_filepath);
    std::string createAbsoluteSpecFilepath(std::string rel_spec_filepath);

    template <typename T>
    static T closest(std::vector<T> const& vec, T value) {
     auto const it = std::lower_bound(vec.begin(), vec.end(), value);
     if (it == vec.end()) { return -1; }
     return *it;
    }

    static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            if(!item.empty())
            {
                elems.push_back(item);
            }
        }
        return elems;
    }

    static std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

};

#endif // HSIMAGE_H

#ifndef LABELFILE_H
#define LABELFILE_H

//Provides a loading and access interface for .lif labelMe label files

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>

#include "jsoncpp/json/json.h"

/*!
 * \brief The classColor typedef creates a simple interface for pairing a class name with a specific OpenCV color.
 */
typedef std::pair<std::string,cv::Vec3b> classColor;

//converters for base64 image data
typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<boost::archive::iterators::remove_whitespace<std::string::const_iterator> > ,8,6> it_binary_t;
typedef boost::archive::iterators::insert_linebreaks<boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<std::string::const_iterator,6,8> >, 72 > it_base64_t;



class LabeledObject
{
public:
    LabeledObject();

    void setPolygon(std::vector<cv::Point> new_polygon);
    void setName(std::string new_name);
    void setColor(cv::Vec3b new_color);

    classColor getInfo();
    std::string getName();
    cv::Vec3b getColor();
    std::vector<cv::Point> getPolygon();

private:
    std::vector<cv::Point> polygon;
    std::string name;
    cv::Vec3b color;
};

class LabelFile
{
public:
    LabelFile();
    LabelFile(std::string filename);

    void loadFile(std::string filename);

    cv::Mat getRGBImage();
    cv::Mat getLabelImage();
    cv::Mat getViewingImage();
    std::vector<classColor> getClassInfo();

private:
    void makeObject();
    cv::Vec3b getRandomColor();
    cv::Mat createLabelImage();
    cv::Mat createRGBImage(Json::Value img_data);

    std::vector<LabeledObject> obj_vector;
    cv::Mat rgb_image;
    cv::Mat label_overlay;

    std::vector<classColor> class_info;
    std::map<std::string,cv::Vec3b> class_map;
};

#endif // LABELFILE_H

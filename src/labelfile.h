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

#include "pybind11_opencv_numpy/pybind11/pybind11.h"
#include "pybind11_opencv_numpy/pybind11/stl.h"
#include "pybind11_opencv_numpy/ndarray_converter.h"

#include "jsoncpp/json/json.h"

/*!
 * \brief The classColor typedef creates a simple interface for pairing a class name with a specific OpenCV color.
 */
typedef std::pair<std::string,cv::Vec3b> classColor;

//converters for base64 image data
typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<boost::archive::iterators::remove_whitespace<std::string::const_iterator> > ,8,6> it_binary_t;
typedef boost::archive::iterators::insert_linebreaks<boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<std::string::const_iterator,6,8> >, 72 > it_base64_t;


/*!
 * \brief The LabeledObject class represents one labeled object in an image with all the necessary information to identify it.
 * 
 * The LabeledObject class is used to store information about labeled image objects with all the information that is available about that object including its name, shape, and  class label color. This information is used to generate a pixel by pixel class representation in memory, but the information is stored on the hard drive as a polygon with corresponding information in a label (.lif) file.
 */
class LabeledObject
{
public:
    /*!
     * \brief Default Constructor for LabeledObject
     */
    LabeledObject();

    /*!
     * \brief Sets the object defining polygon to a new set of points
     * \param new_polygon Polygon to set new object bounds
     */
    void setPolygon(std::vector<cv::Point> new_polygon);
    /*!
     * \brief Sets name of object 
     * \param new_name Name to set as LabeledObject name
     */
    void setName(std::string new_name);
    /*!
     * \brief Sets color of object for label image
     * \param new_color Color to set as LabeledObject color
     */
    void setColor(cv::Vec3b new_color);

    /*!
     * \brief Gets class and name association info
     * \return classColor Vector containing class name and color association information
     */
    classColor getInfo();
    /*!
     * \brief Gets name of LabeledObject 
     * \return std::string Name of LabeledObject
     */
    std::string getName();
    /*!
     * \brief Gets color of LabeledObject
     * \return cv::Vec3b Class color of LabeledObject
     */
    cv::Vec3b getColor();
    /*!
     * \brief Gets shape polygon of LabeledObject
     */
    std::vector<cv::Point> getPolygon();

private:
    std::vector<cv::Point> polygon;
    std::string name;
    cv::Vec3b color;
};

/*!
 * \brief The LabelFile class allows an interface to read and process CSAIL/LabelMe style label (.lif) files. 
 *
 * The LabelFile class is used to open and extract labeled object information from a CSAIL/LabelMe style label file, usually with a .lif extension. This file contains all the information about labeled objects in a specific image, as well as an encoded .png image file containing the RGB image that was used to generate the labeling information. This interface allows you to gather all necessary information from the label file and apply it to classified hyperspectral images.
 */ 
class LabelFile
{
public:
    /*!
     * \brief Default constructor for LabelFile
     */
    LabelFile();
    /*!
     * \brief Constructor to build from file on disk
     */
    LabelFile(std::string filename);
    /*!
     * \brief Load file on disk into LabelFile instance
     */
    void loadFile(std::string filename);
    /*!
     * \brief Get RGB image of original scene the labels were generated from
     * \return cv::Mat Image of scene
     */
    cv::Mat getRGBImage();
    /*!
     * \brief Get image showing class colors and contours for whole label file
     * \return cv::Mat Image of class information
     */
    cv::Mat getLabelImage();
    /*!
     * \brief Get image showing RGB scene with class information overlaid in semi-transparent color. Designed for human viewing, not computer analysis.
     * \return cv::Mat Image showing class overlays on scene
     */
    cv::Mat getViewingImage();
    /*!
     * \brief Get vector of class name and color associations
     * \return std::vector<classColor> Vector of class information
     */
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

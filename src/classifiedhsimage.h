#ifndef CLASSIFIEDHSIMAGE_H
#define CLASSIFIEDHSIMAGE_H

#include <map>
#include <utility>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//#include <boost/python.hpp>

#include <Python.h>

#include "pybind11_opencv_numpy/pybind11/pybind11.h"
#include "pybind11_opencv_numpy/pybind11/stl.h"
#include "pybind11_opencv_numpy/ndarray_converter.h"

#include "hsimage.h"
#include "colormap.h"
#include "labelfile.h"


struct Vec3bCompare
{
    bool operator() (const cv::Vec3b& lhs, const cv::Vec3b& rhs) const
    {
        return cv::norm(lhs, CV_L2) < cv::norm(rhs, CV_L2);
    }
};

/*! \addtogroup cplus_module 
 * \{ 
 */

/*!
 * \brief The classColor typedef creates a simple interface for pairing a class name with a specific OpenCV color.
 */
typedef std::pair<std::string,cv::Vec3b> classColor;

/*!
 * \brief The ClassifiedHSImage class is the interface for interacting with classified hyperspectral images.
 *
 * The ClassifiedHSImage class contains an HSImage as its internal logistic engine and adds functionality aimed at classification and detection research. The class allows for retreival of pixels by class label and the setting of image classes by pixel or by setting an OpenCV cv::Mat object with the pixels colored according to the class label color.
 *
 * Does not current support internal saving of files. Under construction.
 */
class ClassifiedHSImage
{
public:
    ClassifiedHSImage();/*!< Base constructor */
    /*!
     * \brief Constructor with HSImage, cv::Mat containing labeling information and a vector of classColor pairs.
     * \param hsimage Initialized and loaded HSImage object.
     * \param labels cv::Mat containing pixels colored with the label colors specified in c_names.
     * \param c_names Vector of classColor pairs with class names and label colors.
     */
    ClassifiedHSImage(HSImage hsimage, cv::Mat labels, std::vector<classColor> c_names);
    /*!
     * \brief Constructor for loading ClassifiedHSImage from files.
     * \param raw_file .raw hyperspectral image file
     * \param hdr_file .hdr hyperspectral image file
     * \param label_file Image file containing label image
     * \param class_hdr_file .hdr file with class information about color/name pairs
     */
    ClassifiedHSImage(std::string raw_file, std::string hdr_file, std::string label_file, std::string class_hdr_file);
    /*!
     * \brief Constructor for loading ClassifiedHSImage from .lif labeling style
     * \param raw_file .raw hyperspectral image file
     * \param hdr_file .hdr hyperspectral image file
     * \param lif_file .lif LabelMe style label file
     */
    ClassifiedHSImage(std::string raw_file, std::string hdr_file, std::string lif_file);

    HSImage image;/*!< HSImage object containing hyperspectral data. */
    cv::Mat label;/*!< OpenCV cv::Mat image containing labeling data. */
    std::map<std::string, cv::Vec3b> class_names;/*!< std::map containing key value paring betweeen class name and color.*/
    std::map<cv::Vec3b,std::string, Vec3bCompare> class_keys;/*!< std::map containing key value paring betweeen color and class name. Inverse of class_names*/


    /*!
     * \brief Load ClassifiedHSImage from data in memory.
     * \param hsimage Loaded HSImage.
     * \param labels Created OpenCV cv::Mat labeling data image.
     * \param c_names Vector of classColor pairs.
     */
    void load(HSImage hsimage, cv::Mat labels, std::vector<classColor> c_names);
    /*!
     * \brief Returns up to num_spectra pixel spectra of the specified class.
     * \param class_label Class to return spectra from.
     * \param num_spectra Max number of returned spectra.
     * \return Vector of spectra vectors. Returns empty vector if class_label is not present in image.
     */
    std::vector<std::vector<u_int16_t> > getClassSpectra(std::string class_label, unsigned int num_spectra = 100);

    /*!
     * \brief Returns average transfer function for a specfied class
     * \param class_label Class to retrieve.
     * \return Vector containing specified average transfer function intensities.
     */
    std::vector<double> getAvgClassTF(std::string class_label);
    /*!
     * \brief Returns up to num_spectra pixel transfer function intensities of the specified class.
     * \param class_label Class to retrieve.
     * \param num_spectra Max number of returned transfer functions.
     * \return Vector of transfer function vectors.
     */
    std::vector<std::vector<double> > getClassTF(std::string class_label, unsigned int num_spectra = 100);

    /*!
     * \brief Returns class of given pixel
     * \param row Row of pixel
     * \param col Column of pixel
     * \return Class name at pixel(row,col)
     */
    std::string getPixelClass(int row, int col);

    /*!
     * \brief Set class for a pixel by row,column.
     * \param row Row of pixel to set
     * \param col Column of pixel to set
     * \param class_label Label of desired class for specified pixel.
     */
    void setSpectraClass(int row, int col, std::string class_label);
    /*!
     * \brief Set class for a group of pixels specified in a vector
     * \param pixel_locs Vector of std::pairs containing a (row,column) set of pixels to set
     * \param class_label Label of deisred class for specified pixel set
     */
    void setSpectraClass(std::vector<std::pair< int, int > > pixel_locs, std::string class_label);
    /*!
     * \brief Set class for whole image with cv::Mat
     * \param class_labels cv::Mat containing the labeling data for entire image. Must be same number of rows and columns as hyperspectral image.
     * \param class_list Vector of classColor objects containing name and color information for the classes in class_labels.
     */
    void setImageClass(cv::Mat class_labels, std::vector<classColor> class_list);
    /*!
     * \brief Get cv::Mat showing image classes
     * \return cv::Mat with color overlay for each class
     */
    cv::Mat getImageClass();
};

/*! \} */

#endif // CLASSIFIEDHSIMAGE_H

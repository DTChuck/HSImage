#include "pybind11_opencv_numpy/pybind11/pybind11.h"
#include "pybind11_opencv_numpy/pybind11/stl.h"
#include "pybind11_opencv_numpy/pybind11/stl_bind.h"

#include "labelfile.h"
#include "classifiedhsimage.h"
#include "hsimage.h"

#include "pybind11_opencv_numpy/ndarray_converter.h"


//declarations for Python class exports
void export_labelfile(pybind11::module m);
void export_hsimage(pybind11::module m);
void export_classifiedhsimage(pybind11::module m);


//PYBIND11_MAKE_OPAQUE(std::vector<std::string>)
PYBIND11_MAKE_OPAQUE(std::vector<u_int16_t>)
PYBIND11_MAKE_OPAQUE(std::vector<std::vector<u_int16_t> >)
PYBIND11_MAKE_OPAQUE(std::vector<double>)
PYBIND11_MAKE_OPAQUE(std::vector<std::vector<double> >)
PYBIND11_MAKE_OPAQUE(std::vector<float>)
PYBIND11_MAKE_OPAQUE(std::vector<cv::Mat>)
PYBIND11_MAKE_OPAQUE(std::vector<classColor>)


PYBIND11_MODULE(HSI,m)
{
    namespace py = pybind11;

    NDArrayConverter::init_numpy();

//    py::bind_vector<std::vector<std::string> >(m, "StringVector");
    py::bind_vector<std::vector<u_int16_t> >(m, "UInt16Vector");
    py::bind_vector<std::vector<std::vector<u_int16_t> > >(m, "UInt16VectorArray");
    py::bind_vector<std::vector<double> >(m, "DoubleVector");
    py::bind_vector<std::vector<std::vector<double> > >(m, "DoubleVectorArray");
    py::bind_vector<std::vector<float> >(m, "FloatVector");
    py::bind_vector<std::vector<cv::Mat> >(m, "MatVector");
    py::bind_vector<std::vector<classColor> >(m, "ClassInfoVector");

    export_labelfile(m);
    export_hsimage(m);
    export_classifiedhsimage(m);
}


//BOOST_PYTHON_MODULE(HSI)
//{
//    namespace bp = boost::python;

//    // specify that this module is actually a package
//    bp::object package = bp::scope();
//    package.attr("__path__") = "HSI";

//    init_ar();

//   //initialize converters
//    boost::python::to_python_converter<cv::Mat,
//           pbcvt::matToNDArrayBoostConverter>();
//    pbcvt::matFromNDArrayBoostConverter();

//    boost::python::to_python_converter<cv::Vec3b,
//	    cv_vec3b_to_python_converter>();

//    iterable_converter()
//            .from_python<std::vector<float>> () \
//            .from_python<std::vector<std::string>> () \
//    ;

//    std_pair_to_python_converter<std::string, cv::Vec3b>();

//    MAKE_VECTOR_WRAPPER(std::vector<std::string>, stringVector);
//    MAKE_VECTOR_WRAPPER(std::vector<u_int16_t>, uint16Vector);
//    MAKE_VECTOR_WRAPPER(std::vector<std::vector<u_int16_t>>, classSpectraArray);
//    MAKE_VECTOR_WRAPPER(std::vector<std::vector<double>>, classTFArray);
//    MAKE_VECTOR_WRAPPER(std::vector<double>, doubleVector);
//    MAKE_VECTOR_WRAPPER(std::vector<float>, floatVector);
//    MAKE_VECTOR_WRAPPER_LIMITED(std::vector<cv::Mat>, cvMatVector);
//    MAKE_VECTOR_WRAPPER_LIMITED(std::vector<classColor>, classInfoVector);

//    export_labelfile();
//    export_hsimage();
//    export_classifiedhsimage();
//}

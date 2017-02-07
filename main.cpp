#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API

#include <boost/python.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "classifiedhsimage.h"
#include "hsimage.h"
#include "pyboostconverter/pyboostcvconverter.hpp"

//declarations for Python class exports
void export_hsimage();
void export_classifiedhsimage();


#if (PY_VERSION_HEX >= 0x03000000)

    static void *init_ar() {
#else
        static void init_ar(){
#endif
        Py_Initialize();

        import_array();
        return NUMPY_IMPORT_ARRAY_RETVAL;
    }

BOOST_PYTHON_MODULE(HSI)
{
    namespace bp = boost::python;

    // specify that this module is actually a package
    bp::object package = bp::scope();
    package.attr("__path__") = "HSI";

    init_ar();

           //initialize converters
           boost::python::to_python_converter<cv::Mat,
                   pbcvt::matToNDArrayBoostConverter>();
           pbcvt::matFromNDArrayBoostConverter();


    iterable_converter()
            .from_python<std::vector<float>> () \
            .from_python<std::vector<std::string>> () \
    ;


    MAKE_VECTOR_WRAPPER(std::vector<std::string>, stringVector);
    MAKE_VECTOR_WRAPPER(std::vector<u_int16_t>, uint16Vector);
    MAKE_VECTOR_WRAPPER(std::vector<std::vector<u_int16_t>>, classSpectraArray);
    MAKE_VECTOR_WRAPPER(std::vector<std::vector<double>>, classTFArray);
    MAKE_VECTOR_WRAPPER(std::vector<double>, doubleVector);
    MAKE_VECTOR_WRAPPER(std::vector<float>, floatVector);
    MAKE_VECTOR_WRAPPER_LIMITED(std::vector<cv::Mat>, cvMatVector);

    export_hsimage();
    export_classifiedhsimage();
}

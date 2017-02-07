#define PY_ARRAY_UNIQUE_SYMBOL pbcvt_ARRAY_API

#include <boost/python.hpp>

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


    MAKE_VECTOR_WRAPPER(std::vector<std::string>, string_vector);
    MAKE_VECTOR_WRAPPER(std::vector<u_int16_t>, uint16_vector);
    MAKE_VECTOR_WRAPPER(std::vector<double>, double_vector);
    MAKE_VECTOR_WRAPPER(std::vector<cv::Mat>, cvmat_vector);

    export_hsimage();
    export_classifiedhsimage();
}

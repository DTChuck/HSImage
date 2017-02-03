#include <boost/python.hpp>

#include "classifiedhsimage.h"
#include "hsimage.h"
#include "python_utils.h"

//declarations for Python class exports
void export_hsimage();
void export_classifiedhsimage();

BOOST_PYTHON_MODULE(HSI)
{
    namespace bp = boost::python;

    // specify that this module is actually a package
    bp::object package = bp::scope();
    package.attr("__path__") = "HSI";

    MAKE_VECTOR_WRAPPER(std::vector<std::string>, string_vector);
    MAKE_VECTOR_WRAPPER(std::vector<u_int16_t>, uint16_vector);
    MAKE_VECTOR_WRAPPER(std::vector<double>, double_vector);

    export_hsimage();
    export_classifiedhsimage();
}

#include <boost/python.hpp>

#include "classifiedhsimage.h"
#include "hsimage.h"

void export_hsimage();
void export_classifiedhsimage();

BOOST_PYTHON_MODULE(HSI)
{
    namespace bp = boost::python;

    // specify that this module is actually a package
    bp::object package = bp::scope();
    package.attr("__path__") = "HSI";

    export_hsimage();
    export_classifiedhsimage();
}

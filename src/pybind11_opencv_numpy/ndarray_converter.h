# ifndef __NDARRAY_CONVERTER_H__
# define __NDARRAY_CONVERTER_H__

#include <Python.h>
#include <opencv2/core.hpp>
#include "pybind11/pybind11.h"


class NDArrayConverter {
public:
    // must call this first, or the other routines don't work!
    static bool init_numpy();
    
    static bool toMat(PyObject* o, cv::Mat &m);
    static PyObject* toNDArray(const cv::Mat& mat);
};

//
// Define the type converter
//


namespace pybind11 { namespace detail {
    template <> struct type_caster<cv::Vec3b> {
    public:
        PYBIND11_TYPE_CASTER(cv::Vec3b, _("list"));

        bool load(handle src, bool)
        {
            PyObject *source = src.ptr();

            list l =  reinterpret_borrow<list>(source);

            value[0] = l[0].cast<uchar>();
            value[1] = l[1].cast<uchar>();
            value[2] = l[2].cast<uchar>();

            return PyErr_Occurred();
        }

        static handle cast(cv::Vec3b vec, return_value_policy, handle defval)
        {
            list l;
            l.append(vec[0]);
            l.append(vec[1]);
            l.append(vec[2]);
            return l.release();
        }


    };
}}


namespace pybind11 { namespace detail {
    template <> struct type_caster<cv::Mat> {
    public:
        PYBIND11_TYPE_CASTER(cv::Mat,_("numpy.ndarray"));

        bool load(handle src, bool) {
            return NDArrayConverter::toMat(src.ptr(), value);
        }
    
        static handle cast(const cv::Mat &m, return_value_policy, handle defval)
        {
            return handle(NDArrayConverter::toNDArray(m));
        }
};
    
    
}} // namespace pybind11::detail

# endif

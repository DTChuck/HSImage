#ifndef PYTHON_UTILS_H
#define PYTHON_UTILS_H

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


/// @brief Type that allows for registration of conversions from
///        python iterable types.
struct iterable_converter
{
  /// @note Registers converter from a python interable type to the
  ///       provided type.
  template <typename Container>
  iterable_converter&
  from_python()
  {
    boost::python::converter::registry::push_back(
      &iterable_converter::convertible,
      &iterable_converter::construct_from_python<Container>,
      boost::python::type_id<Container>());

    // Support chaining.
    return *this;
  }

  template<class T>
  boost::python::list to_python(std::vector<T> v)
  {
      typename std::vector<T>::iterator iter;
      boost::python::list list;
      for(iter = v.begin();iter != v.end(); ++iter)
      {
          list.append(*iter);
      }
      return list;
  }

  /// @brief Check if PyObject is iterable.
  static void* convertible(PyObject* object)
  {
    return PyObject_GetIter(object) ? object : NULL;
  }

  /// @brief Convert iterable PyObject to C++ container type.
  ///
  /// Container Concept requirements:
  ///
  ///   * Container::value_type is CopyConstructable.
  ///   * Container can be constructed and populated with two iterators.
  ///     I.e. Container(begin, end)
  template <typename Container>
  static void construct_from_python(
    PyObject* object,
    boost::python::converter::rvalue_from_python_stage1_data* data)
  {
    namespace python = boost::python;
    // Object is a borrowed reference, so create a handle indicting it is
    // borrowed for proper reference counting.
    python::handle<> handle(python::borrowed(object));

    // Obtain a handle to the memory block that the converter has allocated
    // for the C++ type.
    typedef python::converter::rvalue_from_python_storage<Container>
                                                                storage_type;
    void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

    typedef python::stl_input_iterator<typename Container::value_type>
                                                                    iterator;

    // Allocate the C++ type into the converter's memory block, and assign
    // its handle to the converter's convertible variable.  The C++
    // container is populated by passing the begin and end iterators of
    // the python object to the container's constructor.
    new (storage) Container(
      iterator(python::object(handle)), // begin
      iterator());                      // end
    data->convertible = storage;
  }

};

// Converts a std::pair instance to a Python tuple.
template <typename T1, typename T2>
struct std_pair_to_tuple
{
  static PyObject* convert(std::pair<T1, T2> const& p)
  {
    return boost::python::incref(
      boost::python::make_tuple(p.first, p.second).ptr());
  }
  static PyTypeObject const *get_pytype () {return &PyTuple_Type; }
};

// Helper for convenience.
template <typename T1, typename T2>
struct std_pair_to_python_converter
{
  std_pair_to_python_converter()
  {
    boost::python::to_python_converter<
      std::pair<T1, T2>,
      std_pair_to_tuple<T1, T2>,
      true //std_pair_to_tuple has get_pytype
      >();
  }
};

struct cv_vec3b_to_python_converter
{
	static PyObject* convert(cv::Vec3b const& vec)
	{
		boost::python::list l;
		l.append(boost::python::object(vec[0]));
		l.append(boost::python::object(vec[1]));
		l.append(boost::python::object(vec[2]));
		return boost::python::incref(l.ptr());
	}
};


////define vector wrapper for Python - std::vector interoperability
#define MAKE_VECTOR_WRAPPER( CPP_NAME , PYTHON_NAME )                         \
   boost::python::class_< CPP_NAME >( #PYTHON_NAME )                          \
        .def(boost::python::vector_indexing_suite< CPP_NAME >())            \
   ;


//vector item struct that allows Python to interact with std::vector
template<class T>
struct vector_item {
   typedef typename T::value_type V;

   static V& get( T& x, int i )
   {
      if( i < 0 ) { i += x.size(); }
      if( i >= 0 && (unsigned)i < x.size() ) { return x[i]; }
      else { /*IndexError()*/; return x[0]; }
   }
   static void set( T& x, int i, V& v )
   {
      if( i < 0 ) { i += x.size(); }
      if( i >= 0 && (unsigned)i < x.size() ) { x[i] = v; }
      else { /*IndexError()*/; }
   }
   static void del( T& x, int i )
   {
      if( i < 0 ) { i += x.size(); }
      if( i >= 0 && (unsigned)i < x.size() ) { x.erase( x.begin() + i ); }
      else { /*IndexError()*/; }
   }
   static void add( T& x, V& v )
   {
      x.push_back( v );
   }

   static bool in( T& x, V& v ) {
      return find_eq( x.begin, x.end, v ) != x.end();
   }
   static int index( T& x, V& v ) {
      int i = 0;
      for( typename T::const_iterator it = x.begin; it != x.end(); ++it, ++i )
         if( *it == v ) { return i; }
      return -1;
   }
};

//define vector wrapper for Python - std::vector interoperability
#define MAKE_VECTOR_WRAPPER_LIMITED( CPP_NAME , PYTHON_NAME )                                        \
   boost::python::class_< CPP_NAME >( #PYTHON_NAME )                                         \
      .def("__len__"     , & CPP_NAME::size)                                                 \
      .def("clear"       , & CPP_NAME::clear)                                                \
      .def("append"      , & vector_item< CPP_NAME >::add                                    \
            , boost::python::with_custodian_and_ward<1, 2>())                                \
      .def("__setitem__" , & vector_item< CPP_NAME >::set                                    \
            , boost::python::with_custodian_and_ward<1, 2>())                                \
      .def("__getitem__" , & vector_item< CPP_NAME >::get                                    \
            , boost::python::return_value_policy<boost::python::copy_non_const_reference>()) \
      .def("__delitem__" , & vector_item< CPP_NAME >::del)                                   \
      .def("__iter__"    , boost::python::iterator< CPP_NAME >() )                           \
;

#endif // PYTHON_UTILS_H

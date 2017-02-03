#ifndef PYTHON_UTILS_H
#define PYTHON_UTILS_H

#include <boost/python.hpp>

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
#define MAKE_VECTOR_WRAPPER( CPP_NAME , PYTHON_NAME )                                        \
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

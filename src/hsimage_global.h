#ifndef HSIMAGE_GLOBAL_H
#define HSIMAGE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HSIMAGE_LIBRARY)
#  define HSIMAGESHARED_EXPORT Q_DECL_EXPORT
#else
#  define HSIMAGESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HSIMAGE_GLOBAL_H

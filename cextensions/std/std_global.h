#ifndef STD_GLOBAL_H
#define STD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STD_LIBRARY)
#  define STDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define STDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // STD_GLOBAL_H

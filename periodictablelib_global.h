#ifndef PERIODICTABLELIB_GLOBAL_H
#define PERIODICTABLELIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PERIODICTABLELIB_LIBRARY)
#  define PERIODICTABLELIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PERIODICTABLELIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PERIODICTABLELIB_GLOBAL_H

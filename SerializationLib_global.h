#pragma once

#include <QtCore/qglobal.h>

#if defined(SERIALIZATIONLIB_LIBRARY)
#  define SERIALIZATIONLIB_EXPORT Q_DECL_EXPORT
#else
#  define SERIALIZATIONLIB_EXPORT Q_DECL_IMPORT
#endif

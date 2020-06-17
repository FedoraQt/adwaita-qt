#ifndef ADWAITAQT_EXPORT_H
#define ADWAITAQT_EXPORT_H

#ifndef ADWAITAQT_EXPORT
#   define ADWAITAQT_EXPORT __attribute__((visibility("default")))
#endif

#ifndef ADWAITAQT_NO_EXPORT
#   define ADWAITAQT_NO_EXPORT __attribute__((visibility("hidden")))
#endif

#ifndef ADWAITAQT_DEPRECATED
#  define ADWAITAQT_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef ADWAITAQT_DEPRECATED_EXPORT
#  define ADWAITAQT_DEPRECATED_EXPORT ADWAITAQT_EXPORT ADWAITAQT_DEPRECATED
#endif

#ifndef ADWAITAQT_DEPRECATED_NO_EXPORT
#  define ADWAITAQT_DEPRECATED_NO_EXPORT ADWAITAQT_NO_EXPORT ADWAITAQT_DEPRECATED
#endif

#endif

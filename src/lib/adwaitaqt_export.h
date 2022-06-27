#ifndef ADWAITAQT_EXPORT_H
#define ADWAITAQT_EXPORT_H

#if defined(_WIN32)
#  if defined (adwaitaqt6priv_EXPORTS) || defined (adwaitaqtpriv_EXPORTS)
#    define ADWAITAQT_EXPORT __declspec(dllexport)
#  else
#    define ADWAITAQT_EXPORT __declspec(dllimport)
#  endif
#elif defined(__GNUC__)
#    define ADWAITAQT_EXPORT __attribute__((visibility("default")))
#else
#    define ADWAITAQT_EXPORT
#endif

#if defined(_WIN32)
#  if defined (adwaitaqt6_EXPORTS) || defined (adwaitaqt_EXPORTS)
#    define ADWAITAQT_MAIN_EXPORT __declspec(dllexport)
#  else
#    define ADWAITAQT_MAIN_EXPORT __declspec(dllimport)
#  endif
#elif defined(__GNUC__)
#    define ADWAITAQT_MAIN_EXPORT __attribute__((visibility("default")))
#else
#    define ADWAITAQT_MAIN_EXPORT
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

#pragma once


#ifdef _WIN32
  #define PROJECT1_EXPORT __declspec(dllexport)
#else
  #define PROJECT1_EXPORT
#endif

PROJECT1_EXPORT void project1();

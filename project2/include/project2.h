#pragma once


#ifdef _WIN32
  #define PROJECT2_EXPORT __declspec(dllexport)
#else
  #define PROJECT2_EXPORT
#endif

PROJECT2_EXPORT void project2();

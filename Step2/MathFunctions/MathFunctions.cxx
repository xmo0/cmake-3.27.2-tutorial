#include "MathFunctions.h"

#include <iostream>
// TODO 11: include cmath

// TODO 10: Wrap the mysqrt include in a precompiled ifdef based on USE_MYMATH
#ifdef USE_MYMATH
#include "mysqrt.h"
#else
#include <cmath>
#endif

namespace mathfunctions {
double sqrt(double x)
{
  // TODO 9: If USE_MYMATH is defined, use detail::mysqrt.
  // Otherwise, use std::sqrt.
  #if defined(USE_MYMATH)
  std::cout << "using mysqrt()!\n";
  return detail::mysqrt(x);
  #else
  std::cout << "using std::sqrt()!\n";
  return std::sqrt(x);
  #endif
}
}

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>


// USINGS //

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


// CONSTANTS //

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


// UTILITY FUNCTIONS //

inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}


// COMMON HEADERS //

#include "Ray.hpp"
#include "Vec3.hpp"

#endif


#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>


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

// Return a random real in [0, 1[ .
inline double random_double() {
  return rand() / (RAND_MAX + 1.0);
}

// Return a random real in [min, max[ .
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}


// COMMON HEADERS //

#include "Ray.hpp"
#include "Vec3.hpp"

#endif


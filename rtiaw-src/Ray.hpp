#ifndef RAY_H
#define RAY_H

#include "Vec3.hpp"

class Ray {
  public:
    // CONSTRUCTORS //
    Ray() {}
    Ray(const Point3& origin, const Vec3& direction)
      : orig(origin), dir(direction)
    {}

    // ACCESSORS //
    Point3 origin() const {
      return orig;
    }

    Vec3 direction() const {
      return dir;
    }

    // MOVING THE RAY //
    Point3 at(double t) const {
      return orig + t * dir;
    }

  // FIELDS //
  public:
    Point3 orig;
    Vec3 dir;
};

#endif


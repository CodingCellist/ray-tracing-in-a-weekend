#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.hpp"

struct hit_record {
  // point of intersection
  Point3 p;
  // surface normal
  Vec3 normal;
  // `t` at which the hit occurred
  double t;
};

class Hittable {
  public:
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif


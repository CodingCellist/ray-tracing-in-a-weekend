#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.hpp"
#include "RTWeekend.hpp"

class Material;

struct hit_record {
  // point of intersection
  Point3 p;
  // surface normal
  Vec3 normal;
  // the material used
  shared_ptr<Material> mat_ptr;
  // `t` at which the hit occurred
  double t;
  // did the ray hit inside or outside?
  bool front_face;

  inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    // if the ray hit the inside of the sphere, negate the normal to point
    // against the ray
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
  public:
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif


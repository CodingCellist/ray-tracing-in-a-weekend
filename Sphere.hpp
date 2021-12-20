#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.hpp"
#include "Vec3.hpp"

// A sphere is hittable, so it extends `Hittable`
class Sphere : public Hittable {
  public:
    Sphere() {}
    Sphere(Point3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(
        const Ray& r, double t_min, double t_max, hit_record& rec) const override;

  public:
    Point3 center;
    double radius;
};

bool Sphere::hit (const Ray& r, double t_min, double t_max, hit_record& rec) const {
  Vec3 oc = r.origin() - center;

  // terms of the quadratic for t, simplified through b = 2h
  auto a = r.direction().length_squared();    // CA . CA = |CA|^2
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;

  auto discriminant = half_b * half_b - a * c;
  // ignoring non-real solutions
  if (discriminant < 0) {
    return false;
  }
  auto sqrt_d = sqrt(discriminant);

  // Find nearest root within [t_min, t_max]
  auto root = (-half_b - sqrt_d) / a;
  if (root < t_min || t_max < root) {
    // consider the (potentially different) root (from the ± when solving for t)
    root = (-half_b + sqrt_d) / a;
    if (root < t_min || t_max < root) {
      return false;   // no root within range
    }
  }

  // store the result in the given `hit_record`
  rec.t = root;
  rec.p = r.at(rec.t);
  rec.normal = (rec.p - center) / radius;

  return true;
}


#endif

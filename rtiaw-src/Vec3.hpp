#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3 {
  public:
    // CONSTRUCTORS //
    Vec3() : e{0, 0, 0} {}
    Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    // ACCESSORS //
    double x() const {
      return e[0];
    }

    double y() const {
      return e[1];
    }

    double z() const {
      return e[2];
    }

    // OPERATOR OVERLOADING //
    Vec3 operator-() const {
      // negate the vector by negating each coord
      return Vec3(-e[0], -e[1], -e[2]);
    }

    double operator[](int i) const {
      return e[i];
    }

    double& operator[](int i) {
      return e[i];
    }

    Vec3& operator+=(const Vec3 &v) {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];
      return *this;
    }

    Vec3& operator*=(const double t) {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;
      return *this;
    }

    Vec3& operator/=(const double t) {
      return *this *= 1 / t;
    }

    // METHODS //
    double length() const {
      return sqrt(length_squared());
    }

    double length_squared() const {
      return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    // create a random Vec3
    inline static Vec3 random() {
      return Vec3(random_double(), random_double(), random_double());
    }

    // create a random Vec3, with (x, y, z) bounded by `min` and `max`
    inline static Vec3 random(double min, double max) {
      return Vec3( random_double(min, max)
                 , random_double(min, max)
                 , random_double(min, max)
                 );
    }

    // return true if vector ~= zero in all dimensions
    bool near_zero() const {
      const auto s = 1e-8;
      return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

  // FIELDS //
  public:
    // the coordinates
    double e[3];
};


// type aliases for Vec3
using Point3 = Vec3;    // 3D point
using Colour = Vec3;    // RGB colour

// Vec3 UTILITY FUNCTIONS //

// output the space-separated coordinates to the given output-stream
inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// pointwise addition
inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
  return Vec3( u.e[0] + v.e[0]
             , u.e[1] + v.e[1]
             , u.e[2] + v.e[2]
             );
}

// pointwise subtraction
inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
  return Vec3( u.e[0] - v.e[0]
             , u.e[1] - v.e[1]
             , u.e[2] - v.e[2]
             );
}

// pointwise multiplication
inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
  return Vec3( u.e[0] * v.e[0]
             , u.e[1] * v.e[1]
             , u.e[2] * v.e[2]
             );
}

// scalar multiplication
inline Vec3 operator*(double t, const Vec3 &v) {
  return Vec3( t * v.e[0]
             , t * v.e[1]
             , t * v.e[2]
             );
}

// scalar multiplication is commutative
inline Vec3 operator*(const Vec3 &v, double t) {
  return t * v;
}

// scalar division
inline Vec3 operator/(Vec3 v, double t) {
  return (1 / t) * v;
}

// dot product
inline double dot(const Vec3 &u, const Vec3 &v) {
  return u.e[0] * v.e[0]
       + u.e[1] * v.e[1]
       + u.e[2] * v.e[2];
}

// cross product
inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
  return Vec3( u.e[1] * v.e[2] - u.e[2] * v.e[1]
             , u.e[2] * v.e[0] - u.e[0] * v.e[2]
             , u.e[0] * v.e[1] - u.e[1] * v.e[0]
             );
}

// compute the vector's unit vector
inline Vec3 unit_vector(Vec3 v) {
  return v / v.length();
}

// generate a random point in the unit sphere
Vec3 random_in_unit_sphere() {
  while(true) {
    // generate random point in unit cube
    auto p = Vec3::random(-1, 1);
    // check if the random point is outside the unit sphere; outside radius 1
    if (p.length_squared() >= 1) continue;
    // if not, we've found a point!
    return p;
  }
}

// generate a random point on the unit sphere
Vec3 random_unit_vector() {
  return unit_vector(random_in_unit_sphere());
}

// generate a random, uniformly scattered vector from the surface normal
Vec3 random_in_hemisphere(const Vec3& normal) {
  Vec3 in_unit_sphere = random_in_unit_sphere();
  // if we're in the same hemisphere as the normal, it's fine
  if (dot(in_unit_sphere, normal) > 0.0) {
    return in_unit_sphere;
  }
  else {
    // otherwise, we're inside the other unit sphere, so flip the vector
    return -in_unit_sphere;
  }
}

// generate a random vector originating in a unit disc
Vec3 random_in_unit_disk() {
  while (true) {
    // generate random vect. bounded by a unit square
    auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
    // if it's outside the unit disc's radius (i.e. |p|^2 >= 1), try again
    if (p.length_squared() >= 1) {
      continue;
    }
    // otherwise, we've found our random vect!
    return p;
  }
}

// reflect the vector `v` off the surface, using the surface normal `n`
//
// (reflection is the same as `v` penetration followed by 2 * (v . n) * n
//  displacement in the opposite direction, since `n` points perpen. to the
//  surface by design)
Vec3 reflect(const Vec3& v, const Vec3& n) {
  return v - 2 * dot(v, n) * n;
}

// Calculate the refracted ray according to the ratio of the refractive indices
Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
  // cos of angle to normal
  auto cos_theta = fmin(dot(-uv, n), 1.0);
  // perpendicular component
  Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  // parallel component
  Vec3 r_out_par = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
  // combine the components to get the ray
  return r_out_perp + r_out_par;
}

#endif

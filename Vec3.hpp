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

#endif

#ifndef CAMERA_H
#define CAMERA_H

#include "RTWeekend.hpp"

class Camera {
  public:
    // CONSTRUCTORS //

    Camera(
        Point3 lookFrom,
        Point3 lookAt,
        Vec3 vup,
        double vfov,    // vertical field-of-view, in degrees
        double aspect_ratio
    ) {
      // camera view geometry
      auto theta = degrees_to_radians(vfov);
      auto h = tan(theta / 2);

      auto viewport_height = 2.0 * h;
      auto viewport_width = aspect_ratio * viewport_height;

      // orthonormal basis for camera orientation
      auto w = unit_vector(lookFrom - lookAt);  // we point the camera at -w
      auto u = unit_vector(cross(vup, w));      // horiz. axis is vup × w
      auto v = cross(w, u);                     // vert. axis is w × u

      origin = lookFrom;                // camera "sees" from `lookFrom` point
      horizontal = viewport_width * u;  // horiz. is: width * horiz. basis vect.
      vertical = viewport_height * v;   // vert. is: width * vert. basis vect.
      lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    // METHODS //

    Ray get_ray(double s, double t) const {
      return Ray( origin
                , lower_left_corner + s * horizontal + t * vertical - origin
                );
    }

  private:
    // FIELDS //

    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
};

#endif


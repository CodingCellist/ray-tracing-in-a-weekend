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
        double aspect_ratio,
        double aperture,
        double focus_dist
    ) {
      // camera view geometry
      auto theta = degrees_to_radians(vfov);
      auto h = tan(theta / 2);

      auto viewport_height = 2.0 * h;
      auto viewport_width = aspect_ratio * viewport_height;

      w = unit_vector(lookFrom - lookAt);  // we point the camera at -w
      u = unit_vector(cross(vup, w));      // horiz. axis is vup × w
      v = cross(w, u);                     // vert. axis is w × u

      // camera "sees" from `lookFrom` point
      origin = lookFrom;
      // horiz. cam. part is: focus_dist * width * horiz. basis vect.
      horizontal = focus_dist * viewport_width * u;
      // vert. cam. part is: focus_dist * width * vert. basis vect.
      vertical = focus_dist * viewport_height * v;

      lower_left_corner =
        origin - horizontal / 2 - vertical / 2 - focus_dist * w;

      // aperture controls effective size of lens
      lens_radius = aperture / 2;
    }

    // METHODS //

    Ray get_ray(double s, double t) const {
      // ray from thin lens
      Vec3 rd = lens_radius * random_in_unit_disk();
      Vec3 offset = u * rd.x() + v * rd.y();

      return Ray(
          origin + offset,
          lower_left_corner + s * horizontal + t * vertical - origin - offset
      );
    }

  private:
    // FIELDS //

    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;   // orthonormal basis for camera orientation
    double lens_radius;
};

#endif


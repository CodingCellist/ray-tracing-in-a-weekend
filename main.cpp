#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

#include <iostream>

// hard-coded maths for whether a ray hits a sphere, returning the `t` at which
// the ray hits the given sphere
double hit_sphere(const Point3& center, double radius, const Ray& r) {
  Vec3 oc = r.origin() - center;

  // terms of the quadratic for t, simplified through b = 2h
  auto a = r.direction().length_squared();    // CA . CA = |CA|^2
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;
  auto discriminant = half_b * half_b - a * c;

  // ignoring non-real solutions
  if (discriminant < 0) {
    return -1.0;
  }
  else {
    return (-half_b - sqrt(discriminant)) / a;
  }
}

// colour of the given ray
Colour ray_colour(const Ray& r) {
  // colour rays that hit the sphere according to the surface normal
  auto t = hit_sphere(Point3(0, 0, -1), 0.5, r);
  if (t > 0.0) {
    // compute the surface normal
    Vec3 N = unit_vector(r.at(t) - Vec3(0, 0, -1));
    // shade the surface normal
    return 0.5 * Colour(N.x() + 1, N.y() + 1, N.z() + 1);
  }
  Vec3 unit_direction = unit_vector(r.direction());
  t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
}

int main() {

  // Image

  const auto aspect_ratio =  16.0 / 9.0;    // 16:9
  const int img_width = 256;
  const int img_height = static_cast<int>(img_width / aspect_ratio);

  // Camera

  auto viewport_height = 2.0;
  auto viewport_width  = aspect_ratio * viewport_height;
  auto focal_length    = 1.0;

  auto origin = Point3(0, 0, 0);    // camera "sees" from 0,0,0
  auto horizontal = Vec3(viewport_width, 0, 0);   // y is horizontal
  auto vertical = Vec3(0, viewport_height, 0);    // x is vertical
  auto lower_left_corner =
    origin - (horizontal / 2) - (vertical / 2) - Vec3(0, 0, focal_length);

  // Render

  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

  for (int j = img_height - 1; j >= 0; --j) {
    // progress indicator
    std::cerr << '\r' << "Scanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < img_width; ++i) {
      // horizontal and vertical components of ray on screen
      auto u = double(i) / (img_width - 1);
      auto v = double(j) / (img_height - 1);

      Ray r( origin
           , lower_left_corner + (u * horizontal) + (v * vertical) - origin);

      Colour pixel_colour = ray_colour(r);
      write_colour(std::cout, pixel_colour);
    }
  }

  // end of progress indicator
  std::cerr << '\n' << "Done.\n";
}


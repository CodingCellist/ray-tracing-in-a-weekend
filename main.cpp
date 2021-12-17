#include "Colour.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

#include <iostream>

// hard-coded maths for whether a ray hits a sphere
bool hit_sphere(const Point3& center, double radius, const Ray& r) {
  Vec3 oc = r.origin() - center;
  // terms of the quadratic for t
  auto a = dot(r.direction(), r.direction()); // (A - C) . (A - C)
  auto b = 2.0 * dot(oc, r.direction());      // 2 x B . (A - C)
  auto c = dot(oc, oc) - radius * radius;     // (B . B) - (r ^ 2)
  // recall highschool maths
  auto discriminant = b * b - 4 * a * c;
  return discriminant > 0;
}

// colour of the given ray
Colour ray_colour(const Ray& r) {
  // colour rays that hit the sphere red
  if (hit_sphere( Point3(0, 0, -1), 0.5, r)) {
    return Colour(1, 0, 0);
  }
  Vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
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


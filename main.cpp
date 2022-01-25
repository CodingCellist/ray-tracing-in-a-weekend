#include "RTWeekend.hpp"

#include "Colour.hpp"
#include "Hittable_List.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"

#include <iostream>

// colour of the given ray
Colour ray_colour(const Ray& r, const Hittable& world) {
  hit_record rec;
  if (world.hit(r, 0, infinity, rec)) {
    // if we hit a sphere, shade according to the surface normal
    return 0.5 * (rec.normal + Colour(1, 1, 1));
  }
  Vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
}


int main() {

  // Image

  const auto aspect_ratio =  16.0 / 9.0;    // 16:9
  const int img_width = 400;
  const int img_height = static_cast<int>(img_width / aspect_ratio);
  const int samples_per_pixel = 100;

  // World

  Hittable_List world;
  world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  // the ground is round
  world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  // Camera

  Camera cam;

  // Render

  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

  for (int j = img_height - 1; j >= 0; --j) {
    // progress indicator
    std::cerr << '\r' << "Scanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < img_width; ++i) {
      // initial colour is black
      Colour pixel_colour(0, 0, 0);
      // Anti-Aliasing
      for (int s = 0; s < samples_per_pixel; ++s) {
        // horizontal and vertical components of ray on screen
        auto u = (i + random_double()) / (img_width - 1);
        auto v = (j + random_double()) / (img_height - 1);

        Ray r = cam.get_ray(u, v);

        pixel_colour += ray_colour(r, world);
      }
      write_colour(std::cout, pixel_colour, samples_per_pixel);
    }
  }

  // end of progress indicator
  std::cerr << '\n' << "Done.\n";
}


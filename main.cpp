#include "RTWeekend.hpp"

#include "Colour.hpp"
#include "Hittable_List.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "Material.hpp"

#include <iostream>

// colour of the given ray
Colour ray_colour(const Ray& r, const Hittable& world, int depth) {
  hit_record rec;

  // if we hit the depth limit, the ray was absorbed
  if (depth <= 0) {
    return Colour(0, 0, 0);
  }

  // checking for hits at 0.001 to account for floating-point approximations
  if (world.hit(r, 0.001, infinity, rec)) {
    Ray scattered;
    Colour attenuation;
    // check if the material scatters the ray
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      // if it scattered, handle that and attenuate the colour accordingly
      return attenuation * ray_colour(scattered, world, depth - 1);
    }
    // absorb the ray if it didn't scatter
    return Colour(0, 0, 0);
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
  const int max_depth = 50;

  // World

  Hittable_List world;

  auto material_ground = make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
  auto material_center = make_shared<Lambertian>(Colour(0.7, 0.3, 0.3));
  auto material_left   = make_shared<Metal>(Colour(0.8, 0.8, 0.8));
  auto material_right  = make_shared<Metal>(Colour(0.8, 0.6, 0.2));

  // the ground is round
  world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, material_ground));
  // pondering my orbs
  world.add(make_shared<Sphere>(Point3( 0, 0, -1), 0.5, material_center));
  world.add(make_shared<Sphere>(Point3(-1, 0, -1), 0.5, material_left));
  world.add(make_shared<Sphere>(Point3( 1, 0, -1), 0.5, material_right));

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

        pixel_colour += ray_colour(r, world, max_depth);
      }
      write_colour(std::cout, pixel_colour, samples_per_pixel);
    }
  }

  // end of progress indicator
  std::cerr << '\n' << "Done.\n";
}


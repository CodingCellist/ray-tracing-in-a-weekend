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

// return the scene used for development
Hittable_List dev_scene() {
  Hittable_List world;

  auto material_ground = make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
  auto material_center = make_shared<Lambertian>(Colour(0.1, 0.2, 0.5));
  auto material_left   = make_shared<Dielectric>(1.5);
  auto material_right  = make_shared<Metal>(Colour(0.8, 0.6, 0.2), 0.0);

  // the ground is round
  world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, material_ground));
  // pondering my orbs
  world.add(make_shared<Sphere>(Point3( 0, 0, -1),  0.5, material_center));
  world.add(make_shared<Sphere>(Point3(-1, 0, -1),  0.5, material_left));
  world.add(make_shared<Sphere>(Point3(-1, 0, -1), -0.45, material_left));
  world.add(make_shared<Sphere>(Point3( 1, 0, -1),  0.5, material_right));

  return world;
}

// produce a scene with lots of random spheres
Hittable_List random_scene() {
  Hittable_List world;

  // radii
  auto ground_radius = 1000.0;
  auto small_radius  = 0.2;
  auto big_radius    = 1.0;

  // the ground is (still) round
  auto ground_material = make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
  world.add(make_shared<Sphere>(Point3(0, -1000, 0), ground_radius, ground_material));


  // generate a bunch of random small spheres
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();

      // random center for the sphere
      Point3 center(
          a + 0.9 * random_double(),
          0.2,
          b + 0.9 * random_double()
          );

      // make sure the spheres are at least a bit in the camera view
      if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<Material> sphere_material;

        // determine the randomly picked material
        if (choose_mat < 0.8) {
          // diffuse (80% likely)
          auto albedo = Colour::random() * Colour::random();
          sphere_material = make_shared<Lambertian>(albedo);
        }
        else if (choose_mat < 0.95) {
          // metal (15% likely)
          auto albedo = Colour::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<Metal>(albedo, fuzz);
        }
        else {
          // glass (5% likely)
          sphere_material = make_shared<Dielectric>(1.5);
        }

        // add a new sphere with the material
        world.add(make_shared<Sphere>(center, small_radius, sphere_material));
      }
    }
  }

  // add 3 big spheres, one of each material type
  auto material1 = make_shared<Dielectric>(1.5);
  world.add(make_shared<Sphere>(Point3(0, 1, 0), big_radius, material1));

  auto material2 = make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
  world.add(make_shared<Sphere>(Point3(-4, 1, 0), big_radius, material2));

  auto material3 = make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<Sphere>(Point3(4, 1, 0), big_radius, material3));

  return world;
}



int main() {

  // Image

  const auto aspect_ratio =  3.0 / 2.0;
  const int img_width = 1200;
  const int img_height = static_cast<int>(img_width / aspect_ratio);
  const int samples_per_pixel = 500;
  const int max_depth = 50;

  // World

  Hittable_List world = random_scene();

  // Camera

  Point3 lookFrom = Point3(13, 2, 3);
  Point3 lookAt   = Point3(0, 0, 0);
  Vec3 vup = Vec3(0, 1, 0);
  auto vfov = 20;
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;
  Camera cam(lookFrom, lookAt, vup, vfov, aspect_ratio, aperture, dist_to_focus);

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


#ifndef MATERIAL_H
#define MATERIAL_H

#include "RTWeekend.hpp"

struct hit_record;

class Material {
  public:
    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Colour& attenuation, Ray& scattered
    ) const = 0;
};

// class representing Lambertian diffuse materials
class Lambertian : public Material {
  public:
    Lambertian(const Colour& a) : albedo(a) {}

    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Colour& attenuation, Ray& scattered
    ) const override {
        // scatter the ray, scaling along the normal
        auto scatter_direction = rec.normal + random_unit_vector();

        // catch degenerate scatter direction
        if (scatter_direction.near_zero()) {
          scatter_direction = rec.normal;
        }

        scattered = Ray(rec.p, scatter_direction);

        // always scatter, but attenuate by the reflection
        // (instead of probability p of scattering & attenuating by albedo/p )
        attenuation = albedo;
        return true;
    }

  public:
    Colour albedo;
};

// class representing reflictive metal material
class Metal : public Material {
  public:
    Metal(const Colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Colour& attenuation, Ray& scattered
    ) const override {
        // reflect the ray perfectly
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        // scatter the ray, factoring in fuzziness
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        // scatter if the direction doesn't cancel the normal
        return dot(scattered.direction(), rec.normal) > 0;
    }

  public:
    Colour albedo;
    double fuzz;
};

#endif

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

// class representing dielectric material
class Dielectric : public Material {
  public:
    Dielectric(double refractive_index) : ri(refractive_index) {}

    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Colour& attenuation, Ray& scattered
    ) const override {
      attenuation = Colour(1.0, 1.0, 1.0);
      double refraction_ratio = rec.front_face ? (1.0 / ri) : ri;

      Vec3 unit_direction = unit_vector(r_in.direction());

      // compute the sin and cos values for the angle to the normal
      double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
      double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

      Vec3 direction;

      // is there a real solution to Snell's Law ?
      bool cannot_refract = refraction_ratio * sin_theta > 1.0;
      // randomly determine whether to reflect
      bool rand_refl = reflectance(cos_theta, refraction_ratio) > random_double();

      // reflect the ray if total internal refl.n or if randomly decided
      if (cannot_refract || rand_refl) {
        direction = reflect(unit_direction, rec.normal);
      }
      else {
        // otherwise, refract the ray
        direction = refract(unit_direction, rec.normal, refraction_ratio);
      }

      scattered = Ray(rec.p, direction);
      return true;
    }

  public:
    // refractive index
    double ri;

  private:
    static double reflectance(double cosine, double ref_idx) {
      // Use Schlick's approx.n for reflectance
      auto r0 = (1 - ref_idx) / (1 + ref_idx);
      r0 = r0 * r0;
      return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif

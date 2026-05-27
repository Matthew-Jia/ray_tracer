#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "vec3.hpp"

class [[nodiscard]] material {
	public:
		virtual ~material() = default;

		virtual bool scatter(
			const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
		) const = 0;
};

class [[nodiscard]] lambertian : public material {
	public:
		explicit lambertian(const color &albedo) : albedo_{albedo} {}

		[[nodiscard]] bool scatter(
			const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
		) const override
		{
			auto scatter_direction = rec.normal + random_unit_vector();

			if (scatter_direction.near_zero())
				scatter_direction = rec.normal;

			scattered = ray(rec.p, scatter_direction, r_in.time());
			attenuation = albedo_;
			return true;
		}

	private:
		color albedo_;
};

class [[nodiscard]] metal : public material {
	public:
		metal(const color &albedo, double fuzz) : albedo_{albedo}, fuzz_{fuzz < 1 ? fuzz : 1} {}

		[[nodiscard]] bool scatter(
			const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
		) const override
		{
			auto reflected = reflect(r_in.direction(), rec.normal);
      reflected = unit_vector(reflected) + (fuzz_ * random_unit_vector());
			scattered = ray(rec.p, reflected, r_in.time());
			attenuation = albedo_;
			return (dot(scattered.direction(), rec.normal) > 0);
		}

	private:
		color albedo_;
		double fuzz_;
};

class [[nodiscard]] dielectric : public material {
	public:
		explicit dielectric(double refraction_index) : refraction_index_{refraction_index} {}

		[[nodiscard]] bool scatter(
			const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
		) const override
		{
      attenuation = color{1.0, 1.0, 1.0};
      double ri = rec.front_face ? (1.0/refraction_index_) : refraction_index_;

      vec3 unit_dir = unit_vector(r_in.direction());
      double cos_theta = std::fmin(dot(-unit_dir, rec.normal), 1.0);
      double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

      bool cannot_refract = ri*sin_theta > 1.0;
      vec3 direction;

      if (cannot_refract || reflectance(cos_theta, ri) > random_double())
        direction = reflect(unit_dir, rec.normal);
      else
        direction = refract(unit_dir, rec.normal, ri);

      scattered = ray(rec.p, direction, r_in.time());
      return true;
		}

	private:
		double refraction_index_;

    [[nodiscard]] static double reflectance(double cosine, double refraction_index_)
    {
      auto r0 = (1-refraction_index_) / (1 + refraction_index_);
      r0 = r0 * r0;
      return r0 + (1-r0)*std::pow((1-cosine), 5);
    }
};



#pragma once

#include "aabb.hpp"
#include "common.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <cmath>
#include <memory>

class material;

class [[nodiscard]] hit_record {
public:
	point3 p;
	vec3 normal;
  std::shared_ptr<material> mat;
	double t;
  double u;
  double v;
	bool front_face;

	constexpr void set_face_normal(const ray &r, const vec3 &outward_normal) noexcept
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default; // makes destructor virtual
	[[nodiscard]] constexpr virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const noexcept = 0;
  constexpr virtual aabb bounding_box() const noexcept = 0;
};

// TODO: make matrix4x4 class and consolodate translate, rotate_x, rotate_y, rotate_z and scale into that one class
class translate : public hittable {
  public:
    constexpr translate(std::shared_ptr<hittable> object, const vec3 &offset) noexcept
      : object_{std::move(object)}, offset_{offset} 
    {
      bbox_ = object_->bounding_box() + offset_;
    }

    [[nodiscard]] constexpr bool hit(const ray &r, interval ray_t, hit_record &rec) const noexcept override
    {
      ray offset_r{r.origin() - offset_, r.direction(), r.time()};

      if (!object_->hit(offset_r, ray_t, rec))
        return false;

      rec.p += offset_;
      return true;
    }

    constexpr aabb bounding_box() const noexcept override { return bbox_; }

  private:
    std::shared_ptr<hittable> object_;
    vec3 offset_;
    aabb bbox_;
};

class rotate_y : public hittable {
  public:
    constexpr rotate_y(std::shared_ptr<hittable> object, double degrees) noexcept : object_{std::move(object)}
    {
      double radians = degrees_to_radians(degrees);
      sin_theta_ = std::sin(radians);
      cos_theta_ = std::cos(radians);
      bbox_ = object_->bounding_box();
      
      point3 min{infinity, infinity, infinity};
      point3 max{-infinity, -infinity, -infinity};

      for (int i = 0; i < 2; ++i)
      {
        for (int j = 0; j < 2; ++j)
        {
          for (int k = 0; k < 2; ++k)
          {
            double x = i*bbox_.x.min + (1-i)*bbox_.x.max;
            double y = j*bbox_.y.min + (1-j)*bbox_.y.max;
            double z = k*bbox_.z.min + (1-k)*bbox_.z.max;

            auto rotated_x = (cos_theta_ * x) + (sin_theta_ * z);
            auto rotated_z = (-sin_theta_ * x) + (cos_theta_ * z);

            point3 rotated_corner{rotated_x, y, rotated_z};

            for (int d = 0; d < 3; ++d)
            {
              min[d] = std::fmin(min[d], rotated_corner[d]);
              max[d] = std::fmax(max[d], rotated_corner[d]);
            }
          }
        }
      }

      assert((min != point3{infinity, infinity, infinity}));
      assert((max != point3{-infinity, -infinity, -infinity}));

      bbox_ = aabb{min, max};
    }

    constexpr bool hit(const ray &r, interval ray_t, hit_record &rec) const noexcept override
    {
      point3 rotated_origin{
        (cos_theta_ * r.origin().x) - (sin_theta_ * r.origin().z),
        r.origin().y,
        (sin_theta_ * r.origin().x) + (cos_theta_ * r.origin().z),
      };
      
      vec3 rotated_direction{
        (cos_theta_ * r.direction().x) - (sin_theta_ * r.direction().z),
        r.direction().y,
        (sin_theta_ * r.direction().x) + (cos_theta_ * r.direction().z),
      };

      ray rotated_r{rotated_origin, rotated_direction, r.time()};

      if (!object_->hit(rotated_r, ray_t, rec))
        return false;

      rec.p = point3{
        (cos_theta_ * r.origin().x) + (sin_theta_ * r.origin().z),
        r.origin().y,
        (-sin_theta_ * r.origin().x) + (cos_theta_ * r.origin().z),
      };

      rec.normal = vec3{
        (cos_theta_ * r.origin().x) + (sin_theta_ * r.origin().z),
        r.origin().y,
        (-sin_theta_ * r.origin().x) + (cos_theta_ * r.origin().z),
      };

      return true;
    }

    constexpr aabb bounding_box() const noexcept override { return bbox_; }

  private:
    std::shared_ptr<hittable> object_;
    double sin_theta_;
    double cos_theta_;
    aabb bbox_;
};


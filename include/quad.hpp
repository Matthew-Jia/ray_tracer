#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "interval.hpp"
#include "vec3.hpp"

#include <memory>

class quad : public hittable {
  public:
    constexpr quad(const point3 &Q, const vec3 &u, const vec3 &v, std::shared_ptr<material> mat) noexcept
      : Q_{Q}, u_{u}, v_{v}, mat_{std::move(mat)}
    {
      auto n = cross(u, v);
      normal_ = unit_vector(n);
      D_ = dot(normal_, Q_);
      w_ = n / dot(n, n);

      set_bounding_box();
    }

    constexpr virtual void set_bounding_box() noexcept
    {
      // Computer the bouding box of all four vertices
      auto bbox_diagonal1 = aabb{Q_, Q_+u_+v_};
      auto bbox_diagonal2 = aabb{Q_+u_, Q_+v_};
      bbox_ = aabb{bbox_diagonal1, bbox_diagonal2};
    }

    constexpr aabb bounding_box() const noexcept override { return bbox_; }

    constexpr bool hit(const ray &r, interval ray_t, hit_record &rec) const noexcept override
    {
      auto denom = dot(normal_, r.direction());

      if (std::fabs(denom) < 1e-8)
        return false;
      
      auto t = (D_ - dot(normal_, r.origin())) / denom;

      if (!ray_t.contains(t))
        return false;

      auto intersection = r.at(t);
      auto p = intersection - Q_;

      auto alpha = dot(w_, cross(p, v_));
      auto beta = dot(w_, cross(u_, p));

      if (!(interval{0,1}.contains(alpha) && interval{0,1}.contains(beta)))
        return false;

      rec.t = t;
      rec.p = intersection;
      rec.set_face_normal(r, normal_);
      rec.u = alpha;
      rec.v = beta;
      rec.mat = mat_;

      return true;
    }

  private:
    point3 Q_;
    vec3 u_, v_;
    vec3 w_;
    std::shared_ptr<material> mat_;
    aabb bbox_;
    vec3 normal_;
    double D_;
};

[[nodiscard]] inline constexpr std::shared_ptr<hittable_list> box(const point3 &a, const point3 &b, std::shared_ptr<material> mat)
{
  auto sides = std::make_shared<hittable_list>();

  auto min = point3{
    std::fmin(a.x, b.x),
    std::fmin(a.y, b.y),
    std::fmin(a.z, b.z),
  };

  auto max = point3{
    std::fmax(a.x, b.x),
    std::fmax(a.y, b.y),
    std::fmax(a.z, b.z),
  };

  auto dx = vec3{max.x - min.x, 0, 0};
  auto dy = vec3{0, max.y - min.y, 0};
  auto dz = vec3{0, 0, max.z - min.z};

  sides->add(std::make_shared<quad>(point3(min.x, min.y, max.z),  dx,  dy, mat)); // front
  sides->add(std::make_shared<quad>(point3(max.x, min.y, max.z), -dz,  dy, mat)); // right
  sides->add(std::make_shared<quad>(point3(max.x, min.y, min.z), -dx,  dy, mat)); // back
  sides->add(std::make_shared<quad>(point3(min.x, min.y, min.z),  dz,  dy, mat)); // left
  sides->add(std::make_shared<quad>(point3(min.x, max.y, max.z),  dx, -dz, mat)); // top
  sides->add(std::make_shared<quad>(point3(min.x, min.y, min.z),  dx,  dz, mat)); // bottom

  return sides;
}


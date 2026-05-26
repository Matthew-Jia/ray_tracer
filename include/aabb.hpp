#pragma once

#include "common.hpp"
#include "hittable.hpp"
#include "interval.hpp"
#include "vec3.hpp"

class [[nodiscard]] aabb {
  public: 
    interval x;
    interval y;
    interval z;

    explicit aabb() {}
    explicit aabb(const interval &x, const interval &y, const interval &z)
      : x{x}, y{y}, z{z} {}

    explicit aabb(const point3 &p1, const point3 &p2)
    {
      x = (p1.x <= p2.x) ? interval{p1.x, p2.x} : interval{p2.x, p1.x};
      y = (p1.y <= p2.y) ? interval{p1.y, p2.y} : interval{p2.y, p1.y};
      z = (p1.z <= p2.z) ? interval{p1.z, p2.z} : interval{p2.z, p1.z};
    }

    explicit aabb(const aabb &b1, const aabb &b2) : 
      x{b1.x, b2.x},
      y{b1.y, b2.y},
      z{b1.z, b2.z}
    {}

    [[nodiscard]] const interval& axis_interval(int n) const
    {
      assert(0 <= n && n <= 2);
      if (n == 0) return x;
      if (n == 1) return y;
      return z;
    }

    [[nodiscard]] bool hit(const ray &r, interval ray_t) const 
    {
      const point3& ray_orig = r.origin();
      const vec3&   ray_dir  = r.direction();

      for (int axis = 0; axis < 3; axis++) {
        const interval &ax = axis_interval(axis);
        const double adinv = 1.0 / ray_dir[axis];

        auto t0 = (ax.min - ray_orig[axis]) * adinv;
        auto t1 = (ax.max - ray_orig[axis]) * adinv;

        if (t0 > t1) {
          std::swap(t0, t1);
        }
        
        ray_t.min = std::fmax(t0, ray_t.min);
        ray_t.max = std::fmin(t1, ray_t.max);

        if (ray_t.max <= ray_t.min)
          return false;
      }
      return true;
    }

};


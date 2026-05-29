#pragma once

#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class [[nodiscard]] aabb {
  public: 
    aabb() {}
    aabb(const interval &x, const interval &y, const interval &z)
      : x_{x}, y_{y}, z_{z} {}

    aabb(const point3 &p1, const point3 &p2)
    {
      x_ = (p1.x <= p2.x) ? interval{p1.x, p2.x} : interval{p2.x, p1.x};
      y_ = (p1.y <= p2.y) ? interval{p1.y, p2.y} : interval{p2.y, p1.y};
      z_ = (p1.z <= p2.z) ? interval{p1.z, p2.z} : interval{p2.z, p1.z};
    }

    aabb(const aabb &b1, const aabb &b2) : 
      x_{b1.x_, b2.x_},
      y_{b1.y_, b2.y_},
      z_{b1.z_, b2.z_}
    {}

    [[nodiscard]] const interval& axis_interval(int n) const
    {
      assert(0 <= n && n <= 2);
      if (n == 0) return x_;
      if (n == 1) return y_;
      return z_;
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

    [[nodiscard]] int longest_axis() const 
    { 
      if (x_.size() > y_.size())
        return x_.size() > z_.size() ? 0 : 2;
      else
        return y_.size() > z_.size() ? 1 : 2;
    }

    static const aabb empty, universe;

  private: 
    interval x_;
    interval y_;
    interval z_;
};

inline const aabb aabb::empty    = aabb{interval::empty,    interval::empty,    interval::empty};
inline const aabb aabb::universe = aabb{interval::universe, interval::universe, interval::universe};


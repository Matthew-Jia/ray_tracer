#pragma once

#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class [[nodiscard]] aabb {
  public: 
    interval x;
    interval y;
    interval z;

    constexpr aabb() noexcept = default;
    constexpr aabb(const interval &x, const interval &y, const interval &z) noexcept
      : x{x}, y{y}, z{z}
    {
      pad_to_minimums();
    }

    constexpr aabb(const point3 &p1, const point3 &p2) noexcept
    {
      x = (p1.x <= p2.x) ? interval{p1.x, p2.x} : interval{p2.x, p1.x};
      y = (p1.y <= p2.y) ? interval{p1.y, p2.y} : interval{p2.y, p1.y};
      z = (p1.z <= p2.z) ? interval{p1.z, p2.z} : interval{p2.z, p1.z};

      pad_to_minimums();
    }

    constexpr aabb(const aabb &b1, const aabb &b2) noexcept
      : x{b1.x, b2.x},
        y{b1.y, b2.y},
        z{b1.z, b2.z}
    {}

    [[nodiscard]] constexpr const interval& axis_interval(int n) const noexcept
    {
      assert(0 <= n && n <= 2);
      if (n == 0) return x;
      if (n == 1) return y;
      return z;
    }

    [[nodiscard]] constexpr bool hit(const ray &r, interval rayt) const noexcept
    {
      const point3& rayorig = r.origin();
      const vec3&   raydir  = r.direction();

      for (int axis = 0; axis < 3; axis++) {
        const interval &ax = axis_interval(axis);
        const double adinv = 1.0 / raydir[axis];

        auto t0 = (ax.min - rayorig[axis]) * adinv;
        auto t1 = (ax.max - rayorig[axis]) * adinv;

        if (t0 > t1) {
          std::swap(t0, t1);
        }
        
        rayt.min = std::fmax(t0, rayt.min);
        rayt.max = std::fmin(t1, rayt.max);

        if (rayt.max <= rayt.min)
          return false;
      }
      return true;
    }

    [[nodiscard]] constexpr int longest_axis() const noexcept
    { 
      if (x.size() > y.size())
        return x.size() > z.size() ? 0 : 2;
      else
        return y.size() > z.size() ? 1 : 2;
    }

    static const aabb empty, universe;

  private: 

    constexpr void pad_to_minimums() noexcept
    {
      double delta = 0.0001;
      if (x.size() < delta) x = x.expand(delta);
      if (y.size() < delta) y = y.expand(delta);
      if (z.size() < delta) z = z.expand(delta);

    }
};

inline const aabb aabb::empty    = aabb{interval::empty,    interval::empty,    interval::empty};
inline const aabb aabb::universe = aabb{interval::universe, interval::universe, interval::universe};

inline constexpr aabb operator+(const aabb &bbox, const vec3 &offset) noexcept
{ 
  return aabb{
    bbox.x + offset.x,
    bbox.y + offset.y,
    bbox.z + offset.z,
  };
}

inline constexpr aabb operator+(const vec3 &offset, const aabb &bbox) noexcept
{ 
  return bbox + offset;
}


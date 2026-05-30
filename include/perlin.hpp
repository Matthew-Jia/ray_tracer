#pragma once

#include "common.hpp"
#include "vec3.hpp"
#include <array>

using corner_gradients = std::array<std::array<std::array<vec3, 2>, 2>, 2>;

class perlin {
  public:
    constexpr perlin() noexcept
    {
      for (auto &i : randvec_) 
        i = unit_vector(vec3::random(-1,1));

      perlin_generate_perm(perm_x_);
      perlin_generate_perm(perm_y_);
      perlin_generate_perm(perm_z_);
    }

    [[nodiscard]] constexpr double noise(const point3 &p) const noexcept
    {
      auto u = p.x - std::floor(p.x);
      auto v = p.y - std::floor(p.y);
      auto w = p.z - std::floor(p.z);

      auto i = int(std::floor(p.x));
      auto j = int(std::floor(p.y));
      auto k = int(std::floor(p.z));

      corner_gradients c;

      for (int di = 0; di < 2; ++di)
        for (int dj = 0; dj < 2; ++dj)
          for (int dk = 0; dk < 2; ++dk)
            c[di][dj][dk] = randvec_[
              perm_x_[(i+di) & 255] ^ 
              perm_y_[(j+dj) & 255] ^
              perm_z_[(k+dk) & 255]
            ];

      return perlin_interp(c, u, v, w);
    }

    [[nodiscard]] constexpr double turb(const point3 &p, int depth) const noexcept
    {
      auto accum = 0.0;
      auto temp_p = p;
      auto weight = 1.0;

      for (int i = 0; i < depth; ++i)
      {
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
      }

      return std::fabs(accum);
    }

  private:
      static constexpr int point_count_ = 256;
      std::array<vec3, point_count_> randvec_;
      std::array<int, point_count_> perm_x_;
      std::array<int, point_count_> perm_y_;
      std::array<int, point_count_> perm_z_;

      static constexpr void perlin_generate_perm(std::array<int, point_count_> &p) noexcept
      {
        for (size_t i = 0; i < p.size(); ++i)
          p[i] = (int)i;

        permute(p);
      }

      static constexpr void permute(std::array<int, point_count_> &p) noexcept
      {
        for (size_t i = p.size() - 1; i-- > 0;)
        {
          int target = random_int(0, (int)i);
          int tmp = p[i];
          p[i] = p[target];
          p[target] = tmp;
        }
      }

      [[nodiscard]] static constexpr double perlin_interp(corner_gradients c, double u, double v, double w) noexcept
      {
        auto uu = u*u*(3-2*u);
        auto vv = v*v*(3-2*v);
        auto ww = w*w*(3-2*w);

        auto accum = 0.0;
        for (int i = 0; i < 2; ++i)
          for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
            {
              vec3 weight_v{u-i, v-j, w-k};
              accum += (i*uu + (1-i)*(1-uu))
                * (j*vv + (1-j)*(1-vv))
                * (k*ww + (1-k)*(1-ww))
                * dot(c[i][j][k], weight_v);
            }
        return accum;
      }
};

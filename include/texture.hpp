#pragma once

#include "color.hpp"
#include "interval.hpp"
#include "perlin.hpp"
#include "rtw_stb_image.hpp"
#include "vec3.hpp"

#include <cassert>
#include <memory>

class [[nodiscard]] texture {
  public:
    virtual ~texture() = default;
    constexpr virtual color value(double u, double v, const point3 &p) const noexcept = 0;
};

class [[nodiscard]] solid_color : public texture {
  public:
    explicit constexpr solid_color(const color &albedo) noexcept : albedo_(albedo) {}

    constexpr solid_color(double red, double green, double blue) noexcept : solid_color{color{red, green, blue}} {}

    constexpr color value(double, double, const point3 &) const noexcept override { return albedo_; }

  private:
    color albedo_;
};

class [[nodiscard]] checker_texture : public texture {
  public:
    constexpr checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd) noexcept
      : inv_scale_{1.0/scale}, even_{std::move(even)}, odd_{std::move(odd)}
    {}

    constexpr checker_texture(double scale, const color &color1, const color &color2)
      : checker_texture{scale, std::make_shared<solid_color>(color1), std::make_shared<solid_color>(color2)}
    {}

    constexpr color value(double u, double v, const point3 &p) const noexcept override
    {
      auto xInt = int(std::floor(inv_scale_ * p.x));
      auto yInt = int(std::floor(inv_scale_ * p.y));
      auto zInt = int(std::floor(inv_scale_ * p.z));

      bool isEven = (xInt + yInt + zInt) % 2 == 0;

      return isEven ? even_->value(u, v, p) : odd_->value(u, v, p);
    }

  private:
    double inv_scale_;
    std::shared_ptr<texture> even_;
    std::shared_ptr<texture> odd_;
};

class [[nodiscard]] image_texture : public texture {
  public:
    constexpr image_texture() : image_{nullptr} {}

    explicit constexpr image_texture(const char *filename) : image_{filename} {}

    constexpr color value(double u, double v, const point3 &) const noexcept override
    {
      if (image_.height() == 0) return color{0, 1, 1};

      u = interval{0,1}.clamp(u);
      v = 1.0 - interval{0,1}.clamp(v);  // Flip V to image coordinates
      
      auto i = int(u * image_.width());
      auto j = int(v * image_.height());
      auto pixel = image_.pixel_data(i,j);

      return pixel / 255.0;
    }

  private:
    rtw_image image_;
};

class [[nodiscard]] noise_texture : public texture {
  public:
    explicit constexpr noise_texture(double scale) noexcept : scale_{scale} {}
    
    constexpr color value(double, double, const point3 &p) const noexcept override
    {
      return color(.5, .5, .5) * (1 + std::sin(scale_ * p.z + 10 * noise_.turb(p, 7)));
    }

  private:
    perlin noise_;
    double scale_;
};




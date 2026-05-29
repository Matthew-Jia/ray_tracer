#pragma once

#include "color.hpp"
#include "interval.hpp"
#include "rtw_stb_image.hpp"
#include "vec3.hpp"

#include <cassert>
#include <memory>

class texture {
  public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color : public texture {
  public:
    explicit solid_color(const color &albedo) : albedo_(albedo) {}
    solid_color(double red, double green, double blue) : solid_color{color{red, green, blue}} {}

    color value(double, double, const point3 &) const override { return albedo_; }

  private:
    color albedo_;
};

class checker_texture : public texture {
  public:
    checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
      : inv_scale_{1.0/scale}, even_{even}, odd_{odd}
    {}

    checker_texture(double scale, const color &color1, const color &color2)
      : checker_texture{scale, std::make_shared<solid_color>(color1), std::make_shared<solid_color>(color2)}
    {}

    color value(double u, double v, const point3 &p) const override
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

class image_texture : public texture {
  public:
    image_texture() : image_{nullptr} {}
    image_texture(const char *filename) : image_{filename} {}

    color value(double u, double v, const point3 &) const override
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




#pragma once

#include "color.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>

class rtw_image {
  public:
    constexpr rtw_image() = default;

    explicit constexpr rtw_image(const char *image_filename) 
    {
      auto filename = std::string(image_filename);

      if (load("../images/" + filename)) return;

      std::cerr << "ERROR: could not load image file '" << image_filename << "'.\n";
    }

    ~rtw_image()
    {
      STBI_FREE(fdata_);
    }

    [[nodiscard]] constexpr bool load(const std::string &filename)
    {
      auto n = bytes_per_pixel_;
      fdata_ = stbi_loadf(filename.c_str(), &image_width_, &image_height_, &n, bytes_per_pixel_);
      if (fdata_ == nullptr) return false;

      bytes_per_scanline_ = image_width_ * bytes_per_pixel_;
      convert_to_bytes();
      return true;
    }

    [[nodiscard]] constexpr int width() const noexcept { return (fdata_ == nullptr) ? 0 : image_width_; }
    [[nodiscard]] constexpr int height() const noexcept { return (fdata_ == nullptr) ? 0 : image_height_; }

    [[nodiscard]] constexpr color pixel_data(int x, int y) const noexcept
    {
      assert(0 <= x && x <= image_width_);
      assert(0 <= y && y <= image_height_);

      static constexpr color magenta{ 255, 0, 255 };

      if (bdata_ == nullptr) return magenta;

      const unsigned char *pixel = bdata_.get() + static_cast<ptrdiff_t>(y*bytes_per_scanline_) + static_cast<ptrdiff_t>(x*bytes_per_pixel_);
      return color{
        static_cast<double>(pixel[0]),
        static_cast<double>(pixel[1]),
        static_cast<double>(pixel[2])
      };
    }

  private:
    const int bytes_per_pixel_ = 3;
    float *fdata_ = nullptr;
    std::unique_ptr<unsigned char> bdata_ = nullptr;
    int image_width_ = 0;
    int image_height_ = 0;
    int bytes_per_scanline_ = 0;

    [[nodiscard]] static constexpr int clamp(int x, int low, int high) noexcept
    {
      if (x < low) return low;
      if (x < high) return x;
      return high - 1;
    }

    [[nodiscard]] static constexpr unsigned char float_to_byte(float value) noexcept
    {
      if (value <= 0.0)
        return 0;
      if (1.0 <= value)
        return 255;
      return static_cast<unsigned char>(256*value);
    }

    constexpr void convert_to_bytes()
    {
      int total_bytes = image_width_ * image_height_ * bytes_per_pixel_;
      bdata_ = std::make_unique<unsigned char>(total_bytes);

      auto *bptr = bdata_.get();
      auto *fptr = fdata_;
      for (auto i = 0; i < total_bytes; ++i, ++fptr, ++bptr)
        *bptr = float_to_byte(*fptr);
    }
};


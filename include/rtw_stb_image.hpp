#pragma once

#include "color.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

class rtw_image {
  public:
    rtw_image() {}

    explicit
    rtw_image(const char *image_filename)
    {
      auto filename = std::string(image_filename);

      if (load("../images/" + filename)) return;

      std::cerr << "ERROR: could not load image file '" << image_filename << "'.\n";
    }

    ~rtw_image()
    {
      delete[] bdata_;
      STBI_FREE(fdata_);
    }

    [[nodiscard]]
    bool load(const std::string &filename)
    {
      auto n = bytes_per_pixel_;
      fdata_ = stbi_loadf(filename.c_str(), &image_width_, &image_height_, &n, bytes_per_pixel_);
      if (fdata_ == nullptr) return false;

      bytes_per_scanline_ = image_width_ * bytes_per_pixel_;
      convert_to_bytes();
      return true;
    }

    [[nodiscard]]
    int width() const { return (fdata_ == nullptr) ? 0 : image_width_; }

    [[nodiscard]]
    int height() const { return (fdata_ == nullptr) ? 0 : image_height_; }

    [[nodiscard]]
    color pixel_data(int x, int y) const
    {
      assert(0 <= x && x <= image_width_);
      assert(0 <= y && y <= image_height_);

      static color magenta{ 255, 0, 255 };

      if (bdata_ == nullptr) return magenta;

      const unsigned char *pixel = bdata_ + y*bytes_per_scanline_ + x*bytes_per_pixel_;
      return color{
        static_cast<double>(pixel[0]),
        static_cast<double>(pixel[1]),
        static_cast<double>(pixel[2])
      };
    }

  private:
    const int bytes_per_pixel_ = 3;
    float *fdata_ = nullptr;
    unsigned char *bdata_ = nullptr;
    int image_width_ = 0;
    int image_height_ = 0;
    int bytes_per_scanline_ = 0;

    [[nodiscard]] static
    int clamp(int x, int low, int high)
    {
      if (x < low) return low;
      if (x < high) return x;
      return high - 1;
    }

    [[nodiscard]] static
    unsigned char float_to_byte(float value)
    {
      if (value <= 0.0)
        return 0;
      if (1.0 <= value)
        return 255;
      return static_cast<unsigned char>(256*value);
    }

    void convert_to_bytes()
    {
      int total_bytes = image_width_ * image_height_ * bytes_per_pixel_;
      bdata_ = new unsigned char[total_bytes];

      auto *bptr = bdata_;
      auto *fptr = fdata_;
      for (auto i = 0; i < total_bytes; ++i, ++fptr, ++bptr)
        *bptr = float_to_byte(*fptr);
    }
};


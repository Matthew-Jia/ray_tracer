#pragma once

#include "color.hpp"
#include "common.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <chrono>
#include <iostream>

class [[nodiscard]] camera {
	public:
		double	aspect_ratio			= 16.0 / 9.0;
		int			image_width				= 400;
		int			samples_per_pixel = 10;
		int			max_depth					= 10;
    
    double  vfov      = 90;
    point3  lookfrom  = point3{0,0,0};
    point3  lookat    = point3{0,0,-1};
    vec3    vup       = vec3{0,1,0};

    // Defocus is pretty confusing. Here, the idea is to make depth of field, or defocus blur. This means objects
    // near the focal distance are clearer and the objects further from the focal distance show up less clear. 
    // In the real world, this happens when the aperture of the camera is increased, allowing for more light to
    // enter the camera. In this ray tracer, light intensity isn't really a thing, so there really isn't much of
    // a reason to increase defocus other than maybe defocus objects that are further from the focus distance.
    //
    // To simulate defocusing, you make a disk near the center acting as the lens. When sampling rays, instead
    // of having all rays coming from the origin, you sample origins from this disk and shoot them as we did
    // before, by sampling a unit circle around the end location. For objects near the focal plane, the sampled
    // rays might be spaced a bit further apart, but will all be shot at a very small disk which means the rays will
    // all agree heavily with each other, making a more focused object. For objects that are further from the focal
    // plane, rays that result from the contact with be spaced further apart. This results in the rays disagreeing 
    // in color, creating a less focused object. When the defocus angle is small, then the defocus disk is smaller,
    // meaning the sampled origins will be closer together, resulting in less disagreement between rays hitting
    // objects far from the focal distance and so objects away from the focus distance show up clearer. The opposite
    // is true for a larger defocus angle
    double defocus_angle = 0;
    double focus_dist = 10;

		void render (const hittable &world)
		{
      auto start = std::chrono::high_resolution_clock::now();
			initialize();

			std::cout << "P3\n" << image_width << ' ' << image_height_ << "\n255\n";

			for (int j = 0; j < image_height_; ++j)
			{
				std::clog << "\r Scanlines remaining: " << (image_height_ - j) << ' ' << std::flush;
				for (int i = 0; i < image_width; ++i)
				{
					color pixel_color{0, 0, 0};
					for (int sample = 0; sample < samples_per_pixel; ++sample)
					{
						ray r = get_ray(i, j);
						pixel_color += ray_color(r, max_depth, world);
					}
					write_color(std::cout, pixel_color / samples_per_pixel);
				}
			}
			std::clog << "\rDone.\n";
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsed = end - start;
      std::clog << "Function ran for: " << elapsed.count() << " ms\n"; 
		}
		

	private:
		int			image_height_;
		point3	center_;
		point3	pixel00_loc_;
		vec3		pixel_delta_u_;
		vec3		pixel_delta_v_;
    vec3    u_, v_, w_;
    vec3    defocus_disk_u_;
    vec3    defocus_disk_v_;

		void initialize()
		{
			image_height_ = int(image_width / aspect_ratio);
			image_height_ = image_height_ < 1 ? 1 : image_height_;

			center_ = lookfrom;

      auto theta = degrees_to_radians(vfov);
      auto h = std::tan(theta/2);
      auto viewport_height = 2 * h * focus_dist;
			auto viewport_width = viewport_height * (double(image_width) / image_height_);

      w_ = unit_vector(lookfrom - lookat);
      u_ = unit_vector(cross(vup, w_));
      v_ = unit_vector(cross(w_, u_));

			auto viewport_u = u_ * viewport_width;
			auto viewport_v = -v_ * viewport_height;

			pixel_delta_u_ = viewport_u / image_width;
			pixel_delta_v_ = viewport_v / image_height_;

			auto viewport_upper_left = center_ - (w_ * focus_dist) - (viewport_u + viewport_v)/2;
			pixel00_loc_ = viewport_upper_left + (pixel_delta_u_ + pixel_delta_v_)/2;

      auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle/2));
      defocus_disk_u_ = defocus_radius * u_;
      defocus_disk_v_ = defocus_radius * v_;
		}

		ray get_ray(int i, int j) const
		{
			auto offset = sample_square();
			auto pixel_sample = pixel00_loc_
													+ ((i + offset.x) * pixel_delta_u_)
													+ ((j + offset.y) * pixel_delta_v_);
			
			auto ray_origin = (defocus_angle == 0) ? center_ : defocus_disk_sample();
			auto ray_dir = pixel_sample - ray_origin;
      auto ray_time = random_double();
			
			return ray(ray_origin, ray_dir, ray_time);
		}

		vec3 sample_square() const { return vec3{random_double() - 0.5, random_double() - 0.5, 0}; }

    point3 defocus_disk_sample() const
    {
      auto p = random_in_unit_disk();
      return center_ + (p[0] * defocus_disk_u_) + (p[1] * defocus_disk_v_);
    }

		color ray_color(const ray &r, int depth, const hittable &world) const
		{
			assert(depth >= 0);

			if (depth == 0) 
				return color{0, 0, 0};

			hit_record rec;
			if (world.hit(r, interval{0.001, infinity}, rec))
			{
				ray scattered;
				color attenuation;
				if (rec.mat->scatter(r, rec, attenuation, scattered))
					return attenuation * ray_color(scattered, depth-1, world);

				return color{0, 0, 0};
			}

			vec3 unit_direction = unit_vector(r.direction());
			auto a = (unit_direction.y + 1.0)/2;
			return (1.0-a)*color{1.0, 1.0, 1.0} + a*color{0.5, 0.7, 1.0};
		}

};


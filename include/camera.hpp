
#include "color.hpp"
#include "common.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class [[nodiscard]] camera {
	public:
		double	aspect_ratio			= 16.0 / 9.0;
		int			image_width				= 400;
		int			samples_per_pixel = 10;

		void render (const hittable &world)
		{
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
						pixel_color += ray_color(r, world);
					}
					write_color(std::cout, pixel_color / samples_per_pixel);
				}
			}
			std::clog << "\rDone.\n";
		}
		

	private:
		int			image_height_;
		point3	center_;
		point3	pixel00_loc_;
		vec3		pixel_delta_u_;
		vec3		pixel_delta_v_;

		void initialize()
		{
			image_height_ = int(image_width / aspect_ratio);
			image_height_ = image_height_ < 1 ? 1 : image_height_;

			center_ = point3{0, 0, 0};

			auto focal_length = 1.0;
			auto viewport_height = 2.0;
			auto viewport_width = viewport_height * (double(image_width) / image_height_);

			auto viewport_u = vec3{viewport_width, 0, 0};
			auto viewport_v = vec3{0, -viewport_height, 0};

			pixel_delta_u_ = viewport_u / image_width;
			pixel_delta_v_ = viewport_v / image_height_;

			auto viewport_upper_left = center_ - vec3{0, 0, focal_length} - (viewport_u + viewport_v)/2;
			pixel00_loc_ = viewport_upper_left + (pixel_delta_u_ + pixel_delta_v_)/2;
		}

		ray get_ray(int i, int j) const
		{
			auto offset = sample_square();
			auto pixel_sample = pixel00_loc_
													+ ((i + offset.x) * pixel_delta_u_)
													+ ((j + offset.y) * pixel_delta_v_);
			
			auto ray_origin = center_;
			auto ray_dir = pixel_sample - center_;
			
			return ray(ray_origin, ray_dir);
		}

		vec3 sample_square() const { return vec3(random_double() - 0.5, random_double() - 0.5, 0); }

		color ray_color(const ray &r, const hittable &world) const
		{
			hit_record rec;
			if (world.hit(r, interval(0, infinity), rec))
				return (rec.normal + color{1, 1, 1}) / 2;

			vec3 unit_direction = unit_vector(r.direction());
			auto a = (unit_direction.y + 1.0)/2;
			return (1.0-a)*color{1.0, 1.0, 1.0} + a*color{0.5, 0.7, 1.0};
		}

};


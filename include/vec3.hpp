#pragma once

#include "common.hpp"

#include <cmath>
#include <iostream>
#include <cassert>

class [[nodiscard]] vec3 {
public:
	union {
    double data[3]; // Must be raw array for type punning NOLINT(modernize-avoid-c-arrays)
		struct { double x, y, z; };
		struct { double r, g, b; };
	};

	constexpr vec3() noexcept = default;
	constexpr vec3(double e0, double e1, double e2) noexcept : data{e0, e1, e2} {}

	constexpr vec3 operator-() const noexcept { return vec3{-data[0], -data[1], -data[2]}; }
	constexpr double operator[](const int i) const noexcept { return data[i]; }
	constexpr double& operator[](const int i) noexcept { return data[i]; }

	constexpr vec3& operator+=(const vec3 &v) noexcept
	{
		data[0] += v.data[0];
		data[1] += v.data[1];
		data[2] += v.data[2];
		return *this;
	}

	constexpr vec3& operator*=(const double t) noexcept
	{
		data[0] *= t;
		data[1] *= t;
		data[2] *= t;
		return *this;
	}

	constexpr vec3& operator/=(const double t) noexcept { return *this *= 1/t; }

	[[nodiscard]] constexpr double length()         const noexcept { return std::sqrt(length_squared()); }
	[[nodiscard]] constexpr double length_squared() const noexcept { return data[0]*data[0] + data[1]*data[1] + data[2]*data[2]; }

	[[nodiscard]] constexpr bool near_zero() const noexcept
	{
		auto s = 1e-8;
		return (std::fabs(data[0]) < s) && (std::fabs(data[1]) < s) && (std::fabs(data[2]) < s);
	}

	static constexpr vec3 random()                       noexcept	{ return vec3{random_double(), random_double(), random_double()}; }
	static constexpr vec3 random(double min, double max) noexcept	{ return vec3{random_double(min,max), random_double(min,max), random_double(min, max)}; }

};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) { return out << v.data[0] << ' ' << v.data[1] << ' ' << v.data[2]; }
inline constexpr vec3 operator+(const vec3 &u, const vec3 &v) noexcept { return vec3{u.data[0]+v.data[0], u.data[1]+v.data[1], u.data[2]+v.data[2]}; }
inline constexpr vec3 operator-(const vec3 &u, const vec3 &v) noexcept { return u + (-v); }
inline constexpr vec3 operator*(const vec3 &u, const vec3 &v) noexcept { return vec3{u.data[0]*v.data[0], u.data[1]*v.data[1], u.data[2]*v.data[2]}; }
inline constexpr vec3 operator*(double t, const vec3 &v) noexcept { return vec3{t*v.data[0], t*v.data[1], t*v.data[2]}; }
inline constexpr vec3 operator*(const vec3 &v, double t) noexcept { return t * v; }
inline constexpr vec3 operator/(const vec3 &v, double t) noexcept { return (1/t) * v; }
[[nodiscard]] inline constexpr bool operator==(const vec3 &u, const vec3 &v) noexcept { return u.data[0] == v.data[0] && u.data[1] == v.data[1] && u.data[2] == v.data[2]; }
[[nodiscard]] inline constexpr double dot(const vec3 &u, const vec3 &v) noexcept { return u.data[0]*v.data[0] + u.data[1]*v.data[1] + u.data[2]*v.data[2]; }

inline constexpr vec3 cross(const vec3 &u, const vec3 &v) noexcept
{ 
	return vec3{
		u.data[1]*v.data[2] - u.data[2]*v.data[1],
		u.data[2]*v.data[0] - u.data[0]*v.data[2],
		u.data[0]*v.data[1] - u.data[1]*v.data[0],
	};
}

inline constexpr vec3 unit_vector(const vec3 &v) noexcept { return v / v.length(); }

inline constexpr vec3 random_in_unit_disk() noexcept {
	for (;;)
	{
		auto p = vec3{random_double(-1,1), random_double(-1,1), 0};
		if (p.length_squared() < 1) [[likely]]
      return p;
	}
	__builtin_unreachable();
}

inline constexpr vec3 random_unit_vector() noexcept {
	for (;;)
	{
		auto p = vec3::random(-1,1);
		auto lensq = p.length_squared();
		if (1e-160 < lensq && lensq <= 1) [[likely]]
			return p / std::sqrt(lensq);
	}
	__builtin_unreachable();
}

inline constexpr vec3 random_on_hemisphere(const vec3 &normal) noexcept {
	vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0)
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

inline constexpr vec3 reflect(const vec3 &v, const vec3 &n) noexcept { return v - 2*dot(v,n)*n; }

inline constexpr vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat) noexcept
{
  auto cos_theta = std::fmin(dot(-uv, n), 1.0);
  vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
  vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}


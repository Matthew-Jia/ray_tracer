#pragma once

#include "common.hpp"
#include <cmath>
#include <iostream>
#include <cassert>

class [[nodiscard]] vec3 {
public:
	union {
    double data[3]; // Must be raw array for type punning
		struct { double x, y, z; };
		struct { double r, g, b; };
	};

	vec3() {};
	vec3(double e0, double e1, double e2) : data{e0, e1, e2} {}

	vec3 operator-() const { return vec3{-data[0], -data[1], -data[2]}; }
	double operator[](const int i) const { return data[i]; }
	double& operator[](const int i) { return data[i]; }


	vec3& operator+=(const vec3 &v)
	{
		data[0] += v.data[0];
		data[1] += v.data[1];
		data[2] += v.data[2];
		return *this;
	}

	vec3& operator*=(const double t)
	{
		data[0] *= t;
		data[1] *= t;
		data[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) { return *this *= 1/t; }

	[[nodiscard]] double length() const { return std::sqrt(length_squared()); }
	[[nodiscard]] double length_squared() const { return data[0]*data[0] + data[1]*data[1] + data[2]*data[2]; }
	[[nodiscard]] bool near_zero() const
	{
		auto s = 1e-8;
		return (std::fabs(data[0]) < s) && (std::fabs(data[1]) < s) && (std::fabs(data[2]) < s);
	}

	static vec3 random()												{ return vec3{random_double(), random_double(), random_double()}; }
	static vec3 random(double min, double max)	{ return vec3{random_double(min,max), random_double(min,max), random_double(min, max)}; }

};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) { return out << v.data[0] << ' ' << v.data[1] << ' ' << v.data[2]; }
inline vec3 operator+(const vec3 &u, const vec3 &v) { return vec3{u.data[0]+v.data[0], u.data[1]+v.data[1], u.data[2]+v.data[2]}; }
inline vec3 operator-(const vec3 &u, const vec3 &v) { return vec3{u.data[0]-v.data[0], u.data[1]-v.data[1], u.data[2]-v.data[2]}; }
inline vec3 operator*(const vec3 &u, const vec3 &v) { return vec3{u.data[0]*v.data[0], u.data[1]*v.data[1], u.data[2]*v.data[2]}; }
inline vec3 operator*(double t, const vec3 &v) { return vec3{t*v.data[0], t*v.data[1], t*v.data[2]}; }
inline vec3 operator*(const vec3 &v, double t) { return t * v; }
inline vec3 operator/(const vec3 &v, double t) { return (1/t) * v; }

inline double dot(const vec3 &u, const vec3 &v) { return u.data[0]*v.data[0] + u.data[1]*v.data[1] + u.data[2]*v.data[2]; }

inline vec3 cross(const vec3 &u, const vec3 &v) 
{ 
	return vec3{
		u.data[1]*v.data[2] - u.data[2]*v.data[1],
		u.data[2]*v.data[0] - u.data[0]*v.data[2],
		u.data[0]*v.data[1] - u.data[1]*v.data[0],
	};
}

inline vec3 unit_vector(const vec3 &v)
{
  return v / v.length();
}

inline vec3 random_in_unit_disk() {
	for (;;)
	{
		auto p = vec3{random_double(-1,1), random_double(-1,1), 0};
		if (p.length_squared() < 1)
      return p;
	}
	__builtin_unreachable();
}

inline vec3 random_unit_vector() {
	for (;;)
	{
		auto p = vec3::random(-1,1);
		auto lensq = p.length_squared();
		if (1e-160 < lensq && lensq <= 1)
			return p / std::sqrt(lensq);
	}
	__builtin_unreachable();
}

inline vec3 random_on_hemisphere(const vec3 &normal) {
	vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0)
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

inline vec3 reflect(const vec3 &v, const vec3 &n)
{
  return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
{
  auto cos_theta = std::fmin(dot(-uv, n), 1.0);
  vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
  vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}


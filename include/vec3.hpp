#pragma once

#include <cmath>
#include <iostream>
#include <cassert>

class [[nodiscard]] vec3 {
public:
	union {
		double data[3];
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

	double length() const { return std::sqrt(length_squared()); }
	double length_squared() const { return data[0]*data[0] + data[1]*data[1] + data[2]*data[2]; }

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
inline vec3 unit_vector(const vec3 &v) { return v / v.length(); }


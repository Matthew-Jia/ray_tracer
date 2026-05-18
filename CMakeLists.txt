cmake_minimum_required(VERSION 3.15...4.1)

project(RayTracer VERSION 1.0
	DESCRIPTION "Ray Tracer in C++"
	LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(MathLib INTERFACE)
add_executable(raytracer)
add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(raytracer PRIVATE MathLib)



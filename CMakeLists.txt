cmake_minimum_required(VERSION 3.15...4.1)

project(RayTracer VERSION 1.0
	DESCRIPTION "Ray Tracer in C++"
	LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

execute_process(
  COMMAND xcrun --show-sdk-path
  OUTPUT_VARIABLE SDK_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CMAKE_OSX_SYSROOT "${SDK_PATH}")

add_library(project_warnings INTERFACE)
target_compile_options(
  project_warnings INTERFACE
    -Wall
    -Wextra
    -Werror
)

add_library(External INTERFACE)
add_library(MathLib INTERFACE)

add_executable(raytracer)

add_subdirectory(external)
add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(raytracer PRIVATE 
  project_warnings
  External
  MathLib
)



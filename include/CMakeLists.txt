target_sources(
	MathLib INTERFACE
    camera.hpp
    color.hpp
    common.hpp
    hittable.hpp
    hittable_list.hpp
    interval.hpp
    material.hpp
    ray.hpp
    sphere.hpp
    vec3.hpp
)

target_include_directories(MathLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(
	MathLib INTERFACE
    aabb.hpp
    bvh_node.hpp
    camera.hpp
    color.hpp
    common.hpp
    hittable.hpp
    hittable_list.hpp
    interval.hpp
    material.hpp
    perlin.hpp
    quad.hpp
    ray.hpp
    rtw_stb_image.hpp
    sphere.hpp
    texture.hpp
    timer.hpp
    vec3.hpp
)

target_include_directories(MathLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

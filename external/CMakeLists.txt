target_sources(
  External INTERFACE
    stb_image.h
)

target_include_directories(External SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

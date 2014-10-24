cmake_minimum_required(VERSION 2.6)

file(GLOB util_sources *.cpp)
add_library(helios-util ${util_sources})
target_link_libraries(
    helios-util
    ${ImageMagick_LIBRARIES}
    "exiv2"
    )


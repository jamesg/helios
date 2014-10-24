cmake_minimum_required(VERSION 2.6)

file(GLOB uri_sources *.cpp)
add_library(helios-uri ${uri_sources})
target_link_libraries(
    helios-uri
    helios-db
    ${ImageMagick_LIBRARIES}
    "exiv2"
    atlas-com-http-server
    )


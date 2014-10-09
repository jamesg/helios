cmake_minimum_required(VERSION 2.6)

project("Helios")

file(GLOB uri_sources *.cpp)
add_library(helios-uri ${uri_sources})
target_link_libraries(
    helios-uri
    atlas-com-http-server
    )


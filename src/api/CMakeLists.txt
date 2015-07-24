cmake_minimum_required(VERSION 2.6)

project("Helios")

file(GLOB api_sources *.cpp)
add_library(helios-api ${api_sources})
target_link_libraries(
    helios-api
    atlas-com-api
    )

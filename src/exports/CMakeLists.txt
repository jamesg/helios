cmake_minimum_required(VERSION 2.6)

file(GLOB exports_sources *.cpp)
add_library(helios-exports-lib ${exports_sources})
target_link_libraries(
    helios-exports-lib
    helios-db
    helios-util
    ${Boost_LIBRARIES}
    )


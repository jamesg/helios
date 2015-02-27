cmake_minimum_required(VERSION 2.6)

project("Helios")


file(GLOB helios_db_sources *.cpp)
add_library(helios-db ${helios_db_sources})
target_link_libraries(
    helios-db
    atlas-com-common
    atlas-com-db
    hades
    )


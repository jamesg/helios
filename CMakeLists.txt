cmake_minimum_required(VERSION 2.6)

project("Helios")
enable_testing()

find_package(CURL)
find_package(Threads)
find_package(OpenSSL)
find_package(Boost COMPONENTS date_time filesystem random regex system thread)
find_package(ImageMagick COMPONENTS Magick++)

include_directories("atlas-com/lib/sqlite3/include")
include_directories("atlas-com/lib/mongoose/include")
include_directories("atlas-com/src")
include_directories("src")
include_directories("commandline/include")
include_directories("hades/include")
include_directories("styx/include")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIRS})

# Documentation
#find_package(Doxygen)
#if(DOXYGEN_FOUND)
    #configure_file(
        #${CMAKE_CURRENT_SOURCE_DIR}/doxygen/doxygen.conf.in
        #${CMAKE_CURRENT_BINARY_DIR}/doxygen/doxygen.conf @ONLY
        #)
    #add_custom_target(
        #doc
        #${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen/doxygen.conf
        #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        #COMMENT "Generating API documentation with Doxygen" VERBATIM
        #)
#endif(DOXYGEN_FOUND)

# Styx components
add_subdirectory(styx)

# Hades components
add_subdirectory(hades)

# Helios web components
add_subdirectory(web)

# Helios components
add_subdirectory(atlas-com)
add_subdirectory(src)

add_executable(helios src/main/server.cpp)
target_link_libraries(
    helios
    helios-server-lib
    )

add_executable(exports src/main/exports.cpp)
target_link_libraries(
    exports
    helios-exports-lib
    )


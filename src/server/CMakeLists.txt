cmake_minimum_required(VERSION 2.6)

project("Helios")

file(GLOB server_sources *.cpp)
add_library(helios-server-lib ${server_sources})
target_link_libraries(
    helios-server-lib
    helios-api
    helios-db
    atlas-com-api
    atlas-com-http-server
    atlas-com-jsonrpc
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    )


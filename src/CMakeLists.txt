cmake_minimum_required(VERSION 2.6)

project("Helios")

add_subdirectory(api)
add_subdirectory(db)
add_subdirectory(exports)
add_subdirectory(uri)
add_subdirectory(util)

file(GLOB server_sources *.cpp)
add_library(helios-server-lib ${server_sources})
target_link_libraries(
    helios-server-lib
    helios-api
    helios-db
    helios-uri
    ${CMAKE_CURRENT_BINARY_DIR}/../web/static.a
    atlas-com-api
    atlas-auth
    atlas-com-http-server
    atlas-com-jsonrpc
    atlas-web
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    )

cmake_minimum_required(VERSION 2.8)

configure_file(package.json package.json COPYONLY)
add_custom_target(
    node_modules
    COMMAND npm install
    )
# Copy the client source to the web directory to be picked up by Webmake.
add_custom_target(
    copy_web
    DEPENDS copy_atlas_web
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/client ${CMAKE_CURRENT_BINARY_DIR}/client
    )
# Copy Atlas Components web directory over the client.  This makes atlas-com
# web modules available in the same scope as this project's modules.
add_custom_target(
    copy_atlas_web
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../atlas-com/web ${CMAKE_CURRENT_BINARY_DIR}/client
    )

add_custom_target(
    bundle
    DEPENDS node_modules copy_web
    COMMAND
        node_modules/webmake/bin/webmake
        ${CMAKE_CURRENT_BINARY_DIR}/client/index.js
        ${CMAKE_CURRENT_BINARY_DIR}/static/bundle.js
    )

add_custom_target(
    web
    DEPENDS bundle
    COMMAND
        node_modules/uglify-js/bin/uglifyjs
        ${CMAKE_CURRENT_BINARY_DIR}/static/bundle.js >
        ${CMAKE_CURRENT_BINARY_DIR}/static/bundle.min.js
    )
#add_custom_target(
    #web
    #DEPENDS bundle
    #COMMAND ${CMAKE_COMMAND} -E copy
    #${CMAKE_CURRENT_BINARY_DIR}/static/bundle.js
    #${CMAKE_CURRENT_BINARY_DIR}/static/bundle.min.js
    #)

configure_file(static/index.html static/index.html COPYONLY)
configure_file(static/favicon.png static/favicon.png COPYONLY)
configure_file(static/pure-min.css static/pure-min.css COPYONLY)
configure_file(static/grids-responsive-min.css static/grids-responsive-min.css COPYONLY)
configure_file(static/grids-responsive-old-ie-min.css static/grids-responsive-old-ie-min.css COPYONLY)
configure_file(
    static/open-iconic/font/css/open-iconic.css
    static/open-iconic/font/css/open-iconic.css
    COPYONLY
    )
configure_file(
    static/open-iconic/font/fonts/open-iconic.ttf
    static/open-iconic/font/fonts/open-iconic.ttf
    COPYONLY
    )
configure_file(
    static/open-iconic/font/fonts/open-iconic.woff
    static/open-iconic/font/fonts/open-iconic.woff
    COPYONLY
    )


atlas_add_objcopy_target(helios index.html index_html.o)
atlas_add_objcopy_target(helios index.js index_js.o)
atlas_add_objcopy_target(helios application.js application_js.o)
atlas_add_objcopy_target(helios models.js models_js.o)
atlas_add_objcopy_target(helios style.css style_css.o)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/static.a
    COMMAND ar -rc ${CMAKE_CURRENT_BINARY_DIR}/static.a
        ${CMAKE_CURRENT_BINARY_DIR}/index_html.o
        ${CMAKE_CURRENT_BINARY_DIR}/index_js.o
        ${CMAKE_CURRENT_BINARY_DIR}/application_js.o
        ${CMAKE_CURRENT_BINARY_DIR}/models_js.o
        ${CMAKE_CURRENT_BINARY_DIR}/style_css.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Archiving static HTML libraries to ${CMAKE_CURRENT_BINARY_DIR}/static.a"
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/index_html.o
        ${CMAKE_CURRENT_BINARY_DIR}/index_js.o
        ${CMAKE_CURRENT_BINARY_DIR}/application_js.o
        ${CMAKE_CURRENT_BINARY_DIR}/models_js.o
        ${CMAKE_CURRENT_BINARY_DIR}/style_css.o
    )

add_custom_target(helios-web ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/static.a)


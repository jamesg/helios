atlas_add_web_library(
    PREFIX helios
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/static.a
    SOURCES
        index.html
        index.js
        application.js
        models.js
        style.css
        )

add_custom_target(helios-web ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/static.a)


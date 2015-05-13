atlas_add_web_library(
    PREFIX helios
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/static.a
    TARGET helios-web
    SOURCES
        index.html
        index.js
        application.js
        models.js
        style.css
        )


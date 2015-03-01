#include "uri/uri.hpp"

#include "http/server/router.hpp"
#include "http/server/server.hpp"

#include "uri/insert_photograph.hpp"
#include "uri/jpeg_image.hpp"
#include "uri/jpeg_image_fullsize.hpp"

void helios::uri::install(hades::connection& conn, atlas::http::server& server)
{
    server.router().install(
        atlas::http::matcher("/insert_photograph", "post"),
        boost::bind(
            &helios::uri::insert_photograph,
            boost::ref(conn),
            _1,
            _2,
            _3,
            _4
            )
        );
    server.router().install(
        "/jpeg_image",
        boost::bind(
            &helios::uri::jpeg_image,
            boost::ref(conn),
            _1,
            _2,
            _3,
            _4
            )
        );
    server.router().install(
        "/jpeg_image_fullsize",
        boost::bind(
            &helios::uri::jpeg_image_fullsize,
            boost::ref(conn),
            _1,
            _2,
            _3,
            _4
            )
        );
}


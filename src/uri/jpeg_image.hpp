#ifndef HELIOS_URI_JPEG_IMAGE_HPP
#define HELIOS_URI_JPEG_IMAGE_HPP

#include "mongoose.h"

#include "atlas/http/server/uri_type.hpp"

namespace hades
{
    class connection;
}
namespace helios
{
    namespace uri
    {
        void jpeg_image(
                hades::connection& conn,
                mg_connection *mg_conn,
                atlas::http::uri_parameters_type,
                atlas::http::uri_callback_type callback_success,
                atlas::http::uri_callback_type callback_failure
                );
    }
}

#endif

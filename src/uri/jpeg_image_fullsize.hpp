#ifndef HELIOS_URI_JPEG_IMAGE_FULLSIZE_HPP
#define HELIOS_URI_JPEG_IMAGE_FULLSIZE_HPP

#include "mongoose.h"

#include "http/server/uri_type.hpp"

namespace hades
{
    class connection;
}
namespace helios
{
    class server;

    namespace uri
    {
        void jpeg_image_fullsize(
                hades::connection&,
                mg_connection*,
                boost::smatch,
                atlas::http::uri_callback_type callback_success,
                atlas::http::uri_callback_type callback_failure
                );
    }
}

#endif


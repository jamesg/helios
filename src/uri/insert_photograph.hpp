#ifndef HELIOS_URI_INSERT_PHOTOGRAPH
#define HELIOS_URI_INSERT_PHOTOGRAPH

#include "mongoose.h"

#include "http/server/uri_type.hpp"

namespace hades
{
    class connection;
}

namespace helios
{
    namespace uri
    {
        int insert_photograph(
                hades::connection& conn,
                mg_connection *mg_conn,
                atlas::http::uri_callback_type callback_success,
                atlas::http::uri_callback_type callback_failure
                );
    }
}


#endif


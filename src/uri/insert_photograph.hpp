#ifndef HELIOS_URI_INSERT_PHOTOGRAPH
#define HELIOS_URI_INSERT_PHOTOGRAPH

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
        void insert_photograph(
                hades::connection& conn,
                mg_connection *mg_conn,
                boost::smatch,
                atlas::http::uri_callback_type callback_success,
                atlas::http::uri_callback_type callback_failure
                );
    }
}


#endif


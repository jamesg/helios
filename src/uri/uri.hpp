#ifndef HELIOS_URI_URI_HPP
#define HELIOS_URI_URI_HPP

namespace hades
{
    class connection;
}
namespace atlas
{
    namespace http
    {
        class server;
    }
}
namespace helios
{
    namespace uri
    {
        /*!
         * \brief Install Helios custom URIs to a HTTP server.
         */
        void install(hades::connection&, atlas::http::server&);
    }
}

#endif


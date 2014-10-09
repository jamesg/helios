#ifndef HELIOS_API_AUTH_HPP
#define HELIOS_API_AUTH_HPP

#include <string>

#include "styx/styx.hpp"

namespace hades
{
    class connection;
}
namespace atlas
{
    namespace api
    {
        class server;
    }
}

namespace helios
{
    namespace api
    {
        namespace auth
        {
            void install(hades::connection&, atlas::api::server&);
        }
    }
}

#endif


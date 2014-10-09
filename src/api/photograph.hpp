#ifndef HELIOS_API_PHOTOGRAPH_HPP
#define HELIOS_API_PHOTOGRAPH_HPP

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
        namespace photograph
        {
            void install(hades::connection&, atlas::api::server&);
        }
    }
}

#endif


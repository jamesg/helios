#ifndef HELIOS_API_STATISTICS_HPP
#define HELIOS_API_STATISTICS_HPP

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
        namespace statistics
        {
            void install(hades::connection&, atlas::api::server&);
        }
    }
}

#endif


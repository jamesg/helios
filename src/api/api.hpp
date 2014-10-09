#ifndef HELIOS_API_API_HPP
#define HELIOS_API_API_HPP

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
        void install(hades::connection&, atlas::api::server&);
    }
}

#endif


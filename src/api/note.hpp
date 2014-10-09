#ifndef HELIOS_API_NOTE_HPP
#define HELIOS_API_NOTE_HPP

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
        namespace note
        {
            void install(hades::connection&, atlas::api::server&);
        }
    }
}

#endif


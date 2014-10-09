#ifndef HELIOS_DB_CACHE_HPP
#define HELIOS_DB_CACHE_HPP

namespace hades
{
    class connection;
}
namespace helios
{
    namespace db
    {
        namespace cache
        {
            void create(hades::connection&);
        }
    }
}

#endif


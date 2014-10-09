#ifndef HELIOS_DB_CREATE_HPP
#define HELIOS_DB_CREATE_HPP

namespace hades
{
    class connection;
}
namespace helios
{
    namespace db
    {
        void create(hades::connection& conn);
    }
}

#endif


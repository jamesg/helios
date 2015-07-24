#ifndef HELIOS_DB_CACHE_HPP
#define HELIOS_DB_CACHE_HPP

#include <vector>

#include "styx/styx.hpp"

namespace hades
{
    class connection;
}
namespace helios
{
    struct jpeg_cache_db
    {
        styx::int_type photograph_id;
        styx::int_type height;
        styx::int_type width;
        std::vector<unsigned char> data;

        jpeg_cache_db() :
            photograph_id(0),
            height(0),
            width(0)
        {
        }
    };
    namespace db
    {
        namespace cache
        {
            void create(hades::connection&);
            bool has(
                    hades::connection&,
                    styx::int_type photograph_id,
                    styx::int_type height,
                    styx::int_type width
                    );
            helios::jpeg_cache_db get(
                    hades::connection&,
                    styx::int_type photograph_id,
                    styx::int_type height,
                    styx::int_type width
                    );
            void insert(jpeg_cache_db&, hades::connection&);
        }
    }
}

#endif

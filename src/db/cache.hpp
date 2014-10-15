#ifndef HELIOS_DB_CACHE_HPP
#define HELIOS_DB_CACHE_HPP

#include <vector>

namespace hades
{
    class connection;
}
namespace helios
{
    struct jpeg_cache_db
    {
        int photograph_id;
        int height;
        int width;
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
                    int photograph_id,
                    int height,
                    int width
                    );
            helios::jpeg_cache_db get(
                    hades::connection&,
                    int photograph_id,
                    int height,
                    int width
                    );
            void insert(jpeg_cache_db&, hades::connection&);
        }
    }
}

#endif


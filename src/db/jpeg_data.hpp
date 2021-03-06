#ifndef HELIOS_DB_JPEG_DATA_HPP
#define HELIOS_DB_JPEG_DATA_HPP

#include <vector>

#include "styx/styx.hpp"

namespace hades
{
    class connection;
}

namespace helios
{
    /*!
     * \brief Raw JPEG data for a photograph.
     */
    struct jpeg_data_db
    {
        styx::int_type photograph_id;
        std::vector<unsigned char> data;

        jpeg_data_db() :
            photograph_id(0)
        {
        }
    };
    namespace db
    {
        namespace jpeg_data
        {
            jpeg_data_db get_by_id(hades::connection&, styx::int_type);
            void insert(const jpeg_data_db&, hades::connection&);
        }
    }
}

#endif

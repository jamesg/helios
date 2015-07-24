#ifndef HELIOS_DB_STATISTICS_HPP
#define HELIOS_DB_STATISTICS_HPP

#include "hades/tuple.hpp"

namespace hades
{
    class connection;
}
namespace helios
{
    namespace db
    {
        namespace attr
        {
            namespace statistics
            {
                extern const char total_count[];
            }
        }
    }

    struct statistics :
        public hades::tuple<db::attr::statistics::total_count>
    {
        statistics()
        {
        }
        statistics(const styx::element& e) :
            styx::object(e)
        {
        }
        /*!
         * \brief The total number of photographs stored in the database.
         */
        styx::int_type& total_count()
        {
            return get_int<db::attr::statistics::total_count>();
        }
    };

    namespace db
    {
        helios::statistics get_statistics(hades::connection&);
    }
}

#endif

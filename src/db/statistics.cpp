#include "statistics.hpp"

#include "hades/custom_select.hpp"

const char helios::db::attr::statistics::total_count[] = "total_count";

helios::statistics helios::db::get_statistics(hades::connection& conn)
{
    styx::list stats = hades::custom_select<
        helios::statistics,
        db::attr::statistics::total_count
        >(
            conn,
            "SELECT COUNT(photograph_id) AS total_count FROM helios_photograph "
            );
    if(stats.size() > 0)
        return helios::statistics(stats[0]);
    else
        throw std::runtime_error("computing statistics");
}


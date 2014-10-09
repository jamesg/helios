#include "statistics.hpp"

#include "api/server.hpp"
#include "db/statistics.hpp"

void helios::api::statistics::install(
        hades::connection& conn,
        atlas::api::server& server
        )
{
    server.install<styx::element>(
            "statistics",
            [&conn]() {
                helios::statistics s = db::get_statistics(conn);
                return s.get_element();
            }
            );
}


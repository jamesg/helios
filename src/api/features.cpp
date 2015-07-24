#include "features.hpp"

#include <boost/scoped_ptr.hpp>

#include "hades/crud.ipp"

#include "atlas/api/server.hpp"

namespace
{
    static boost::scoped_ptr<helios::features> g_features;
}

helios::features helios::get_features()
{
    if(!g_features)
        g_features.reset(new helios::features);

    return *g_features;
}

void helios::api::features::install(
        hades::connection& conn,
        atlas::api::server& server
        )
{
    server.install<styx::element, styx::int_type>(
            "features",
            [&conn](styx::int_type id) {
                return get_features();
            }
            );
}

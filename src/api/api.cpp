#include "api.hpp"

#include "api/server.hpp"

#include "auth.hpp"
#include "features.hpp"
#include "note.hpp"
#include "photograph.hpp"
#include "statistics.hpp"

void helios::api::install(
        hades::connection& conn,
        atlas::api::server& server
        )
{
    helios::api::auth::install(conn, server);
    helios::api::features::install(conn, server);
    helios::api::note::install(conn, server);
    helios::api::photograph::install(conn, server);
    helios::api::statistics::install(conn, server);
}


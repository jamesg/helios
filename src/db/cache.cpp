#include "cache.hpp"

#include "hades/devoid.hpp"

void helios::db::cache::create(hades::connection& conn)
{
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS jpeg_cache ( "
            " photograph_id INTEGER, "
            " width INTEGER, "
            " height INTEGER, "
            " data BLOB, "
            " PRIMARY KEY(photograph_id, width, height) "
            " ) ",
            conn
            );
}


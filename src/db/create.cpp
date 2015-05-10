#include "helios/db/create.hpp"

#include "atlas/db/auth.hpp"
#include "cache.hpp"
#include "note.hpp"
#include "photograph.hpp"

void helios::db::create(hades::connection& conn)
{
    atlas::db::auth::create(conn);
    cache::create(conn);
    note::create(conn);
    photograph::create(conn);
}


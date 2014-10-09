#include "create.hpp"

#include "auth.hpp"
#include "cache.hpp"
#include "note.hpp"
#include "photograph.hpp"

void helios::db::create(hades::connection& conn)
{
    auth::create(conn);
    cache::create(conn);
    note::create(conn);
    photograph::create(conn);
}


#include "cache.hpp"

#include <sqlite3.h>

#include "hades/connection.hpp"
#include "hades/devoid.hpp"
#include "hades/step.hpp"

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

bool helios::db::cache::has(
        hades::connection& conn,
        int photograph_id,
        int height,
        int width
        )
{
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            conn.handle(),
            "SELECT COUNT(photograph_id) FROM jpeg_cache "
            "WHERE photograph_id = ? AND "
            " height = ? AND "
            " width = ? ",
            -1,
            &stmt,
            nullptr
            );
    sqlite3_bind_int(stmt, 1, photograph_id);
    sqlite3_bind_int(stmt, 2, height);
    sqlite3_bind_int(stmt, 3, width);
    hades::step(stmt);
    int c = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return (c == 1);
}

helios::jpeg_cache_db helios::db::cache::get(
        hades::connection& conn,
        int photograph_id,
        int height,
        int width
        )
{
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            conn.handle(),
            "SELECT photograph_id, data FROM jpeg_cache "
            "WHERE photograph_id = ? AND "
            " height = ? AND "
            " width = ? ",
            -1,
            &stmt,
            nullptr
            );
    sqlite3_bind_int(stmt, 1, photograph_id);
    sqlite3_bind_int(stmt, 2, height);
    sqlite3_bind_int(stmt, 3, width);
    hades::step(stmt);
    helios::jpeg_cache_db out;
    out.photograph_id = sqlite3_column_int(stmt, 0);
    out.height = height;
    out.width = width;
    out.data = std::vector<unsigned char>(
            (unsigned char*)sqlite3_column_blob(stmt, 1),
            (unsigned char*)sqlite3_column_blob(stmt, 1) + sqlite3_column_bytes(stmt, 1)
            );
    sqlite3_finalize(stmt);
    return out;
}

void helios::db::cache::insert(
        helios::jpeg_cache_db& data,
        hades::connection& conn
        )
{
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            conn.handle(),
            "INSERT INTO jpeg_cache(photograph_id, height, width, data) VALUES (?, ?, ?, ?)",
            -1,
            &stmt,
            nullptr
            );
    hades::bind(1, data.photograph_id, stmt);
    hades::bind(2, data.height, stmt);
    hades::bind(3, data.width, stmt);
    hades::bind(4, &(data.data[0]), data.data.size(), stmt);
    hades::step(stmt);
    sqlite3_finalize(stmt);
}


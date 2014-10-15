#include "jpeg_data.hpp"

#include <sqlite3.h>

#include "hades/bind_values.hpp"
#include "hades/connection.hpp"
#include "hades/step.hpp"

helios::jpeg_data_db helios::db::jpeg_data::get_by_id(
        hades::connection& conn,
        int photograph_id
        )
{
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            conn.handle(),
            "SELECT photograph_id, data FROM jpeg_data WHERE photograph_id = ?",
            -1,
            &stmt,
            nullptr
            );
    sqlite3_bind_int(stmt, 1, photograph_id);
    hades::step(stmt);
    helios::jpeg_data_db out;
    out.photograph_id = sqlite3_column_int(stmt, 0);
    out.data = std::vector<unsigned char>(
            (unsigned char*)sqlite3_column_blob(stmt, 1),
            (unsigned char*)sqlite3_column_blob(stmt, 1) + sqlite3_column_bytes(stmt, 1)
            );
    return out;
}

void helios::db::jpeg_data::insert(
        const helios::jpeg_data_db& data,
        hades::connection& conn
        )
{
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            conn.handle(),
            "INSERT INTO jpeg_data(photograph_id, data) VALUES (?, ?)",
            -1,
            &stmt,
            nullptr
            );
    hades::bind(1, data.photograph_id, stmt);
    hades::bind(2, &(data.data[0]), data.data.size(), stmt);
    hades::step(stmt);
    sqlite3_finalize(stmt);
}


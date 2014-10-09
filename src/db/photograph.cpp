#include "photograph.hpp"

#include "hades/crud.ipp"
#include "hades/devoid.hpp"
#include "hades/join.hpp"

const char helios::db::attr::photograph::photograph_id[] = "photograph_id";
const char helios::db::attr::photograph::title[] = "title";
const char helios::db::attr::photograph::caption[] = "caption";
const char helios::db::attr::photograph::filename[] = "filename";
const char helios::db::attr::photograph::taken[] = "taken";
const char helios::db::attr::photograph_tagged::tag[] = "tag";
const char helios::db::attr::photograph_location::location[] = "location";
const char helios::db::attr::album::album_id[] = "album_id";
const char helios::db::attr::album::name[] = "name";
const char helios::db::relvar::photograph[] = "photograph";
const char helios::db::relvar::photograph_tagged[] = "photograph_tagged";
const char helios::db::relvar::photograph_in_album[] = "photograph_in_album";
const char helios::db::relvar::album[] = "album";

void helios::db::photograph::create(hades::connection& conn)
{
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS  photograph          ( "
            "    photograph_id           INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    title                   VARCHAR NOT NULL, "
            "    caption                 VARCHAR NULL, "
            "    filename                VARCHAR NOT NULL, "
            "    location                VARCHAR NULL, "
            "    taken                   VARCHAR NULL, "
            "    UNIQUE(photograph_id, title, caption, filename, location, taken) "
            "    ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS  jpeg_data           ( "
            "    photograph_id           INTEGER PRIMARY KEY, "
            "    data                    BLOB NOT NULL, "
            "    FOREIGN KEY( photograph_id ) REFERENCES photograph(photograph_id) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED "
            "    ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS  album               ( "
            "    album_id                INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    name                    VARCHAR NOT NULL, "
            "    caption                 VARCHAR NULL, "
            "    UNIQUE(name) "
            "    ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS  photograph_tagged    ( "
            "    photograph_id           INTEGER NOT NULL, "
            "    tag                     VARCHAR NOT NULL, "
            "    FOREIGN KEY( photograph_id ) REFERENCES photograph( photograph_id ) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    UNIQUE(photograph_id, tag) "
            "    ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS  photograph_in_album   ( "
            "    photograph_id           INTEGER NOT NULL, "
            "    album_id                INTEGER NOT NULL, "
            "    FOREIGN KEY( photograph_id ) REFERENCES photograph( photograph_id ) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    FOREIGN KEY( album_id )      REFERENCES album( album_id ) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    UNIQUE(photograph_id, album_id) "
            "    ) ",
            conn
            );
}

styx::list helios::db::get_photographs_by_album(
        hades::connection& conn,
        const int album_id
        )
{
    boost::fusion::vector<int> params(album_id);
    return hades::join<helios::photograph, helios::photograph_in_album>(
            conn,
            hades::where<int>("album_id = ?", hades::row<int>(album_id))
            );
}


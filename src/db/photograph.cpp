#define HADES_ENABLE_DEBUGGING
#include "photograph.hpp"

#include <boost/tokenizer.hpp>

#include "hades/crud.ipp"
#include "hades/devoid.hpp"
#include "hades/join.hpp"

const char helios::db::attr::photograph::photograph_id[] = "photograph_id";
const char helios::db::attr::photograph::title[] = "title";
const char helios::db::attr::photograph::caption[] = "caption";
const char helios::db::attr::photograph::taken[] = "taken";
const char helios::db::attr::photograph_tagged::tag[] = "tag";
const char helios::db::attr::photograph_location::location[] = "location";
const char helios::db::attr::album::album_id[] = "album_id";
const char helios::db::attr::album::name[] = "name";
const char helios::db::attr::tag::tag[] = "tag";
const char helios::db::attr::tag::photograph_count[] = "photograph_count";
const char helios::db::attr::location::location[] = "location";
const char helios::db::attr::location::photograph_count[] = "photograph_count";
const char helios::db::relvar::photograph[] = "photograph";
const char helios::db::relvar::photograph_tagged[] = "photograph_tagged";
const char helios::db::relvar::photograph_location[] = "photograph_location";
const char helios::db::relvar::photograph_in_album[] = "photograph_in_album";
const char helios::db::relvar::album[] = "album";

void helios::db::photograph::create(hades::connection& conn)
{
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS photograph ( "
            " photograph_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            " title VARCHAR NOT NULL, "
            " caption VARCHAR NULL, "
            " taken VARCHAR NULL "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS photograph_location ( "
            " photograph_id INTEGER PRIMARY KEY, "
            " location VARCHAR, "
            " FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id) "
            "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS jpeg_data ( "
            " photograph_id INTEGER PRIMARY KEY, "
            " data BLOB NOT NULL, "
            " FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id) "
            "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS album ( "
            " album_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            " name VARCHAR NOT NULL, "
            " caption VARCHAR NULL, "
            " UNIQUE(name) "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS photograph_tagged ( "
            " photograph_id INTEGER NOT NULL, "
            " tag VARCHAR NOT NULL, "
            " FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id) "
            "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            " UNIQUE(photograph_id, tag) "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS photograph_in_album ( "
            " photograph_id INTEGER NOT NULL, "
            " album_id INTEGER NOT NULL, "
            " FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id) "
            "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            " FOREIGN KEY(album_id) REFERENCES album(album_id) "
            "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            " UNIQUE(photograph_id, album_id) "
            " ) ",
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

std::vector<std::string> helios::db::photograph_tags(
        hades::connection& conn,
        helios::photograph::id_type id
        )
{
    auto where = hades::where<int>(
        "photograph_id = ?",
        hades::row<int>(
            id.get_int<db::attr::photograph::photograph_id>()
            )
        );
    styx::list tags = helios::photograph_tagged::get_collection(conn, where);
    std::vector<std::string> out;
    for(styx::element e : tags)
        out.push_back(
                helios::photograph_tagged(e)
                    .get_string<db::attr::photograph_tagged::tag>()
                );
    return out;
}

void helios::db::set_photograph_tags(
        hades::connection& conn,
        helios::photograph::id_type id,
        std::vector<std::string> tags
        )
{
    hades::transaction tr(conn, "helios_db_set_photograph_tags");
    hades::devoid(
            "DELETE FROM photograph_tagged WHERE photograph_id = ?",
            hades::row<int>(id.get_int<db::attr::photograph::photograph_id>()),
            conn
            );
    for(const std::string& tag : tags)
    {
        helios::photograph_tagged t;
        t.get_int<db::attr::photograph::photograph_id>() =
            id.get_int<db::attr::photograph::photograph_id>();
        t.get_string<db::attr::photograph_tagged::tag>() = tag;
        t.insert(conn);
    }
    tr.commit();
}

void helios::db::set_photograph_tags(
        hades::connection& conn,
        helios::photograph::id_type id,
        std::string tags
        )
{
    std::vector<std::string> tags_vector;
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
    tokenizer t(tags, boost::escaped_list_separator<char>());
    for(auto it = t.begin(); it != t.end(); ++it)
        tags_vector.push_back(*it);
    set_photograph_tags(conn, id, tags_vector);
}


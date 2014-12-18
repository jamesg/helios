#include "note.hpp"

#include <locale>

#include <boost/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/fusion/include/vector.hpp>

#include "hades/crud.ipp"
#include "hades/devoid.hpp"
#include "hades/join.hpp"
#include "hades/transaction.hpp"

#include "api/server.hpp"
#include "db/note.hpp"

void helios::api::note::install(
        hades::connection& conn,
        atlas::api::server& server
        )
{
    server.install<styx::element, int, int>(
        "markdown_data_get",
        [&conn](const int markdown_id, const int phase) {
            return styx::null();
        }
        );
    server.install<styx::element, styx::element>(
        "markdown_data_update",
        [&conn](const styx::element markdown_e) {
            helios::markdown markdown(markdown_e);
            markdown.save(conn);
            return markdown;
        }
        );
    server.install<styx::element, int>(
        "note_get",
        [&conn](const int id) {
            helios::note note;
            note.from_id(conn, helios::note::id_type{id});
            return note;
        }
        );
    server.install<styx::list>(
        "note_list",
        boost::bind(helios::note::get_collection, boost::ref(conn))
        );
    server.install<styx::element, int, int>(
        "note_version_get",
        [&conn](const int note_id, const int phase) {
            helios::note_version note_version;
            note_version.from_id(conn, helios::note_version::id_type{note_id, phase});
            return note_version;
        }
        );
    server.install<styx::element, styx::element>(
        "note_save",
        [&conn](const styx::element note_e) {
            helios::note note(note_e);
            note.save(conn);
            return note;
        }
        );
    server.install<styx::element, std::string>(
        "note_create_draft",
        [&conn](const std::string title) {
            hades::transaction tr(conn, "note_create_draft");
            helios::note note;
            note.save(conn);

            helios::note_version note_version;
            note_version.get_int<db::attr::note::note_id>() =
                note.get_int<db::attr::note::note_id>();
            note_version.get_int<db::attr::note_version::phase>() =
                helios::note_version::draft;
            note_version.get_string<db::attr::note_version::title>() = title;
            // TODO: note temporal

            tr.commit();
            return note_version;
        }
        );
    server.install<bool, int>(
        "note_destroy",
        [&conn](const int note_id) {
            helios::note note;
            note.from_id(conn, helios::note::id_type{note_id});
            return note.destroy(conn);
        }
        );
    server.install<styx::list>(
        "note_published_list",
        boost::bind(
            &hades::join<helios::note, helios::note_version>,
            boost::ref(conn),
            hades::where(
                "note.note_id = note_version.note_id AND note.phase = ?",
                hades::row<int>(helios::note_version::published)
                )
            )
        );
    server.install<styx::null, int>(
        "note_publish",
        [&conn](const int note_id) {
            helios::note note;
            note.from_id(conn, helios::note::id_type{note_id});
            return styx::null();
        }
        );
        //// Delete any old note versions.
        //db::delete_note_version(
                //note_id,
                //::photoalbum::note_version::published,
                //conn
                //);

        //// Copy the markdown data to a new markdown row.
        //sqlite::devoid(
                //"INSERT INTO markdown(data) "
                //"SELECT data "
                //"FROM markdown NATURAL JOIN note_version "
                //"WHERE note_id = ? AND phase = ? ",
                //boost::fusion::vector<int, int>(
                    //note_id,
                    //note_version::draft
                    //),
                //conn
                //);
        //// Copy the note version.
        //boost::gregorian::date today =
            //boost::date_time::day_clock<boost::gregorian::date>::local_day();
        //sqlite::devoid(
                //"INSERT INTO note_version( "
                //" note_id, markdown_id, phase, modified) "
                //"VALUES(?, last_insert_rowid(), ?, ?) ",
                //boost::fusion::vector<int, int, std::string>(
                    //note_id,
                    //::photoalbum::note_version::published,
                    //boost::gregorian::to_iso_extended_string(today)
                    //),
                //conn
                //);
        //// Copy photographs used by the note version.
        //sqlite::devoid(
                //"INSERT INTO note_version_uses_photograph( "
                //" note_version_id, photograph_id, filename "
                //" ) "
                //"SELECT note_version_id AS a, photograph_id AS b, "
                //" filename AS c "
                //"FROM note_version_uses_photograph "
                //"NATURAL JOIN note_version "
                //"WHERE note_id = ? AND phase = ? ",
                //boost::fusion::vector<int, int>(
                    //note_id,
                    //::photoalbum::note_version::draft
                    //),
                //conn
                //);
}


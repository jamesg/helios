#include "note.hpp"

#include "hades/crud.ipp"
#include "hades/devoid.hpp"
#include "hades/join.hpp"

const char helios::db::attr::note::note_id[] = "note_id";
const char helios::db::attr::note_version::title[] = "title";
const char helios::db::attr::note_version::phase[] = "phase";
const char helios::db::attr::markdown::markdown_id[] = "markdown_id";
const char helios::db::attr::markdown::markdown_data[] = "markdown_data";
const char helios::db::relvar::note[] = "note";
const char helios::db::relvar::note_version[] = "note_version";
const char helios::db::relvar::note_version_modified[] = "note_version_modified";
const char helios::db::relvar::note_created[] = "note_created";
const char helios::db::relvar::markdown[] = "markdown";

void helios::db::note::create(hades::connection& conn)
{
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS note ( "
            "    note_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    created VARCHAR NOT NULL, "
            "    title   INTEGER NOT NULL "
            "    ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS note_version ( "
            "    note_version_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    note_id         INTEGER NOT NULL, "
            "    markdown_id     INTEGER NOT NULL, "
            "    phase           INTEGER NOT NULL, "
            "    FOREIGN KEY(note_id) REFERENCES note(note_id) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    FOREIGN KEY(markdown_id) REFERENCES markdown(markdown_id) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    UNIQUE(note_id, phase) "
            "    ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS note_version_modified ( "
            "    note_version_id PRIMARY KEY REFERENCES note_version(note_version_id), "
            "    date VARCHAR "
            "    ) ",
            conn
            );
    hades::devoid(
            "CREATE TRIGGER IF NOT EXISTS note_version_delete_markdown "
            "AFTER DELETE ON note_version "
            "FOR EACH ROW "
            "    BEGIN "
            "        DELETE FROM markdown "
            "        WHERE markdown_id = OLD.markdown_id; "
            "    END ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS markdown ( "
            "    markdown_id             INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    data                    VARCHAR NOT NULL DEFAULT '' "
            "    ) ",
            conn
            );
}

styx::list helios::db::note::published_notes(hades::connection& conn)
{
    hades::join<helios::note, helios::note_version>(
            conn,
            hades::where<int>(
                "note.note_id = note_version.note_id AND phase = ? ",
                hades::row<int>(note_version::published)
                )
            );
}


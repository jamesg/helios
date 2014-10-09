#ifndef HELIOS_DB_NOTE_HPP
#define HELIOS_DB_NOTE_HPP

#include "hades/crud.hpp"
#include "hades/has_candidate_key.hpp"
#include "hades/relation.hpp"
#include "hades/tuple.hpp"

namespace helios
{
    namespace db
    {
        namespace attr
        {
            namespace note
            {
                extern const char note_id[];
            }
            namespace note_version
            {
                extern const char title[];
                extern const char phase[];
            }
            namespace markdown
            {
                extern const char markdown_id[];
                extern const char markdown_data[];
            }
        }
        namespace relvar
        {
            extern const char note[];
            extern const char note_version[];
            extern const char note_version_modified[];
            extern const char note_created[];
            extern const char markdown[];
        }
    }

    struct note :
        public hades::has_candidate_key<db::attr::note::note_id>,
        public hades::tuple<db::attr::note::note_id>,
        public hades::relation<db::relvar::note>,
        public hades::crud<note>
    {
        note()
        {
        }
        note(const styx::element& e) :
            styx::object_accessor(e)
        {
        }
        //std::string& title()    const { return get_string("title"); }
        /*!
         * Date the note was created as an ISO date (YYYY-MM-DD).
         */
        //std::string& created()  const { return get_string("created"); }
    };

    struct note_version :
        public hades::has_candidate_key<
            db::attr::note::note_id,
            db::attr::note_version::phase>,
        public hades::tuple<
            db::attr::note::note_id,
            db::attr::note_version::phase,
            db::attr::note_version::title>,
        public hades::relation<db::relvar::note_version>,
        public hades::crud<note_version>
    {
        enum phase_type
        {
            draft     = 0,
            published = 1
        };

        note_version()
        {
        }
        note_version(const styx::element& e) :
            styx::object_accessor(e)
        {
        }
    };

    /*!
     * Storage for markdown data.
     */
    struct markdown :
        public hades::has_candidate_key<db::attr::markdown::markdown_id>,
        public hades::tuple<
            db::attr::markdown::markdown_id,
            db::attr::markdown::markdown_data>,
        public hades::relation<db::relvar::markdown>,
        public hades::crud<markdown>
    {
        markdown()
        {
        }
        markdown(const styx::element& e) :
            styx::object_accessor(e)
        {
        }
    };

    namespace db
    {
        namespace note
        {
            void create(hades::connection&);
            styx::list published_notes(hades::connection& conn);
        }
    }
}

#endif


#ifndef HELIOS_DB_PHOTOGRAPH_HPP
#define HELIOS_DB_PHOTOGRAPH_HPP

#include <vector>

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
            namespace photograph
            {
                extern const char photograph_id[];
                extern const char title[];
                extern const char caption[];
                extern const char taken[];
            }
            namespace photograph_tagged
            {
                extern const char tag[];
            }
            namespace photograph_location
            {
                extern const char location[];
            }
            namespace album
            {
                extern const char album_id[];
                extern const char name[];
            }
            namespace location
            {
                extern const char location[];
                extern const char photograph_count[];
            }
            namespace tag
            {
                extern const char tag[];
                extern const char photograph_count[];
            }
        }
        namespace relvar
        {
            extern const char photograph[];
            extern const char photograph_tagged[];
            extern const char photograph_location[];
            extern const char photograph_in_album[];
            extern const char album[];
        }
    }
    struct photograph :
        public hades::has_candidate_key<db::attr::photograph::photograph_id>,
        public hades::tuple<
            db::attr::photograph::photograph_id,
            db::attr::photograph::title,
            db::attr::photograph::caption,
            db::attr::photograph::taken>,
        public hades::relation<db::relvar::photograph>,
        public hades::crud<photograph>
    {
        photograph()
        {
        }
        photograph(const styx::element& e) :
            styx::object(e)
        {
        }
        std::string& tags() { return styx::object::get_string("tags"); }
    };
    struct photograph_in_album :
        public hades::has_candidate_key<
            db::attr::photograph::photograph_id,
            db::attr::album::album_id>,
        public hades::tuple<
            db::attr::photograph::photograph_id,
            db::attr::album::album_id>,
        public hades::relation<db::relvar::photograph_in_album>,
        public hades::crud<photograph_in_album>
    {
        photograph_in_album()
        {
        }
        photograph_in_album(styx::element& e) :
            styx::object(e)
        {
        }
        photograph_in_album(id_type id) :
            styx::object(id)
        {
        }
        //int& photograph_id() const { return get_int("photograph_id"); }
        //int& album_id() const { return get_int("album_id"); }
    };
    struct photograph_location :
        public hades::has_candidate_key<
            db::attr::photograph::photograph_id>,
        public hades::tuple<
            db::attr::photograph::photograph_id,
            db::attr::photograph_location::location>,
        public hades::relation<db::relvar::photograph_location>,
        public hades::crud<photograph_location>
    {
        photograph_location()
        {
        }
        photograph_location(const styx::element& e) :
            styx::object(e)
        {
        }
    };
    struct photograph_tagged :
        public hades::has_candidate_key<
            db::attr::photograph::photograph_id,
            db::attr::photograph_tagged::tag>,
        public hades::tuple<
            db::attr::photograph::photograph_id,
            db::attr::photograph_tagged::tag>,
        public hades::relation<db::relvar::photograph_tagged>,
        public hades::crud<photograph_tagged>
    {
        photograph_tagged()
        {
        }
        photograph_tagged(styx::element& e) :
            styx::object(e)
        {
        }
        //int& photograph_id() const { return get_int("photograph_id"); }
        //std::string& tag() const { return get_string("tag"); }
    };
    struct album :
        public hades::has_candidate_key<db::attr::album::album_id>,
        public hades::tuple<
            db::attr::album::album_id,
            db::attr::album::name>,
        public hades::relation<db::relvar::album>,
        public hades::crud<album>
    {
        album()
        {
        }
        album(const styx::element& e) :
            styx::object(e)
        {
        }
        //std::string& name() const { return get_string("name"); }
        //std::string& caption() const { return get_string("caption"); }
        //std::string& fromdate() const { return get_string("fromdate"); }
        //std::string& todate() const { return get_string("todate"); }
    };
    /*!
     * \brief View onto photograph_location.
     */
    struct location :
        public hades::tuple<
            db::attr::location::location,
            db::attr::location::photograph_count>,
        public hades::relation<db::relvar::photograph_location>
    {
    };
    /*!
     * \brief Location details.
     */
    struct basic_location :
        public hades::tuple<db::attr::location::location>,
        public hades::relation<db::relvar::photograph_location>
    {
    };
    /*!
     * \brief View onto photograph_tagged.
     */
    struct tag :
        public hades::tuple<
            db::attr::tag::tag,
            db::attr::tag::photograph_count>,
        public hades::relation<db::relvar::photograph_tagged>
    {
    };
    /*!
     * \brief Tag details.
     */
    struct basic_tag :
        public hades::tuple<db::attr::tag::tag>,
        public hades::relation<db::relvar::photograph_tagged>
    {
    };

    namespace db
    {
        namespace photograph
        {
            void create(hades::connection& photograph_db);
        }

        styx::list get_photographs_by_album(
                hades::connection&,
                const styx::int_type album_id
                );
        std::vector<std::string> photograph_tags(
                hades::connection&,
                helios::photograph::id_type
                );
        void set_photograph_tags(
                hades::connection&,
                helios::photograph::id_type,
                std::vector<std::string> tags
                );
        void set_photograph_tags(
                hades::connection&,
                helios::photograph::id_type,
                std::string tags
                );
    }
}

#endif

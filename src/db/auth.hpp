#ifndef HELIOS_DB_AUTH_HPP
#define HELIOS_DB_AUTH_HPP

#include "hades/crud.hpp"
#include "hades/flag.hpp"
#include "hades/has_candidate_key.hpp"
#include "hades/has_flags.hpp"
#include "hades/tuple.hpp"
#include "hades/relation.hpp"

#include "db/temporal.hpp"

namespace helios
{
    namespace db
    {
        namespace attr
        {
            namespace user
            {
                extern const char user_id[];
                extern const char username[];
            }
            namespace user_password
            {
                extern const char password[];
            }
            namespace user_permission
            {
                extern const char permission[];
            }
            namespace auth_token
            {
                extern const char token[];
                extern const char created[];
                extern const char expires[];
            }
        }
        namespace flag
        {
            namespace user
            {
                extern const char enabled[];
                extern const char super[];
            }
        }
        namespace relvar
        {
            extern const char user[];
            extern const char user_created[];
            extern const char user_password[];
            extern const char user_permission[];
            extern const char auth_token[];
        }
    }

    struct user :
        public hades::has_candidate_key<db::attr::user::user_id>,
        public hades::tuple<
            db::attr::user::user_id,
            db::attr::user::username>,
        public hades::relation<db::relvar::user>,
        public hades::has_flags<
            db::flag::user::enabled,
            db::flag::user::super>,
        public hades::crud<user>
    {
        user()
        {
        }
        user(const styx::element& e) :
            styx::object_accessor(e)
        {
        }
    };
    typedef atlas::db::semi_temporal<user::id_type, db::relvar::user_created>
        user_created;
    typedef hades::flag<user::id_type, db::flag::user::enabled> user_enabled;
    typedef hades::flag<user::id_type, db::flag::user::super> user_super;
    struct user_password :
        public hades::has_candidate_key<db::attr::user::user_id>,
        public hades::tuple<
            db::attr::user::user_id,
            db::attr::user_password::password>,
        public hades::relation<db::relvar::user_password>,
        public hades::crud<user_password>
    {
        user_password()
        {
        }
        user_password(const styx::element& e) :
            styx::object_accessor(e)
        {
        }
        bool check(const std::string& password)
        {
            return get_string<db::attr::user_password::password>() == password;
        }
    };
    struct user_permission :
        public hades::has_candidate_key<
            db::attr::user::user_id,
            db::attr::user_permission::permission>,
        public hades::tuple<
            db::attr::user::user_id,
            db::attr::user_permission::permission>,
        public hades::relation<db::relvar::user_permission>,
        public hades::crud<user_permission>
    {
        user_permission()
        {
        }
        user_permission(const styx::element& e) :
            styx::object_accessor(e)
        {
        }
    };
    struct auth_token :
        public hades::has_candidate_key<db::attr::auth_token::token>,
        public hades::tuple<
            db::attr::auth_token::token,
            db::attr::auth_token::created,
            db::attr::auth_token::expires>,
        public hades::relation<db::relvar::auth_token>,
        public hades::crud<auth_token>
    {
        auth_token()
        {
        }
        auth_token(const styx::element& e) :
            styx::object_accessor(e)
        {
        }
    };
    namespace db
    {
        namespace user_permission
        {
            std::vector<helios::user_permission> user_permissions(
                    hades::connection&,
                    helios::user::id_type
                    );
        }
        namespace auth
        {
            /*!
             * \brief The length of an authentication token when represented as
             * a string.  Does not include the null terminator.
             */
            static const int token_length = 64;

            void create(hades::connection& auth_db);

            /*!
             * \brief Check if a token is valid at the present time.
             *
             * Tokens are considered valid if their creation date is in the
             * past, their expiry date is in the future and they have not been
             * explicitly invalidated.
             */
            bool token_valid(const std::string& token, hades::connection&);

            /*!
             * Issue a token.
             */
            helios::auth_token issue_token(
                    const std::string& token,
                    helios::user::id_type,
                    hades::connection&
                    );

            /*!
             * Invalidate a token.
             */
            void invalidate(const std::string& token, hades::connection&);

            /*!
             * Get the user associated with a token.
             */
            helios::user token_user(
                    hades::connection& auth_db,
                    const std::string& token
                    );

            /*!
             * Get the user by their username.
             */
            helios::user username_user(
                    hades::connection& auth_db,
                    const std::string& username
                    );

            /*!
             * Delete tokens that have expired because the expiry time has
             * passed.
             */
            void delete_old_tokens(hades::connection& auth_db);
        }
    }
}

#endif


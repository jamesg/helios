#include "api/auth.hpp"

#include <boost/bind.hpp>
#include <boost/nondet_random.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_int.hpp>

#include "hades/crud.ipp"

#include "api/exception.hpp"
#include "api/server.hpp"
#include "db/auth.hpp"

void helios::api::auth::install(
        hades::connection& conn,
        atlas::api::server& server
        )
{
    /*
     * Generate a token if the username and password are correct.
     *
     * Should be called when the user logs in.  The generated token
     * should be sent with all future API requests.
     *
     * Parameters: [ username, password ].
     */
    server.install<std::string, std::string, std::string>(
        "login",
        [&conn](const std::string username, const std::string password) {
            helios::user user = db::auth::username_user(conn, username);
            helios::user_password user_password;
            user_password.from_id(conn, user.id());

            if(user_password.get_string<db::attr::user_password::password>() == password)
            {
                // Generate a token.
                std::ostringstream oss;
                boost::random_device rng;
                // Only characters that don't require URI encoding are used.
                static const std::string chars =
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz"
                     "0123456789";//+/";
                boost::variate_generator<boost::random_device&, boost::uniform_int<>>
                    gen(rng, boost::uniform_int<>(0, chars.size()-1));
                for(int i = 0; i < 64; ++i)
                    oss << chars[gen()];
                const std::string token = oss.str();

                // Store the token.
                db::auth::issue_token(token, user.id(), conn);
                return token;
            }
            else if(user.id_set())
            {
                throw atlas::api::exception("Incorrect password.");
            }
            else
            {
                throw atlas::api::exception("Unknown user.");
            }
        }
        );
    /*
     * Immediately invalidate a token.
     *
     * Parameters: [ token ].
     *
     * Should be called when the user logs out.  The authentication
     * token will fail in all future requests, so the client should
     * forget the token after calling this function.
     */
    server.install<styx::null, std::string>(
        "logout",
        [&conn](const std::string token) {
            db::auth::invalidate(token, conn);
            return styx::null();
        }
        );

    server.install<bool, std::string>(
        "token_valid",
        [&conn](const std::string token) {
            return db::auth::token_valid(token, conn);
        }
        );

    server.install<styx::element, styx::element>(
        "user_update",
        [&conn](const styx::element user_e) {
            helios::user new_user(user_e);
            helios::user user;
            user.from_id(conn, new_user.id());
            //[>if(user.username() != "root" && user.id() != new_user.id())<]
                ////throw api::exception("Not authorised.");
            //[>else <]if(db_user.password() != new_user.get_string("current_password"))
                //throw atlas::api::exception("Current password doesn't match.");
            //else if(db_user.username() != new_user.username())
                //throw atlas::api::exception("Changing usernames is not allowed.");
            //else if(new_user.password().length() == 0)
                //throw atlas::api::exception("Empty passwords are not allowed.");
            //else
            if(new_user.get_string<db::attr::user::username>() != user.get_string<db::attr::user::username>())
                throw atlas::api::exception("Changing usernames is not allowed.");

            new_user.save(conn);
            return new_user.get_element();
        }
        );
}


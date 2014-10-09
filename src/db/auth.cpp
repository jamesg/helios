#include "db/auth.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "hades/crud.ipp"
#include "hades/devoid.hpp"
#include "hades/get_collection.hpp"
#include "hades/join.hpp"

const char helios::db::attr::user::user_id[] = "user_id";
const char helios::db::attr::user::username[] = "username";
const char helios::db::attr::user_password::password[] = "password";
const char helios::db::attr::user_permission::permission[] = "permission";
const char helios::db::attr::auth_token::token[] = "token";
const char helios::db::attr::auth_token::created[] = "created";
const char helios::db::attr::auth_token::expires[] = "expires";
const char helios::db::flag::user::enabled[] = "enabled";
const char helios::db::flag::user::super[] = "super";
const char helios::db::relvar::user[] = "user";
const char helios::db::relvar::user_created[] = "user_created";
const char helios::db::relvar::user_password[] = "user_password";
const char helios::db::relvar::user_permission[] = "user_permission";
const char helios::db::relvar::auth_token[] = "auth_token";

void helios::db::auth::create(hades::connection& conn)
{
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS user ( "
            " user_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            " username VARCHAR NOT NULL "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS user_password ( "
            " user_id INTEGER PRIMARY KEY REFERENCES user(user_id), "
            " password VARCHAR NOT NULL "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS auth_token ( "
            " user_id INTEGER NOT NULL REFERENCES user(user_id), "
            " token VARCHAR PRIMARY KEY, "
            " created VARCHAR NOT NULL, "
            " expires VARCHAR NOT NULL, "
            " invalidated BOOLEAN NOT NULL DEFAULT false "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE IF NOT EXISTS user_details ( "
            " user_id PRIMARY KEY REFERENCES user(user_id), "
            " first_name VARCHAR NOT NULL DEFAULT '', "
            " last_name VARCHAR NOT NULL DEFAULT '' "
            " ) ",
            conn
            );

    styx::list root_users = hades::get_collection<helios::user>(
            conn,
            hades::where<>("username = 'root' ")
            );
    if(root_users.size() == 0)
    {
        helios::user u;
        u.get_string<db::attr::user::username>() = "root";
        u.save(conn);

        helios::user_password p;
        p.set_id(u.id());
        p.get_string<db::attr::user_password::password>() = "root";
        p.save(conn);
    }
}

bool helios::db::auth::token_valid(
    const std::string& token,
    hades::connection& conn
    )
{
    boost::posix_time::ptime current_time(
            boost::posix_time::second_clock::universal_time()
            );
    styx::list tokens = hades::get_collection<helios::auth_token>(
            conn,
            hades::where<std::string, std::string, std::string>(
                "WHERE token = ? "
                "AND expires > ? "
                "AND created <= ? "
                "AND NOT invalidated ",
                hades::row<std::string, std::string, std::string>(
                    token,
                    boost::posix_time::to_iso_extended_string(current_time),
                    boost::posix_time::to_iso_extended_string(current_time)
                    )
                )
            );
    return !tokens.empty();
}

helios::auth_token helios::db::auth::issue_token(
    const std::string& token_str,
    helios::user::id_type user_id,
    hades::connection& conn
    )
{
    boost::posix_time::ptime current_time(
            boost::posix_time::second_clock::universal_time()
            );
    boost::posix_time::ptime expires_time(
            current_time +
            boost::posix_time::hours(24)
            );
    helios::auth_token token;
    token.get_int<db::attr::user::user_id>() = user_id.get_int<db::attr::user::user_id>();
    token.get_string<db::attr::auth_token::token>() = token_str;
    token.get_string<db::attr::auth_token::created>() =
        boost::posix_time::to_iso_extended_string(current_time),
    token.get_string<db::attr::auth_token::expires>() =
        boost::posix_time::to_iso_extended_string(expires_time);
    token.save(conn);
    return token;
}

void helios::db::auth::invalidate(
    const std::string& token,
    hades::connection& conn
    )
{
    hades::devoid(
            "UPDATE auth_token SET invalidated = true WHERE token = ? ",
            hades::row<std::string>(token),
            conn
            );
}

helios::user helios::db::auth::token_user(
        hades::connection& conn,
        const std::string& token
        )
{
    styx::list users = hades::join<helios::user, helios::auth_token>(
            conn,
            hades::where<std::string>(
                "WHERE auth_user.user_id = auth_token.user_id "
                "AND token = ? ",
                hades::row<std::string>(token)
                )
            );
    if(users.size() == 1)
        return helios::user(users.at(0));
    else
        throw std::runtime_error("no user with given token");
}

helios::user helios::db::auth::username_user(
        hades::connection& conn,
        const std::string& username
        )
{
    styx::list users = hades::get_collection<helios::user>(
            conn,
            hades::where<std::string>(
                "user.username = ?",
                hades::row<std::string>(username)
                )
            );
    if(users.size() == 1)
        return helios::user(users.at(0));
    else
        throw std::runtime_error("no user with given username");
}

void helios::db::auth::delete_old_tokens(hades::connection& conn)
{
    boost::posix_time::ptime current_time(
            boost::posix_time::second_clock::universal_time()
            );
    hades::devoid(
            "DELETE FROM auth_token WHERE expires < ? ",
            hades::row<std::string>(
                boost::posix_time::to_iso_extended_string(current_time)
                ),
            conn
            );
}


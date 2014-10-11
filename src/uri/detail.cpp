#include "detail.hpp"

#include <sstream>

#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "db/auth.hpp"

std::string helios::uri::detail::extract_token(mg_connection *mg_conn)
{
    // Prefer the token in the header.
    {
        const char *token_str = mg_get_header(mg_conn, "Authorization");
        if(token_str)
            return std::string(token_str);
    }

    // Fall back: try to extract a token from the GET parameter.
    {
        char token_str[db::auth::token_length+1];
        mg_get_var(mg_conn, "token", token_str, sizeof(token_str));
        return std::string(token_str);
    }
}

std::string helios::uri::detail::http_date(const boost::posix_time::ptime& time)
{
    boost::local_time::local_time_facet* lf(
            new boost::local_time::local_time_facet("%a, %d %b %Y %H:%M:%S GMT")
            );
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), lf));
    ss << time;
    return ss.str();
}

void helios::uri::detail::text_response(
        mg_connection *mg_conn,
        int status_code,
        const std::string& response
        )
{
    mg_send_status(mg_conn, status_code);
    mg_send_header(mg_conn, "Content-type", "text/plain");
    mg_send_data(mg_conn, response.c_str(), response.length());
}


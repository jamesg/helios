#ifndef PHOTOGRAPH_URI_DETAIL_HPP
#define PHOTOGRAPH_URI_DETAIL_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <string>

#include "mongoose.h"

namespace boost
{
    namespace posix_time
    {
        class ptime;
    }
}

namespace photoalbum
{
    namespace uri
    {
        namespace detail
        {
            static const int status_ok = 200;
            static const int status_unauthorized = 401;

            /*!
             * Extract the 'token' GET variable.
             */
            std::string extract_token(mg_connection *conn);
            /*!
             * Format a date correctly for HTTP date headers.
             */
            std::string http_date(const boost::posix_time::ptime&);
            /*!
             * Send a plain text response.
             */
            void text_response(
                    mg_connection* conn,
                    int status_code,
                    const std::string& response=""
                    );
        }
    }
}

#endif


#ifndef HELIOS_URI_DETAIL_HPP
#define HELIOS_URI_DETAIL_HPP

#include <string>

#include "mongoose.h"

namespace boost
{
    namespace posix_time
    {
        class ptime;
    }
}

namespace helios
{
    namespace uri
    {
        namespace detail
        {
            static const int status_ok = 200;
            static const int status_unauthorized = 401;

            /*!
             * \brief Extract the 'token' GET variable.
             */
            std::string extract_token(mg_connection*);
            /*!
             * \brief Format a date correctly for HTTP date headers.
             */
            std::string http_date(const boost::posix_time::ptime&);
            /*!
             * \brief Send a plain text response.
             */
            void text_response(
                    mg_connection*,
                    int status_code,
                    const std::string& response=""
                    );
        }
    }
}

#endif


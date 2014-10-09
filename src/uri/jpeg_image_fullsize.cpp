/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "jpeg_image_fullsize.hpp"

#include <boost/lexical_cast.hpp>

#include "db/auth.hpp"
#include "db/photograph/jpeg_data.hpp"
#include "server.hpp"
#include "uri/detail.hpp"

int photoalbum::uri::jpeg_image_fullsize(
        const server& serve,
        mg_connection *conn,
        mg_event ev,
        sqlite::connection& photograph_db,
        sqlite::connection& auth_db
        )
{
    if(ev != MG_REQUEST)
        return MG_FALSE;
    if(!db::auth::token_valid(detail::extract_token(conn), auth_db))
    {
        detail::text_response(conn, detail::status_unauthorized);
        return MG_TRUE;
    }
    mg_send_status(conn, 200);
    mg_send_header(conn, "Content-type", "image/jpeg");
    // Images cannot be modified after they are uploaded, so we can safely give
    // the start time of the server as the last modified time.
    mg_send_header(
            conn,
            "Last-Modified",
            detail::http_date(serve.start_time()).c_str()
            );

    char field[100];
    mg_get_var(conn, "photograph_id", field, sizeof(field));

    try
    {
        int photo_id = boost::lexical_cast<int>(field);
        json::object data_o;
        jpeg_data_db data(data_o);
        db::get_by_id(
                photo_id,
                photograph_db,
                data
                );
        mg_send_data(conn, &(data.data[0]), data.data.size());
    }
    catch(const std::exception& e)
    {
        std::cerr << "in jpeg_image_fullsize: " << e.what() << std::endl;
        return MG_FALSE;
    }
    return MG_TRUE;
}


#include "jpeg_image_fullsize.hpp"

#include <iostream>

#include <boost/lexical_cast.hpp>

#include "atlas/http/server/error.hpp"

#include "db/jpeg_data.hpp"
#include "detail.hpp"

void helios::uri::jpeg_image_fullsize(
        hades::connection& conn,
        mg_connection *mg_conn,
        atlas::http::uri_parameters_type,
        atlas::http::uri_callback_type callback_success,
        atlas::http::uri_callback_type callback_failure
        )
{
    //if(!atlas::auth::is_signed_in(conn, detail::extract_token(mg_conn)))
    //{
        //atlas::http::error(403, "not authorised", mg_conn);
        //callback_success();
        //return;
    //}

    mg_send_status(mg_conn, 200);
    mg_send_header(mg_conn, "Content-type", "image/jpeg");
    // Images cannot be modified after they are uploaded, so we can safely give
    // the start time of the server as the last modified time.
    //TODO
    //mg_send_header(
            //conn,
            //"Last-Modified",
            //detail::http_date(serve.start_time()).c_str()
            //);

    char field[100];
    mg_get_var(mg_conn, "photograph_id", field, sizeof(field));

    try
    {
        int photo_id = boost::lexical_cast<int>(field);
        helios::jpeg_data_db data = db::jpeg_data::get_by_id(conn, photo_id);
        mg_send_data(mg_conn, &(data.data[0]), data.data.size());
    }
    catch(const std::exception& e)
    {
        std::cerr << "in jpeg_image_fullsize: " << e.what() << std::endl;
        callback_failure();
        return;
    }
    callback_success();
}

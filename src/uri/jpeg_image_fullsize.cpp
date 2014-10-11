#include "jpeg_image_fullsize.hpp"

#include <boost/lexical_cast.hpp>

#include "db/jpeg_data.hpp"
#include "uri/detail.hpp"

void helios::uri::jpeg_image_fullsize(
        hades::connection& conn,
        mg_connection *mg_conn,
        atlas::http::uri_callback_type callback_success,
        atlas::http::uri_callback_type callback_failure
        )
{
    //if(!db::auth::token_valid(detail::extract_token(conn), auth_db))
    //{
        //detail::text_response(conn, detail::status_unauthorized);
        //return MG_TRUE;
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


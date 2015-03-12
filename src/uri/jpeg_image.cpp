#include "jpeg_image.hpp"

#include <boost/fusion/include/at_c.hpp>
#include <boost/lexical_cast.hpp>

#include <exiv2/exiv2.hpp>
#include <Magick++.h>

#include "atlas/http/server/error.hpp"
#include "hades/mkstr.hpp"

#include "db/cache.hpp"
#include "db/jpeg_data.hpp"
#include "uri/detail.hpp"

void helios::uri::jpeg_image(
        //const server& serve,
        hades::connection& conn,
        mg_connection *mg_conn,
        boost::smatch,
        atlas::http::uri_callback_type callback_success,
        atlas::http::uri_callback_type callback_failure
        )
{
    //if(!db::auth::token_valid(detail::extract_token(conn), auth_db))
    //{
        //detail::text_response(conn, detail::status_unauthorized, "Not authorised");
        //return MG_TRUE;
    //}
    char id_s[10], width_s[10], height_s[10];
    mg_get_var(mg_conn, "photograph_id", id_s, sizeof(id_s));
    mg_get_var(mg_conn, "width", width_s, sizeof(width_s));
    mg_get_var(mg_conn, "height", height_s, sizeof(height_s));

    int width = 100, height = 100, photo_id = 0;
    try
    {
        width = boost::lexical_cast<int>(width_s);
    }
    catch(const std::exception&)
    {
    }
    try
    {
        height = boost::lexical_cast<int>(height_s);
    }
    catch(const std::exception&)
    {
    }
    try
    {
        photo_id = boost::lexical_cast<int>(id_s);
    }
    catch(const std::exception&)
    {
        // Cannot provide a photograph if no id was given.
        atlas::http::error(400, "no photograph id provided", mg_conn);
        callback_success();
        return;
    }

    if(db::cache::has(conn, photo_id, height, width))
    {
        helios::jpeg_cache_db data =
            db::cache::get(conn, photo_id, height, width);
        mg_send_status(mg_conn, 200);
        mg_send_header(mg_conn, "Content-type", "image/jpeg");
        //TODO
        //mg_send_header(
                //mg_conn,
                //"Last-Modified",
                //detail::http_date(serve.start_time()).c_str()
                //);
        mg_send_data(mg_conn, &(data.data[0]), data.data.size());
        callback_success();
        return;
    }

    Magick::Blob blob;
    try
    {
        helios::jpeg_data_db data = db::jpeg_data::get_by_id(conn, photo_id);
        Magick::Image image(Magick::Blob(
            reinterpret_cast<const void*>(&(data.data[0])), data.data.size())
            );

        short orientation = 1;
        try
        {   // Retrieve orientation
            auto exiv_image = Exiv2::ImageFactory::open(
                reinterpret_cast<const unsigned char*>(&(data.data[0])),
                data.data.size()
                );
            exiv_image->readMetadata();

            Exiv2::ExifKey key("Exif.Image.Orientation");
            Exiv2::ExifData::iterator pos = exiv_image->exifData().findKey(key);

            if( pos != exiv_image->exifData().end() )
                orientation = pos->getValue()->toLong();
        }
        catch(const std::exception&)
        {
            // Some images don't have an orientation.
        }

        // Scale the image
        std::ostringstream oss;
        switch(orientation)
        {
        case 6:
        case 8:
            // Swap width and height because the image is about to be rotated
            std::swap(width, height);
        }
        // Scale to fit within this box
        oss << width << "x" << height << "^";
        image.scale(Magick::Geometry(oss.str()));

        // Rotate the image
        switch(orientation)
        {
            case 3:
                image.rotate(180);
            case 6:
                image.rotate(90);
            case 8:
                image.rotate(270);
        }

        Magick::Image out_image( image.size(), Magick::Color(255,255,255) );
        out_image.composite(image, 0, 0);
        out_image.write(&blob, "JPEG");

        helios::jpeg_cache_db cache_data;
        cache_data.photograph_id = photo_id;
        cache_data.height = height;
        cache_data.width = width;
        cache_data.data =
            std::vector<unsigned char>(
                (const unsigned char*)(blob.data()),
                (const unsigned char*)(blob.data()) + blob.length()
                );
        db::cache::insert(cache_data, conn);

        //sqlite::insert(
                //"jpeg_cache",
                //{ "photograph_id", "width", "height", "data" },
                //boost::fusion::vector<int, int, int, std::string>(
                    //photo_id, width, height, std::string((const char*)(blob.data()), blob.length())
                    //),
                //cache_db
                //);
    }
    catch(const std::exception& e)
    {
        atlas::http::error(400, hades::mkstr() << "scaling the image: " << e.what(), mg_conn);
        callback_success();
        return;
    }

    mg_send_status(mg_conn, 200);
    mg_send_header(mg_conn, "Content-type", "image/jpeg");
    //TODO
    //mg_send_header(
            //mg_conn,
            //"Last-Modified",
            //detail::http_date(serve.start_time()).c_str()
            //);
    mg_send_data(mg_conn, blob.data(), blob.length());

    callback_success();
}


#include "insert_photograph.hpp"

#include <cstring>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/tokenizer.hpp>

#include <exiv2/exiv2.hpp>

#include "atlas/auth/auth.hpp"
#include "atlas/http/server/error.hpp"
#include "atlas/jsonrpc/request.hpp"
#include "atlas/jsonrpc/result.hpp"
#include "hades/crud.ipp"
#include "styx/serialise_json.hpp"

#include "db/jpeg_data.hpp"
#include "db/photograph.hpp"
#include "detail.hpp"

namespace
{
    std::string photograph_date(
            const unsigned char* jpeg_data,
            const int jpeg_data_len
            )
    {
        try
        {
            auto image = Exiv2::ImageFactory::open(
                    reinterpret_cast<const unsigned char*>(jpeg_data),
                    jpeg_data_len
                    );
            image->readMetadata();
            Exiv2::ExifKey key("Exif.Photo.DateTimeOriginal");

            // Try to find the date key
            Exiv2::ExifData::iterator pos = image->exifData().findKey(
                    Exiv2::ExifKey("Exif.Image.DateTimeOriginal")
                    );
            if( pos == image->exifData().end() )
                pos = image->exifData().findKey(
                        Exiv2::ExifKey("Exif.Image.DateTime")
                        );

            // If an acceptable key was found
            if( pos != image->exifData().end() )
            {
                std::string date = pos->getValue()->toString();

                // Exif date format
                auto *df = new boost::date_time::time_input_facet<
                    boost::posix_time::ptime,
                    char
                    >("%Y:%m:%d %H:%M:%S");

                std::stringstream date_stream(date);
                date_stream.imbue(std::locale(date_stream.getloc(), df));
                boost::posix_time::ptime taken;
                date_stream >> taken;

                return boost::posix_time::to_iso_extended_string(taken);
            }
        }
        catch(const std::exception&)
        {
            // No error handling, caller expects empty string.
        }
        return "";
    }
}

void helios::uri::insert_photograph(
        hades::connection& conn,
        mg_connection *mg_conn,
        atlas::http::uri_parameters_type,
        atlas::http::uri_callback_type callback_success,
        atlas::http::uri_callback_type callback_failure
        )
{
    // TODO: None of this nice logic will work until the client sets an
    // Authorization header on outgoing requests.
    //if(!atlas::auth::is_signed_in(conn, detail::extract_token(mg_conn)))
    //{
        //atlas::http::error(403, "not authorised", mg_conn);
        //callback_success();
        //return;
    //}

    helios::photograph photo;
    helios::jpeg_data_db data_db;

    const char *data;
    int data_len;
    char var_name[100], file_name[100];

    std::string location, tags;

    int skip = 0, skip_total = 0;
    while((skip = mg_parse_multipart(
                mg_conn->content+skip_total, mg_conn->content_len-skip_total,
                var_name, sizeof(var_name),
                file_name, sizeof(file_name),
                &data, &data_len
                )
         ) > 0)
    {
        skip_total += skip;

        if(strcmp(var_name, "title") == 0)
            photo.get_string<db::attr::photograph::title>() =
                std::string(data, data_len);
        if(strcmp(var_name, "caption") == 0)
            photo.get_string<db::attr::photograph::caption>() =
                std::string(data, data_len);
        if(strcmp(var_name, "location") == 0)
            location = std::string(data, data_len);
        if(strcmp(var_name, "tags") == 0)
            tags = std::string(data, data_len);
        if(strcmp(var_name, "file") == 0)
        {
            photo.get_string<db::attr::photograph::taken>() = photograph_date(
                    reinterpret_cast<const unsigned char*>(data),
                    data_len
                    );
            data_db.data.insert(
                    data_db.data.end(),
                    reinterpret_cast<const unsigned char*>(data),
                    reinterpret_cast<const unsigned char*>(data)+data_len
                    );
        }
    }

    photo.save(conn);

    data_db.photograph_id =
        photo.get_int<db::attr::photograph::photograph_id>();
    db::jpeg_data::insert(data_db, conn);

    db::set_photograph_tags(
            conn,
            photo.id(),
            tags
            );

    helios::photograph_location photograph_location;
    photograph_location.set_id(photo.id());
    photograph_location.get_string<db::attr::photograph_location::location>() =
        location;
    photograph_location.save(conn);

    mg_send_status(mg_conn, 200);
    mg_send_header(mg_conn, "Content-type", "text/json");

    atlas::jsonrpc::result res;

    res.data() = styx::element("Success");

    std::string json_s = styx::serialise_json(res);
    mg_send_data(mg_conn, json_s.c_str(), json_s.length());

    callback_success();
}

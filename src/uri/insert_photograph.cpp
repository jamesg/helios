/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "insert_photograph.hpp"

#include <cstring>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/tokenizer.hpp>

#include <exiv2/exiv2.hpp>

#include "json/json.hpp"

#include "db/photograph/jpeg_data.hpp"
#include "db/photograph.hpp"
#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"

namespace
{
    std::string photograph_date(
            const unsigned char* jpeg_data,
            const int            jpeg_data_len
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

int photoalbum::uri::insert_photograph(
        mg_connection       *conn,
        mg_event            ev,
        sqlite::connection& photo_db
        )
{
    if(ev != MG_REQUEST)
        return MG_FALSE;
    json::object photo_o, data_o;
    photograph photo(photo_o);
    jpeg_data_db data_db(data_o);

    const char *data;
    int data_len;
    char var_name[100], file_name[100];

    std::string tags;

    int skip = 0, skip_total = 0;
    while((skip = mg_parse_multipart(
                conn->content+skip_total, conn->content_len-skip_total,
                var_name, sizeof(var_name),
                file_name, sizeof(file_name),
                &data, &data_len
                )
         ) > 0)
    {
        skip_total += skip;

        if(strcmp(var_name, "title") == 0)
            photo.title() = std::string(data, data_len);
        if(strcmp(var_name, "caption") == 0)
            photo.caption() = std::string(data, data_len);
        if(strcmp(var_name, "location") == 0)
            photo.location() = std::string(data, data_len);
        if(strcmp(var_name, "tags") == 0)
            tags = std::string(data, data_len);
        if(strcmp(var_name, "file") == 0)
        {
            photo.taken() = photograph_date(
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

    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
    tokenizer t(tags, boost::escaped_list_separator<char>());
    for(auto it = t.begin(); it != t.end(); ++it)
        photo.tags().append(*it);

    int photo_id = db::insert(
            photo,
            photo_db
            );

    data_db.id() = photo_id;
    db::insert(
            data_db,
            photo_db
            );

    mg_send_status(conn, 200);
    mg_send_header(conn, "Content-type", "text/json");

    json::object res_o;
    jsonrpc::result res(res_o);

    res.data() = json::object("Success");

    std::string json_s = json::serialise_json(res_o);
    mg_send_data(conn, json_s.c_str(), json_s.length());

    return MG_TRUE;
}


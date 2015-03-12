#include "uri.hpp"

#include "atlas/http/server/router.hpp"
#include "atlas/http/server/server.hpp"
#include "hades/crud.ipp"
#include "hades/join.hpp"
#include "styx/serialise_json.hpp"
#include "styx/serialisers/vector.hpp"

#include "db/photograph.hpp"
#include "uri/insert_photograph.hpp"
#include "uri/jpeg_image.hpp"
#include "uri/jpeg_image_fullsize.hpp"

void helios::uri::install(hades::connection& conn, atlas::http::server& server)
{
    server.router().install(
        atlas::http::matcher("/insert_photograph", "post"),
        boost::bind(
            &helios::uri::insert_photograph,
            boost::ref(conn),
            _1,
            _2,
            _3,
            _4
            )
        );
    server.router().install(
        "/jpeg_image",
        boost::bind(
            &helios::uri::jpeg_image,
            boost::ref(conn),
            _1,
            _2,
            _3,
            _4
            )
        );
    server.router().install(
        "/jpeg_image_fullsize",
        boost::bind(
            &helios::uri::jpeg_image_fullsize,
            boost::ref(conn),
            _1,
            _2,
            _3,
            _4
            )
        );
    server.router().install<int>(
        "/photograph/(.+)",
        [&conn](const int photograph_id) {
            auto where = hades::where(
                "photograph.photograph_id = ?",
                hades::row<int>(photograph_id)
                );
            styx::list l = hades::equi_outer_join<
                helios::photograph,
                helios::photograph_location>(
                    conn,
                    where
                    );
            if(l.size() != 1)
                return atlas::http::json_error_response("Photograph not found");

            helios::photograph photograph(l.at(0));
            std::vector<std::string> tags = db::photograph_tags(
                conn,
                photograph.id()
                );
            std::ostringstream oss;
            styx::serialise(
                tags,
                [](const std::string& tag, std::ostream& os) {
                    if(std::find(tag.cbegin(), tag.cend(), ' ') == tag.cend())
                        os << tag;
                    else
                        os << "\"" << tag << "\"";
                },
                " ",
                oss
                );
            photograph.get_string("tags") = oss.str();

            return atlas::http::json_response(photograph);
        }
        );
    server.router().install<>(
        "/album",
        [&conn]() {
            return atlas::http::json_response(
                helios::album::get_collection(conn)
                );
        }
        );
    server.router().install<int>(
        "/album/([^/]+)",
        [&conn](const int album_id) {
            helios::album album(
                hades::get_one<helios::album>(
                    conn,
                    hades::where("album.album_id = ?", hades::row<int>(album_id))
                    )
                );
            return atlas::http::json_response(album);
        }
        );
    server.router().install<int>(
        "/album/(.+)/photograph",
        [&conn](const int album_id) {
            return atlas::http::json_response(
                hades::join<
                    helios::photograph,
                    helios::photograph_in_album,
                    helios::photograph_location,
                    helios::album>(
                    conn,
                    hades::filter(
                        hades::where(
                            "photograph.photograph_id = photograph_in_album.photograph_id AND "
                            "photograph_in_album.album_id = album.album_id AND "
                            "photograph.photograph_id = photograph_location.photograph_id AND "
                            "album.album_id = ?",
                            hades::row<int>(album_id)
                            ),
                        hades::order_by("photograph.taken ASC")
                        )
                    )
                );
        }
        );
    server.router().install_json<int>(
        atlas::http::matcher("/photograph/(.+)", "put"),
        [&conn](const styx::element photograph_e, const int photograph_id) {
            helios::photograph photograph(photograph_e);
            if(photograph.get_int<db::attr::photograph::photograph_id>() != photograph_id)
                return atlas::http::json_error_response("Photograph id does not match.");

            photograph.save(conn);
            helios::photograph_location(photograph_e).save(conn);
            db::set_photograph_tags(conn, photograph.id(), photograph.get_string("tags"));
            return atlas::http::json_response(
                hades::get_by_id<helios::photograph>(conn, photograph.id())
                );
        }
        );
    server.router().install<int>(
        atlas::http::matcher("/photograph/(.*)", "delete"),
        [&conn](int photograph_id) {
            helios::photograph photograph;
            photograph.get_int<db::attr::photograph::photograph_id>() = photograph_id;
            if(photograph.destroy(conn))
                return atlas::http::json_response(photograph);
            else
                return atlas::http::json_error_response("deleting photograph");
        }
        );
    server.router().install<std::string>(
        atlas::http::matcher("/tag/([^/]*)/photograph"),
        [&conn](const std::string tag) {
            return atlas::http::json_response(
                hades::join<helios::photograph, helios::basic_tag>(
                    conn,
                    hades::filter(
                        hades::where(
                            "photograph.photograph_id = "
                            " photograph_tagged.photograph_id AND "
                            "photograph_tagged.tag = ? ",
                            hades::row<std::string>(tag)
                            ),
                        hades::order_by("photograph.taken ASC")
                        )
                    )
                );
        }
        );
    server.router().install<std::string>(
        atlas::http::matcher("/location/([^/]*)/photograph"),
        [&conn](const std::string location) {
            return atlas::http::json_response(
                hades::join<helios::photograph, helios::basic_location>(
                    conn,
                    hades::filter(
                        hades::where(
                            "photograph.photograph_id = "
                            " photograph_location.photograph_id AND "
                            "photograph_location.location = ? ",
                            hades::row<std::string>(location)
                            ),
                        hades::order_by("photograph.taken ASC")
                        )
                    )
                );
        }
        );
}


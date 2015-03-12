#include "uri.hpp"

#include "atlas/http/server/router.hpp"
#include "atlas/http/server/server.hpp"
#include "hades/crud.ipp"
#include "hades/join.hpp"
#include "styx/serialise_json.hpp"

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

            return atlas::http::json_response(l.at(0));
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
            atlas::log::test("put photograph") << "saving " << styx::serialise_json(photograph);
            atlas::log::test("put photograph") << "id " << photograph.get_int<db::attr::photograph::photograph_id>() << " incoming " << photograph_id;
            if(photograph.get_int<db::attr::photograph::photograph_id>() != photograph_id)
                return atlas::http::json_error_response("Photograph id does not match.");
            photograph.save(conn);
            atlas::log::test("put photograph") << "saved " << styx::serialise_json(photograph);
            return atlas::http::json_response(
                hades::get_by_id<helios::photograph>(conn, photograph.id())
                );
        }
        );
}


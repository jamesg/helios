#include "helios/router.hpp"

#include <boost/bind.hpp>

#include "atlas/api/auth.hpp"
#include "atlas/api/server.hpp"
#include "atlas/http/server/mimetypes.hpp"
#include "atlas/http/server/static_text.hpp"
#include "atlas/jsonrpc/uri.hpp"
#include "hades/crud.ipp"
#include "hades/custom_select.hpp"
#include "hades/get_by_id.hpp"
#include "hades/get_one.hpp"
#include "hades/join.hpp"
#include "styx/styx.hpp"

#include "api/api.hpp"
#include "db/photograph.hpp"
#include "uri/insert_photograph.hpp"
#include "uri/jpeg_image.hpp"
#include "uri/jpeg_image_fullsize.hpp"

#define HELIOS_DECLARE_STATIC_STRING(PREFIX) \
    extern "C" { \
        extern char helios_binary_##PREFIX##_start; \
        extern char helios_binary_##PREFIX##_end; \
        extern size_t helios_binary_##PREFIX##_size; \
    }

#define HELIOS_STATIC_STD_STRING(PREFIX) \
    std::string(&helios_binary_##PREFIX##_start, &helios_binary_##PREFIX##_end)

HELIOS_DECLARE_STATIC_STRING(index_html)
HELIOS_DECLARE_STATIC_STRING(index_js)
HELIOS_DECLARE_STATIC_STRING(application_js)
HELIOS_DECLARE_STATIC_STRING(models_js)
HELIOS_DECLARE_STATIC_STRING(style_css)

helios::router::router(
    boost::shared_ptr<boost::asio::io_service> io,
    hades::connection& conn
) :
    atlas::http::application_router(io),
    m_api_server(io)
{
    api::install(conn, m_api_server);
    install(
            atlas::http::matcher("/api_call", "POST"),
            boost::bind(
                &atlas::jsonrpc::uri,
                io,
                m_api_server,
                _1,
                _2,
                _3,
                _4
                )
           );

    //
    // Install static files.
    //

    install_static_text("/", "html", HELIOS_STATIC_STD_STRING(index_html));
    install_static_text("/index.html", HELIOS_STATIC_STD_STRING(index_html));
    install_static_text("/index.js", HELIOS_STATIC_STD_STRING(index_js));
    install_static_text("/application.js", HELIOS_STATIC_STD_STRING(application_js));
    install_static_text("/models.js", HELIOS_STATIC_STD_STRING(models_js));
    install_static_text("/style.css", HELIOS_STATIC_STD_STRING(style_css));

    install(
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
    install(
        atlas::http::matcher("/jpeg_image", "GET"),
        boost::bind(
            &helios::uri::jpeg_image,
            boost::ref(conn),
            _1,
            _2,
            _3,
            _4
            )
        );
    install(
        atlas::http::matcher("/jpeg_image_fullsize", "GET"),
        boost::bind(
            &helios::uri::jpeg_image_fullsize,
            boost::ref(conn),
            _1,
            _2,
            _3,
            _4
            )
        );
    install<>(
        atlas::http::matcher("/photograph/random", "GET"),
        [&conn]() {
            styx::list l = hades::equi_outer_join<
                helios::photograph,
                helios::photograph_location>(
                    conn,
                    hades::order_by("RANDOM()", 1)
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
    install<styx::int_type>(
        atlas::http::matcher("/photograph/([0-9]+)", "GET"),
        [&conn](const styx::int_type photograph_id) {
            styx::list l = hades::equi_outer_join<
                helios::photograph,
                helios::photograph_location>(
                    conn,
                    hades::where(
                        "helios_photograph.photograph_id = ?",
                        hades::row<styx::int_type>(photograph_id)
                        )
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
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install<>(
        atlas::http::matcher("/album", "GET"),
        [&conn]() {
            auto filter = hades::order_by("helios_album.name ASC");
            return atlas::http::json_response(
                helios::album::get_collection(conn, filter)
                );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install_json<styx::element>(
        atlas::http::matcher("/album", "post"),
        [&conn](const styx::element& album_e) {
            helios::album album(album_e);
            album.insert(conn);
            return atlas::http::json_response(album);
        }
        );
    install<styx::int_type>(
        atlas::http::matcher("/album/([^/]+)", "GET"),
        [&conn](const styx::int_type album_id) {
            helios::album album(
                hades::get_one<helios::album>(
                    conn,
                    hades::where(
                        "helios_album.album_id = ?",
                        hades::row<styx::int_type>(album_id)
                    )
                )
            );
            return atlas::http::json_response(album);
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install<styx::int_type>(
        atlas::http::matcher("/album/([^/]+)/photograph", "GET"),
        [&conn](const styx::int_type album_id) {
            return atlas::http::json_response(
                hades::join<
                    helios::photograph,
                    helios::photograph_in_album,
                    helios::photograph_location,
                    helios::album>(
                    conn,
                    hades::filter(
                        hades::where(
                            "helios_photograph.photograph_id = helios_photograph_in_album.photograph_id AND "
                            "helios_photograph_in_album.album_id = helios_album.album_id AND "
                            "helios_photograph.photograph_id = helios_photograph_location.photograph_id AND "
                            "helios_album.album_id = ?",
                            hades::row<styx::int_type>(album_id)
                            ),
                        hades::order_by("helios_photograph.taken ASC")
                        )
                    )
                );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install<>(
        atlas::http::matcher("/uncategorised/photograph", "GET"),
        [&conn]() {
        return atlas::http::json_response(
            hades::custom_select<helios::photograph, db::attr::photograph::photograph_id, db::attr::photograph::title>(
                conn,
                "SELECT helios_photograph.photograph_id, title "
                "FROM helios_photograph "
                "LEFT OUTER JOIN helios_photograph_in_album "
                "ON helios_photograph_in_album.photograph_id = helios_photograph.photograph_id "
                "WHERE helios_photograph_in_album.album_id IS NULL"
                )
            );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install_json<styx::element, styx::int_type>(
        atlas::http::matcher("/photograph/([^/]+)", "put"),
        [&conn](const styx::element& photograph_e, const styx::int_type photograph_id) {
            helios::photograph photograph(photograph_e);
            if(photograph.get_int<db::attr::photograph::photograph_id>() != photograph_id)
                return atlas::http::json_error_response("Photograph id does not match.");

            photograph.save(conn);
            helios::photograph_location(photograph_e).save(conn);
            db::set_photograph_tags(conn, photograph.id(), photograph.get_string("tags"));
            return atlas::http::json_response(
                hades::get_by_id<helios::photograph>(conn, photograph.id())
            );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install<styx::int_type>(
        atlas::http::matcher("/photograph/([^/]+)", "delete"),
        [&conn](styx::int_type photograph_id) {
            helios::photograph photograph;
            photograph.get_int<db::attr::photograph::photograph_id>() = photograph_id;
            if(photograph.destroy(conn))
                return atlas::http::json_response(photograph);
            else
                return atlas::http::json_error_response("deleting photograph");
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install<styx::int_type>(
        atlas::http::matcher("/photograph/([^/]+)/album", "GET"),
        [&conn](styx::int_type photograph_id) {
            return atlas::http::json_response(
                hades::join<helios::photograph_in_album, helios::album>(
                    conn,
                    hades::where(
                        "helios_photograph_in_album.album_id = helios_album.album_id AND "
                        "helios_photograph_in_album.photograph_id = ?",
                        hades::row<styx::int_type>(photograph_id)
                        )
                    )
                );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install<styx::int_type, styx::int_type>(
        atlas::http::matcher("/photograph/([0-9]+)/album/([0-9]+)", "DELETE"),
        [&conn](styx::int_type photograph_id, styx::int_type album_id) {
            return atlas::http::json_response(
                photograph_in_album(
                    photograph_in_album::id_type{photograph_id, album_id}
                    ).destroy(conn)
            );
        }
    );
    install_json<album, styx::int_type>(
        atlas::http::matcher("/photograph/([0-9]+)/album", "POST"),
        [&conn](album a, styx::int_type photograph_id) {
            photograph_in_album in_album;
            in_album.set_id(
                photograph_in_album::id_type{
                    photograph_id,
                    a.get_int<db::attr::album::album_id>()
                }
                );
            in_album.save(conn);
            return atlas::http::json_response(
                hades::get_by_id<album>(conn, a.id())
                );
        }
        );
    install<std::string>(
        atlas::http::matcher("/tag/([^/]*)/photograph", "GET"),
        [&conn](const std::string tag) {
            return atlas::http::json_response(
                hades::join<helios::photograph, helios::basic_tag>(
                    conn,
                    hades::filter(
                        hades::where(
                            "helios_photograph.photograph_id = "
                            " helios_photograph_tagged.photograph_id AND "
                            "helios_photograph_tagged.tag = ? ",
                            hades::row<std::string>(tag)
                            ),
                        hades::order_by("helios_photograph.taken ASC")
                        )
                    )
                );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install<std::string>(
        atlas::http::matcher("/location/([^/]*)/photograph", "GET"),
        [&conn](const std::string location) {
            return atlas::http::json_response(
                hades::join<helios::photograph, helios::basic_location>(
                    conn,
                    hades::filter(
                        hades::where(
                            "helios_photograph.photograph_id = "
                            " helios_photograph_location.photograph_id AND "
                            "helios_photograph_location.location = ? ",
                            hades::row<std::string>(location)
                            ),
                        hades::order_by("helios_photograph.taken ASC")
                        )
                    )
                );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install<>(
        atlas::http::matcher("/tag", "GET"),
        [&conn]() {
            return atlas::http::json_response(
                hades::custom_select<
                    helios::tag,
                    db::attr::tag::tag,
                    db::attr::tag::photograph_count>(
                        conn,
                        "SELECT tag, COUNT(photograph_id) FROM helios_photograph_tagged "
                        "WHERE tag IS NOT NULL AND tag != '' "
                        "GROUP BY tag ORDER BY tag ASC "
                        )
                    );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    install<>(
        atlas::http::matcher("/location", "GET"),
        [&conn]() {
            return atlas::http::json_response(
                hades::custom_select<
                    helios::photograph_location,
                    db::attr::photograph_location::location,
                    db::attr::tag::photograph_count>(
                        conn,
                        "SELECT location, COUNT(photograph_id) FROM helios_photograph_location "
                        "WHERE location IS NOT NULL AND location != '' "
                        "GROUP BY location ORDER BY location ASC "
                        )
                    );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );

    boost::shared_ptr<atlas::api::server> auth_api_server(new atlas::api::server(io));
    atlas::api::auth::install(conn, *auth_api_server);
    install(
        atlas::http::matcher("/auth", "POST"),
        boost::bind(&atlas::api::server::serve, auth_api_server, _1, _2, _3, _4)
        );
}

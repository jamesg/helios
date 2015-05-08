#include "router.hpp"

#include <boost/bind.hpp>

#include "atlas/api/auth.hpp"
#include "atlas/api/server.hpp"
#include "atlas/http/server/mimetypes.hpp"
#include "atlas/http/server/static_text.hpp"
#include "hades/crud.ipp"
#include "hades/custom_select.hpp"
#include "hades/get_by_id.hpp"
#include "hades/get_one.hpp"
#include "hades/join.hpp"
#include "styx/styx.hpp"

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

boost::shared_ptr<atlas::http::router> helios::router(hades::connection& conn, boost::shared_ptr<boost::asio::io_service> io)
{
    boost::shared_ptr<atlas::http::router> out(new atlas::http::router);

    atlas::http::mimetypes mime_information;

    auto install_static_text = [&mime_information, out](
            const std::string& url,
            const std::string& text
            )
    {
        std::string extension;
        {
            std::string::size_type dot_pos = url.find_last_of('.');
            if(dot_pos != std::string::npos)
                extension = url.substr(dot_pos+1);
        }
        out->install(
                atlas::http::matcher(url, "GET"),
                boost::bind(
                    &atlas::http::static_text,
                    mime_information.content_type(extension),
                    text,
                    _1,
                    _2,
                    _3,
                    _4
                    )
                );
    };

    // Special case; index.html should be served on requests to /, but as the
    // file extension cannot be deduced from the URL the MIME type must be
    // specified.
    out->install(
            atlas::http::matcher("/", "GET"),
            boost::bind(
                &atlas::http::static_text,
                mime_information.content_type("html"),
                HELIOS_STATIC_STD_STRING(index_html),
                _1,
                _2,
                _3,
                _4
                )
            );

    //
    // Install static files.
    //

    install_static_text("/index.html", HELIOS_STATIC_STD_STRING(index_html));
    install_static_text("/index.js", HELIOS_STATIC_STD_STRING(index_js));
    install_static_text("/application.js", HELIOS_STATIC_STD_STRING(application_js));
    install_static_text("/models.js", HELIOS_STATIC_STD_STRING(models_js));
    install_static_text("/style.css", HELIOS_STATIC_STD_STRING(style_css));

    out->install(
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
    out->install(
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
    out->install(
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
    out->install<>(
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
    out->install<int>(
        atlas::http::matcher("/photograph/([0-9]+)", "GET"),
        [&conn](const int photograph_id) {
            styx::list l = hades::equi_outer_join<
                helios::photograph,
                helios::photograph_location>(
                    conn,
                    hades::where(
                        "photograph.photograph_id = ?",
                        hades::row<int>(photograph_id)
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
    out->install<>(
        atlas::http::matcher("/album", "GET"),
        [&conn]() {
            auto filter = hades::order_by("album.name ASC");
            return atlas::http::json_response(
                helios::album::get_collection(conn, filter)
                );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install_json<>(
        atlas::http::matcher("/album", "post"),
        [&conn](styx::element album_e) {
            helios::album album(album_e);
            album.insert(conn);
            return atlas::http::json_response(album);
        }
        );
    out->install<int>(
        atlas::http::matcher("/album/([^/]+)", "GET"),
        [&conn](const int album_id) {
            helios::album album(
                hades::get_one<helios::album>(
                    conn,
                    hades::where("album.album_id = ?", hades::row<int>(album_id))
                    )
                );
            return atlas::http::json_response(album);
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install<int>(
        atlas::http::matcher("/album/([^/]+)/photograph", "GET"),
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
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install<>(
        atlas::http::matcher("/uncategorised/photograph", "GET"),
        [&conn]() {
        return atlas::http::json_response(
            hades::custom_select<helios::photograph, db::attr::photograph::photograph_id, db::attr::photograph::title>(
                conn,
                "SELECT photograph.photograph_id, title "
                "FROM photograph "
                "LEFT OUTER JOIN photograph_in_album "
                "ON photograph_in_album.photograph_id = photograph.photograph_id "
                "WHERE photograph_in_album.album_id IS NULL"
                )
            );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install_json<int>(
        atlas::http::matcher("/photograph/([^/]+)", "put"),
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
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install<int>(
        atlas::http::matcher("/photograph/([^/]+)", "delete"),
        [&conn](int photograph_id) {
            helios::photograph photograph;
            photograph.get_int<db::attr::photograph::photograph_id>() = photograph_id;
            if(photograph.destroy(conn))
                return atlas::http::json_response(photograph);
            else
                return atlas::http::json_error_response("deleting photograph");
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install<int>(
        atlas::http::matcher("/photograph/([^/]+)/album", "GET"),
        [&conn](int photograph_id) {
            return atlas::http::json_response(
                hades::join<helios::photograph_in_album, helios::album>(
                    conn,
                    hades::where(
                        "photograph_in_album.album_id = album.album_id AND "
                        "photograph_in_album.photograph_id = ?",
                        hades::row<int>(photograph_id)
                        )
                    )
                );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install<std::string>(
        atlas::http::matcher("/tag/([^/]*)/photograph", "GET"),
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
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install<std::string>(
        atlas::http::matcher("/location/([^/]*)/photograph", "GET"),
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
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install<>(
        atlas::http::matcher("/tag", "GET"),
        [&conn]() {
            return atlas::http::json_response(
                hades::custom_select<
                    helios::tag,
                    db::attr::tag::tag,
                    db::attr::tag::photograph_count>(
                        conn,
                        "SELECT tag, COUNT(photograph_id) FROM photograph_tagged "
                        "WHERE tag IS NOT NULL AND tag != '' "
                        "GROUP BY tag ORDER BY tag ASC "
                        )
                    );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );
    out->install<>(
        atlas::http::matcher("/location", "GET"),
        [&conn]() {
            return atlas::http::json_response(
                hades::custom_select<
                    helios::photograph_location,
                    db::attr::photograph_location::location,
                    db::attr::tag::photograph_count>(
                        conn,
                        "SELECT location, COUNT(photograph_id) FROM photograph_location "
                        "WHERE location IS NOT NULL AND location != '' "
                        "GROUP BY location ORDER BY location ASC "
                        )
                    );
        },
        boost::bind(&atlas::auth::is_signed_in, boost::ref(conn), _1)
        );

    boost::shared_ptr<atlas::api::server> auth_api_server(new atlas::api::server(io));
    atlas::api::auth::install(conn, *auth_api_server);
    out->install(
        atlas::http::matcher("/auth", "POST"),
        boost::bind(&atlas::api::server::serve, auth_api_server, _1, _2, _3, _4)
        );

    return out;
}


#include "server.hpp"

#include "hades/connection.hpp"
#include "hades/mkstr.hpp"

#include "atlas/api/auth.hpp"
#include "atlas/http/server/router.hpp"
#include "atlas/http/server/install_static_file.hpp"
#include "atlas/http/server/static_file.hpp"
#include "atlas/jsonrpc/uri.hpp"
#include "atlas/log/log.hpp"

#include "api/api.hpp"
#include "db/create.hpp"
#include "uri/uri.hpp"

helios::server::server(
        const server::options& options,
        boost::shared_ptr<boost::asio::io_service> io_
        ) :
    m_io(io_),
    m_http_server(
        m_io,
        options.address.c_str(),
        options.port.c_str()
        ),
    m_mime_information(new atlas::http::mimetypes())
{
    if(!options.port.length())
        throw std::runtime_error("port number is required");
    if(!options.db_file.length())
        throw std::runtime_error("database file is required");

    atlas::log::information("server::server") << "opening db file " << options.db_file;
    m_connection.reset(new hades::connection(options.db_file));
    db::create(*m_connection);

    auto install_static_file = [this, &options](const char *filename) {
        atlas::http::install_static_file(
                m_http_server,
                *m_mime_information,
                hades::mkstr() << options.html_root << filename,
                filename
                );
    };

    atlas::http::install_static_file(
            m_http_server,
            *m_mime_information,
            hades::mkstr() << options.html_root << "/index.html",
            "/"
            );
    install_static_file("/index.html");
    install_static_file("/backbone.js");
    install_static_file("/underscore.js");
    install_static_file("/backbone-min.js");
    install_static_file("/underscore-min.js");
    install_static_file("/jquery.js");
    install_static_file("/modal.css");
    install_static_file("/modal.js");
    install_static_file("/application.js");
    install_static_file("/stacked_application.js");
    install_static_file("/models.js");
    install_static_file("/style.css");
    install_static_file("/pure-min.css");
    install_static_file("/open-iconic/font/css/open-iconic.css");
    install_static_file("/open-iconic/font/fonts/open-iconic.ttf");
    install_static_file("/open-iconic/font/fonts/open-iconic.woff");
    uri::install(*m_connection, m_http_server);

    api::install(*m_connection, m_api_server);
    m_http_server.router().install(
        atlas::http::matcher("/api_call", "post"),
        boost::bind(&atlas::jsonrpc::uri, m_io, boost::ref(m_api_server), _1, _2, _3, _4)
        );
    atlas::api::auth::install(*m_connection, m_auth_api_server);
    m_http_server.router().install(
        atlas::http::matcher("/auth", "post"),
        boost::bind(&atlas::jsonrpc::uri, m_io, boost::ref(m_auth_api_server), _1, _2, _3, _4)
        );

    atlas::log::information("server::server") << "server listening on port " <<
        options.port;
}

void helios::server::start()
{
    atlas::log::information("server::start") << "running server";

    m_http_server.start();
}

void helios::server::stop()
{
    atlas::log::information("server::stop") << "stopping server";
    m_http_server.stop();
}

hades::connection& helios::server::db()
{
    return *m_connection;
}

boost::shared_ptr<boost::asio::io_service> helios::server::io()
{
    return m_io;
}


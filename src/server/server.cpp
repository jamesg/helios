#include "server.hpp"

#include "hades/connection.hpp"
#include "hades/mkstr.hpp"

#include "api/api.hpp"
#include "db/create.hpp"
#include "http/server/handler.hpp"
#include "http/server/install_static_file.hpp"
#include "http/server/static_file.hpp"
#include "jsonrpc/uri.hpp"
#include "log/log.hpp"
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

    atlas::http::install_static_file(m_http_server, *m_mime_information, "index.html", "/");
    atlas::http::install_static_file(m_http_server, *m_mime_information, "index.html");
    atlas::http::install_static_file(m_http_server, *m_mime_information, "bundle.js");
    atlas::http::install_static_file(m_http_server, *m_mime_information, "pure-min.css");
    atlas::http::install_static_file(m_http_server, *m_mime_information, "favicon.png");
    atlas::http::install_static_file(m_http_server, *m_mime_information, "grids-responsive-old-ie-min.css");
    atlas::http::install_static_file(m_http_server, *m_mime_information, "grids-responsive-min.css");
    atlas::http::install_static_file(m_http_server, *m_mime_information, "open-iconic/font/css/open-iconic.css");
    atlas::http::install_static_file(m_http_server, *m_mime_information, "open-iconic/font/fonts/open-iconic.ttf");
    atlas::http::install_static_file(m_http_server, *m_mime_information, "open-iconic/font/fonts/open-iconic.woff");
    uri::install(*m_connection, m_http_server);

    api::install(*m_connection, m_api_server);
    m_http_server.router().install(
        "/api_call",
        boost::bind(&atlas::jsonrpc::uri, m_io, boost::ref(m_api_server), _1, _2, _3)
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


#include "server.hpp"

#include "hades/connection.hpp"
#include "hades/mkstr.hpp"

#include "atlas/api/auth.hpp"
#include "atlas/http/server/router.hpp"
#include "atlas/http/server/install_static_file.hpp"
#include "atlas/http/server/static_file.hpp"
#include "atlas/http/server/static_files.hpp"
#include "atlas/jsonrpc/uri.hpp"
#include "atlas/log/log.hpp"

#include "api/api.hpp"
#include "db/create.hpp"
#include "router.hpp"

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

    boost::shared_ptr<atlas::http::router> helios_router(new helios::router(*m_connection, m_io));
    m_http_server.router().install(
        atlas::http::matcher("(.*)", 1),
        boost::bind(&atlas::http::router::serve, helios_router, _1, _2, _3, _4)
        );

    boost::shared_ptr<atlas::http::router> atlas_router(atlas::http::static_files());
    m_http_server.router().install(
        atlas::http::matcher("/atlas(.*)"),
        boost::bind(&atlas::http::router::serve, atlas_router, _1, _2, _3, _4)
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


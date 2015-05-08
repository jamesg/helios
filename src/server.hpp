#ifndef HELIOS_SERVER_SERVER_HPP
#define HELIOS_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "atlas/api/server.hpp"
#include "atlas/http/server/mimetypes.hpp"
#include "atlas/http/server/server.hpp"

namespace hades
{
    class connection;
}
namespace atlas
{
    namespace http
    {
        class mimetypes;
    }
}
namespace helios
{
    class server
    {
    public:
        struct options
        {
            std::string address;
            std::string db_file;
            std::string port;
            options() :
                address("0.0.0.0")
            {
            }
        };

        server(const options&, boost::shared_ptr<boost::asio::io_service>);

        void start();
        void stop();

        hades::connection& db();
        boost::shared_ptr<boost::asio::io_service> io();
    private:
        boost::shared_ptr<boost::asio::io_service> m_io;

        // This is a scoped_ptr rather than a plain member to allow deferred
        // initialisation.
        boost::scoped_ptr<hades::connection> m_connection;
        atlas::http::server m_http_server;
        boost::scoped_ptr<atlas::http::mimetypes> m_mime_information;
    };
}

#endif


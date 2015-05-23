#ifndef HELIOS_ROUTER_HPP
#define HELIOS_ROUTER_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include "atlas/api/server.hpp"
#include "atlas/http/server/application_router.hpp"
#include "atlas/http/server/mimetypes.hpp"

namespace hades
{
    class connection;
}
namespace helios
{
    class router : public atlas::http::application_router
    {
    public:
        router(hades::connection&, boost::shared_ptr<boost::asio::io_service>);
    private:
        atlas::api::server m_api_server;
    };
}

#endif


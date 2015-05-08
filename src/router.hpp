#ifndef HELIOS_ROUTER_HPP
#define HELIOS_ROUTER_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include "atlas/http/server/router.hpp"

namespace hades
{
    class connection;
}
namespace helios
{
    boost::shared_ptr<atlas::http::router> router(hades::connection&, boost::shared_ptr<boost::asio::io_service>);
}

#endif


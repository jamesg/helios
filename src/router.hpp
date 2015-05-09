#ifndef HELIOS_ROUTER_HPP
#define HELIOS_ROUTER_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include "atlas/http/server/mimetypes.hpp"
#include "atlas/http/server/router.hpp"

namespace hades
{
    class connection;
}
namespace helios
{
    class router : public atlas::http::router
    {
    public:
        router(hades::connection&, boost::shared_ptr<boost::asio::io_service>);

    private:
        void install_static_text(
                const std::string& url,
                const std::string& text
                );
        atlas::http::mimetypes m_mime_information;
    };
}

#endif


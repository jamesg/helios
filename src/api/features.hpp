#ifndef HELIOS_API_FEATURES_HPP
#define HELIOS_API_FEATURES_HPP

#include "styx/object.hpp"

namespace hades
{
    class connection;
}
namespace atlas
{
    namespace api
    {
        class server;
    }
}

namespace helios
{
    namespace api
    {
        namespace features
        {
            void install(hades::connection&, atlas::api::server&);
        }
    }
    struct features :
        styx::object
    {
        bool& gazetteer() { return get_bool("gazetteer"); }
        bool& photographs() { return get_bool("photographs"); }

        features()
        {
            gazetteer() = false;
            photographs() = true;
        }
    };

    features get_features();
}

#endif


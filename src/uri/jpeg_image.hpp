#ifndef PHOTOGRAPH_URI_JPEG_IMAGE_HPP
#define PHOTOGRAPH_URI_JPEG_IMAGE_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "mongoose.h"

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    class server;

    namespace uri
    {
        int jpeg_image(
                const server&,
                mg_connection*,
                mg_event,
                sqlite::connection& photograph_db,
                sqlite::connection& auth_db,
                sqlite::connection& cache_db
                );
    }
}

#endif


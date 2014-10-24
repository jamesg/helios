#ifndef HELIOS_EXPORTS_EXPORTS_HPP
#define HELIOS_EXPORTS_EXPORTS_HPP

namespace helios
{
    namespace exports
    {
        /*!
         * Entry point.  Export photographs in a database to the filesystem.
         *
         * The output directory is specified with the --out parameter.  A
         * directory is created below the output directory for each album in
         * the database.  Images in each album are exported to the album's
         * directory named according to the date of the image.  The naming
         * scheme is "yyyy-mm-dd_xxxx.jpg", where xxxx is a counter that is
         * reset for each date.
         */
        int main(int argc, const char* argv[]);
    }
}

#endif


#ifndef HELIOS_UTIL_SCALE_HPP
#define HELIOS_UTIL_SCALE_HPP

#include <string>
#include <vector>

namespace helios
{
    namespace util
    {
        /*!
         * Scale an image.
         *
         * \param geometry An ImageMagick geometry string.
         */
        void scale(
                const std::vector<unsigned char> image,
                const std::string& geometry,
                std::vector<unsigned char>& scaled_image
                );

        /*!
         * Scale an image.
         *
         * \param width Width of the resulting image.
         * \param height Height of the resulting image.
         */
        void scale(
                const std::vector<unsigned char> image,
                const int width,
                const int height,
                std::vector<unsigned char>& scaled_image
                );
    }
}

#endif


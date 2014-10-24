#ifndef HELIOS_UTIL_SCALE_HPP
#define HELIOS_UTIL_SCALE_HPP

#include <string>
#include <vector>

namespace helios
{
    namespace util
    {
        /*!
         * \brief Scale an image and rotate it if the Exif orientation flag suggests it should be.
         *
         * \param height Height of the output image (after rotation).
         * \param width Width of the output image (after rotation).
         */
        std::vector<unsigned char> scale_and_rotate(
                const std::vector<unsigned char>& image,
                int height,
                int width
                );
        /*!
         * \brief Scale an image.
         *
         * \param geometry An ImageMagick geometry string.
         */
        void scale(
                const std::vector<unsigned char>& image,
                const std::string& geometry,
                std::vector<unsigned char>& scaled_image
                );

        /*!
         * \brief Scale an image.
         *
         * \param width Width of the resulting image.
         * \param height Height of the resulting image.
         */
        void scale(
                const std::vector<unsigned char>& image,
                const int width,
                const int height,
                std::vector<unsigned char>& scaled_image
                );
    }
}

#endif


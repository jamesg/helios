#include "util/scale.hpp"

#include <sstream>

#include <Magick++.h>

void helios::util::scale(
    const std::vector<unsigned char> data,
    const std::string& geometry,
    std::vector<unsigned char>& scaled_image
    )
{
    Magick::Image image(Magick::Blob(
        reinterpret_cast<const void*>(&(data[0])), data.size())
        );
    image.scale(Magick::Geometry(geometry));

    Magick::Image out_image(image.size(), Magick::Color(255,255,255));
    out_image.composite(image, 0, 0);

    Magick::Blob blob;
    out_image.write(&blob, "JPEG");

    scaled_image = std::vector<unsigned char>(
            (const unsigned char*)blob.data(),
            (const unsigned char*)blob.data() + blob.length()
            );
}

void helios::util::scale(
        const std::vector<unsigned char> image,
        const int width,
        const int height,
        std::vector<unsigned char>& scaled_image
        )
{
    std::ostringstream oss;
    oss << width << "x" << height;
    scale(image, oss.str(), scaled_image);
}


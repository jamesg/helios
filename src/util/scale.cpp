#include "util/scale.hpp"

#include <sstream>

#include <exiv2/exiv2.hpp>
#include <Magick++.h>

std::vector<unsigned char> helios::util::scale_and_rotate(
    const std::vector<unsigned char>& data,
    int height,
    int width
    )
{
    Magick::Image image(Magick::Blob(
        reinterpret_cast<const void*>(&(data[0])), data.size())
        );

    short orientation = 1;
    try
    {   // Retrieve orientation
        auto exiv_image = Exiv2::ImageFactory::open(
            reinterpret_cast<const unsigned char*>(&(data[0])),
            data.size()
            );
        exiv_image->readMetadata();

        Exiv2::ExifKey key("Exif.Image.Orientation");
        Exiv2::ExifData::iterator pos = exiv_image->exifData().findKey(key);

        if( pos != exiv_image->exifData().end() )
            orientation = pos->getValue()->toLong();
    }
    catch(const std::exception&)
    {
        // Some images don't have an orientation.
    }

    switch(orientation)
    {
    case 6:
    case 8:
        // Swap width and height because the image is about to be rotated
        std::swap(width, height);
    }

    std::ostringstream geometry;
    geometry << width << "x" << height;

    image.scale(Magick::Geometry(geometry.str()));

    switch(orientation)
    {
        case 3:
            image.rotate(180);
        case 6:
            image.rotate(90);
        case 8:
            image.rotate(270);
    }

    Magick::Image out_image(image.size(), Magick::Color(255,255,255));
    out_image.composite(image, 0, 0);

    Magick::Blob blob;
    out_image.write(&blob, "JPEG");

    return std::vector<unsigned char>(
            (const unsigned char*)blob.data(),
            (const unsigned char*)blob.data() + blob.length()
            );
}

void helios::util::scale(
    const std::vector<unsigned char>& data,
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
        const std::vector<unsigned char>& image,
        const int width,
        const int height,
        std::vector<unsigned char>& scaled_image
        )
{
    std::ostringstream oss;
    oss << width << "x" << height;
    scale(image, oss.str(), scaled_image);
}


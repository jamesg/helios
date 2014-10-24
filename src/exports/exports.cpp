#include "exports/exports.hpp"

#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>

#include "commandline/commandline.hpp"
#include "hades/connection.hpp"
#include "hades/crud.ipp"
#include "hades/filter.hpp"
#include "hades/join.hpp"
#include "hades/row.hpp"

#include "db/photograph.hpp"
#include "db/jpeg_data.hpp"
#include "util/scale.hpp"

int helios::exports::main(int argc, const char* argv[])
{
    bool show_help = false, fullsize = false;
    std::string db_file, output_directory, height_s, width_s, geometry;
    std::vector<commandline::option> options{
        commandline::parameter("db", db_file, "Database file path"),
        commandline::parameter("out", output_directory, "Output directory"),
        commandline::parameter("height", height_s, "Maximum height of output"),
        commandline::parameter("width", width_s, "Maximum width of output"),
        commandline::flag("fullsize", fullsize, "Do not scale images"),
        commandline::flag("help", show_help, "Show a help message")
    };
    commandline::parse(argc, argv, options);

    if(show_help)
    {
        commandline::print(argc, argv, options);
        return 0;
    }

    if(db_file.empty())
    {
        std::cerr << "database file not specified" << std::endl;
        commandline::print(argc, argv, options);
        return 1;
    }
    if(output_directory.empty())
    {
        std::cerr << "output directory not specified" << std::endl;
        commandline::print(argc, argv, options);
        return 1;
    }

    const bool geometry_empty = height_s.empty() || width_s.empty();
    if((!fullsize && geometry_empty) || (fullsize && !geometry_empty))
    {
        std::cerr <<
            "either --fullsize or --height and --width must be specified" <<
            std::endl;
        commandline::print(argc, argv, options);
        return 1;
    }

    int height = 0, width = 0;
    try
    {
        if(!geometry_empty)
        {
            height = boost::lexical_cast<int>(height_s);
            width = boost ::lexical_cast<int>(width_s);
        }
    }
    catch(const std::exception&)
    {
        std::cerr << "both --height and --width must be integers" << std::endl;
        commandline::print(argc, argv, options);
        return -1;
    }

    hades::connection conn(db_file);
    boost::filesystem::path out_dir(output_directory);

    styx::list albums = helios::album::get_collection(conn);
    for(const styx::element& album_e : albums)
    {
        helios::album album(album_e);
        boost::filesystem::path album_dir =
            out_dir/album.get_string<db::attr::album::name>();
        std::cerr << "exporting to directory " << album_dir << std::endl;
        boost::filesystem::create_directory(album_dir);

        auto where =
            hades::where<int>(
                "album.album_id = ? AND "
                "photograph.photograph_id = photograph_in_album.photograph_id AND "
                "album.album_id = photograph_in_album.album_id ",
                hades::row<int>(album.get_int<db::attr::album::album_id>())
                );
        auto order = hades::order_by("photograph.taken ASC");
        styx::list photographs = hades::join<
            helios::photograph,
            helios::photograph_in_album,
            helios::album>(
                conn,
                hades::filter<hades::where<int>>(where, order)
                );

        static const char *unknown_string = "unknown";
        std::string last_taken = unknown_string;
        int count = 0;
        for(const styx::element& photograph_e : photographs)
        {
            helios::photograph photograph(photograph_e);
            int photograph_id = photograph.get_int<db::attr::photograph::photograph_id>();
            helios::jpeg_data_db data =
                db::jpeg_data::get_by_id(conn, photograph_id);

            // Write out the photograph.
            const std::string taken_raw = photograph.get_string<db::attr::photograph::taken>();
            const std::string taken = taken_raw.length()?
                std::string(taken_raw, 0, 10):unknown_string;
            if(taken == last_taken)
                count++;
            else
            {
                count = 1;
                last_taken = taken;
            }

            std::ostringstream out_filename;
            out_filename << taken << "_" << std::setfill('0') <<
                std::setw(4) << count << ".jpg";

            const boost::filesystem::path jpeg_file = album_dir/out_filename.str();
            std::cerr << "export photograph to " << jpeg_file << std::endl;
            boost::filesystem::ofstream os(jpeg_file);

            if(fullsize)
            {
                os << std::string((const char*)(&(data.data[0])), data.data.size());
            }
            else
            {
                std::vector<unsigned char> data_scaled =
                    util::scale_and_rotate(data.data, height, width);
                os << std::string((const char*)(&(data_scaled[0])), data_scaled.size());
            }
            os.close();
        }
    }
}


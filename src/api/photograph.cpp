#include "photograph.hpp"

#include <locale>

#include <boost/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/fusion/include/vector.hpp>

#include "hades/crud.ipp"
#include "hades/get_by_id.hpp"
#include "hades/get_collection.hpp"
#include "hades/join.hpp"

#include "api/server.hpp"
#include "db/photograph.hpp"

void helios::api::photograph::install(
        hades::connection& conn,
        atlas::api::server& server
        )
{
    server.install<styx::element, int>(
            "photograph_get",
            [&conn](int id) {
                helios::photograph p;
                p.from_id(conn, helios::photograph::id_type{id});
                return p.get_element();
            }
            );
    server.install<styx::list>(
            "photograph_list",
            boost::bind(&helios::photograph::get_collection, boost::ref(conn))
            );

    // Album methods.
    //server.install(
        //"add_photograph_to_album",
        //boost::bind(photoalbum::api::add_photograph_to_album, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"album_list",
        //boost::bind(photoalbum::api::album_list, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"delete_album",
        //boost::bind(photoalbum::api::delete_album, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"insert_album",
        //boost::bind(photoalbum::api::insert_album, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"update_album",
        //boost::bind(photoalbum::api::update_album, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);

    // JPEG data methods.
    //api_server.install(
        //"insert_jpeg_data",
        //boost::bind(photoalbum::api::insert_jpeg_data, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"jpeg_data",
        //boost::bind(photoalbum::api::jpeg_data, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"jpeg_data_scaled",
        //boost::bind(photoalbum::api::jpeg_data_scaled, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);

    //api_server.install(
        //"locations",
        //boost::bind(photoalbum::api::locations, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"tags",
        //boost::bind(photoalbum::api::tags, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"tags_alphabetical",
        //boost::bind(photoalbum::api::tags_alphabetical, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"tags_popular",
        //boost::bind(photoalbum::api::tags_popular, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);

    // Photograph methods.
    //api_server.install(
        //"photograph",
        //boost::bind(photoalbum::api::get_photograph, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"delete_photograph",
        //boost::bind(photoalbum::api::delete_photograph, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"insert_photograph",
        //boost::bind(photoalbum::api::insert_photograph, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"photograph_albums",
        //boost::bind(photoalbum::api::photograph_albums, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"photograph_list",
        //boost::bind(photoalbum::api::photograph_list, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"photographs_in_album",
        //boost::bind(photoalbum::api::photographs_in_album, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"photographs_with_location",
        //boost::bind(photoalbum::api::photographs_with_location, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"photographs_with_tag",
        //boost::bind(photoalbum::api::photographs_with_tag, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"recent_photographs",
        //boost::bind(photoalbum::api::recent_photographs, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"remove_photograph_from_album",
        //boost::bind(photoalbum::api::remove_photograph_from_album, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"uncategorised_photographs",
        //boost::bind(photoalbum::api::uncategorised_photographs, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"update_photograph",
        //boost::bind(photoalbum::api::update_photograph, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);

    // Note methods.
    //api_server.install(
        //"markdown_data",
        //boost::bind(photoalbum::api::markdown_data, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"note",
        //boost::bind(photoalbum::api::note, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"note_list",
        //boost::bind(photoalbum::api::note_list, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"note_version",
        //boost::bind(photoalbum::api::note_version, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"note_phase_version",
        //boost::bind(photoalbum::api::note_phase_version, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
            //"create_draft_note",
            //boost::bind(photoalbum::api::create_draft_note, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
            //);
    //api_server.install(
        //"delete_note",
        //boost::bind(photoalbum::api::delete_note, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"update_markdown_data",
        //boost::bind(photoalbum::api::update_markdown_data, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"publish_note",
        //boost::bind(photoalbum::api::publish_note, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
    //api_server.install(
        //"published_notes",
        //boost::bind(photoalbum::api::published_notes, _1, _2, boost::ref(db)),
        //boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        //);
}

//void photoalbum::api::get_photograph(
        //jsonrpc::request&   request,
        //jsonrpc::result&    result,
        //sqlite::connection& db
        //)
//{
    //try
    //{
        //int id = request.params().get<int>(0);
        //json::object photo_obj = json::map();
        //photograph_t photo(photo_obj);
        //db::get_by_id(id, db, photo);
        //result.data() = photo.get_object();
    //}
    //catch( const std::exception& e )
    //{
        //std::cerr << "In API function photograph: " << e.what() << std::endl;
    //}
//}

//void photoalbum::api::delete_photograph(
        //jsonrpc::request&   request,
        //jsonrpc::result&    result,
        //sqlite::connection& db
        //)
//{
    //try
    //{
        //sqlite::devoid(
                //"DELETE FROM photograph WHERE photograph_id = ?",
                //boost::fusion::vector<int>(request.params().get<int>(0)),
                //db
                //);
    //}
    //catch( const std::exception& e )
    //{
        //result.error() = "Error updating photograph";
    //}
//}

//void photoalbum::api::insert_photograph(
        //jsonrpc::request&   request,
        //jsonrpc::result&    result,
        //sqlite::connection& db
        //)
//{
    //int id;
    //try
    //{
        //id = db::insert(::photoalbum::photograph(request.params()[0]), db);
    //}
    //catch( const std::exception& e )
    //{
        //result.error() = "Error inserting photograph details.";
    //}
    //json::map o = json::map();
    //o.get_int("photograph_id") = id;
    //result.data() = o;
//}

//void photoalbum::api::photograph_albums(
        //jsonrpc::request& request,
        //jsonrpc::result& result,
        //sqlite::connection& db
        //)
//{
    //json::list photographs = json::list();
    //int pid = request.params().get<int>(0);
    //sqlite::select<album>(
            //db,
            //"SELECT DISTINCT album_id, name, caption, fromdate, todate "
            //"FROM photograph_in_album NATURAL JOIN album "
            //"WHERE photograph_id = ? ",
            //boost::fusion::vector<int>(pid),
            //photographs
            //);
    //result.data() = photographs;
//}

//void photoalbum::api::photograph_list(
        //jsonrpc::request&   request,
        //jsonrpc::result&    result,
        //sqlite::connection& db
        //)
//{
    //json::list photo_list = json::list();
    //db::get_photograph_list(db, photo_list);
    //result.data() = photo_list;
//}

//void photoalbum::api::photographs_in_album(
        //jsonrpc::request&   request,
        //jsonrpc::result&    result,
        //sqlite::connection& db
        //)
//{
    //int id = request.params().get<int>(0);
    //json::list photographs = json::list();
    //db::get_photographs_by_album(db, id, photographs);
    //result.data() = photographs;
//}

//void photoalbum::api::remove_photograph_from_album(
        //jsonrpc::request&   request,
        //jsonrpc::result&    result,
        //sqlite::connection& db
        //)
//{
    //try
    //{
        //sqlite::devoid(
                //"DELETE FROM photograph_in_album "
                //"WHERE photograph_id = ? AND album_id = ?",
                //sqlite::row<int, int>(
                    //request.params().get<int>(0),
                    //request.params().get<int>(1)
                    //),
                //db
                //);
    //}
    //catch( const std::exception& e )
    //{
    //}
//}

//void photoalbum::api::update_photograph(
        //jsonrpc::request&   request,
        //jsonrpc::result&    result,
        //sqlite::connection& db
        //)
//{
    //try
    //{
        //::photoalbum::photograph photo(request.params()[0]);
        //db::update(photo, db);
    //}
    //catch( const std::exception& e )
    //{
        //result.error() = "Error updating photograph";
    //}
//}

//void photoalbum::api::photographs_with_tag(
            //jsonrpc::request&   request,
            //jsonrpc::result&    result,
            //sqlite::connection& db
            //)
//{
    //json::list photographs;
    //sqlite::select< ::photoalbum::photograph>(
            //db,
            //"SELECT DISTINCT photograph.photograph_id, title, caption, "
            //"filename, location, taken "
            //"FROM photograph NATURAL JOIN photograph_tagged "
            //"WHERE tag = ? ",
            //boost::fusion::vector<std::string>(
                //request.params().get<std::string>(0)
                //),
            //photographs
            //);
    //result.data() = photographs;
//}

//void photoalbum::api::photographs_with_location(
        //jsonrpc::request&   request,
        //jsonrpc::result&    result,
        //sqlite::connection& db
        //)
//{
    //json::list photographs;
    //sqlite::select< ::photoalbum::photograph>(
            //db,
            //"SELECT DISTINCT "
            //"photograph_id, title, caption, filename, location, taken "
            //"FROM photograph "
            //"WHERE location = ? ",
            //boost::fusion::vector<std::string>(
                //request.params().get<std::string>(0)
                //),
            //photographs);
    //result.data() = photographs;
//}


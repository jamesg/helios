var _ = require('underscore');
var Backbone = require('backbone');

var api = require('../service/api');

var Photograph = require('../model/photograph').Photograph;

exports.PhotographAlbum = Backbone.Collection.extend(
    {
        initialize: function(options) {
            this.album = options.album;
            console.log('init', this.album);
        },
        model: Photograph,
        sync: function(method, model, options) {
            console.log('PhotographAlbum sync');
            return api.backboneSyncFunction(
                {
                    read: api.rpc.bind(this, 
                              {
                                  method: 'photographs_in_album',
                                  params: [this.album.get('album_id')]
                              }
                              )
                    //read: _.partial(
                              //api.rpc,
                              //{
                                  //method: 'photographs_in_album',
                                  //params: this.album.get('album_id')
                              //}
                              //)
                          //api.rpcFunction('photographs_in_album'),
                          //this.album.get('album_id')
                          //)
                }
                )(method, model, options);
        }
        //api.backboneSyncFunction(
            //{
                //read: _.partial(
                      //api.rpcFunction('photographs_in_album'),
                      //this.album.get('album_id')
                      //)
            //}
            //)
    }
    );


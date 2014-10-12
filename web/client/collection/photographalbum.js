var _ = require('underscore');
var Backbone = require('backbone');

var api = require('../service/api');

var Photograph = require('../model/photograph').Photograph;

/*!
 * \brief Collection of Photographs in a given album.
 *
 * \param options Map of options.  Should contain an Album model as 'album'.
 */
exports.PhotographAlbum = Backbone.Collection.extend(
    {
        initialize: function(options) {
            this.album = options.album;
        },
        model: Photograph,
        sync: function(method, model, options) {
            return api.backboneSyncFunction(
                {
                    read: api.rpc.bind(
                        this,
                        {
                            method: 'photographs_in_album',
                            params: [this.album.get('album_id')]
                        }
                        )
                }
                )(method, model, options);
        }
    }
    );


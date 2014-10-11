var Backbone = require('backbone');

var api = require('../service/api');

var Album = require('../model/album').Album;

exports.AlbumCollection = Backbone.Collection.extend(
    {
        model: Album,
        sync: api.backboneSyncFunction({ read: 'album_list' })
    }
    );


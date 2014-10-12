var Backbone = require('backbone');
var Album = require('../model/album').Album;
var api = require('../service/api');

exports.AlbumCollection = Backbone.Collection.extend(
    {
        model: Album,
        sync: api.backboneSyncFunction({ read: 'album_list' })
    }
    );


var Backbone = require('backbone');
var api = require('../service/api');
var Photograph = require('../model/photograph').Photograph;

/*!
 * \brief Collection of Photographs that were taken recently (the date limit or
 * number of photographs returned is determined by the server).
 */
exports.RecentlyTaken = Backbone.Collection.extend(
    {
        model: Photograph,
        sync: api.backboneSyncFunction({ read: 'photograph_recent' })
    }
    );


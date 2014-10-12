var Backbone = require('backbone');
var api = require('../service/api');
var Photograph = require('../model/photograph').Photograph;

/*!
 * \brief Collection of photographs that are not a member of any album.
 */
exports.Uncategorised = Backbone.Collection.extend(
    {
        model: Photograph,
        sync: api.backboneSyncFunction({ read: 'photograph_uncategorised' })
    }
    );


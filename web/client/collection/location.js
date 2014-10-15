var Backbone = require('backbone');
var Location = require('../model/location').Location;
var api = require('../service/api');

exports.LocationCollection = Backbone.Collection.extend(
    {
        model: Location,
        sync: api.backboneSyncFunction({ read: 'location_list' })
    }
    );


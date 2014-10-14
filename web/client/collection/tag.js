var Backbone = require('backbone');
var Tag = require('../model/tag').Tag;
var api = require('../service/api');

exports.TagCollection = Backbone.Collection.extend(
    {
        model: Tag,
        sync: api.backboneSyncFunction({ read: 'tag_list' })
    }
    );


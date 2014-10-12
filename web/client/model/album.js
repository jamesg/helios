var _ = require('underscore');
var Backbone = require('backbone');

var api = require('../service/api');

exports.Album = Backbone.Model.extend(
    {
        defaults: {
            name: ''
        },
        idAttribute: 'album_id',
        sync: api.backboneSyncFunction(
            {
                create: 'album_save',
                delete: 'album_destroy',
                update: 'album_save'
            }
            ),
        validate: function() {
            var errors = {};
            if(this.get('name') == '')
                errors['name'] = 'Name is required';
            if(!_.isEmpty(errors))
                return errors;
        }
    }
    );


var _ = require('underscore');
var Backbone = require('backbone');

var api = require('../service/api');

/*!
 * \brief Extended information on a location.
 *
 * \note photograph_count will not be saved.
 */
exports.Location = Backbone.Model.extend(
    {
        defaults: {
            location: '',
            photograph_count: 0
        },
        idAttribute: 'location',
        sync: api.backboneSyncFunction(
            {
                delete: 'location_destroy',
                update: 'location_save'
            }
            ),
        validate: function() {
            var errors = {};
            if(this.get('location') == '')
                errors['location'] = 'Name is required';
            if(!_.isEmpty(errors))
                return errors;
        }
    }
    );


var _ = require('underscore');
var Backbone = require('backbone');

var api = require('../service/api');

/*!
 * \brief Extended information on a tag.
 *
 * \note photograph_count will not be saved.
 */
exports.Tag = Backbone.Model.extend(
    {
        defaults: {
            tag: '',
            photograph_count: 0
        },
        idAttribute: 'tag',
        sync: api.backboneSyncFunction(
            {
                delete: 'tag_destroy',
                update: 'tag_save'
            }
            ),
        validate: function() {
            var errors = {};
            if(this.get('tag') == '')
                errors['tag'] = 'Name is required';
            if(!_.isEmpty(errors))
                return errors;
        }
    }
    );


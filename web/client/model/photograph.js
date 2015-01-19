var _ = require('underscore');
var Backbone = require('backbone');

var api = require('../service/api');

exports.Photograph = Backbone.Model.extend(
    {
        defaults: {
            title: '',
            taken: '',
            tags: ''
        },
        idAttribute: 'photograph_id',
        validate: function() {
            var errors = {};
            if(this.get('title') == '')
                errors['title'] = 'Title is required';
            if(!_.isEmpty(errors))
                return errors;
        },
        sync: function(method, model, options) {
            return api.backboneSyncFunction(
                {
                    create: 'photograph_save',
                    delete: 'photograph_delete',
                    read: api.rpc.bind(
                        this,
                        {
                            method: 'photograph_get',
                            params: [this.get('photograph_id')]
                        }
                        ),
                    update: 'photograph_save'
                }
                )(method, model, options);
        }
    }
    );


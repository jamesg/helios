var Backbone = require('backbone');

var api = require('../service/api');

exports.Photograph = Backbone.Model.extend(
    {
        defaults: {
            title: '',
            taken: ''
        },
        idAttribute: 'photograph_id',
        validate: function() {
            var errors = {};
            if(this.get('title') == '')
                errors['title'] = 'Title is required';
            if(!_.isEmpty(errors))
                return errors;
        }
    }
    );


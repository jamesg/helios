var Backbone = require('backbone');
var api = require('../service/api');
var Photograph = require('../model/photograph').Photograph;

/*!
 * \brief List of photographs taken at a given location.
 *
 * \param location Location model.
 */
exports.LocationPhotographs = Backbone.Collection.extend(
    {
        initialize: function(options) {
            this.location = options.location;
        },
        model: Photograph,
        sync: function(method, model, options) {
            return api.backboneSyncFunction(
                {
                    read: api.rpc.bind(
                        this,
                        {
                            method: 'photographs_with_location',
                            params: [this.location.get('location')]
                        }
                        )
                }
                )(method, model, options);
        }
    }
    );


var Backbone = require('backbone');
var api = require('../service/api');
var Photograph = require('../model/photograph').Photograph;

/*!
 * \brief List of photographs with a given tag.
 *
 * \param tag Tag model.
 */
exports.TagPhotographs = Backbone.Collection.extend(
    {
        initialize: function(options) {
            this.tag = options.tag;
        },
        model: Photograph,
        sync: function(method, model, options) {
            return api.backboneSyncFunction(
                {
                    read: api.rpc.bind(
                        this,
                        {
                            method: 'photographs_with_tag',
                            params: [this.tag.get('tag')]
                        }
                        )
                }
                )(method, model, options);
        }
    }
    );


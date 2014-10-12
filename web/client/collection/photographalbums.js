var AlbumCollection = require('./album').AlbumCollection;
var api = require('../service/api');

/*!
 * \brief Collection of albums that a given photograph is a member of.
 */
exports.PhotographAlbums = AlbumCollection.extend(
    {
        initialize: function(options) {
            this.photograph = options.photograph;
        },
        sync: function(method, model, options) {
            return api.backboneSyncFunction(
                {
                    read: api.rpc.bind(
                          this,
                          {
                              method: 'photograph_albums',
                              params: [this.photograph.get('photograph_id')]
                          }
                          )
                }
                )(method, model, options);
        }
    }
    );


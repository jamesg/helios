var _ = require('underscore');
var Album = require('../model/album').Album;
var StaticView = require('../view/static').StaticView;

/*!
 * \brief Abstract album form class.
 *
 * \note template is not implemented.
 */
exports.AlbumForm = StaticView.extend(
    {
        initialize: function(options) {
            if(!_.has(this, 'model'))
                this.model = new Album;
            this.render();
        }
    }
    );


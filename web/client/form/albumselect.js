var _ = require('underscore');
var AlbumCollection = require('../collection/album').AlbumCollection;
var StaticView = require('../view/static').StaticView;

/*!
 * \brief Form for selecting a single album from a list.  Emits 'addToAlbum'
 * with the album id when the form is submitted.
 *
 * \param buttonText Text to be displayed on the button.
 */
exports.AlbumSelectForm = StaticView.extend(
    {
        initialize: function(options) {
            this.buttonText = options.buttonText;
            StaticView.prototype.initialize.apply(this, arguments);
            this.albumCollection = new AlbumCollection;
            this.listenTo(this.albumCollection, 'all', this.render.bind(this));
            this.albumCollection.fetch();
        },
        template: function() {
            var album = select({ name: 'album' });
            if(_.has(this.albumCollection, 'models'))
                _.each(
                    this.albumCollection.models,
                    function(model) {
                        album(
                            option(
                                { value: model.get('album_id') },
                                model.get('name')
                                )
                            );
                    }
                    );
            return form(
                {
                    class: 'pure-form',
                    onsubmit: (function() {
                        this.trigger('addToAlbum', album().value);
                        return false;
                    }).bind(this)
                },
                album,
                button(
                    { type: 'submit', class: 'pure-button pure-button-primary' },
                    this.buttonText
                    )
                );
        }
    }
    );


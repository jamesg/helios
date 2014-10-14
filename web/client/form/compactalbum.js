var AlbumForm = require('./album').AlbumForm;
var ui = require('../ui');

exports.CompactAlbumForm = AlbumForm.extend(
    {
        template: function() {
            var name = input({ type: 'text', placeholder: 'Name', value: this.model.get('name') });
            var caption = input({ type: 'text', placeholder: 'Caption', value: this.model.get('caption') });
            return form(
                {
                    class: 'pure-form',
                    onsubmit: (function() {
                        this.model.set('name', name().value);
                        this.model.set('caption', caption().value);
                        this.trigger('save', this.model);
                        return false;
                    }).bind(this)
                },
                fieldset(
                    legend('Album Details'),
                    name,
                    caption,
                    ui.saveButton(
                        this.model.isNew()?'Create Album':'Save Album'
                        )
                    )
                );
        }
    }
    );


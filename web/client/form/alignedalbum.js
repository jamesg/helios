var AlbumForm = require('./album').AlbumForm;
var ui = require('../ui');

exports.AlignedAlbumForm = AlbumForm.extend(
    {
        template: function() {
            var name = input({ type: 'text', value: this.model.get('name') });
            var caption = input({ type: 'text', value: this.model.get('caption') });
            return form(
                {
                    class: 'pure-form pure-form-aligned',
                    onsubmit: (function() {
                        this.model.set('name', name().value);
                        this.model.set('caption', caption().value);
                        this.trigger('save', this.model);
                        return false;
                    }).bind(this)
                },
                fieldset(
                    legend('Album Details'),
                    ui.inlineInput('Name', name),
                    ui.inlineInput('Caption', caption),
                    ui.inlineInput(
                        '',
                        ui.saveButton(
                            this.model.isNew()?'Create Album':'Save Album'
                            )
                        )
                    )
                );
        }
    }
    );


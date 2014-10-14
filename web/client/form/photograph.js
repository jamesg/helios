var _ = require('underscore');
var Photograph = require('../model/photograph').Photograph;
var StaticView = require('../view/static').StaticView;
var ui = require('../ui');

/*!
 * \brief Aligned photograph form for entering title, caption, date and
 * location.  'saved' is emitted with the current model when the form is
 * submitted.
 */
exports.PhotographForm = StaticView.extend(
    {
        initialize: function(options) {
            if(!_.has(this, 'model'))
                this.model = new Photograph;
            this.render();
        },
        template: function() {
            var title = input({ type: 'text', name: 'title', value: this.model.get('title') });
            var caption = input({ type: 'text', name: 'caption', value: this.model.get('caption') });
            var date = input({ type: 'text', name: 'date', value: this.model.get('date') });
            var location = input({ type: 'text', name: 'location', value: this.model.get('location') });
            return form(
                {
                    class: 'pure-form pure-form-aligned',
                    onsubmit: (function() {
                        this.model.set('title', title().value);
                        this.model.set('caption', caption().value);
                        this.model.set('date', date().value);
                        this.model.set('location', location().value);
                        this.model.save();
                        this.trigger('saved', this.model);
                        return false;
                    }).bind(this)
                },
                ui.inlineInput('Title', title),
                ui.inlineInput('Caption', caption),
                ui.inlineInput('Date', date),
                ui.inlineInput('Location', location),
                ui.inlineInput(
                    '',
                    ui.saveButton()
                    )
                );
        }
    }
    );


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
            StaticView.prototype.initialize.apply(this, arguments);
            if(!_.has(this, 'model'))
                this.model = new Photograph;
            this.render();
        },
        template: function() {
            var title = input({ type: 'text', name: 'title', value: this.model.get('title') });
            var caption = input({ type: 'text', name: 'caption', value: this.model.get('caption') });
            var date = input({ type: 'text', name: 'date', value: this.model.get('taken') });
            var location = input({ type: 'text', name: 'location', value: this.model.get('location') });
            var tags = input({ type: 'text', name: 'tags', value: this.model.get('tags') });
            return form(
                {
                    class: 'pure-form pure-form-aligned',
                    onsubmit: (function() {
                        this.model.set('title', title().value);
                        this.model.set('caption', caption().value);
                        this.model.set('taken', date().value);
                        this.model.set('location', location().value);
                        this.model.set('tags', tags().value);
                        this.model.save();
                        this.trigger('saved', this.model);
                        return false;
                    }).bind(this)
                },
                ui.inlineInput('Title', title),
                ui.inlineInput('Caption', caption),
                ui.inlineInput('Date', date),
                ui.inlineInput('Location', location),
                ui.inlineInput('Tags', tags),
                ui.inlineInput(
                    '',
                    ui.saveButton()
                    )
                );
        }
    }
    );


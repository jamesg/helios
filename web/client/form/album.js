var _ = require('underscore');
var Album = require('../model/album').Album;
var StaticView = require('../view/static').StaticView;
var ui = require('../ui');

exports.AlbumForm = StaticView.extend(
    {
        initialize: function(options) {
            if(_.has(options, 'model'))
                this.model = options.model;
            if(!_.has(this, 'model'))
                this.model = new Album;
            this.render();
        },
        template: function() {
            var name = input({ type: 'text', value: this.model.get('name') });
            var caption = input({ type: 'text', value: this.model.get('caption') });
            return form(
                {
                    class: 'pure-form pure-form-stacked',
                    onsubmit: (function() {
                        this.model.set('name', name().value);
                        this.model.set('caption', caption().value);
                        this.trigger('save', this.model);
                        return false;
                    }).bind(this)
                },
                label('Name:', name),
                label('Caption:', caption),
                button(
                    {
                        class: 'pure-button pure-button-primary',
                        type: 'submit'
                    },
                    ui.icon('data-transfer-download'),
                    this.model.isNew()?'Create Album':'Save Album'
                    )
                );
        }
    }
    );


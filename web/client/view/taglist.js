var GenericListView = require('./genericlist').GenericListView;
var StaticView = require('./static').StaticView;
var ui = require('../ui');

var TagLiView = StaticView.extend(
    {
        tagName: 'li',
        className: 'editable-li',
        template: function() {
            a(
                {
                    onclick: this.trigger.bind(this, 'click', this.model)
                },
                ui.icon('tags'), this.model.get('tag'),
                ' (', this.model.get('photograph_count'), ' ',
                    (this.model.get('photograph_count')>1)?
                    'photographs':'photograph',
                    ')'
                );
        }
    }
    );

exports.TagListView = GenericListView.extend(
    {
        view: TagLiView
    }
    );


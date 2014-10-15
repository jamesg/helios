var GenericListView = require('./genericlist').GenericListView;
var StaticView = require('./static').StaticView;
var ui = require('../ui');

var LocationLiView = StaticView.extend(
    {
        tagName: 'li',
        className: 'editable-li',
        template: function() {
            a(
                {
                    onclick: this.trigger.bind(this, 'click', this.model)
                },
                ui.icon('map'), this.model.get('location'),
                ' (', this.model.get('photograph_count'), ' ',
                    (this.model.get('photograph_count')>1)?
                    'photographs':'photograph',
                    ')'
                );
        }
    }
    );

exports.LocationListView = GenericListView.extend(
    {
        view: LocationLiView
    }
    );


var domjs = require('domjs')(document);
var _ = require('underscore');
var Backbone = require('backbone');
Backbone.$ = require('jbone');

var CollectionView = require('./collection').CollectionView;

var StaticView = require('./static').StaticView;

var LiView = StaticView.extend(
    {
        tagName: 'li',
        className: 'editable-li',
        template: function() {
            return a(
                {
                    onclick: this.trigger.bind(this, 'click', this.model)
                },
                this.model.get('name')
                );
        }
    }
    );

var UlView = CollectionView.extend(
    {
        view: LiView,
        tagName: 'ul',
        initializeView: function(view) {
            this.listenTo(view, 'click', this.trigger.bind(this, 'click'));
        }
    }
   );

exports.AlbumList = Backbone.View.extend(
    {
        tagName: 'div',
        className: 'pure-menu pure-menu-vertical pure-menu-open',
        initialize: function(options) {
            this.ulView = new UlView({ model: this.model });
            this.listenTo(this.ulView, 'click', this.trigger.bind(this, 'click'));
            this.render();
        },
        render: function() {
            this.$el.empty();
            this.el.appendChild(this.ulView.el);
            return this;
        }
    }
    );


var Backbone = require('backbone');

var CollectionView = require('./collection').CollectionView;
var StaticView = require('./static').StaticView;
var PhotographThumbView = require('./photographthumb').PhotographThumbView;

var LiView = StaticView.extend(
    {
        tagName: 'li',
        className: 'editable-li',
        initialize: function(options) {
            this.thumb = new PhotographThumbView({ model: options.model });
            this.listenTo(
                this.thumb,
                'click',
                this.trigger.bind(this, 'click', this.model)
                );
            this.render();
        },
        render: function() {
            this.$el.empty();
            this.el.appendChild(this.thumb.el);
            return this;
        }
    }
    );

var UlView = CollectionView.extend(
    {
        view: LiView,
        tagName: 'ul',
        className: 'thumblist',
        initializeView: function(view) {
            this.listenTo(view, 'click', this.trigger.bind(this, 'click'));
        }
    }
   );

exports.PhotographThumbList = Backbone.View.extend(
    {
        tagName: 'div',
        className: '',
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


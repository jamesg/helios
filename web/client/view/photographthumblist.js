var GenericListView = require('./genericlist').GenericListView;
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

exports.PhotographThumbListView = GenericListView.extend(
    {
        tagName: 'div',
        className: 'thumblist',
        view: LiView,
        initialize: function() {
            console.log('view', this.view);
            GenericListView.prototype.initialize.apply(this, arguments);
        }
    }
    );


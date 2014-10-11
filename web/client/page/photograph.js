var PageView = require('../view/page').PageView;
var StaticView = require('../view/static').StaticView;

var Photograph = require('../model/photograph').Photograph;

var DetailView = StaticView.extend(
    {
        template: function() {
            h2(this.model.get('Title'));
            caption(
                img(
                    {
                        alt: this.model.get('title'),
                        src: '/jpeg_image?photograph_id=' +
                            this.model.get('photograph_id') +
                            '&height=500&width=500'
                    }
                   ),
                span(this.model.get('title'))
                );
        }
    }
    );

exports.PhotographPage = PageView.extend(
    {
        fullPage: true,
        initialize: function(options) {
            this.detail = new DetailView({ model: options.model });
            this.render();
        },
        render: function() {
            this.$el.empty();
            this.$el.append(this.detail.el);
        }
    }
    );


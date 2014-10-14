var StaticView = require('./static').StaticView;

exports.PhotographThumbView = StaticView.extend(
    {
        template: function() {
            caption(
                img(
                    {
                        alt: this.model.get('title'),
                        src: '/jpeg_image?photograph_id=' + this.model.get('photograph_id'),
                        onclick: (function() {
                            this.trigger('click');
                        }).bind(this)
                    }
                   )/*,*/
                /*span(this.model.get('title'))*/
                );
        }
    }
    );


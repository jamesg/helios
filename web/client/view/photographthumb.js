var StaticView = require('./static').StaticView;

exports.PhotographThumbView = StaticView.extend(
    {
        template: function() {
            span({ class: 'vertical-align-helper' });
            img(
                {
                    alt: this.model.get('title'),
                    src:
                        '/jpeg_image?photograph_id=' +
                            this.model.get('photograph_id') +
                            '&height=140&width=140',
                    onclick: (function() {
                        this.trigger('click');
                    }).bind(this)
                }
               );
        }
    }
    );


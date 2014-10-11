var PageView = require('../view/page').PageView;

var AlbumsPage = require('./albums').AlbumsPage;
var UploadPage = require('./upload').UploadPage;

var icon = require('../ui').icon;

exports.HomePage = PageView.extend(
    {
        initialize: function(options) {
            this.application = options.application;
            this.render();
        },
        template: function() {
            var menuButton = (function(constructor, content) {
                return button(
                    {
                        class: 'pure-button',
                        onclick: this.application.gotoView.bind(this.application, constructor)
                    },
                    content
                    );
            }).bind(this);

            h2('Helios: ', small('Photo Storage'));
            div(
                { class: 'mainmenu' },
                menuButton(AlbumsPage, span(icon('book'), 'Albums')),
                menuButton(UploadPage, span(icon('data-transfer-upload'), 'Upload'))
               );
        }
    }
    );


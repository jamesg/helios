var PageView = require('../view/page').PageView;

var AlbumsPage = require('./albums').AlbumsPage;
var TagsPage = require('./tags').TagsPage;
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
                        onclick: this.application.gotoPage.bind(this.application, constructor)
                    },
                    content
                    );
            }).bind(this);

            return div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2('Helios: ', small('Photo Storage')),
                    div(
                        { class: 'mainmenu' },
                        menuButton(AlbumsPage, span(icon('book'), 'Albums')),
                        menuButton(TagsPage, span(icon('tags'), 'Tags')),
                        menuButton(UploadPage, span(icon('data-transfer-upload'), 'Upload'))
                       )
                   )
                );
        }
    }
    );


var PageView = require('../view/page').PageView;

var AlbumsPage = require('./albums').AlbumsPage;
var LocationsPage = require('./locations').LocationsPage;
var TagsPage = require('./tags').TagsPage;
var UploadPage = require('./upload').UploadPage;

var icon = require('../ui').icon;
var ui = require('./ui');

exports.HomePage = PageView.extend(
    {
        pageTitle: 'Home',
        initialize: function(options) {
            this.application = options.application;
            this.render();
        },
        template: function() {
            var menuButton = (function(constructor, content) {
                return div(
                    { class: 'pure-u-12-24 pure-u-md-8-24 pure-u-lg-6-24 pure-u-xl-4-24' },
                    button(
                        {
                            class: 'pure-button',
                            onclick: this.application.pushPage
                                    .bind(this.application, constructor)
                        },
                        content
                        )
                    );
            }).bind(this);

            var menuButton = ui.menuButton.bind(this, this.application);
            return div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2('Helios ', small('Photograph Album')),
                    ui.mainMenu(
                        menuButton(AlbumsPage, span(icon('book'), 'Albums')),
                        menuButton(TagsPage, span(icon('tags'), 'Tags')),
                        menuButton(LocationsPage, span(icon('map'), 'Locations')),
                        menuButton(UploadPage, span(icon('data-transfer-upload'), 'Upload'))
                        )
                   )
                );
        }
    }
    );


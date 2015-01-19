var CompactAlbumForm = require('../form/compactalbum').CompactAlbumForm;
var PageView = require('../view/page').PageView;
var PhotographAlbum = require('../collection/photographalbum').PhotographAlbum;
var PhotographPage = require('./photograph').PhotographPage;
var PhotographThumbListView = require('../view/photographthumblist').PhotographThumbListView;
var ui = require('../ui');

exports.AlbumPage = PageView.extend(
    {
        pageTitle: function() {
            return this.model.get('name');
        },
        fullPage: true,
        initialize: function(options) {
            this.application = options.application;
            this.model = options.model;
            this.form = new CompactAlbumForm({ model: this.model });
            var photographCollection = new PhotographAlbum(
                { album: options.model }
                );
            photographCollection.fetch();
            this.thumbList = new PhotographThumbListView({ model: photographCollection });
            this.listenTo(this.thumbList, 'click', this.gotoPhotograph.bind(this));
            this.render();
        },
        reset: function() {
            this.model.fetch();
        },
        gotoPhotograph: function(photograph) {
            console.log('gotoPhotograph', this.application);
            this.application.pushPage(
                new PhotographPage(
                    {
                        application: this.application,
                        model: photograph,
                        inAlbum: this.model
                    }
                    )
                );
        },
        template: function() {
            return div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2(this.model.get('name')),
                    div(this.form.el),
                    div(
                        button(
                            {
                                type: 'button',
                                class: 'pure-button pure-button-error',
                                onclick: (function() {
                                    console.log('delete');
                                }).bind(this)
                            },
                            ui.icon('delete'), 'Delete'
                            )
                       ),
                    div(this.thumbList.el)
                   )
               );
        }
    }
    );


var _ = require('underscore');
var Album = require('../model/album').Album;
var AlbumCollection = require('../collection/album').AlbumCollection;
var AlignedAlbumForm = require('../form/alignedalbum').AlignedAlbumForm;
var AlbumListView = require('../view/albumlist').AlbumListView;
var AlbumPage = require('./album').AlbumPage;
var PageView = require('../view/page').PageView;
var PhotographPage = require('./photograph').PhotographPage;
var PhotographThumbListView = require('../view/photographthumblist').PhotographThumbListView;
var RecentlyTaken = require('../collection/recentlytaken').RecentlyTaken;
var StaticView = require('../view/static').StaticView;
var Uncategorised = require('../collection/uncategorised').Uncategorised;
var ui = require('../ui');

var CategoryPage = PageView.extend(
    {
        pageTitle: 'Category',
        fullPage: true,
        initializeList: function(options) {
            this.application = options.application;
            this.thumbList = new PhotographThumbListView({ model: this.model });
            this.listenTo(this.thumbList, 'click', this.gotoPhotograph.bind(this));
            this.render();
        },
        reset: function() {
            this.model.fetch();
        },
        gotoPhotograph: function(photograph) {
            this.application.pushPage(new PhotographPage({ model: photograph }));
        },
        template: function() {
            return div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2(''),
                    div(this.thumbList.el)
                   )
                );
        }
    }
    );

var RecentlyTakenPage = CategoryPage.extend(
    {
        pageTitle: 'Recently Taken',
        initialize: function(options) {
            this.model = new RecentlyTaken;
            this.listenTo(this.model, 'all', this.render.bind(this));
            this.model.fetch();
            this.initializeList(options);
        },
        template: function() {
            console.log('len', this.model.length);
            return div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2('Recently Taken Photographs'),
                    (this.model.length > 0)?
                        div(this.thumbList.el):
                        p('There are no recently taken photographs.')
                   )
                );
        }
    }
    );

var UncategorisedPage = CategoryPage.extend(
    {
        pageTitle: 'Uncategorised',
        initialize: function(options) {
            this.model = new Uncategorised;
            this.listenTo(this.model, 'all', this.render.bind(this));
            this.model.fetch();
            this.initializeList(options);
        },
        template: function() {
            return div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2('Uncategorised Photographs'),
                    p('These photographs are not yet a member of any album.'),
                    (this.model.length > 0)?
                        div(this.thumbList.el):
                        p('There are currently no uncategorised photographs.')
                   )
                );
        }
    }
    );

exports.AlbumsPage = PageView.extend(
    {
        pageTitle: 'Albums',
        fullPage: true,
        initialize: function(options) {
            this.application = options.application;
            var albumCollection = new AlbumCollection;
            albumCollection.fetch();

            this.albumForm = new AlignedAlbumForm;
            this.listenTo(
                this.albumForm,
                'save',
                (function(model) {
                    if(model.isValid())
                        model.save().then(
                            (function() {
                                albumCollection.add(model);
                                console.log('saved');
                                this.albumForm.setModel(new Album);
                            }).bind(this),
                            function() {
                                console.log('error');
                            }
                            );
                    else
                        console.log('model not valid');
                }).bind(this)
                );

            this.albumList = new AlbumListView({ model: albumCollection });
            this.listenTo(this.albumList, 'click', this.gotoAlbum.bind(this));
            this.render();
        },
        gotoAlbum: function(album) {
            this.application.pushPage(
                new AlbumPage({ application: this.application, model: album })
                );
        },
        gotoRecent: function() {
            this.application.pushPage(RecentlyTakenPage);
        },
        gotoUncategorised: function() {
            this.application.pushPage(UncategorisedPage);
        },
        template: function() {
            return div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2('Albums'),
                    div(
                        button(
                            {
                                class: 'pure-button',
                                onclick: this.gotoRecent.bind(this)
                            },
                            ui.icon('clock'), 'Recently Taken'
                            ),
                        button(
                            {
                                class: 'pure-button',
                                onclick: this.gotoUncategorised.bind(this)
                            },
                            ui.icon('question-mark'), 'Uncategorised Photographs'
                            )
                       )
                   ),
                div(
                    { class: 'pure-u-1-1' },
                    div(this.albumList.el)
                   ),
                div(
                    { class: 'pure-u-1-1' },
                    div(this.albumForm.el)
                    )
                );
        }
    }
    );


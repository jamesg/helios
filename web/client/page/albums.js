var _ = require('underscore');
var Album = require('../model/album').Album;
var AlbumCollection = require('../collection/album').AlbumCollection;
var AlbumForm = require('../form/album').AlbumForm;
var AlbumList = require('../view/albumlist').AlbumList;
var AlbumPage = require('./album').AlbumPage;
var PageView = require('../view/page').PageView;
var PhotographThumbList = require('../view/photographthumblist').PhotographThumbList;
var RecentlyTaken = require('../collection/recentlytaken').RecentlyTaken;
var StaticView = require('../view/static').StaticView;
var Uncategorised = require('../collection/uncategorised').Uncategorised;
var ui = require('../ui');

var CategoryPage = PageView.extend(
    {
        fullPage: true,
        initializeList: function(options) {
            this.application = options.application;
            this.thumbList = new PhotographThumbList({ model: this.model });
            this.listenTo(this.thumbList, 'click', this.gotoPhotograph.bind(this));
            this.render();
        },
        gotoPhotograph: function(photograph) {
            this.application.gotoPage(new PhotographPage({ model: photograph }));
        },
        template: function() {
            h2('');
            div(this.thumbList.el);
        }
    }
    );

var RecentlyTakenPage = CategoryPage.extend(
    {
        initialize: function(options) {
            this.model = new RecentlyTaken;
            this.model.fetch();
            this.initializeList(options);
        },
        template: function() {
            h2('Recently Taken Photographs');
            if(this.thumbList.length > 0)
                div(this.thumbList.el);
            else
                p('There are no recently taken photographs.');
        }
    }
    );

var UncategorisedPage = CategoryPage.extend(
    {
        initialize: function(options) {
            this.model = new Uncategorised;
            this.model.fetch();
            this.initializeList(options);
        },
        template: function() {
            h2('Uncategorised Photographs');
            p('These photographs are not yet a member of any album.');
            if(this.thumbList.length > 0)
                div(this.thumbList.el);
            else
                p('There are currently no uncategorised photographs.');
        }
    }
    );

exports.AlbumsPage = PageView.extend(
    {
        fullPage: true,
        initialize: function(options) {
            this.application = options.application;
            var albumCollection = new AlbumCollection;
            albumCollection.fetch();

            this.albumForm = new AlbumForm;
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

            this.albumList = new AlbumList({ model: albumCollection });
            this.listenTo(this.albumList, 'click', this.gotoAlbum.bind(this));
            this.render();
        },
        gotoAlbum: function(album) {
            this.application.gotoPage(
                new AlbumPage({ application: this.application, model: album })
                );
        },
        gotoRecent: function() {
            this.application.gotoPage(RecentlyTakenPage);
        },
        gotoUncategorised: function() {
            this.application.gotoPage(UncategorisedPage);
        },
        template: function() {
            h2('Albums');
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
               );
            div(this.albumList.el);
            div(this.albumForm.el);
        }
    }
    );


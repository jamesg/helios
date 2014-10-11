var PageView = require('../view/page').PageView;

var AlbumCollection = require('../collection/album').AlbumCollection;
var AlbumList = require('../view/albumlist').AlbumList;
var AlbumPage = require('./album').AlbumPage;

var icon = require('../ui').icon;

exports.AlbumsPage = PageView.extend(
    {
        fullPage: true,
        initialize: function(options) {
            this.application = options.application;
            var albumCollection = new AlbumCollection;
            albumCollection.fetch();
            this.albumList = new AlbumList({ model: albumCollection });
            this.listenTo(this.albumList, 'click', this.gotoAlbum.bind(this));
            this.render();
        },
        gotoAlbum: function(album) {
            this.application.gotoView(
                new AlbumPage({ application: this.application, model: album })
                );
        },
        gotoRecent: function() {
        },
        gotoUncategorised: function() {
        },
        template: function() {
            h2('Albums');
            div(
                button(
                    {
                        class: 'pure-button',
                        onclick: this.gotoRecent.bind(this)
                    },
                    icon('clock'), 'Recently Taken'
                    ),
                button(
                    {
                        class: 'pure-button',
                        onclick: this.gotoUncategorised.bind(this)
                    },
                    icon('question-mark'), 'Uncategorised Photographs'
                    )
               );
            div(this.albumList.el);
        }
    }
    );


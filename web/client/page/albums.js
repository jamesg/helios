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
        template: function() {
            h2('Albums');
            div(this.albumList.el);
        }
    }
    );


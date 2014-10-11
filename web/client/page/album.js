var PageView = require('../view/page').PageView;

var PhotographAlbum = require('../collection/photographalbum').PhotographAlbum;
var PhotographThumbList = require('../view/photographthumblist').PhotographThumbList;
var PhotographPage = require('./photograph').PhotographPage;

var icon = require('../ui').icon;

exports.AlbumPage = PageView.extend(
    {
        fullPage: true,
        initialize: function(options) {
            this.application = options.application;
            this.model = options.model;
            var photographCollection = new PhotographAlbum(
                { album: options.model }
                );
            photographCollection.fetch();
            this.thumbList = new PhotographThumbList({ model: photographCollection });
            this.listenTo(this.thumbList, 'click', this.gotoPhotograph.bind(this));
            this.render();
        },
        gotoPhotograph: function(photograph) {
            this.application.gotoView(new PhotographPage({ model: photograph }));
        },
        template: function() {
            h2('Album ', this.model.get('name'));
            div(this.thumbList.el);
        }
    }
    );


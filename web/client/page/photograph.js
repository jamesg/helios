var _ = require('underscore');
var AlbumCollection = require('../collection/album').AlbumCollection;
var AlbumListView = require('../view/albumlist').AlbumListView;
var AlbumSelectForm = require('../form/albumselect').AlbumSelectForm;
var MessageBox = require('../view/messagebox').MessageBox;
var PageView = require('../view/page').PageView;
var Photograph = require('../model/photograph').Photograph;
var PhotographAlbums = require('../collection/photographalbums').PhotographAlbums;
var PhotographForm = require('../form/photograph').PhotographForm;
var StaticView = require('../view/static').StaticView;
var api = require('../service/api');
var ui = require('../ui');

var PhotographDetails = StaticView.extend(
    {
        template: function() {
            return dl(
                { compact: 'compact' },
                dt('Title'), dd(this.model.get('title')),
                dt('Caption'), dd(this.model.get('caption')),
                dt('Date'), dd(this.model.get('taken')),
                dt('Location'), dd(this.model.get('location')),
                dt('Tags'), dd(this.model.get('tags'))
                );
        }
    }
    );

/*!
 * \brief Detailed information for a single photograph.
 *
 * \param inAlbum Optionally consider the photograph to be a member of an
 * album.  The page will display additional tools to remove the photograph from
 * this album.
 */
exports.PhotographPage = PageView.extend(
    {
        pageTitle: function() {
            return this.model.get('title');
        },
        fullPage: true,
        initialize: function(options) {
            console.log('fetching photograph model');
            this.model.fetch();
            this.photographDetails =
                new PhotographDetails({ model: this.model });
            if(_(options).has('inAlbum'))
                this.inAlbum = options.inAlbum;
            this.form = new PhotographForm({ model: this.model });
            this.messageBox = new MessageBox;
            var photographAlbums = new PhotographAlbums({ photograph: this.model });
            photographAlbums.fetch();
            this.albumForm = new AlbumSelectForm({ buttonText: 'Add' });
            this.listenTo(
                this.albumForm,
                'addToAlbum',
                (function(albumId) {
                    this.addPhotographToAlbum(this.model.get('photograph_id'), albumId);
                }).bind(this)
                );
            this.albumList = new AlbumListView({ model: photographAlbums });
            this.render();
        },
        addPhotographToAlbum: function(photographId, albumId) {
            api.rpcFunction('add_photograph_to_album')(
                photographId,
                albumId
                )
            .then(
                (function() {
                    console.log('success');
                    this.messageBox.displaySuccess('Added photgraph to album.');
                    this.photographAlbums.fetch();
                }).bind(this),
                (function(err) {
                    this.messageBox.displayError('Error adding photograph to album.');
                    console.log('adding photograph to album', err);
                }).bind(this)
                );
        },
        tagName: 'div',
        className: 'pure-g',
        /*
         * The photograph page template consists of a the photograph,
         * descriptive text, a form, an album list and a tools list.
         *
         * On small screens, everything is displayed in one column.  On medium
         * and large screens, the photograph has its own row, the description
         * and form are on the second row, and the album list and tools list
         * are on a third row.
         *
         * +-------------------------+
         * |photograph               |
         * +------------+------------+
         * |description |form        |
         * +------------+------------+
         * |album list  |tools list  |
         * +------------+------------+
         */
        template: function() {
            div(
                { class: 'pure-u-1-1' },
                h2(this.model.get('title')),
                this.messageBox.el
                );
            div(
                { class: 'pure-u-1-1' },
                img(
                    {
                        class: 'pure-img',
                        alt: this.model.get('title'),
                        src: '/jpeg_image?photograph_id=' +
                            this.model.get('photograph_id') +
                            '&height=800&width=1000'
                    }
                   )
               );
            div(
                { class: 'pure-u-1-1 pure-u-md-10-24' },
                h3('Photograph Details'),
                this.photographDetails.el
               );
            div(
                { class: 'pure-u-1-1 pure-u-md-14-24' },
                h3('Edit'),
                this.form.el
               );
            div(
                { class: 'pure-u-1-1 pure-u-md-12-24' },
                h3('Albums'),
                p('Add this photograph to another album.'),
                this.albumForm.el,
                this.albumList.el
               );
            div(
                { class: 'pure-u-1-1 pure-u-md-12-24' },
                h3('Tools'),
                p('Download the image at a different size.'),
                div(
                    { class: 'pure-menu pure-menu-vertical pure-menu-open' },
                    ul(
                        li(a({ href: '/jpeg_image?height=1000&width=1200' }, '1200x1000')),
                        li(a({ href: '/jpeg_image_fullsize' }, 'Fullsize'))
                      )
                   )
               );
        }
    }
    );


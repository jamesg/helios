var _ = require('underscore');
var AlbumList = require('../view/albumlist').AlbumList;
var MessageBox = require('../view/messagebox').MessageBox;
var PageView = require('../view/page').PageView;
var Photograph = require('../model/photograph').Photograph;
var PhotographAlbums = require('../collection/photographalbums').PhotographAlbums;
var StaticView = require('../view/static').StaticView;
var ui = require('../ui');

var PhotographForm = StaticView.extend(
    {
        initialize: function(options) {
            if(_.has(options, 'model'))
                this.model = options.model;
            if(!_.has(this, 'model'))
                this.model = new Photograph;
            this.render();
        },
        template: function() {
            var title = input({ type: 'text', name: 'title', value: this.model.get('title') });
            var caption = input({ type: 'text', name: 'caption', value: this.model.get('caption') });
            var date = input({ type: 'text', name: 'date', value: this.model.get('date') });
            var location = input({ type: 'text', name: 'location', value: this.model.get('location') });
            var inlineInput = function(input_, label_) {
                return div(
                        { class: 'pure-control-group' },
                        label({ for: input_().name }, label_),
                        input_
                        );
            }
            return form(
                {
                    class: 'pure-form pure-form-aligned',
                    onsubmit: (function() {
                        this.model.set('title', title().value);
                        this.model.set('caption', caption().value);
                        this.model.set('date', date().value);
                        this.model.set('location', location().value);
                        return false;
                    }).bind(this)
                },
                inlineInput(title, 'Title'),
                inlineInput(caption, 'Caption'),
                inlineInput(date, 'Date'),
                inlineInput(location, 'Location'),
                inlineInput(
                    button(
                        { type: 'submit', class: 'pure-button pure-button-primary' },
                        ui.icon('data-transfer-download'), 'Save'
                        ),
                    ''
                    )
                );
        }
    }
    );

var DetailView = StaticView.extend(
    {
        initialize: function() {
            this.form = new PhotographForm({ model: this.model });
            var photographAlbums = new PhotographAlbums({ photograph: this.model });
            photographAlbums.fetch();
            this.albumList = new AlbumList({ model: photographAlbums });
            this.render();
        },
        tagName: 'div',
        className: 'pure-g',
        /*
         * The photograph page template consists of a the photograph,
         * descriptive text, a form, an album list and a tools list.
         *
         * On small screens, everything is displayed in one column.
         *
         * +-------------------------+
         * |photograph               |
         * +-------------------------+
         * |description              |
         * +-------------------------+
         * |form                     |
         * +-------------------------+
         * |album list               |
         * +-------------------------+
         * |tools list               |
         * +-------------------------+
         *
         * On medium screens, the photograph has its own row, the description
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
         *
         * On large screens, the photograph and description share a row and all
         * following components are on a second row.
         *
         * +----------------+-----------------+
         * |photograph      |description      |
         * +----------+-----+-----+-----------+
         * |form      |album list |tools list |
         * +----------+-----------+-----------+
         */
        template: function() {
            div({ class: 'pure-u-1-1' }, h2(this.model.get('title')));
            div(
                { class: 'pure-u-1-1 pure-u-md-1-1 pure-u-lg-17-24 pure-u-xl-17-24' },
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
                { class: 'pure-u-1-1 pure-u-md-7-24 pure-u-lg-7-24 pure-u-xl-7-24' },
                dl(
                    { compact: 'compact' },
                    dt('Title'), dd(this.model.get('title')),
                    dt('Caption'), dd(this.model.get('caption')),
                    dt('Date'), dd(this.model.get('taken')),
                    dt('Location'), dd(this.model.get('location'))
                  )
               );
            div(
                { class: 'pure-u-1-1 pure-u-md-17-24 pure-u-lg-11-24 pure-u-xl-8-24' },
                this.form.el
               );
            div(
                { class: 'pure-u-1-1 pure-u-md-12-24 pure-u-lg-7-24 pure-u-xl-8-24' },
                this.albumList.el
               );
            div(
                { class: 'pure-u-1-1 pure-u-md-12-24 pure-u-lg-6-24 pure-u-xl-8-24' },
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

exports.PhotographPage = PageView.extend(
    {
        fullPage: true,
        initialize: function(options) {
            this.detail = new DetailView({ model: options.model });
            this.render();
        },
        render: function() {
            this.$el.empty();
            this.$el.append(this.detail.el);
        }
    }
    );


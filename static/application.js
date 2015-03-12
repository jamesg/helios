var TbodyView = CollectionView.extend(
    {
        tagName: 'tbody',
        initialize: function(options) {
            CollectionView.prototype.initialize.apply(this, arguments);
            this.initializeTrView = options.initializeTrView;
        },
        initializeView: function(view) {
            this.listenTo(view, 'click', this.trigger.bind(this, 'click'));
            this.initializeTrView(view);
        }
    }
    );

/*!
 * \brief A generic table view that can be adapted to any kind of table with a
 * header.  Provide a view constructor for the table header (thead) as the
 * theadView option and a view constructor for the table rows (tr) as the
 * trView option.
 *
 * \param emptyTemplate Template to use if the table is empty (defaults to
 * displaying no rows).
 */
var TableView = ModelView.extend(
    {
        tagName: 'div',
        initialize: function(options) {
            ModelView.prototype.initialize.apply(this, arguments);
            if(_.has(options, 'theadView'))
                this.theadView = options.theadView;
            if(_.has(options, 'trView'))
                this.trView = options.trView;
            if(_.has(options, 'initializeTrView'))
                this.initializeTrView = options.initializeTrView;
            if(_.has(options, 'emptyTemplate'))
                this.emptyTemplate = options.emptyTemplate;

            this._thead = new this.theadView;
            this._thead.render();
            this._tbody = new TbodyView(
                {
                    initializeTrView: this.initializeTrView,
                    view: this.trView,
                    model: this.model
                }
                );
            this._tbody.render();

            this.listenTo(this.model, 'all', this.render);
            this.listenTo(this._tbody, 'click', this.trigger.bind(this, 'click'));

            this.render();
        },
        template: function() {
            return this.build(
                    'table',
                    { class: 'pure-table' },
                    this._thead.el,
                    this._tbody.el
                    );
        },
        initializeTrView: function(trView) {
        }
    }
    );

var PageView = StaticView.extend(
    {
        pageTitle: 'Untitled Page',
        initialize: function(options) {
            StaticView.prototype.initialize.apply(this, arguments);
            if(_.has(options, 'application'))
                this.application = options.application;
        },
        reset: function() {
        }
    }
    );

var HomePage = PageView.extend(
    {
        pageTitle: 'Home',
        initialize: function() {
            PageView.prototype.initialize.apply(this, arguments);
            this.render();
        },
        render: function() {
            this.$el.html(this.template());
            this.$('button[name=albums]').on(
                'click',
                (function() {
                    this.application.pushPage(AlbumsPage);
                }).bind(this)
                );
        },
        template: _.template($('#main-menu').html())
    }
    );

var PhotographEditForm = Backbone.View.extend(
    {
        tagName: 'form',
        className: 'pure-form pure-form-aligned',
        events: {
            'submit': 'save'
        },
        save: function() {
            this.model.set(
                {
                    'title': this.$title.val(),
                    'caption': this.$caption.val(),
                    'taken': this.$date.val(),
                    'location': this.$location.val(),
                    'tags': this.$tags.val()
                }
                );
            this.model.save(
                { },
                {
                    wait: true,
                    success: this.trigger.bind(this, 'save'),
                    error: function(err) { console.log('error saving photograph', err); }
                }
                );
            return false;
        },
        initialize: function() {
            this.render();
        },
        render: function() {
            this.$el.html(this.template(this.model.attributes));
            this.$title = this.$('[name=title]');
            this.$caption = this.$('[name=caption]');
            this.$date = this.$('[name=date]');
            this.$location = this.$('[name=location]');
            this.$tags = this.$('[name=tags]');
        },
        template: _.template($('#photograph-edit-form').html())
    }
    );

var PhotographNewForm = Backbone.View.extend(
    {
        tagName: 'form',
        className: 'pure-form',
        events: {
            'submit': 'create'
        },
        create: function() {
            this.model.set(
                {
                    'title': this.$title.val(),
                    'caption': this.$caption.val(),
                    'date': this.$date.val(),
                    'location': this.$location.val(),
                    'tags': this.$tags.val()
                }
                );
            this.model.save(
                { },
                {
                    wait: true,
                    success: function() { console.log('saved photograph'); },
                    error: function(err) { console.log('error saving photograph', err); }
                }
                );
            return false;
        },
        initialize: function() {
            this.render();
        },
        render: function() {
            this.$el.html(this.template(this.model.attributes));
            this.$title = this.$('[name=title]');
        },
        template: _.template($('#photograph-new-form').html())
    }
    );

var UploadPage = PageView.extend(
    {
        pageTitle: 'Upload Photograph',
        initialize: function() {
            PageView.prototype.initialize.apply(this, arguments);
            this._form = new PhotographNewForm({ model: new Photograph });
        },
        render: function() {
            this.$el.empty();
            this.$el.append(this._form.$el);
        }
    }
    );

var PhotographDetailsView = StaticView.extend(
    {
        template: $('#photograph-view').html()
    }
    );

var PhotographPage = PageView.extend(
    {
        pageTitle: function() { return this.model.get('title'); },
        initialize: function() {
            this._detailsView = new PhotographDetailsView({ model: this.model });
            this._detailsView.render();
            this._form = new PhotographEditForm({ model: this.model });
            this._form.render();
            this.listenTo(this._form, 'save', this.model.fetch.bind(this.model));
            this.render();
        },
        reset: function() {
            this.model.fetch();
        },
        render: function() {
            this.$el.empty();
            this.$el.append(this._detailsView.$el);
            this.$el.append(this._form.$el);
        }
    }
    );

var AlbumPage = PageView.extend(
    {
        pageTitle: function() { return this._album.get('name'); },
        initialize: function(options) {
            this._album = options.album;
            this._photographs =
                    new PhotographsInAlbum([], { album: this._album });
            this._photographs.fetch();
            this._photographsView = new CollectionView(
                {
                    tagName: 'ul',
                    className: 'album',
                    model: this._photographs,
                    view: ModelView.extend(
                        {
                            tagName: 'li',
                            template: $('#photograph-thumb-view').html(),
                            events: { 'click': 'gotoPhotograph' },
                            gotoPhotograph: function() {
                                var page = new PhotographPage(
                                    {
                                        application: options.application,
                                        model: this.model
                                    }
                                    );
                                options.application.pushPage(page);
                            }
                        }
                        )
                }
                );
            this.render();
        },
        reset: function() {
            this._photographs.fetch();
        },
        render: function() {
            this.$el.empty();
            this._photographsView.render();
            this.$el.append(this._photographsView.$el);
        }
    }
    );

var AlbumsPage = PageView.extend(
    {
        pageTitle: 'Albums',
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            this._albums = new AlbumCollection;
            this._albums.fetch();
            this._albumsView = new CollectionView(
                {
                    tagName: 'ul',
                    className: 'album-list',
                    model: this._albums,
                    view: ModelView.extend(
                        {
                            tagName: 'li',
                            template: '<%-name%>',
                            events: { 'click': 'gotoAlbum' },
                            gotoAlbum: function() {
                                var page = new AlbumPage(
                                    {
                                        application: options.application,
                                        album: this.model
                                    }
                                    );
                                options.application.pushPage(page);
                            }
                        }
                        )
                }
                );
            this.render();
        },
        reset: function() {
            this._albums.fetch();
        },
        render: function() {
            this.$el.empty();
            this._albumsView.render();
            this.$el.append(this._albumsView.$el);
        }
    }
    );


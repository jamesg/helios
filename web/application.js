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

/*
 * A generic table view that can be adapted to any kind of table with a header.
 * Provide a view constructor for the table header (thead) as the theadView
 * option and a view constructor for the table rows (tr) as the trView option.
 *
 * model: An instance of Backbone.Collection.
 * emptyView: View to use if the table is empty (defaults to displaying no
 * rows).
 * theadView: Header view constructor.
 * trView: Row view constructor.
 */
var TableView = StaticView.extend(
    {
        tagName: 'table',
        className: 'pure-table',
        initialize: function(options) {
            StaticView.prototype.initialize.apply(this, arguments);

            this._thead = new options.theadView;
            this._thead.render();
            this._tbody = new TbodyView(
                {
                    initializeTrView: this.initializeTrView,
                    view: options.trView,
                    model: this.model
                }
                );
            this._tbody.render();

            this._empty = new options.emptyView;

            this.listenTo(this.model, 'all', this.render);
            this.listenTo(this._tbody, 'click', this.trigger.bind(this, 'click'));

            this.render();
        },
        render: function() {
            this.$el.empty();
            this.$el.append(this._thead.$el);
            if(this.model.length == 0)
                this.$el.append(this._empty.$el);
            else
                this.$el.append(this._tbody.$el);
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
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            this._application = options.application;
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
            this.$('button[name=locations]').on(
                'click',
                (function() {
                    this.application.pushPage(LocationsPage);
                }).bind(this)
                );
            this.$('button[name=random]').on(
                'click',
                (function() {
                    var model = new RandomPhotograph;
                    // TODO Is it possble/sensible/reasonable to display the
                    // page immediately?
                    // The main problem is that an empty photograph model will
                    // not have an id and the PhotographPage templates choke on
                    // this.
                    model.fetch(
                        {
                            success: (function() {
                                this.application.pushPage(
                                    new PhotographPage({ model: model })
                                    );
                            }).bind(this)
                        }
                        );
                }).bind(this)
                );
            this.$('button[name=signin]').on(
                'click',
                (function() {
                    this.application.pushPage(SignInPage);
                }).bind(this)
                );
            this.$('button[name=signout]').on(
                'click',
                function() {
                    jsonRpc(
                        {
                            url: 'auth',
                            method: 'sign_out',
                            params: [ storage.get('token') ],
                            success: function() {
                                console.log('signed out');
                                storage.remove('token');
                            },
                            error: function(err) {
                                console.log('signing out', err);
                            }
                        }
                        );
                }
                ),
            this.$('button[name=uncategorised]').on(
                'click',
                (function() {
                    var model = new UncategorisedPhotographs;
                    var page = new ThumbnailPage(
                        {
                            application: this._application,
                            model: model,
                            name: 'Uncategorised'
                        }
                        );
                    this._application.pushPage(page);
                }).bind(this)
                );
            this.$('button[name=tags]').on(
                'click',
                (function() {
                    this.application.pushPage(TagsPage);
                }).bind(this)
                );
            this.$('button[name=upload]').on(
                'click',
                (function() {
                    this.application.pushPage(UploadPage);
                }).bind(this)
                );
        },
        template: _.template($('#homepage').html())
    }
    );

var SignInPage = PageView.extend(
    {
        pageTitle: 'Sign In',
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            this._application = options.application;
            this.initRender();
            this._messageBox = new MessageBox(
                    {
                        el: this.$('div[name=messagebox]')
                    }
                    );
            this._messageBox.render();
        },
        events: {
            'submit form': 'signIn'
        },
        signIn: function() {
            var application = this._application;
            var messageBox = this._messageBox;
            jsonRpc(
                {
                    url: 'auth',
                    method: 'sign_in',
                    params: [
                        this.$('input[name=username]').val(),
                        this.$('input[name=password]').val()
                    ],
                    success: function(user) {
                        storage.set('token', user.token);
                        application.popPage();
                    },
                    error: function(err) {
                        messageBox.displayError('Sign in failed; ' + err);
                    }
                }
                );
            return false;
        },
        template: _.template($('#signin-page').html()),
        initRender: function() {
            this.$el.html(this.template());
            this.$username = this.$('input[name=username]');
            this.$password = this.$('input[name=password]');
        }
    }
    );

var UploadPage = PageView.extend(
    {
        events: {
            'submit form[name=photographform]': 'create'
        },
        initialize: function() {
            PageView.prototype.initialize.apply(this, arguments);
            PageView.prototype.render.apply(this, arguments);
            this._messageBox = new MessageBox({
                el: this.$('div[name=messagebox]')
            });
        },
        render: function() {},
        template: $('#uploadpage-template').html(),
        create: function() {
            var el = this.$('form[name=photographform]')[0];
            var photographData = new FormData(el);
            var messageBox = this._messageBox;
            var reqListener = function() {
                el.disabled = false;
                if(this.response['error'])
                    messageBox.displayError('Upload failed: ' + this.response.error);
                else
                    messageBox.displaySuccess('Upload complete');
            };
            var xhr = new XMLHttpRequest();
            xhr.open(
                    'post',
                    'insert_photograph',
                    true
                    );
            xhr.onload = reqListener;
            xhr.addEventListener(
                    'progress',
                    function(event) {
                        if(event.lengthComputable) {
                            var percent = (event.loaded/event.total)*100;
                            console.log('upload percentage', percent);
                        }
                    }
                    );
            xhr.send(photographData);

            return false;
        },
    }
    );

var PhotographAlbumsView = StaticView.extend(
    {
        initialize: function(options) {
            StaticView.prototype.initialize.apply(this, arguments);
            StaticView.prototype.render.apply(this, arguments);
            this._photograph = options.photograph;

            var photographAlbums =
                new PhotographAlbums([], { photograph: options.photograph });
            this._photographAlbums = photographAlbums;
            photographAlbums.fetch();

            var albums = new AlbumCollection;
            this._albums = albums;
            albums.fetch();

            (new CollectionView({
                el: this.$('ul[name=photograph-albums]'),
                model: photographAlbums,
                view: StaticView.extend({
                    tagName: 'li',
                    template: '<%-name%> (<a>Remove</a>)',
                    events: {
                        'click a': 'removeFromAlbum'
                    },
                    removeFromAlbum: function() {
                        this.model.destroy({
                            url: restUri('photograph/' + options.photograph.id + '/album/' + this.model.id),
                            success: (function() {
                                photographAlbums.fetch();
                            }).bind(this),
                            error: function(err) { console.log('err',err); }
                        });
                        return false;
                    }
                })
            })).render();
            (new CollectionView({
                el: this.$('select[name=albums]'),
                model: albums,
                view: StaticView.extend({
                    tagName: 'option',
                    template: '<%-name%>'
                })
            })).render();
        },
        render: function() {},
        reset: function() {
            this._photographAlbums.fetch();
        },
        events: {
            'click button[name=add]': 'addPhotographToAlbum'
        },
        addPhotographToAlbum: function() {
            var album = this._albums.at(this.$('select[name=albums]')[0].selectedIndex);
            album.save(
                {
                },
                {
                    method: 'POST',
                    url: restUri('photograph/' + this._photograph.id + '/album'),
                    success: (function() {
                        this._photographAlbums.fetch();
                    }).bind(this),
                    error: function(err) { console.log('err',err); }
                }
                );
            return false;
        },
        template: $('#albums-form').html()
    }
    );

var PhotographPage = PageView.extend(
    {
        pageTitle: function() { return this.model.get('title'); },
        template: _.template($('#photograph-page').html()),
        events: {
            'click button[name=delete]': 'showDeletePhotograph'
        },
        showDeletePhotograph: function() {
            var m = new Modal({
                view: StaticView.extend({
                    template: $('#deletephotograph-template').html()
                }),
                model: this.model,
                buttons: [
                    StandardButton.cancel(),
                    StandardButton.destroy(this.deletePhotograph.bind(this))
                ]
            });
            gApplication.modal(m);
        },
        deletePhotograph: function() {
            this.model.destroy();
            gApplication.popPage();
        },
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            console.log(this.model.attributes);
            this.$el.html(
                this.template(
                    _.extend(
                        this.model.toJSON(),
                        {
                            disablePrev: !_.has(options, 'prevPhotograph'),
                            disableNext: !_.has(options, 'nextPhotograph')
                        }
                        )
                    )
                );

            var PhotographDetailsView = StaticView.extend(
                {
                    template: $('#photograph-view').html()
                }
                );
            this._detailsView = new PhotographDetailsView(
                {
                    template: $('#photograph-view').html(),
                    el: this.$('div[name=details]'),
                    model: this.model
                }
                );
            this._detailsView.render();

            var PhotographEditForm = Backbone.View.extend(
                {
                    events: {
                        'submit': 'save'
                    },
                    save: function() {
                        this.model.set(
                            {
                                'title': this.$('[name=title]').val(),
                                'caption': this.$('[name=caption]').val(),
                                'taken': this.$('[name=date]').val(),
                                'location': this.$('[name=location]').val(),
                                'tags': this.$('[name=tags]').val()
                            }
                            );
                        this.model.save(
                            { },
                            {
                                wait: true,
                                success: this.trigger.bind(this, 'save'),
                                error: function(err) {
                                    console.log('error saving photograph', err);
                                }
                            }
                            );
                        return false;
                    },
                    initialize: function(options) {
                        this.listenTo(this.model, 'change', this.render);
                        this.render();
                    },
                    render: function() {
                        this.$el.html(this.template(this.model.attributes));
                    },
                    template: _.template($('#photograph-edit-form').html())
                }
                );
            this._form = new PhotographEditForm(
                {
                    el: this.$('form[name=edit]'),
                    model: this.model,
                    application: options.application
                }
                );
            this._form.render();
            this._albumsForm = new PhotographAlbumsView(
                { el: this.$('form[name=albums]'), photograph: this.model }
                );
            this._albumsForm.render();

            this.listenTo(
                this._form,
                'save',
                this.model.fetch.bind(this.model)
                );

            if(_(options).has('prevPhotograph'))
                this.$('button[name=prev]').on('click', options.prevPhotograph);
            if(_(options).has('nextPhotograph'))
                this.$('button[name=next]').on('click', options.nextPhotograph);
        },
        render: function() {
        },
        reset: function() {
            this.model.fetch();
        }
    }
    );

var ThumbnailPage = PageView.extend(
    {
        pageTitle: function() { return this._name; },
        gotoPhotograph: function(photograph) {
            photograph.fetch();
            var opts =
                {
                    application: this.application,
                    model: photograph
                };
            var index = this.model.indexOf(photograph);
            if(index > 0) {
                var prev = this.model.at(index - 1);
                opts.prevPhotograph = (function() {
                    this.application.popPage();
                    this.gotoPhotograph(prev);
                }).bind(this);
            }
            if(index < this.model.length - 1) {
                opts.nextPhotograph = (function() {
                    var next = this.model.at(index + 1);
                    this.application.popPage();
                    this.gotoPhotograph(next);
                }).bind(this);
            }
            var page = new PhotographPage(opts);
            this.application.pushPage(page);
        },
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            PageView.prototype.render.apply(this, arguments);
            this._name = options.name;
            this.model.fetch();
            var thumbnailPage = this;
            this._photographsView = new CollectionView(
                {
                    el: this.$('ul[name=thumbnails]'),
                    model: this.model,
                    view: StaticView.extend(
                        {
                            tagName: 'li',
                            template: $('#photograph-thumb-view').html(),
                            events: { 'click': 'gotoPhotograph' },
                            gotoPhotograph: function() {
                                // var index =
                                //     thumbnailPage.model.indexOf(this.model);
                                thumbnailPage.gotoPhotograph(this.model);
                            }
                        }
                        )
                }
                );
            this._photographsView.render();
        },
        render: function() {},
        reset: function() {
            this.model.fetch();
        },
        template: _.template($('#thumbnail-page').html()),
        render: function() {
        }
    }
    );

/*
 * Present a list of album names.  Clicking an album name takes the user to a
 * thumbnail page of photographs in that album.
 */
var AlbumsPage = PageView.extend(
    {
        pageTitle: 'Albums',
        template: _.template($('#albums-page').html()),
        events: {
            'click button[name=new-album]': 'showCreateAlbum'
        },
        showCreateAlbum: function() {
            var m = new Modal({
                view: StaticView.extend({
                    template: $('#albumform-template').html(),
                    initialize: function() {
                        StaticView.prototype.initialize.apply(this, arguments);
                        StaticView.prototype.render.apply(this, arguments);
                        this.on('create', this.create.bind(this));
                        this._messageBox =
                            new MessageBox({ el: this.$('[name=messagebox]') });
                    },
                    render: function() {},
                    create: function() {
                        this.model.set({
                            name: this.$('input[name=name]').val()
                        });
                        this.model.save(
                            {},
                            {
                                success: (function() {
                                    this.trigger('finished');
                                }).bind(this),
                                error: (function() {
                                    this._messageBox.displayError('Failed to save item');
                                }).bind(this)
                            }
                            );
                    }
                }),
                buttons: [ StandardButton.cancel(), StandardButton.create() ],
                model: new Album
            });
            this.listenTo(m, 'finished', this.reset.bind(this));
            gApplication.modal(m);
        },
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            PageView.prototype.render.apply(this, arguments);
            this._albums = new AlbumCollection;
            this._albums.fetch();
            this._albumsView = new CollectionView({
                el: this.$('ul[name=albums-list]'),
                model: this._albums,
                view: StaticView.extend({
                    tagName: 'li',
                    template: '<a><%-name%></a>',
                    events: { 'click': 'gotoAlbum' },
                    gotoAlbum: function() {
                        var model = new PhotographsInAlbum(
                            [],
                            { album: this.model }
                            );
                        var page = new ThumbnailPage(
                            {
                                application: options.application,
                                model: model,
                                name: this.model.get('name')
                            }
                            );
                        options.application.pushPage(page);
                    }
                })
            });
            this._albumsView.render();
        },
        render: function() {},
        reset: function() {
            this._albums.fetch();
        }
    }
    );

/*
 * Present a list of tags.  Clicking a tag takes the user to a thumbnail page
 * of photographs with that tag.
 */
var TagsPage = PageView.extend(
    {
        pageTitle: 'Tags',
        template: $('#tagspage-template').html(),
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            PageView.prototype.render.apply(this, arguments);
            this._tags = new TagCollection;
            this._tags.fetch();
            this._tagsView = new CollectionView(
                {
                    el: this.$('ul[name=tags]'),
                    model: this._tags,
                    view: StaticView.extend(
                        {
                            tagName: 'li',
                            template: '<%-tag%> (<%-photograph_count%>)',
                            events: { 'click': 'gotoAlbum' },
                            gotoAlbum: function() {
                                var tag = this.model.get('tag');
                                var model = new PhotographsWithTag([], { tag: tag });
                                var page = new ThumbnailPage(
                                    {
                                        application: options.application,
                                        model: model,
                                        name: this.model.get('tag')
                                    }
                                    );
                                options.application.pushPage(page);
                            }
                        }
                        )
                }
                );
        },
        reset: function() {
            this._tags.fetch();
        },
        render: function() { }
    }
    );

/*
 * Present a list of locations.  Clicking a location takes the user to a
 * thumbnail page of photographs taken at that location.
 */
var LocationsPage = PageView.extend(
    {
        pageTitle: 'Locations',
        template: $('#locationspage-template').html(),
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            PageView.prototype.render.apply(this, arguments);
            this._locations = new LocationCollection;
            this._locations.fetch();
            (new CollectionView({
                el: this.$('ul[name=locations]'),
                model: this._locations,
                view: StaticView.extend({
                    tagName: 'li',
                    template: '<%-location%> (<%-photograph_count%>)',
                    events: { 'click': 'gotoLocation' },
                    gotoLocation: function() {
                        var location = this.model.get('location');
                        var model = new PhotographsWithLocation(
                            [],
                            { location: location }
                            );
                        var page = new ThumbnailPage({
                            application: options.application,
                            model: model,
                            name: this.model.get('location')
                        });
                        options.application.pushPage(page);
                    }
                })
            })).render();
        },
        reset: function() {
            this._locations.fetch();
        },
        render: function() { }
    }
    );

/*
 * Functions for accessing the browser's local storage API.  Some old browsers
 * do not support local storage; fall back to a global array if this is the
 * case.
 */
var storage = {};

if(
    (function() {
        console.log('testing localStorage');
        try {
            localStorage.setItem('mod', 'mod');
            localStorage.removeItem('mod');
            console.log('found localStorage');
            return true;
        } catch(exception) {
            console.log('did not find localStorage');
            return false;
        }
    }()
    ))
{
    storage = {
        get: function(key) {
            return window.localStorage.getItem(key);
        },
        has: function(key) {
            return _.has(gStorage, key);
        },
        set: function(key, value) {
            window.localStorage.setItem(key, value);
        },
        remove: function(key) {
            window.localStorage.setItem(key, null);
        }
    };
} else {
    var gStorage = {};
    storage = {
        get: function(key) {
            return gStorage[key];
        },
        has: function(key) {
            return (window.localStorage.getItem(key) != null);
        },
        set: function(key, value) {
            gStorage[key] = value;
        },
        remove: function(key) {
            delete gStorage['key'];
        }
    };
}

/*
 * Make a JSONRPC request.
 *
 * 'options' is a map of:
 *
 * success: Function called with the JSONRPC result if the request is successful.
 * error: Function called with the JSONRPC error message if the request fails.
 * url: JSONRPC endpoint.
 * method: JSONRPC method to call.
 * params: Parameters to the method.
 */
var jsonRpc = function(options) {
    var url = _.has(options, 'url')?options.url:'/api_call';
    var req = _.has(options, 'xhr')?options.xhr:new XMLHttpRequest;
    var reqListener = function() {
        var jsonIn = JSON.parse(this.responseText);
        if(_.has(jsonIn, 'result'))
            options.success(jsonIn.result);
        else if(_.has(jsonIn, 'error'))
            options.error(jsonIn.error);
        else
            options.error();
    }

    var requestContent = _.pick(options, 'method', 'params');
    console.log(requestContent);
    req.open('post', url, true);
    req.setRequestHeader('Authorization', storage.get('token'));
    req.onload = reqListener;
    req.send(JSON.stringify(requestContent));
};

/*
 * Single message to be visualised in a MessageBox.
 */
var Message = Backbone.Model.extend(
    {
        defaults: { severity: 'information', message: '', closeButton: true },
        timeout: function(delay) {
            setTimeout(
                (function() {
                    this.trigger('fadeout');
                    setTimeout(this.destroy.bind(this), 1000);
                }).bind(this),
            delay
            );
        }
    }
    );

var MessageCollection = Backbone.Collection.extend(
    {
        model: Message
    }
    );

/*
 * A single message displayed in a box with appropriate styling.
 */
var MessageView = StaticView.extend(
    {
        initialize: function() {
            StaticView.prototype.initialize.apply(this, arguments);
            this.render();
        },
        model: Message,
        className: function() {
            return 'messagebox messagebox-' + this.model.get('severity');
        },
        fadeout: function() {
            this.$el.attr('style', 'opacity: 0;')
        },
        events: {
            'click button[name=close]': function() { this.model.destroy(); }
        },
        template: '<span><%-message%></span><button name="close">Close</button>'
    }
    );

var MessageCollectionView = CollectionView.extend(
    {
        view: MessageView,
        initializeView: function(view) {
            this.listenTo(view.model, 'fadeout', view.fadeout.bind(view));
        }
    }
    );

/*
 * Default message box dismissal timeout in milliseconds.
 */
var defaultTimeout = 5000;

/*
 * View onto a list of messages which inform the user of recent events.
 * Messages may be dismissed by clicking the (optional) close button, or will
 * be dismissed automatically after a timeout.
 */
var MessageBox = StaticView.extend(
    {
        initialize: function(options) {
            StaticView.prototype.initialize.apply(this, arguments);
            if(!_.has(this, 'model'))
                this.model = new MessageCollection;
            this._collectionView = new MessageCollectionView({ model: this.model });
            this._collectionView.render();
            this.render();
        },
        displayError: function(str) {
            var message = new Message({ severity: 'error', message: str });
            message.timeout(defaultTimeout);
            this.model.add(message);
        },
        displayInformation: function(str) {
            var message = new Message({ severity: 'information', message: str });
            message.timeout(defaultTimeout);
            this.model.add(message);
        },
        displaySuccess: function(str) {
            var message = new Message({ severity: 'success', message: str });
            message.timeout(defaultTimeout);
            this.model.add(message);
        },
        displayWarning: function(str) {
            var message = new Message({ severity: 'warning', message: str });
            message.timeout(defaultTimeout);
            this.model.add(message);
        },
        render: function() {
            this.$el.empty();
            this.$el.append(this._collectionView.$el);
        }
    }
    );


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
var TableView = ModelView.extend(
    {
        tagName: 'table',
        className: 'pure-table',
        initialize: function(options) {
            ModelView.prototype.initialize.apply(this, arguments);

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
        className: 'pure-form pure-form-aligned',
        events: {
            'submit': 'create'
        },
        create: function() {
            var photographData = new FormData(this.el);
            var el = this.el;
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
                    '/insert_photograph',
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
        initialize: function() {
            this.initRender();
            this._messageBox = new MessageBox(
                    {
                        el: this.$('div[name=message-box]')
                    }
                    );
        },
        render: function() {
        },
        initRender: function() {
            this.$el.html(this.template(this.model.attributes));
            this.$title = this.$('[name=title]');
            this.$caption = this.$('[name=caption]');
            this.$location = this.$('[name=location]');
            this.$tags = this.$('[name=tags]');
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
            this._form.render();
            this.render();
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

var PhotographAlbumsView = StaticView.extend(
    {
        initialize: function(options) {
            StaticView.prototype.initialize.apply(this, arguments);
            this._photograph = options.photograph;
            this.initRender();
            //console.log('$photographAlbums', this.$photographAlbums);
            //console.log('$albums', this.$albums[0]);
            var photographAlbums =
                new PhotographAlbums([], { photograph: options.photograph });
            this._photographAlbums = photographAlbums;
            photographAlbums.fetch();
            var albums = new AlbumCollection;
            this._albums = albums;
            albums.fetch();
            var albumsList = new CollectionView(
                {
                    el: this.$photographAlbums,
                    model: photographAlbums,
                    view: ModelView.extend(
                        {
                            tagName: 'li',
                            template: '<%-name%> (<button class="display-link">Remove</button>)',
                            events: {
                                'click button': 'removeFromAlbum'
                            },
                            removeFromAlbum: function() {
                                // RPC
                                jsonRpc(
                                    {
                                        method: 'remove_photograph_from_album',
                                        params: [
                                            options.photograph.get('photograph_id'),
                                            this.model.get('album_id')
                                        ],
                                        success: (function() {
                                            photographAlbums.fetch();
                                        }).bind(this),
                                        error: function(err) { console.log('err',err); }
                                    }
                                    );
                            }
                        }
                        )
                }
                );
            albumsList.render();
            console.log('$albums', this.$albums);
            console.log('$el', this.$el);
            this._albumsField = new CollectionView(
                {
                    el: this.$albums,
                    model: albums,
                    view: ModelView.extend(
                        {
                            tagName: 'option',
                            template: '<%-name%>'
                        }
                        )
                }
                );
            this._albumsField.render();
        },
        reset: function() {
            this._photographAlbums.fetch();
        },
        events: {
            'click button[name=add]': 'addPhotographToAlbum'
        },
        addPhotographToAlbum: function() {
            var albumId =
                this._albums.at(this.$albums[0].selectedIndex).get('album_id');
            jsonRpc(
                {
                    method: 'add_photograph_to_album',
                    params: [
                        this._photograph.get('photograph_id'),
                        albumId
                    ],
                    success: (function() {
                        this._photographAlbums.fetch();
                    }).bind(this),
                    error: function(err) { console.log('err',err); }
                }
                );
            return false;
        },
        template: _.template($('#albums-form').html()),
        initRender: function() {
            this.$el.html(this.template());
            this.$albums = this.$('select[name=albums]');
            this.$photographAlbums = this.$('ul[name=photograph-albums]');
        },
        render: function() {
        }
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
            this._albumsForm = new PhotographAlbumsView({ photograph: this.model });
            this._albumsForm.render();
            this.listenTo(this._form, 'save', this.model.fetch.bind(this.model));
            this.render();
        },
        reset: function() {
            this.model.fetch();
        },
        render: function() {
            this.$el.empty();
            this.$el.append(this._detailsView.$el);
            this.$el.append(this._albumsForm.$el);
            this.$el.append(this._form.$el);
        }
    }
    );

var ThumbnailPage = PageView.extend(
    {
        pageTitle: function() { return this._name; },
        initialize: function(options) {
            this._name = options.name;
            this.model.fetch();
            this._photographsView = new CollectionView(
                {
                    tagName: 'ul',
                    className: 'thumbnail-list',
                    model: this.model,
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

/*
 * Present a list of album names.  Clicking an album name takes the user to a
 * thumbnail page of photographs in that album.
 */
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
                    className: 'album-list album-list-large',
                    model: this._albums,
                    view: ModelView.extend(
                        {
                            tagName: 'li',
                            template: '<%-name%>',
                            events: { 'click': 'gotoAlbum' },
                            gotoAlbum: function() {
                                var model = new PhotographsInAlbum([], { album: this.model });
                                var page = new ThumbnailPage(
                                    {
                                        application: options.application,
                                        model: model,
                                        name: this.model.get('name')
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

/*
 * Present a list of tags.  Clicking a tag takes the user to a thumbnail page
 * of photographs with that tag.
 */
var TagsPage = PageView.extend(
    {
        pageTitle: 'Tags',
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            this._tags = new TagCollection;
            this._tags.fetch();
            this._tagsView = new CollectionView(
                {
                    tagName: 'ul',
                    className: 'album-list album-list-large',
                    model: this._tags,
                    view: ModelView.extend(
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
            this.render();
        },
        reset: function() {
            this._tags.fetch();
        },
        render: function() {
            this.$el.empty();
            this._tagsView.render();
            this.$el.append(this._tagsView.$el);
        }
    }
    );

/*
 * Present a list of locations.  Clicking a location takes the user to a
 * thumbnail page of photographs taken at that location.
 */
var LocationsPage = PageView.extend(
    {
        pageTitle: 'Locations',
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            this._locations = new LocationCollection;
            this._locations.fetch();
            this._locationsView = new CollectionView(
                {
                    tagName: 'ul',
                    className: 'album-list album-list-large',
                    model: this._locations,
                    view: ModelView.extend(
                        {
                            tagName: 'li',
                            template: '<%-location%> (<%-photograph_count%>)',
                            events: { 'click': 'gotoLocation' },
                            gotoLocation: function() {
                                var location = this.model.get('location');
                                var model = new PhotographsWithLocation(
                                    [],
                                    { location: location }
                                    );
                                var page = new ThumbnailPage(
                                    {
                                        application: options.application,
                                        model: model,
                                        name: this.model.get('location')
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
            this._locations.fetch();
        },
        render: function() {
            this.$el.empty();
            this._locationsView.render();
            this.$el.append(this._locationsView.$el);
        }
    }
    );


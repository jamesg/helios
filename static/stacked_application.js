var StaticView = Backbone.View.extend(
    {
        initialize: function(options) {
            Backbone.View.prototype.initialize.apply(this, arguments);
            if(_.has(this, 'model'))
                this.listenTo(this.model, 'change', this.render.bind(this));
        },
        template: '',
        templateParams: function() {
            return _(this).has('model')?this.model.toJSON():{};
        },
        render: function() {
            this.$el.html(
                _.template(this.template).apply(this, [this.templateParams()])
                );
        }
    }
    );

var ModelView = StaticView.extend({});

var CollectionView = Backbone.View.extend(
    {
        initialize: function(options) {
            Backbone.View.prototype.initialize.apply(this, arguments);
            _(this).bindAll('add', 'remove');
            if(_.has(options, 'view')) this.view = options.view;
            this._views = [];
            this.model.each(this.add);
            this.listenTo(this.model, 'add', this.add);
            this.listenTo(this.model, 'remove', this.remove);
            this.listenTo(this.model, 'reset', this.reset);
        },
        initializeView: function(view) {
        },
        add: function(model) {
            var view = new this.view({
                model: model
            });
            this._views.push(view);
            this.initializeView(view);
            if(this._rendered) {
                view.render();
                this.$el.append(view.$el);
            }
        },
        remove: function(model) {
            var viewToRemove = _(this._views).select(
                function(cv) { return cv.model === model; }
                )[0];
            this._views = _(this._views).without(viewToRemove);
            if(this._rendered) viewToRemove.$el.remove();
        },
        reset: function() {
            this._views = [];
            this._rendered = false;
            this.model.each(
                function(model) {
                    var view = new this.view({
                        model: model
                    });
                    this._views.push(view);
                    this.initializeView(view);
                    },
                this
                );
            this.render();
        },
        render: function() {
            this._rendered = true;
            this.$el.empty();
            _(this._views).each(
                function(dv) { dv.render(); this.$el.append(dv.el); },
                this
                );
        }
    }
    );

var Breadcrumb = Backbone.Model.extend(
    {
        defaults: {
            page_title: '',
            view: null
        }
    }
    );

var BreadcrumbCollection = Backbone.Collection.extend(
    {
        model: Breadcrumb
    }
    );

var BreadcrumbView = ModelView.extend(
    {
        tagName: 'span',
        revisit: function revisit() {
            this.application.revisit(this.model);
        },
        events: {
            // TODO: should this be 'button click'?
            'click': 'revisit'
        },
        template:
            '<span aria-hidden="aria-hidden" data-glyph="chevron-right" class="oi"> </span>' +
            '<button class="display-link"><%-page_title%></button>  '
    }
    );

var BreadcrumbsView = CollectionView.extend(
    {
        view: BreadcrumbView,
        initialize: function(options) {
            this.application = options.application;
            CollectionView.prototype.initialize.apply(this, arguments);
            this.render();
        },
        initializeView: function(view) {
            view.application = this.application;
            view.render();
        },
        tagName: 'ul'
    }
    );

var Navigation = Backbone.View.extend(
    {
        tagName: 'div',
        className: 'navigation',
        initialize: function(options) {
            this.application = options.application;
            this._breadcrumbs = options.breadcrumbs;
            this._breadcrumbsView = new BreadcrumbsView(
                { application: this.application, model: this._breadcrumbs }
                );
            this._active = true;
            this.render();
        },
        setBreadcrumbs: function(breadcrumbs) {
            this._breadcrumbs = breadcrumbs;
            this.render();
        },
        render: function() {
            this.$el.empty();
            this.$el.append(this._breadcrumbsView.$el);
            return this;
        },
        templateParams: function() { return {}; },
        template: ''
        //function() {
            //div({ class: 'signin' }, this._signInView.el);
            //div(
                //{ class: 'navigation navigation-' + (this._active?'active':'inactive') },
                //h1('Atlas ', small('Intelligent Heating Controller'))
                //);
            //div({ class: 'breadcrumbs' }, this._breadcrumbsView.el);
        //}
    }
    );

var StackedApplication = function() {
    this.breadcrumbs = new BreadcrumbCollection;
    this.navigation = new Navigation(
            { application: this, breadcrumbs: this.breadcrumbs }
            );
    document.getElementById('template_header')
        .appendChild(this.navigation.render().el);

    this.goHome();
};

_.extend(StackedApplication, Backbone.Events);

StackedApplication.prototype._setElement = function(el) {
    document.getElementById('template_content').innerHTML = '';
    document.getElementById('template_content').appendChild(el);
};

StackedApplication.prototype._createPage = function(constructor) {
    return (_.isFunction(constructor))?
        (new constructor({ application: this })):
        constructor;
};

StackedApplication.prototype._createBreadcrumb = function(view) {
    return new Breadcrumb(
        {
            page_title: _.isFunction(view.pageTitle)?
                    view.pageTitle():view.pageTitle,
            view: view
        }
        );
};

StackedApplication.prototype._setPage = function(view) {
    this._setElement(view.el);
};

StackedApplication.prototype.goHome = function() {
    this.breadcrumbs.reset();
    var home = this._createPage(HomePage);
    this.breadcrumbs.add(this._createBreadcrumb(home));
    this._setPage(home);
};

StackedApplication.prototype.gotoPage = function(constructor) {
    this.breadcrumbs.reset();
    var home = this._createPage(HomePage);
    this.breadcrumbs.add(this._createBreadcrumb(home));
    this.pushPage(constructor);
};

StackedApplication.prototype.pushPage = function(constructor) {
    var view = this._createPage(constructor);
    this.breadcrumbs.add(this._createBreadcrumb(view));
    this._setPage(view);
};

StackedApplication.prototype.popPage = function() {
    this.breadcrumbs.pop();
    if(this.breadcrumbs.length == 0)
        this.goHome();
    else
        this._setPage(this.breadcrumbs.at(this.breadcrumbs.length-1).get('view'));
    this.currentPage().reset();
};

StackedApplication.prototype.currentPage = function() {
    if(this.breadcrumbs.length < 1)
        throw 'there is not current page';
    return this.breadcrumbs.at(this.breadcrumbs.length-1).get('view');
};

StackedApplication.prototype.revisit = function(breadcrumb) {
    var i = 0;
    while(i < this.breadcrumbs.length) {
        if(this.breadcrumbs.at(i) == breadcrumb) {
            this.breadcrumbs.remove(
                    this.breadcrumbs.slice(i+1, this.breadcrumbs.length)
                    );
            this._setPage(this.breadcrumbs.at(i).get('view'));
            this.currentPage().reset();
            return;
        }
        ++i;
    }
};

/*!
 * \brief Handle an authentication error by displaying a sign in page over the
 * current page.  Will only display a sign in page if none is currently
 * displayed.
 */
StackedApplication.prototype.authenticationError = function() {
    if(!(this.currentPage() instanceof AuthenticationRequiredPage)) {
        this.pushPage(AuthenticationRequiredPage);
    }
};


var BreadcrumbCollection = require('../collection/breadcrumb').BreadcrumbCollection;
var CollectionView = require('../view/collection').CollectionView;
var StaticView = require('./static').StaticView;

var icon = require('../ui').icon;

var HomePage = require('../page/home').HomePage;
var UploadPage = require('../page/upload').UploadPage;

var BreadcrumbView = StaticView.extend(
    {
        tagName: 'span',
        template: function() {
            span(' '), icon('caret-right'), span(' '),
            button(
                {
                    class: 'display-link',
                    onclick: (function() {
                        this.application.revisit(this.model);
                    }).bind(this)
                },
                this.model.get('page_title')
                );
        }
    }
    );

var BreadcrumbsView = CollectionView.extend(
    {
        view: BreadcrumbView,
        initialize: function(options) {
            this.application = options.application;
            CollectionView.prototype.initialize.apply(this, arguments);
        },
        initializeView: function(view) {
            view.application = this.application;
            view.render();
        },
        tagName: 'ul'
    }
    );

exports.Navigation = StaticView.extend(
    {
        tagName: 'div',
        initialize: function(options) {
            this.application = options.application;
            this.breadcrumbs = options.breadcrumbs;
            this.breadcrumbsView = new BreadcrumbsView(
                { application: this.application, model: this.breadcrumbs }
                );
            this._active = true;
            StaticView.prototype.initialize.apply(this, arguments);
        },
        activate: function() {
            this._active = true;
            this.render();
        },
        deactivate: function() {
            this._active = false;
            this.render();
        },
        setBreadcrumbs: function(breadcrumbs) {
            this.breadcrumbs = breadcrumbs;
            this.render();
        },
        template: function() {
            div(
                { class: 'navigation navigation-' + (this._active?'active':'inactive') },
                h1('Helios ', small('Photograph Album'))/*,*/
                //ul(
                    //li(
                        //button(
                            //{
                                //onclick: (function() {
                                    //gApplication.gotoPage(HomePage);
                                //}).bind(this)
                            //},
                            //icon('home'), 'Home'
                            //)
                      //)
                  /*)*/
                );
            div({ class: 'breadcrumbs' }, this.breadcrumbsView.el);
        }
    }
    );


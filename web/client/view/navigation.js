var BreadcrumbsView = require('../view/breadcrumbs');
var StaticView = require('./static').StaticView;

var icon = require('../ui').icon;

var HomePage = require('../page/home').HomePage;
var UploadPage = require('../page/upload').UploadPage;

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


var _ = require('underscore');
var Backbone = require('backbone');
Backbone.$ = require('jbone');
var domjs = require('domjs')(document);

var Breadcrumb = require('./model/breadcrumb').Breadcrumb;
var BreadcrumbCollection = require('./collection/breadcrumb').BreadcrumbCollection;
var HomePage = require('./page/home').HomePage;
var Navigation = require('./view/navigation').Navigation;

exports.Application = function() {
    this.breadcrumbs = new BreadcrumbCollection;
    this.navigation = new Navigation({ application: this, breadcrumbs: this.breadcrumbs });
    document.getElementById('template_header')
        .appendChild(this.navigation.render().el);

    this.goHome();
};

_.extend(exports.Application, Backbone.Events);

exports.Application.prototype._setElement = function(el) {
    document.getElementById('template_content').innerHTML = '';
    document.getElementById('template_content').appendChild(el);
};

exports.Application.prototype._createPage = function(constructor) {
    return (_.isFunction(constructor))?
        (new constructor({ application: this })):
        constructor;
};

exports.Application.prototype._createBreadcrumb = function(view) {
    return new Breadcrumb(
        {
            page_title: _.isFunction(view.pageTitle)?
                    view.pageTitle():view.pageTitle,
            view: view
        }
        );
};

exports.Application.prototype._setPage = function(view) {
    if(view.fullPage)
        this.navigation.deactivate();
    else
        this.navigation.activate();

    this._setElement(view.el);
};

exports.Application.prototype.goHome = function() {
    this.breadcrumbs.reset();
    var home = this._createPage(HomePage);
    this.breadcrumbs.add(this._createBreadcrumb(home));
    this._setPage(home);
};

exports.Application.prototype.gotoPage = function(constructor) {
    this.breadcrumbs.reset();
    var home = this._createPage(HomePage);
    this.breadcrumbs.add(this._createBreadcrumb(home));
    this.pushPage(constructor);
};

exports.Application.prototype.pushPage = function(constructor) {
    var view = this._createPage(constructor);
    this.breadcrumbs.add(this._createBreadcrumb(view));
    this._setPage(view);
};

exports.Application.prototype.popPage = function() {
    this.breadcrumbs.pop();
    if(this.breadcrumbs.length == 0)
        this.goHome();
    else
        this._setPage(this.breadcrumbs.at(this.breadcrumbs.length-1).view);
};

exports.Application.prototype.revisit = function(breadcrumb) {
    var i = 0;
    while(i < this.breadcrumbs.length) {
        if(this.breadcrumbs.at(i) == breadcrumb) {
            this.breadcrumbs.remove(this.breadcrumbs.slice(i+1, this.breadcrumbs.length));
            this._setPage(this.breadcrumbs.at(i).get('view'));
            return;
        }
        ++i;
    }
};


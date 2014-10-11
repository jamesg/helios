var _ = require('underscore');
var Backbone = require('backbone');
Backbone.$ = require('jbone');
var domjs = require('domjs')(document);

var HomePage = require('./page/home').HomePage;
var Navigation = require('./view/navigation').Navigation;

exports.Application = function() {
    this.navigation = new Navigation;
    document.getElementById('template_header')
        .appendChild(this.navigation.render().el);

    this.gotoView(HomePage);
};

exports.Application.prototype._setElement = function(el) {
    document.getElementById('template_content').innerHTML = '';
    document.getElementById('template_content').appendChild(el);
};

/*!
 * \brief Show a view, adjusting the navigation area of the page if necessary.
 *
 * \param constructor A view or a constructor inheriting from PageView and
 * taking no arguments.
 */
exports.Application.prototype.gotoView = function(constructor) {
    var view = (_.isFunction(constructor))?
        (new constructor({ application: this })):
        constructor;

    if(view.fullPage)
        this.navigation.deactivate();
    else
        this.navigation.activate();

    this._setElement(view.el);
};


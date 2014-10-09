var _ = require('underscore');
var Backbone = require('backbone');
Backbone.$ = require('jbone');
var domjs = require('domjs')(document);

var CategoryCollection = require('./model/categorycollection').CategoryCollection;
var CategoryList = require('./view/categorylist').CategoryList;
var Home = require('./home').Home;
var Navigation = require('./view/navigation').Navigation;

exports.Application = function() {
    var header = new Navigation;
    document.getElementById('template_header').appendChild(header.render().el);

    var home = new Home;
    this.setElement(home.render().el);

    var categories = new CategoryCollection;
    categories.fetch();
    var categoryList = new CategoryList({ model: categories });
    document.getElementById('template_side').appendChild(categoryList.render().el);
};

exports.Application.prototype.setElement = function(el) {
    document.getElementById('template_content').innerHTML = '';
    document.getElementById('template_content').appendChild(el);
};


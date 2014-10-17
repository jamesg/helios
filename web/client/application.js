var _ = require('underscore');
var Backbone = require('backbone');
Backbone.$ = require('jbone');
var domjs = require('domjs')(document);

var Breadcrumb = require('./model/breadcrumb').Breadcrumb;
var BreadcrumbCollection = require('./collection/breadcrumb').BreadcrumbCollection;
var HomePage = require('./page/home').HomePage;
var Navigation = require('./view/navigation').Navigation;
var StackedApplication = require('./stackedapplication').StackedApplication;

exports.Application = StackedApplication;


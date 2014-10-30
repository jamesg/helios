var _ = require('underscore');
var PageView = require('../view/page').PageView;
var PhotographPage = require('../page/photograph').PhotographPage;
var PhotographThumbListView = require('../view/photographthumblist').PhotographThumbListView;
var TagCollection = require('../collection/tag').TagCollection;
var TagPhotographs = require('../collection/tagphotographs').TagPhotographs;
var TagListView = require('../view/taglist').TagListView;
var ui = require('../ui');

var stringComparator = function(x, y) {
    if(x < y) return -1;
    if(x == y) return 0;
    if(x > y) return 1;
};

var alphaComparator = function(x, y) {
    return stringComparator(x.get('tag'), y.get('tag'));
};

var popularityComparator = function(x, y) {
    var pcX = parseInt(x.get('photograph_count'));
    var pcY = parseInt(y.get('photograph_count'));
    if(pcX < pcY) return 1;
    if(pcX == pcY) return 0;
    if(pcX > pcY) return -1;
};

var TagPage = PageView.extend(
    {
        pageTitle: function() {
            return this.tag.get('tag');
        },
        fullPage: true,
        initialize: function(options) {
            this.application = options.application;
            // Pass the tag on to new PhotographPages.
            this.tag = options.model;
            this.model = new TagPhotographs({ tag: this.tag });
            this.listenTo(this.model, 'all', this.render.bind(this));
            this.model.fetch();
            this.thumbList = new PhotographThumbListView({ model: this.model });
            this.listenTo(this.thumbList, 'click', this.gotoPhotograph.bind(this));
            this.render();
        },
        reset: function() {
            this.model.fetch();
        },
        gotoPhotograph: function(photograph) {
            this.application.pushPage(new PhotographPage({ model: photograph, withTag: this.tag }));
        },
        template: function() {
            return div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2('Photographs tagged "', this.tag.get('tag'), '"'),
                    div(this.thumbList.el)
                   )
                );
        }
    }
    );

exports.TagsPage = PageView.extend(
    {
        pageTitle: 'Tags',
        fullPage: true,
        initialize: function(options) {
            this.application = options.application;
            this.tagCollection = new TagCollection(
                {
                    comparator: alphaComparator
                }
                );
            this.tagCollection.fetch();
            this.tagList = new TagListView({ model: this.tagCollection });
            this.listenTo(this.tagList, 'click', this.gotoTag.bind(this));
            PageView.prototype.initialize.apply(this, arguments);
        },
        template: function() {
            div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2('Tags'),
                    button(
                        {
                            type: 'button',
                            class: 'pure-button',
                            onclick: (function() {
                                this.sortAlphabetically();
                            }).bind(this)
                        },
                        ui.icon('sort-ascending'), 'Sort alphabetically'
                        ),
                    button(
                        {
                            type: 'button',
                            class: 'pure-button',
                            onclick: (function() {
                                console.log('sort', this);
                                this.sortPopularity();
                            }).bind(this)
                        },
                        ui.icon('sort-descending'), 'Sort by popularity'
                        )
                   ),
                div(
                    { class: 'pure-u-1-1' },
                    this.tagList.el
                   )
               );
        },
        gotoTag: function(tag) {
            this.application.pushPage(
                new TagPage({ application: this.application, model: tag })
                );
        },
        sortAlphabetically: function() {
            this.tagCollection.comparator = alphaComparator;
            this.tagCollection.sort();
            this.tagList.collectionView().reset();
        },
        sortPopularity: function() {
            this.tagCollection.comparator = popularityComparator;
            this.tagCollection.sort();
            this.tagList.collectionView().reset();
        }
    }
    );


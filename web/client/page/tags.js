var _ = require('underscore');
var PageView = require('../view/page').PageView;
var PhotographThumbListView = require('../view/photographthumblist').PhotographThumbListView;
var TagCollection = require('../collection/tag').TagCollection;
var TagPhotographs = require('../collection/tagphotographs').TagPhotographs;
var TagListView = require('../view/taglist').TagListView;
var ui = require('../ui');

var stringComparator = function(x, y) {
    if(x < y) return -1;
    if(x == y) return 0;
    if(x > y) return -1;
};

var alphaComparator = function(x, y) {
    return stringComparator(x.get('tag'), y.get('tag'));
};

var popularityComparator = function(x, y) {
    return stringComparator(x.get('photograph_count'), y.get('photograph_count'));
};

var TagPage = PageView.extend(
    {
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
        gotoPhotograph: function(photograph) {
            this.application.gotoPage(new PhotographPage({ model: photograph, inAlbum: this.tag }));
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
                            onclick: this.sortAlphabetically.bind(this)
                        },
                        ui.icon('sort-descending'), 'Sort alphabetically'
                        ),
                    button(
                        {
                            type: 'button',
                            class: 'pure-button',
                            onclick: this.sortPopularity.bind(this)
                        },
                        ui.icon('sort-descending'), 'Sort by popularity'
                        ),
                    this.tagList.el
                   )
               );
        },
        gotoTag: function(tag) {
            this.application.gotoPage(
                new TagPage({ application: this.application, model: tag })
                );
        },
        sortAlphabetically: function() {
            this.tagCollection.comparator = alphaComparator;
            this.tagCollection.sort();
        },
        sortPopularity: function() {
            this.tagCollection.comparator = popularityComparator;
            this.tagCollection.sort();
        }
    }
    );


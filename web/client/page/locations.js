var _ = require('underscore');
var PageView = require('../view/page').PageView;
var PhotographPage = require('../page/photograph').PhotographPage;
var PhotographThumbListView = require('../view/photographthumblist').PhotographThumbListView;
var LocationCollection = require('../collection/location').LocationCollection;
var LocationPhotographs = require('../collection/locationphotographs').LocationPhotographs;
var LocationListView = require('../view/locationlist').LocationListView;
var ui = require('../ui');

var stringComparator = function(x, y) {
    if(x < y) return -1;
    if(x == y) return 0;
    if(x > y) return 1;
};

var alphaComparator = function(x, y) {
    return stringComparator(x.get('location'), y.get('location'));
};

var popularityComparator = function(x, y) {
    var pcX = parseInt(x.get('photograph_count'));
    var pcY = parseInt(y.get('photograph_count'));
    if(pcX < pcY) return 1;
    if(pcX == pcY) return 0;
    if(pcX > pcY) return -1;
};

var LocationPage = PageView.extend(
    {
        pageTitle: function() {
            return this.location.get('location');
        },
        fullPage: true,
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            // Pass the location on to new PhotographPages.
            this.location = options.model;
            this.model = new LocationPhotographs({ location: this.location });
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
            this.application.pushPage(
                    new PhotographPage(
                        {
                            application: this.application,
                            model: photograph,
                            atLocation: this.location
                        }
                        )
                    );
        },
        template: function() {
            return div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2('Photographs taken at ', this.location.get('location')),
                    div(this.thumbList.el)
                   )
                );
        }
    }
    );

exports.LocationsPage = PageView.extend(
    {
        pageTitle: 'Locations',
        fullPage: true,
        initialize: function(options) {
            PageView.prototype.initialize.apply(this, arguments);
            this.locationCollection = new LocationCollection(
                {
                    comparator: alphaComparator
                }
                );
            this.locationCollection.fetch();
            this.locationList = new LocationListView({ model: this.locationCollection });
            this.listenTo(this.locationList, 'click', this.gotoLocation.bind(this));
            this.render();
        },
        template: function() {
            div(
                { class: 'pure-g' },
                div(
                    { class: 'pure-u-1-1' },
                    h2('Locations'),
                    button(
                        {
                            type: 'button',
                            class: 'pure-button',
                            onclick: this.sortAlphabetically.bind(this)
                        },
                        ui.icon('sort-ascending'), 'Sort alphabetically'
                        ),
                    button(
                        {
                            type: 'button',
                            class: 'pure-button',
                            onclick: this.sortPopularity.bind(this)
                        },
                        ui.icon('sort-descending'), 'Sort by popularity'
                        )
                   ),
                div(
                    { class: 'pure-u-1-1' },
                    this.locationList.el
                   )
               );
        },
        gotoLocation: function(location) {
            this.application.pushPage(
                new LocationPage({ application: this.application, model: location })
                );
        },
        sortAlphabetically: function() {
            this.locationCollection.comparator = alphaComparator;
            this.locationCollection.sort();
            this.locationList.collectionView().reset();
        },
        sortPopularity: function() {
            this.locationCollection.comparator = popularityComparator;
            this.locationCollection.sort();
            this.locationList.collectionView().reset();
        }
    }
    );


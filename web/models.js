/*
 * Get the URI for a REST API handler.
 */
var restUri = function(fragment) {
    return fragment;
};

var Album = RestModel.extend(
    {
        defaults: {
            name: ''
        },
        idAttribute: 'album_id',
        validate: function() {
            var errors = {};
            if(this.get('name') == '')
                errors['name'] = 'Name is required';
            if(!_.isEmpty(errors))
                return errors;
        },
        url: function() {
            return restUri(
                this.isNew()?'album':('album/' + this.id())
                );
        }
    }
    );

var AlbumCollection = RestCollection.extend(
    {
        model: Album,
        url: restUri('album')
    }
    );

var Tag = RestModel.extend(
    {
        defaults: {
            photograph_count: 0
        },
        idAttribute: 'tag'
    }
    );

var TagCollection = RestCollection.extend(
    {
        model: Tag,
        url: restUri('tag')
    }
    );

var Location = RestModel.extend(
    {
        defaults: {
            photograph_count: 0
        },
        idAttribute: 'location'
    }
    );

var LocationCollection = RestCollection.extend(
    {
        model: Location,
        url: restUri('location')
    }
    );

var Photograph = RestModel.extend(
    {
        defaults: {
            title: '',
            caption: '',
            location: '',
            taken: '',
            tags: ''
        },
        idAttribute: 'photograph_id',
        validate: function() {
            var errors = {};
            if(this.get('title') == '')
                errors['title'] = 'Title is required';
            if(this.get('location') == '')
                errors['location'] = 'Location is required.';
            if(!_.isEmpty(errors))
                return errors;
        },
        url: function() {
            return restUri(
                this.isNew()?'photograph':('photograph/' + this.get('photograph_id'))
                );
        }
    }
    );

var RandomPhotograph = Photograph.extend(
    {
        url: function() {
            return restUri(
                this.isNew()?'photograph/random':('photograph/' + this.get('photograph_id'))
                );
        }
    }
    );

var PhotographsInAlbum = RestCollection.extend(
    {
        initialize: function(models, options) {
            RestCollection.prototype.initialize.apply(this, options);
            this._album = options.album;
        },
        model: Photograph,
        url: function() {
            return restUri(
                'album/' + this._album.get('album_id') + '/photograph'
                );
        }
    }
    );

var UncategorisedPhotographs = RestCollection.extend(
    {
        model: Photograph,
        url: restUri('uncategorised/photograph')
    }
    );

var PhotographsWithTag = RestCollection.extend(
    {
        initialize: function(models, options) {
            RestCollection.prototype.initialize.apply(this, options);
            this._tag = options.tag;
        },
        model: Photograph,
        url: function() {
            return restUri('tag/' + this._tag + '/photograph');
        }
    }
    );

var PhotographsWithLocation = RestCollection.extend(
    {
        initialize: function(models, options) {
            RestCollection.prototype.initialize.apply(this, arguments);
            this._location = options.location;
        },
        model: Photograph,
        url: function() {
            return restUri('location/' + this._location + '/photograph');
        }
    }
    );

var PhotographAlbums = RestCollection.extend(
    {
        initialize: function(models, options) {
            RestCollection.prototype.initialize.apply(this, arguments);
            this._photograph = options.photograph;
        },
        model: Album,
        url: function() {
            return restUri(
                'photograph/' + this._photograph.get('photograph_id') + '/album'
                );
        }
    }
    );

